// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#include <QObject>

#include "populatedialog.h"


PopulateDialog::PopulateDialog(QWidget* parent) : QDialog(parent)
{
    ui.setupUi(this);
}


int PopulateDialog::distribution(void) const
{
    if (ui.comboBoxDistribution->currentText() == tr("Even"))
        return PopulateDialog::EvenDistribution;
    else if (ui.comboBoxDistribution->currentText() == tr("Random"))
        return PopulateDialog::RandomDistribution;
    return PopulateDialog::UnknownDistribution;
}


int PopulateDialog::number(void) const
{
    return ui.spinBoxNumber->value();
}


void PopulateDialog::setDistribution(int d)
{
    ui.comboBoxDistribution->setCurrentIndex(d);
}


void PopulateDialog::setNumber(int n)
{
    ui.spinBoxNumber->setValue(n);
}


int PopulateDialog::distributionIndex(void) const
{
    return ui.comboBoxDistribution->currentIndex();
}


void PopulateDialog::on_pushButtonOK_clicked()
{
    accept();
}


void PopulateDialog::on_pushButtonCancel_clicked()
{
    reject();
}
