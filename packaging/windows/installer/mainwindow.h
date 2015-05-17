#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QThread>
#include <memory>

namespace Ui {
class MainWindow;
}

// Forward declarations
class FindPathsThread;
class InstallThread;

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MainWindow
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_changeQtDirectoryButton_clicked();
	void on_findPathsThreadFinished();
	void on_installButton_clicked();
	void on_progressUpdated(int progress, int maxProgress);
	void on_installThreadFailed(QString message);

private:
	Ui::MainWindow *ui;

	std::unique_ptr<FindPathsThread> m_findPathsThread;
	std::unique_ptr<InstallThread> m_installThread;
	int m_progress;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FindPathsThread
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class FindPathsThread : public QThread
{
	Q_OBJECT

public:
	FindPathsThread(QDir rootDirectory);

	const QDir &qtDirectory() const;
	const QDir &qtCreatorDirectory() const;

private:
	void run();
	void find(const QDir &directory);

	QDir m_rootDirectory;
	QDir m_qtDirectory;
	QDir m_qtCreatorDirectory;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// InstallThread
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class InstallThread : public QThread
{
	Q_OBJECT

public:
	InstallThread(QDir qtDirectory, QDir qtCreatorDirectory);

private:
	int m_progress;
	int m_maxProgress;
	QDir m_qtDirectory;
	QDir m_qtCreatorDirectory;

	void run();
	void increaseProgress();
	int countFiles(QString directoryName);
	void installFile(QString fileName, QString destination);
	void installDirectory(QString directoryName, QString destination);

signals:
	void progressUpdated(int, int);
	void failed(QString message);
};

#endif // MAINWINDOW_H
