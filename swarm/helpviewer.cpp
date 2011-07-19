// Copyright (c) 2005-2008 Oliver Lau <ola@ctmagazin.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#include <QIODevice>
#include <QFile>
#include <QTextStream>

#include "helpviewer.h"

HelpViewer::HelpViewer(QWidget* parent) : QDialog(parent)
{
  setWindowTitle(tr("Help on Particle Swarm Optimization"));

  // create widgets
  textEdit = new QTextEdit();
  textEdit->setReadOnly(true);
  QFile* in = new QFile(":/Resources/help.html");
  in->open(QIODevice::ReadOnly);
  QTextStream html(in);
  textEdit->setHtml(html.readAll());
  in->close();
  delete in;

  buttonOK = new QPushButton();
  buttonOK->setText(tr("Close window"));
  buttonOK->setMinimumSize(128, 24);
  buttonOK->setMaximumSize(128, 24);

  QVBoxLayout* vboxLayout = new QVBoxLayout(this);

  // fill layouts
  vboxLayout->addWidget(textEdit);
  vboxLayout->addWidget(buttonOK);

  QObject::connect(buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
}


HelpViewer::~HelpViewer(void)
{
  delete buttonOK;
  delete textEdit;
}
