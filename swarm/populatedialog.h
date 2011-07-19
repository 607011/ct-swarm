// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef __POPULATEDIALOG_H_
#define __POPULATEDIALOG_H_

#include <QObject>
#include <QDialog>
#include <QWidget>
#include "ui_populatedialog.h"

class PopulateDialog : public QDialog
{
    Q_OBJECT

public:
    PopulateDialog(QWidget* parent = 0);
    int distribution(void) const;
    void setNumber(int);
    int number(void) const;
    void setDistribution(int);
    int distributionIndex(void) const;
    enum Distribution {
        UnknownDistribution,
        RandomDistribution,
        EvenDistribution
    };

private:
    Ui::PopulateDialogClass ui;

private slots:
    void on_pushButtonCancel_clicked();
    void on_pushButtonOK_clicked();
};

#endif // __POPULATEDIALOG_H_
