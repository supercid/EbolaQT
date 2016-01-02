#include <QApplication>
#include "mainwindow.h"
#include "thread.h"

int main(int argc,char** argv) {
    QApplication app(argc, argv);
	
    app.setApplicationName("Cellular Automata");
    app.setOrganizationDomain("http://www.itb.ie/");
    app.setOrganizationName("ITB");
    app.setApplicationVersion("0.1a");
	
    thread mThread1;
    mThread1.setObjectName("Infection");
    mThread1.start();

    thread mThread2;
    mThread2.setObjectName("Medicine");
    mThread2.start();


    MainWindow w;
    w.show();
    return app.exec();
}
