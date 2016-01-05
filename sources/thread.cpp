#include "thread.h"
#include <QtCore>
#include <QDebug>
#include "mainwindow.h"


thread::thread(){

}

void thread::run(){
    qDebug() << this->currentThread()<< "Running";
//    for (int i = 0; i<10; i++){
//        qDebug() << this->currentThread() << i;
//    }

    if (this->objectName()=="Medicine"){
        qDebug() << "Filter working";
//        nextTurnMedicine();

    }
}

