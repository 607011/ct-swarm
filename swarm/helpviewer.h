// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef __HELPVIEWER_H_
#define __HELPVIEWER_H_

#include <QObject>
#include <QSize>
#include <QWidget>
#include <QTextEdit>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

class HelpViewer : public QDialog
{
    Q_OBJECT

private: // variables
    QTextEdit* textEdit;
    QPushButton* buttonOK;

public: // methods
    HelpViewer(QWidget* parent = NULL);
    ~HelpViewer();

    QSize sizeHint(void) const { return QSize(400, 600); }
    QSize minimumSizeHint(void) const { return QSize(300, 200); }
};

#endif // __HELPVIEWER_H_
