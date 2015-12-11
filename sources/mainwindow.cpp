#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    QSettings settings(qApp->organizationName(),qApp->applicationName());

    m_view = 0;
    m_active = false;
    m_timer = new QTimer;

    QFont font;
        font.setBold(true);

    m_numX = new QSpinBox;
        m_numX->setMinimum(3);
        if (settings.contains("width"))
            m_numX->setValue(settings.value("width").toInt());
        else m_numX->setValue(50);
    m_numY = new QSpinBox;
        m_numY->setMinimum(3);
        if (settings.contains("height"))
            m_numY->setValue(settings.value("height").toInt());
        else m_numY->setValue(30);
    m_reset = new QPushButton(tr("Reset with new size"));
        m_reset->setFont(font);
    m_state = new QPushButton(tr("Start"));
        m_state->setFont(font);
    m_next = new QPushButton(tr("Next Turn"));
        m_next->setFont(font);
    m_clear = new QPushButton(tr("Clear"));
        m_clear->setFont(font);
    m_speed = new QSlider(Qt::Horizontal);
        m_speed->setMaximum(5);
        m_speed->setTickPosition(QSlider::TicksBothSides);
        if (settings.contains("speed"))
            m_speed->setValue(settings.value("speed").toInt());
        else m_speed->setValue(5);

    reset(true);
}

void MainWindow::saveSettings() {
    QSettings settings(qApp->organizationName(),qApp->applicationName());
    if (!settings.contains("remember")) return;
    settings.setValue("a_color",m_view->activeColor());
    settings.setValue("na_color",m_view->inactiveColor());
    settings.setValue("speed",m_speed->value());
    settings.setValue("width",m_numX->value());
    settings.setValue("height",m_numY->value());
}

void MainWindow::reset(bool isAtStartup) {
    if (!isAtStartup && m_numX->value() == m_view->width() && m_numY->value() == m_view->height())
        return;
    if (m_view != 0)
        delete m_view;
    m_view = new GraphicsViewer(m_numX->value(),m_numY->value(),10,10);
    if (isAtStartup) {
        connect(m_reset,SIGNAL(clicked()),this,SLOT(reset()));
        connect(m_state,SIGNAL(clicked()),this,SLOT(stateChange()));
        connect(m_next,SIGNAL(clicked()),this,SLOT(nextTurn()));
        connect(m_timer,SIGNAL(timeout()),this,SLOT(nextTurn()));
        connect(m_speed,SIGNAL(valueChanged(int)),this,SLOT(setInterval(int)));
    }
    connect(m_clear,SIGNAL(clicked()),m_view,SLOT(clear()));
    m_timer->stop();
    m_active = false;
    m_state->setText(tr("Start"));

    QVBoxLayout* mainLayout = new QVBoxLayout;
        QHBoxLayout* sizeLayout = new QHBoxLayout;
        mainLayout->addLayout(sizeLayout);
        QHBoxLayout* subMainLayout = new QHBoxLayout;
            subMainLayout->addWidget(m_view);
            QVBoxLayout* buttonLayout = new QVBoxLayout;
                buttonLayout->addWidget(m_state);
                buttonLayout->addWidget(m_next);
                buttonLayout->addWidget(m_clear);
            subMainLayout->addLayout(buttonLayout);
        mainLayout->addLayout(subMainLayout);
    QWidget* dummy = new QWidget;
        dummy->setLayout(mainLayout);
    setCentralWidget(dummy);

    QTimer::singleShot(500,this,SLOT(adjustToMinimalSize()));
    saveSettings();
}

void MainWindow::callNAColor() {
    if (m_active) m_timer->stop();
    m_view->newInactiveColor();
    if (m_active) m_timer->start();
    saveSettings();
}

void MainWindow::callAColor() {
    if (m_active) m_timer->stop();
    m_view->newActiveColor();
    if (m_active) m_timer->start();
    saveSettings();
}

void MainWindow::callDefColor() {
    if (m_active) m_timer->stop();
    m_view->setDefaultColor();
    if (m_active) m_timer->start();
    saveSettings();
}

void MainWindow::adjustToMinimalSize() {
    resize(1,1);
}

void MainWindow::setInterval(int value) {
    m_timer->setInterval(1000-((value+1)*150));
    saveSettings();
}

void MainWindow::stateChange() {
    m_active = !m_active;
    m_timer->setInterval(1000-((m_speed->value()+1)*150));
    if (m_active) {
        m_state->setText(tr("Stop"));
        m_timer->start();
    }
    else {
        m_state->setText(tr("Start"));
        m_timer->stop();
    }
}

void MainWindow::nextTurn() {
    QList<QList<int> > nextState = m_view->state();
    for (int y = 0; y < m_view->height(); y++)
        for (int x = 0; x < m_view->width(); x++) {
            int n = numberOfNeighboor(x,y,m_view->state());
            if (n < 2)
                nextState[x][y] = false;
            if (n == 3)
                nextState[x][y] = true;
            if (n > 3)
                nextState[x][y] = false;
        }
    for (int y = 0; y < m_view->height(); y++)
        for (int x = 0; x < m_view->width(); x++) {
            if (nextState[x][y] != m_view->state()[x][y])
                m_view->setState(x,y,nextState[x][y]);
        }
}

int MainWindow::numberOfNeighboor(int rx,int ry,const QList<QList<int> >& state) {
    int n = 0;
    for (int y = ry-1; y < ry+2; y++)
        for (int x = rx-1; x < rx+2; x++) {
            if (x == rx && y == ry)
                continue;
            if (x >= 0 && x < m_view->width() && y >= 0 && y < m_view->height())
                if (state[x][y])
                    n++;
        }
    return n;
}