#ifndef THREAD_H
#define THREAD_H
#include <QtCore>

class thread : public QThread{
public:
    thread();
    void run();
};

#endif // THREAD_H
