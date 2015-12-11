#include <QApplication>
//#include <QTranslator>
//#include <QLocale>
//#include <QLibraryInfo>

#include "mainwindow.h"

int main(int argc,char** argv) {
    QApplication app(argc, argv);
	
    app.setApplicationName("Cellular Automata");
    app.setOrganizationDomain("http://www.itb.ie/");
    app.setOrganizationName("ITB");
    app.setApplicationVersion("0.1a");
	
    MainWindow w;
    w.show();
    return app.exec();
}
