// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef __PSO_H_
#define __PSO_H_

#include <QObject>
#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QLCDNumber>
#include <QButtonGroup>

class ThreeDWidget;
class TopoGraphWidget;
class FitnessGraphWidget;
class Particle;

class PSO : public QWidget
{
    Q_OBJECT

private: // variables
    QPushButton* pushButtonGo;
    QPushButton* pushButtonReset;
    QCheckBox* checkBoxStopOnStagnation;
    QCheckBox* checkBoxFullAuto;
    QCheckBox* checkBoxRandomization;
    QLineEdit* lineEditNumParticles;
    QLineEdit* lineEditIterations;
    QLineEdit* lineEditBestX;
    QLineEdit* lineEditBestY;
    QDoubleSpinBox* spinBoxVmax;
    QDoubleSpinBox* spinBoxW;
    QDoubleSpinBox* spinBoxC1;
    QDoubleSpinBox* spinBoxC2;
    QSlider* sliderT;
    QLCDNumber* speedLCD;
    TopoGraphWidget* topoGraph;
    FitnessGraphWidget* fitnessGraph;
    ThreeDWidget* threeDWidget;
    QTimer* timer;
    bool running;
    int iterations;
    double prevBest;

public:
    PSO(QWidget* parent = NULL);
    TopoGraphWidget* graph(void) const;
    void set3DWidget(ThreeDWidget* w);
    bool randomization(void) const;

public slots:
    void setW(double);
    void setVMax(double);
    void setC1(double);
    void setC2(double);
    void resetButtonClicked(void);
    void toggleStartStop(void);
    void iterate(void);
    void slotTimerIntervalChanged(int);
    void slotRandomizationStateChanged(int);
    void slotNumberOfParticlesChanged(int);
};

#endif // __PSO_H_
