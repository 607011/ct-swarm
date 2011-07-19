// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDateTime>
#include <QtCore>

#include "mainwindow.h"

int main(int argc, char* argv[])
{
#if defined (_OPENMP)
#if _OPENMP == 200505
    const char* version = "2.5";
#elif _OPENMP == 200203
    const char* version = "2.0";
#elif _OPENMP == 199810
    const char* version = "1.0";
#else
    const char* version = "x";
#endif
    qDebug("compiled with OpenMP %s support (%d)", version, _OPENMP);
#endif
    qsrand(QDateTime::currentDateTime().toTime_t());
    QApplication app(argc, argv);
    QTranslator swarmTranslator;
    swarmTranslator.load("swarm-" + QLocale::system().name());
    app.installTranslator(&swarmTranslator);
    MainWindow w;
    w.show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    return app.exec();
}
