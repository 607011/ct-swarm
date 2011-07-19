// Copyright (c) 2005-2008 Oliver Lau <ola@ctmagazin.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#if defined(__INTEL_COMPILER)
#include <mathimf.h>
#else
#include <math.h>
#endif

#include <float.h>

#include <QSizePolicy>
#include <QtDebug>

#include <swarm.h>
#include <particle.h>
#include <fitness.h>

#include "pso.h"
#include "topographwidget.h"
#include "3dwidget.h"
#include "fitnessgraph.h"


PSO::PSO(QWidget* parent) : QWidget(parent), threeDWidget(NULL)
{
    prevBest = -DBL_MAX;
    running = false;
    iterations = 0;
    timer = new QTimer(this);
    timer->setSingleShot(false);

    // create widgets
    pushButtonGo = new QPushButton();
    pushButtonGo->setText(tr("Start"));
    pushButtonGo->setMinimumSize(64, 24);
    pushButtonGo->setMaximumSize(64, 24);

    pushButtonReset = new QPushButton();
    pushButtonReset->setText(tr("Clear"));
    pushButtonReset->setMinimumSize(64, 24);
    pushButtonReset->setMaximumSize(64, 24);

    QLabel* labelNumParticles = new QLabel(tr("Particles"));

    lineEditNumParticles = new QLineEdit();
    lineEditNumParticles->setMinimumWidth(48);
    lineEditNumParticles->setMaximumWidth(200);
    lineEditNumParticles->setAlignment(Qt::AlignRight);
    lineEditNumParticles->setEnabled(false);

    QLabel* labelIterations = new QLabel(tr("Iterations"));

    lineEditIterations = new QLineEdit();
    lineEditIterations->setMinimumWidth(48);
    lineEditIterations->setMaximumWidth(200);
    lineEditIterations->setAlignment(Qt::AlignRight);
    lineEditIterations->setEnabled(false);

    QLabel* labelBest = new QLabel(tr("Best Particle (X,Y)"));

    lineEditBestX = new QLineEdit();
    lineEditBestX->setMinimumWidth(48);
    lineEditBestX->setMaximumWidth(200);
    lineEditBestX->setAlignment(Qt::AlignRight);
    lineEditBestX->setEnabled(false);

    lineEditBestY = new QLineEdit();
    lineEditBestY->setMinimumWidth(48);
    lineEditBestY->setMaximumWidth(200);
    lineEditBestY->setAlignment(Qt::AlignRight);
    lineEditBestY->setEnabled(false);

    QLabel* labelFitness = new QLabel(tr("Fitness"));

    QLabel* labelW = new QLabel(tr("W"));

    spinBoxW = new QDoubleSpinBox();
    spinBoxW->setRange(0, 2);
    spinBoxW->setDecimals(3);
    spinBoxW->setSingleStep(0.1);

    QLabel* labelVmax = new QLabel(tr("Vmax"));

    spinBoxVmax = new QDoubleSpinBox();
    spinBoxVmax->setRange(0.1, 100);
    spinBoxVmax->setDecimals(1);
    spinBoxVmax->setSingleStep(0.5);

    QLabel* labelC1 = new QLabel(tr("C1"));

    spinBoxC1 = new QDoubleSpinBox();
    spinBoxC1->setRange(0, 10);
    spinBoxC1->setDecimals(1);
    spinBoxC1->setSingleStep(0.5);

    QLabel* labelC2 = new QLabel(tr("C2"));

    spinBoxC2 = new QDoubleSpinBox();
    spinBoxC2->setRange(0, 10);
    spinBoxC2->setDecimals(1);
    spinBoxC2->setSingleStep(0.5);

    QLabel* labelSpeed = new QLabel(tr("Speed"));

    sliderT = new QSlider();
    sliderT->setRange(20, 1000);
    sliderT->setTickInterval(100);
    sliderT->setTickPosition(QSlider::TicksLeft);
    sliderT->setBaseSize(32, 128);
    sliderT->setMaximumHeight(100);

    speedLCD = new QLCDNumber(4);
    speedLCD->setBaseSize(64, 24);
    speedLCD->setMaximumSize(64, 24);
    speedLCD->setSegmentStyle(QLCDNumber::Filled);
    speedLCD->setFrameStyle(QFrame::NoFrame);

    topoGraph = new TopoGraphWidget();
    topoGraph->setFocus();
    topoGraph->setFocusPolicy(Qt::StrongFocus);
    topoGraph->setMinimumSize(QSize(400, 400));
    topoGraph->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    fitnessGraph = new FitnessGraphWidget();
    fitnessGraph->setMinimumSize(QSize(100, 100));
    fitnessGraph->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    checkBoxStopOnStagnation = new QCheckBox(tr("Stop on stagnation"));
    checkBoxStopOnStagnation->setChecked(false);

    checkBoxFullAuto = new QCheckBox(tr("Fully automatic"));
    checkBoxFullAuto->setChecked(true);

    checkBoxRandomization = new QCheckBox(tr("Randomize movement"));
    QObject::connect(checkBoxRandomization, SIGNAL(stateChanged(int)), this, SLOT(slotRandomizationStateChanged(int)));
    checkBoxRandomization->setChecked(true);

    // create layouts
    QGridLayout* gridLayoutMain = new QGridLayout(this);
    gridLayoutMain->setColumnStretch(0, 10);
    gridLayoutMain->setColumnStretch(1,  1);

    QGridLayout* gridLayoutParams = new QGridLayout();
    QGridLayout* gridLayoutResults = new QGridLayout();

    QGridLayout* gridLayoutSpeed = new QGridLayout();
    gridLayoutSpeed->setMargin(0);

    QHBoxLayout* hboxLayoutButtons = new QHBoxLayout();

    QVBoxLayout* vboxLayoutRight = new QVBoxLayout();

    // fill layouts
    gridLayoutParams->addWidget(labelW, 0, 0, Qt::AlignTop);
    gridLayoutParams->addWidget(spinBoxW, 0, 1, Qt::AlignTop);
    gridLayoutParams->addWidget(labelVmax, 1, 0, Qt::AlignTop);
    gridLayoutParams->addWidget(spinBoxVmax, 1, 1, Qt::AlignTop);
    gridLayoutParams->addWidget(labelC1, 2, 0, Qt::AlignTop);
    gridLayoutParams->addWidget(spinBoxC1, 2, 1, Qt::AlignTop);
    gridLayoutParams->addWidget(labelC2, 3, 0, Qt::AlignTop);
    gridLayoutParams->addWidget(spinBoxC2, 3, 1, Qt::AlignTop);

    gridLayoutSpeed->addWidget(labelSpeed, 0, 0, 1, 1, Qt::AlignTop);
    gridLayoutSpeed->addWidget(speedLCD, 1, 0, Qt::AlignTop);
    gridLayoutSpeed->addWidget(sliderT, 0, 1, 2, 1, Qt::AlignTop);

    hboxLayoutButtons->addWidget(pushButtonGo);
    hboxLayoutButtons->addWidget(pushButtonReset);

    gridLayoutResults->addWidget(labelNumParticles, 0, 0, Qt::AlignTop);
    gridLayoutResults->addWidget(lineEditNumParticles, 0, 1, Qt::AlignTop);
    gridLayoutResults->addWidget(labelIterations, 1, 0, Qt::AlignTop);
    gridLayoutResults->addWidget(lineEditIterations, 1, 1, Qt::AlignTop);
    gridLayoutResults->addWidget(labelBest, 2, 0, 1, 2, Qt::AlignTop);
    gridLayoutResults->addWidget(lineEditBestX, 3, 0, 1, 1, Qt::AlignTop);
    gridLayoutResults->addWidget(lineEditBestY, 3, 1, 1, 1, Qt::AlignTop);

    vboxLayoutRight->addLayout(gridLayoutParams);
    vboxLayoutRight->addLayout(gridLayoutSpeed);
    vboxLayoutRight->addLayout(hboxLayoutButtons);
    vboxLayoutRight->addStretch();
    vboxLayoutRight->addLayout(gridLayoutResults);
    vboxLayoutRight->addWidget(labelFitness);
    vboxLayoutRight->addWidget(fitnessGraph);
    vboxLayoutRight->addWidget(checkBoxStopOnStagnation);
    vboxLayoutRight->addWidget(checkBoxFullAuto);
    vboxLayoutRight->addWidget(checkBoxRandomization);

    gridLayoutMain->addWidget(topoGraph, 0, 0);
    gridLayoutMain->addLayout(vboxLayoutRight, 0, 1);

    // main connections
    QObject::connect(pushButtonGo, SIGNAL(clicked()), this, SLOT(toggleStartStop()));
    QObject::connect(sliderT, SIGNAL(valueChanged(int)), this, SLOT(slotTimerIntervalChanged(int)));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(iterate()));
    QObject::connect(pushButtonReset, SIGNAL(clicked()), this, SLOT(resetButtonClicked()));

    // TopoGraphWidget connections
    QObject::connect(spinBoxW, SIGNAL(valueChanged(double)), topoGraph, SLOT(slot_wChanged(double)));
    QObject::connect(spinBoxVmax, SIGNAL(valueChanged(double)), topoGraph, SLOT(slot_vMaxChanged(double)));
    QObject::connect(spinBoxC1, SIGNAL(valueChanged(double)), topoGraph, SLOT(slot_c1Changed(double)));
    QObject::connect(spinBoxC2, SIGNAL(valueChanged(double)), topoGraph, SLOT(slot_c2Changed(double)));

    QObject::connect(topoGraph, SIGNAL(numberOfParticlesChanged(int)), this, SLOT(slotNumberOfParticlesChanged(int)));

    sliderT->setValue(950);
}


void PSO::resetButtonClicked(void)
{
    iterations = 0;
    lineEditIterations->setText(QString("%0").arg(iterations));
    topoGraph->swarm()->kill();
    topoGraph->update();
    fitnessGraph->reset();
}


void PSO::setW(double w)
{
    spinBoxW->setValue(w);
}


void PSO::setVMax(double vmax)
{
    spinBoxVmax->setValue(vmax);
}


void PSO::setC1(double c1)
{ 
    spinBoxC1->setValue(c1);
}


void PSO::setC2(double c2)
{
    spinBoxC2->setValue(c2);
}


void PSO::toggleStartStop(void)
{
    if (running)
    {
        pushButtonReset->setDisabled(false);
        running = false;
        pushButtonGo->setText(tr("&Start"));
        timer->stop();
    }
    else
    {
        if (topoGraph->swarm()->particles().count() > 2)
        {
            pushButtonReset->setDisabled(true);
            running = true;
            pushButtonGo->setText(tr("&Stop"));
            timer->start();
        }
    }
}


void PSO::iterate(void)
{
    Swarm* s = topoGraph->swarm();
    s->iterate();
    topoGraph->update();
    ++iterations;
    double best = s->fitness()->f(s->best());
    prevBest = best;
    fitnessGraph->add(best);
    lineEditIterations->setText(QString("%0").arg(iterations));
    lineEditBestX->setText(QString("%L0").arg(s->best().x(), 0, 'g', 7));
    lineEditBestY->setText(QString("%L0").arg(s->best().y(), 0, 'g', 7));
    if (checkBoxStopOnStagnation->isChecked() && fitnessGraph->stagnating())
        toggleStartStop();
    if (checkBoxFullAuto->isChecked() && fitnessGraph->stagnating())
        setVMax(0.9 * spinBoxVmax->value());
    if (threeDWidget)
        threeDWidget->particlesChanged();
}


void PSO::slotTimerIntervalChanged(int v)
{
    timer->setInterval(sliderT->maximum() - v);
    speedLCD->display(v);
}


bool PSO::randomization(void) const
{
    return checkBoxRandomization->checkState() == Qt::Checked;
}


void PSO::slotRandomizationStateChanged(int state)
{
    const ParticleList& particles = topoGraph->swarm()->particles();
    switch (state)
    {
    case Qt::Checked:
        topoGraph->setRandomizationState(true);
        for (int i = 0; i < particles.size(); ++i)
            particles.at(i)->setRandomization(true);
        topoGraph->update();
        break;
    case Qt::Unchecked:
        topoGraph->setRandomizationState(false);
        for (int i = 0; i < particles.size(); ++i)
            particles.at(i)->setRandomization(false);
        topoGraph->update();
        break;
    default:
        // ignore
        break;
    }
}


void PSO::slotNumberOfParticlesChanged(int n)
{
    lineEditNumParticles->setText(QString("%0").arg(n));
}


void PSO::set3DWidget(ThreeDWidget* w)
{
    threeDWidget = w;
}


TopoGraphWidget* PSO::graph(void) const
{
    return topoGraph;
}
