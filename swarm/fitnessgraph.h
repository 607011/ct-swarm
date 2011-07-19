// Copyright (c) 2005-2008 Oliver Lau <ola@ctmagazin.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef __FITNESSGRAPH_H_
#define __FITNESSGRAPH_H_

#include <QWidget>
#include <QQueue>

typedef QQueue< double > FitnessResults;

class FitnessGraphWidget : public QWidget
{
    Q_OBJECT

private: // variables
    FitnessResults results;
    double current;
    double firstResult;
    double firstDiff;
    double percentage;

public: // methods
    FitnessGraphWidget(QWidget* parent = 0);
    void add(double);
    void reset(void);
    bool stagnating(void);
    double error(void);

protected: // methods
    void paintEvent(QPaintEvent*);

};

#endif // __FITNESSGRAPH_H_
