// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#include <QLocale>
#include <QApplication>
#include <QMessageBox>
#include <QDialog>
#include <QVariant>
#include <QDockWidget>
#include <QIcon>

#include <particle.h>
#include <swarm.h>
#include <fitness.h>

#include "mainwindow.h"
#include "pso.h"
#include "topographwidget.h"
#include "3dwidget.h"
#include "helpviewer.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    // create widgets
    pso = new PSO(this);

    populateDialog = new PopulateDialog();
    populateDialog->setModal(true);
    populateDialog->setSizeGripEnabled(false);

    // read settings
    settings = new QSettings("PSO.ini", QSettings::IniFormat);

    settings->beginGroup("mainwindow");
    resize(settings->value("size", QSize(512, 478)).toSize());
    move(settings->value("pos", QPoint(200, 200)).toPoint());
    settings->endGroup();

    settings->beginGroup("swarm");
    double w = settings->value("w", 1.0).toDouble();
    if (w < 0.0 || w > 2.0)
        w = 1.0;
    pso->setW(w);
    double c1 = settings->value("c1", 2.0).toDouble();
    if (c1 < 0.0 || c1 > 10.0)
        c1 = 2.0;
    pso->setC1(c1);
    double c2 = settings->value("c2", 2.0).toDouble();
    if (c2 < 0.0 || c2 > 10.0)
        c2 = 2.0;
    pso->setC2(c2);
    double vmax = settings->value("vmax", 20.0).toDouble();
    if (vmax <= 0.0 || vmax > 1000.0)
        vmax = 20;
    pso->setVMax(vmax);
    settings->endGroup();

    int size = settings->beginReadArray("peak");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        double hh = settings->value("h").toDouble();
        double ww = settings->value("w").toDouble();
        double XX = settings->value("X").toDouble();
        double YY = settings->value("Y").toDouble();
        pso->graph()->fitness()->addPeak(hh, ww, XX, YY);
    }
    settings->endArray();

    settings->beginGroup("population");
    populateDialog->setNumber(settings->value("number").toInt());
    populateDialog->setDistribution(settings->value("distribution").toInt());
    settings->endGroup();

    // create actions
    newAct = new QAction(QIcon(":/Resources/filenew.png"), tr("&New"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    QObject::connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+X"));
    QObject::connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    view3DAct = new QAction(tr("3D"), this);
    view3DAct->setCheckable(true);
    view3DAct->setChecked(true);
    QObject::connect(view3DAct, SIGNAL(triggered(bool)), this, SLOT(view3D(bool)));

    populateAct = new QAction(QIcon(":/Resources/populate.png"), tr("&Populate"), this);
    populateAct->setShortcut(tr("Ctrl+P"));
    QObject::connect(populateAct, SIGNAL(triggered()), this, SLOT(populate()));

    aboutAct = new QAction(QIcon(":/Resources/info.png"), tr("&About"), this);
    aboutAct->setShortcut(tr("Ctrl+A"));
    QObject::connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    helpAct = new QAction(tr("&Help"), this);
    helpAct->setShortcut(tr("F1"));
    QObject::connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));

    bearModeAct = new QAction(QIcon(":/Resources/particles.png"), tr("Create particles"), this);
    bearModeAct->setCheckable(true);
    bearModeAct->setChecked(true);

    QObject::connect(bearModeAct, SIGNAL(triggered(bool)), pso->graph(), SLOT(slot_enterBearMode(bool)));
    QObject::connect(bearModeAct, SIGNAL(triggered(bool)), this, SLOT(enterBearMode(bool)));

    buildModeAct = new QAction(QIcon(":/Resources/plus.png"), tr("Build peaks"), this);
    buildModeAct->setCheckable(true);
    QObject::connect(buildModeAct, SIGNAL(triggered(bool)), pso->graph(), SLOT(slot_enterBuildMode(bool)));
    QObject::connect(buildModeAct, SIGNAL(triggered(bool)), this, SLOT(enterBuildMode(bool)));

    destructModeAct = new QAction(QIcon(":/Resources/minus.png"), tr("Remove peaks"), this);
    destructModeAct->setCheckable(true);
    QObject::connect(destructModeAct, SIGNAL(triggered(bool)), pso->graph(), SLOT(slot_enterDestructMode(bool)));
    QObject::connect(destructModeAct, SIGNAL(triggered(bool)), this, SLOT(enterDestructMode(bool)));

    // create toolbars
    drawingTools = addToolBar(tr("Drawing Tools"));
    drawingTools->addAction(bearModeAct);
    drawingTools->addAction(buildModeAct);
    drawingTools->addAction(destructModeAct);

    toolButtons = new QButtonGroup();

    // create menus
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(populateAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(view3DAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(helpAct);

    setWindowTitle(tr("Particle Swarm Optimization"));
    setCentralWidget(pso);

    swarmChanged = false;
    graphChanged = false;

    // create 3D widget
    gl = new ThreeDWidget();
    gl->setWindowTitle(tr("PSO 3D View"));
    gl->resize(gl->sizeHint());
    gl->setMinimumSize(gl->minimumSizeHint());
    gl->setGraph(pso->graph());
    QObject::connect(pso->graph(), SIGNAL(graphChanged()), gl, SLOT(graphChanged()));
    QObject::connect(pso->graph(), SIGNAL(graphChanged()), gl, SLOT(graphChanged()));
    QObject::connect(gl, SIGNAL(closed()), this, SLOT(threeDWidgetClosed()));

    pso->set3DWidget(gl);

    // create signal/slot connections
    QObject::connect(this, SIGNAL(numberOfParticlesChanged(int)), pso, SLOT(slotNumberOfParticlesChanged(int)));
}


void MainWindow::closeEvent(QCloseEvent* e)
{
    if (maybeSave())
        writeSettings();
    gl->close();
    e->accept();
}


void MainWindow::showEvent(QShowEvent*)
{
    if (view3DAct->isChecked())
        gl->show();
}


void MainWindow::writeSettings(void)
{
    settings->beginGroup("swarm");
    settings->setValue("vmax", pso->graph()->swarm()->vMax());
    settings->setValue("w", pso->graph()->swarm()->w());
    settings->setValue("c1", pso->graph()->swarm()->c1());
    settings->setValue("c2", pso->graph()->swarm()->c2());
    settings->endGroup();

    settings->beginGroup("population");
    settings->setValue("number", populateDialog->number());
    settings->setValue("distribution", populateDialog->distributionIndex());
    settings->endGroup();

    settings->beginGroup("mainwindow");
    settings->setValue("size", size());
    settings->setValue("pos", pos());
    settings->endGroup();

    settings->beginWriteArray("peak");
    for (int i = 0; i < pso->graph()->fitness()->params().count(); ++i) {
        const FitnessParam p = pso->graph()->fitness()->params().at(i);
        settings->setArrayIndex(i);
        settings->setValue("h", p.h);
        settings->setValue("w", p.w);
        settings->setValue("X", p.X);
        settings->setValue("Y", p.Y);
    }
    settings->endArray();
}


bool MainWindow::maybeSave(void)
{
    if (graphChanged || swarmChanged) {
        return QMessageBox::question(0, tr("Save changes?"),
                                     tr("Your environment has changed.\n"
                                        "Do you want to save the changes before continuing?"),
                                     QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes;
    }
    return false;
}


void MainWindow::newFile(void)
{
    if (maybeSave())
        writeSettings();
    pso->graph()->fitness()->params().clear();
    pso->resetButtonClicked();
    pso->graph()->regenerate();
    graphChanged = true;
    swarmChanged = true;
}


void MainWindow::view3D(bool checked)
{
    if (checked)
        gl->show();
    else
        gl->hide();
}


void MainWindow::threeDWidgetClosed(void)
{
    view3DAct->setChecked(false);
}


void MainWindow::about(void)
{
    QMessageBox::about(this, tr("About Particle Swarm Optimization"),
                       tr("<p>The <strong>Particle&nbsp;Swarm&nbsp;Optimization</strong> example demonstrates how a swarm of particles behaves finding the global peak on a 3D surface.</p>"
                          "<p>Copyright (c)&nbsp;2005-201 Oliver Lau &lt;<a href=\"mailto:ola@ctmagazin.de\">ola@ct.de</a>&gt;, Heise Zeitschriften Verlag. All rights reserved.</p>"));
}


void MainWindow::help(void)
{
    HelpViewer helpViewer;
    helpViewer.resize(helpViewer.sizeHint());
    helpViewer.setMinimumSize(helpViewer.minimumSizeHint());
    helpViewer.exec();
}


void MainWindow::populate(void)
{
    if (populateDialog->exec() == QDialog::Accepted) {
        const int n = populateDialog->number();
        switch (populateDialog->distribution()) {
        case PopulateDialog::RandomDistribution:
            pso->graph()->swarm()->beginUpdate();
            for (int i = 0; i < n; ++i) {
                const double x = (double)qrand() / (double)RAND_MAX * pso->graph()->width();
                const double y = (double)qrand() / (double)RAND_MAX * pso->graph()->height();
                pso->graph()->swarm()->add(new Particle(pso->graph()->swarm(), QPointF(x, y), pso->randomization()));
            }
            pso->graph()->swarm()->endUpdate();
            swarmChanged = true;
            emit numberOfParticlesChanged(pso->graph()->swarm()->size());
            break;
		case PopulateDialog::EvenDistribution:
            pso->graph()->swarm()->beginUpdate();
            {
                const double width = (double) pso->graph()->width();
                const double height = (double) pso->graph()->height();
                const double nsqrt = sqrt((double) n) - 1;
                const double xstep = width / nsqrt;
                const double ystep = height / nsqrt;
                for (double x = 0; x <= width; x += xstep) {
                    for (double y = 0; y <= height; y += ystep) {
                        pso->graph()->swarm()->add(new Particle(pso->graph()->swarm(), QPointF(x, y), pso->randomization()));
                    }
                }
            }
            pso->graph()->swarm()->endUpdate();
            swarmChanged = true;
            emit numberOfParticlesChanged(pso->graph()->swarm()->size());
            break;
		case PopulateDialog::UnknownDistribution:
            // fall-through
		default:
            qDebug("Oops! Should've never have gone here.");
            break;
        }
    }
}


void MainWindow::fitnessChanged(void)
{
    graphChanged = true;
}


void MainWindow::enterBearMode(bool really)
{
    if (really) {
        buildModeAct->setChecked(false);
        destructModeAct->setChecked(false);
    }
    else {
        bearModeAct->setChecked(true);
    }
}


void MainWindow::enterBuildMode(bool really)
{
    if (really) {
        bearModeAct->setChecked(false);
        destructModeAct->setChecked(false);
    }
    else {
        buildModeAct->setChecked(true);
    }
}

void MainWindow::enterDestructMode(bool really)
{
    if (really) {
        bearModeAct->setChecked(false);
        buildModeAct->setChecked(false);
    }
    else {
        destructModeAct->setChecked(true);
    }
}
