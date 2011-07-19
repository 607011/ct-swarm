// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#include "zfinder.h"

ZFinder::ZFinder(QWidget *parent) : QDialog(parent)
{
    // create widgets
    slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 20);

    lineEdit = new QLineEdit();

    buttonOK = new QPushButton();
    buttonOK->setText(tr("&OK"));
    buttonOK->setMinimumSize(64, 24);
    buttonOK->setMaximumSize(64, 24);

    buttonCancel = new QPushButton();
    buttonCancel->setText(tr("&Cancel"));
    buttonCancel->setMinimumSize(64, 24);
    buttonCancel->setMaximumSize(64, 24);

    // create layouts
    QGridLayout* gridLayout = new QGridLayout(this);

    // fill layouts
    gridLayout->addWidget(slider, 0, 0, 1, 1);
    gridLayout->addWidget(lineEdit, 0, 1, 1, 1);
    gridLayout->addWidget(buttonOK, 1, 0, 1, 1);
    gridLayout->addWidget(buttonCancel, 1, 1, 1, 1);

    // connections
    QObject::connect(buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));

    slider->setValue(5);
}

void ZFinder::sliderChanged(int v)
{
    lineEdit->setText(QString("%0").arg(v));
}
