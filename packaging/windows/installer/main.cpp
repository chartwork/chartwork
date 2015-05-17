#include "mainwindow.h"
#include <QApplication>
#include <QtPlugin>

Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
{
	int argc = 0;
	QApplication a(argc, nullptr);
	MainWindow w;
	w.show();

	return a.exec();
}
