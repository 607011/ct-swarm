// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef __ZFINDER_H_
#define __ZFINDER_H_

#include <QObject>
#include <QSize>
#include <QWidget>
#include <QDialog>
#include <QSlider>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>

class ZFinder :	public QDialog
{
    Q_OBJECT

private: // variables
    QSlider* slider;
    QPushButton* buttonOK;
    QPushButton* buttonCancel;
    QLineEdit* lineEdit;

private slots:
    void sliderChanged(int v);

public: // methods
    ZFinder(QWidget* parent = 0);
    ~ZFinder(void) {}

    QSize sizeHint(void) const
    {
        return QSize(160, 100);
    }

    QSize minimumSizeHint(void) const
    {
        return QSize(155, 80);
    }

    int value(void) const
    {
        return slider->value();
    }

};

#endif // __ZFINDER_H_
