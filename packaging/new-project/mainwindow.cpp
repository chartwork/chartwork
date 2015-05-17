#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::loop()
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Your code here
    //
	////////////////////////////////////////////////////////////////////////////////////////////////
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this))
{
	ui->setupUi(this);

    // FPS rate in milliseconds
    timer->setInterval(50);
    timer->setSingleShot(false);
    timer->start();

	connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
}

MainWindow::~MainWindow()
{
	delete ui;
    delete timer;
}
