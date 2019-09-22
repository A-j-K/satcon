

#include "mainwindow/mainwindow.h"
#include "capturewindow/capturewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mw;
	CaptureWindow cw;
    mw.show();
	cw.show();
    return app.exec();
}
