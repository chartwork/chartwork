#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QTemporaryFile>
#include <QTextStream>
#include <iostream>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MainWindow
//
////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	setFixedSize(geometry().size());
	setWindowFlags(Qt::WindowCloseButtonHint);

	ui->qtPathLabel->hide();
	ui->qtPath->hide();
	ui->qtCreatorPathLabel->hide();
	ui->qtCreatorPath->hide();

	ui->progressBar->hide();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
    delete ui;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_changeQtDirectoryButton_clicked()
{
	ui->changeQtDirectoryButton->setEnabled(false);
	ui->installButton->setEnabled(false);

	const QString directory = QFileDialog::getExistingDirectory(this, "Qt Directory", ".");

	if (directory.isEmpty())
		return;

	const QDir rootDirectory(directory);

	ui->qtPathLabel->hide();
	ui->qtCreatorPathLabel->hide();
	ui->qtPath->hide();
	ui->qtCreatorPath->hide();

	if (m_findPathsThread)
	{
		m_findPathsThread->wait();
		disconnect(m_findPathsThread.get(), &FindPathsThread::finished, this, &MainWindow::on_findPathsThreadFinished);
	}

	ui->progressBar->setMinimum(0);
	ui->progressBar->setMaximum(0);
	ui->progressBar->show();

	m_findPathsThread = std::unique_ptr<FindPathsThread>(new FindPathsThread(rootDirectory));
	connect(m_findPathsThread.get(), &FindPathsThread::finished, this, &MainWindow::on_findPathsThreadFinished);
	m_findPathsThread->start();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_findPathsThreadFinished()
{
	const auto &qtDirectory = m_findPathsThread->qtDirectory();
	const auto &qtCreatorDirectory = m_findPathsThread->qtCreatorDirectory();

	if (qtDirectory == QDir())
	ui->qtPath->setText("");
	else
	ui->qtPath->setText(qtDirectory.path());

	if (qtCreatorDirectory == QDir())
	ui->qtCreatorPath->setText("");
	else
	ui->qtCreatorPath->setText(qtCreatorDirectory.path());

	ui->changeQtDirectoryButton->setEnabled(true);
	ui->progressBar->hide();

	if (qtDirectory == QDir() || qtCreatorDirectory == QDir())
	{
		QMessageBox messageBox;
		messageBox.setText("Could not find Qt or Qt Creator.");
		messageBox.setWindowTitle("Error");
		messageBox.exec();
		return;
	}

	ui->qtPathLabel->show();
	ui->qtPath->show();
	ui->qtCreatorPathLabel->show();
	ui->qtCreatorPath->show();

	ui->installButton->setEnabled(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_installButton_clicked()
{
	ui->changeQtDirectoryButton->setEnabled(false);
	ui->installButton->setEnabled(false);

	if (m_installThread)
	{
		m_installThread->wait();
		disconnect(m_installThread.get(), &InstallThread::progressUpdated, this, &MainWindow::on_progressUpdated);
		disconnect(m_installThread.get(), &InstallThread::failed, this, &MainWindow::on_installThreadFailed);
	}

	ui->progressBar->setMaximum(0);
	ui->progressBar->setValue(0);
	ui->progressBar->show();

	m_installThread = std::unique_ptr<InstallThread>(new InstallThread(m_findPathsThread->qtDirectory(), m_findPathsThread->qtCreatorDirectory()));
	connect(m_installThread.get(), &InstallThread::progressUpdated, this, &MainWindow::on_progressUpdated);
	connect(m_installThread.get(), &InstallThread::failed, this, &MainWindow::on_installThreadFailed);
	m_installThread->start();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_progressUpdated(int progress, int maxProgress)
{
	ui->progressBar->setValue(progress);
	ui->progressBar->setMaximum(maxProgress);

	if (progress == maxProgress)
	{
		QMessageBox messageBox;
		messageBox.setText("Installation complete!");
		messageBox.setWindowTitle("Installation Complete");
		messageBox.exec();
		exit(EXIT_SUCCESS);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_installThreadFailed(QString message)
{
	QMessageBox messageBox;
	messageBox.setText(message);
	messageBox.setWindowTitle("Error");
	messageBox.exec();
	exit(EXIT_FAILURE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FindPathsThread
//
////////////////////////////////////////////////////////////////////////////////////////////////////

FindPathsThread::FindPathsThread(QDir rootDirectory)
:	m_rootDirectory(rootDirectory)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const QDir &FindPathsThread::qtDirectory() const
{
	return m_qtDirectory;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const QDir &FindPathsThread::qtCreatorDirectory() const
{
	return m_qtCreatorDirectory;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void FindPathsThread::run()
{
	find(m_rootDirectory);

	if (m_qtDirectory != QDir())
		m_qtDirectory.cdUp();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void FindPathsThread::find(const QDir &directory)
{
	QFileInfoList list = directory.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs);

	for (int i = 0; i < list.size(); ++i)
	{
		const auto &fileInfo = list.at(i);

		if (fileInfo.isFile())
		{
			if (fileInfo.fileName() == "qmake")
				m_qtDirectory = fileInfo.absoluteDir();
		}
		else if (fileInfo.isDir())
		{
			if (fileInfo.fileName() == "Qt Creator.app")
				m_qtCreatorDirectory = QDir(fileInfo.absoluteFilePath());
			else
				find(QDir(fileInfo.filePath()));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// InstallThread
//
////////////////////////////////////////////////////////////////////////////////////////////////////

InstallThread::InstallThread(QDir qtDirectory, QDir qtCreatorDirectory)
:	m_progress(0),
	m_maxProgress(0),
	m_qtDirectory(qtDirectory),
	m_qtCreatorDirectory(qtCreatorDirectory)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void InstallThread::run()
{
	m_maxProgress = countFiles(":/files/") + 1;

	auto runProcess = [&](QString command, QStringList arguments = QStringList())
	{
		QProcess process;
		process.setWorkingDirectory(m_qtDirectory.path());
		process.start(command, arguments);

		if (!process.waitForStarted()
			|| !process.waitForFinished())
		{
			QMessageBox messageBox;
			messageBox.setText("Could not call " + command + ".");
			messageBox.setWindowTitle("Error");
			messageBox.exec();
			exit(EXIT_FAILURE);
		}
		else
		{
			increaseProgress();
			std::cout << "Called " << command.toStdString() << "." << std::endl;
		}
	};

	QTemporaryFile installScriptFile;

	if (!installScriptFile.open())
	{
		QMessageBox messageBox;
		messageBox.setText("Could not create temporary file.");
		messageBox.setWindowTitle("Error");
		messageBox.exec();
		exit(EXIT_FAILURE);
	}

	QTextStream installScript(&installScriptFile);

	const QString libchartworkDylib = m_qtDirectory.filePath("lib/libchartwork.dylib");
	const QString libchartworkDebugDylib = m_qtDirectory.filePath("lib/libchartworkd.dylib");

	installScript
		<< "#!/bin/bash" << "\n"
		<< "chmod 0644 " << libchartworkDylib << "\n"
		<< "chmod 0644 " << libchartworkDebugDylib << "\n"
		<< "install_name_tool -id " << libchartworkDylib << " " << libchartworkDylib << "\n"
		<< "install_name_tool -id " << libchartworkDebugDylib << " " << libchartworkDebugDylib << "\n"
		<< "install_name_tool -change QTDIR/lib/QtDesigner.framework/Versions/5/QtDesigner " << m_qtDirectory.filePath("lib/QtDesigner.framework/Versions/5/QtDesigner") << " " << libchartworkDylib << "\n"
		<< "install_name_tool -change QTDIR/lib/QtDesigner.framework/Versions/5/QtDesigner " << m_qtDirectory.filePath("lib/QtDesigner.framework/Versions/5/QtDesigner") << " " << libchartworkDebugDylib << "\n"
		<< "install_name_tool -change QTDIR/lib/QtWidgets.framework/Versions/5/QtWidgets " << m_qtDirectory.filePath("lib/QtWidgets.framework/Versions/5/QtWidgets") << " " << libchartworkDylib << "\n"
		<< "install_name_tool -change QTDIR/lib/QtWidgets.framework/Versions/5/QtWidgets " << m_qtDirectory.filePath("lib/QtWidgets.framework/Versions/5/QtWidgets") << " " << libchartworkDebugDylib << "\n"
		<< "install_name_tool -change QTDIR/lib/QtGui.framework/Versions/5/QtGui " << m_qtDirectory.filePath("lib/QtGui.framework/Versions/5/QtGui") << " " << libchartworkDylib << "\n"
		<< "install_name_tool -change QTDIR/lib/QtGui.framework/Versions/5/QtGui " << m_qtDirectory.filePath("lib/QtGui.framework/Versions/5/QtGui") << " " << libchartworkDebugDylib << "\n"
		<< "install_name_tool -change QTDIR/lib/QtCore.framework/Versions/5/QtCore " << m_qtDirectory.filePath("lib/QtCore.framework/Versions/5/QtCore") << " " << libchartworkDylib << "\n"
		<< "install_name_tool -change QTDIR/lib/QtCore.framework/Versions/5/QtCore " << m_qtDirectory.filePath("lib/QtCore.framework/Versions/5/QtCore") << " " << libchartworkDebugDylib << "\n"
		<< "install_name_tool -change QTDIR/lib/QtXml.framework/Versions/5/QtXml " << m_qtDirectory.filePath("lib/QtXml.framework/Versions/5/QtXml") << " " << libchartworkDylib << "\n"
		<< "install_name_tool -change QTDIR/lib/QtXml.framework/Versions/5/QtXml " << m_qtDirectory.filePath("lib/QtXml.framework/Versions/5/QtXml") << " " << libchartworkDebugDylib << "\n";

	installScriptFile.close();

	installDirectory(":/files/data/plugin", m_qtCreatorDirectory.filePath("Contents/PlugIns/designer"));
	installDirectory(":/files/data/lib", m_qtDirectory.filePath("lib"));
	installDirectory(":/files/data/include", m_qtDirectory.filePath("include"));
	runProcess("sh", QStringList() << installScriptFile.fileName());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void InstallThread::increaseProgress()
{
	m_progress++;
	emit progressUpdated(m_progress, m_maxProgress);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int InstallThread::countFiles(QString directoryName)
{
	int count = 0;

	QDir sourceDirectory(directoryName);

	QFileInfoList list = sourceDirectory.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs);

	for (int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);

		if (fileInfo.isFile())
			count++;
		else if (fileInfo.isDir())
			count += countFiles(sourceDirectory.filePath(fileInfo.fileName()));
	}

	return count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void InstallThread::installFile(QString fileName, QString destination)
{
	if (!QFile::exists(fileName))
	{
		emit failed("Error: Missing resource " + fileName + ".");
		return;
	}

	QFile destinationFile(destination);

	if (destinationFile.exists())
	{
		std::cout << "Overwriting " << destination.toStdString() << "." << std::endl;

		if (!destinationFile.setPermissions(QFile::WriteOther | QFile::ReadOther) || !destinationFile.remove())
			std::cout << "Could not overwrite " << destinationFile.fileName().toStdString() << "." << std::endl;
	}

	if (QFile::copy(fileName, destinationFile.fileName()))
	{
		std::cout << "Installed " << destination.toStdString() << "." << std::endl;
		increaseProgress();
		return;
	}

	emit failed("Error: Could not install " + destination + ".");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void InstallThread::installDirectory(QString directoryName, QString destination)
{
	QDir destinationDirectory(destination);

	if (!destinationDirectory.exists())
		destinationDirectory.mkpath(".");

	QDir sourceDirectory(directoryName);

	QFileInfoList list = sourceDirectory.entryInfoList();

	for (int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);

		if (fileInfo.isFile())
			installFile(sourceDirectory.filePath(fileInfo.fileName()), destinationDirectory.filePath(fileInfo.fileName()));
		else if (fileInfo.isDir())
			installDirectory(sourceDirectory.filePath(fileInfo.fileName()), destinationDirectory.filePath(fileInfo.fileName()));
	}
}
