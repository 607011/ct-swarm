// Copyright (c) 2005-2008 Oliver Lau <ola@ctmagazin.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDateTime>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
#if defined (_OPENMP)
  char *version;
#if _OPENMP == 200505
  version = "2.5";
#elif _OPENMP == 200203
  version = "2.0";
#elif _OPENMP == 199810
  version = "1.0";
#else
  version = "";
#endif
  qDebug("compiled with OpenMP %s support (%d)", version, _OPENMP);
#endif
  srand(QDateTime::currentDateTime().toTime_t());
  QApplication app(argc, argv);
  QTranslator swarmTranslator;
  swarmTranslator.load("swarm-" + QLocale::system().name());
  app.installTranslator(&swarmTranslator);
  MainWindow w;
  w.show();
  app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
  return app.exec();
}
