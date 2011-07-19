// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#if defined (_OPENMP)
#include <omp.h>
#else
// disable warning "unknown pragma"
#pragma warning (disable:4068)
#endif

#if defined(__INTEL_COMPILER)
#include <mathimf.h>
#else
#include <math.h>
#endif

#include <QtGlobal>
#include <QWidget>
#include <QPainter>
#include <QPointF>

#include <fitness.h>
#include <swarm.h>

#include "topographwidget.h"
#include "zfinder.h"


TopoGraphWidget::TopoGraphWidget(QWidget* parent) : QWidget(parent)
{
    setMouseTracking(true);
    forceGenerateGraph = false;
    randomizationState = false;
    peakHasFocus = false;
    drawMode = TopoGraphWidget::CreateParticles;
    buildPeaksMode = TopoGraphWidget::Pending;
    m_fitness = new Fitness();
    m_swarm = new Swarm(m_fitness);
    zfinder = new ZFinder();
    zfinder->setMinimumSize(zfinder->minimumSizeHint());
    zfinder->setModal(true);
    zfinder->setSizeGripEnabled(false);
    zfinder->resize(zfinder->sizeHint());
}


TopoGraphWidget::~TopoGraphWidget()
{
    if (m_swarm)
        delete m_swarm;
    if (m_fitness)
        delete m_fitness;
    if (zfinder)
        delete zfinder;
}


void TopoGraphWidget::paintEvent(QPaintEvent*)
{
    generateGraph();
    QPainter p(this);
    p.drawImage(0, 0, img);
    p.setPen(QColor(240, 16, 16));
    p.setBrush(QColor(240, 16, 16));
    const ParticleList& particles = m_swarm->particles();
    const int n = particles.size();
    for (int i = 0; i < n; ++i) {
        if (particles.at(i)->randomization()) {
            p.setBrush(QColor(16, 16, 240));
            p.setPen(QColor(16, 16, 240));
        }
        else {
            p.setBrush(QColor(240, 16, 16));
            p.setPen(QColor(240, 16, 16));
        }
        p.drawEllipse((int) particles.at(i)->pos().x() - ParticleRadius / 2,
                      (int) particles.at(i)->pos().y() - ParticleRadius / 2,
                      ParticleRadius, ParticleRadius);
    }
    if (buildPeaksMode == TopoGraphWidget::CenterSelected) {
        p.setPen(QColor(16, 16, 224));
        p.drawLine(newPeak, newPeakRadius);
    }
    if (peakHasFocus) {
        p.setPen(QColor(255, 255, 24));
        p.setBrush(QColor(255, 255, 24, 96));
        p.drawEllipse((int) snappedPeak.X - 6, (int) snappedPeak.Y - 6, 12, 12);
    }
}


void TopoGraphWidget::keyPressEvent(QKeyEvent* e)
{
    switch (e->key())
    {
    case Qt::Key_Escape:
        if (drawMode == TopoGraphWidget::BuildPeaks) {
            buildPeaksMode = TopoGraphWidget::Pending;
            drawMode = TopoGraphWidget::BuildPeaks;
            update();
        }
        break;
    default:
        break;
    }
}


void TopoGraphWidget::resizeEvent(QResizeEvent*)
{
    emit graphChanged();
}


void TopoGraphWidget::mouseMoveEvent(QMouseEvent* e)
{
    switch (drawMode)
    {
    case TopoGraphWidget::BuildPeaks:
        if (buildPeaksMode == TopoGraphWidget::CenterSelected) {
            newPeakRadius = QPointF((double) e->x(), (double) e->y());
            update();
        }
        break;
	case TopoGraphWidget::DestructPeaks:
        {
            const double snapDistance = 12;
            for (int i = 0; i < m_fitness->params().count(); ++i) {
                FitnessParam p = m_fitness->params().at(i);
                bool xsnap = (e->x() > p.X - snapDistance) && (e->x() < p.X + snapDistance);
                bool ysnap = (e->y() > p.Y - snapDistance) && (e->y() < p.Y + snapDistance);
                if (xsnap && ysnap) {
                    snappedPeak = p;
                    peakHasFocus = true;
                    break;
                }
                else {
                    peakHasFocus = false;
                }
            }
            update();
        }
        break;
	default:
        break;
    }
}


void TopoGraphWidget::mousePressEvent(QMouseEvent* e)
{
    switch (drawMode)
    {
    case TopoGraphWidget::BuildPeaks:
        if ((e->button() & Qt::LeftButton) == Qt::LeftButton) {
            switch (buildPeaksMode)
            {
            case TopoGraphWidget::Pending:
                newPeak = QPointF((double) e->x(), (double) e->y());
                newPeakRadius = newPeak;
                buildPeaksMode = TopoGraphWidget::CenterSelected;
                break;
            case TopoGraphWidget::CenterSelected:
                if (zfinder->exec() == QDialog::Accepted) {
                    const double z = zfinder->value();
                    QPointF arc = newPeakRadius - newPeak;
                    double radius = sqrt(arc.x() * arc.x() + arc.y() * arc.y());
                    m_fitness->addPeak(z, -99 * radius, newPeak.x(), newPeak.y());
                    emit graphChanged();
                    regenerate();
                }
                buildPeaksMode = TopoGraphWidget::Pending;
                break;
            default:
                qDebug("switch (buildPeaksMode) >> Oops! Should never gone here.");
                break;
            }
        }
        break;
    case TopoGraphWidget::DestructPeaks:
        if (peakHasFocus) {
            m_fitness->deletePeak(snappedPeak.h, snappedPeak.w, snappedPeak.X, snappedPeak.Y);
            peakHasFocus = false;
            emit graphChanged();
            regenerate();
        }
        break;
	case TopoGraphWidget::CreateParticles:
        if ((e->button() & Qt::LeftButton) == Qt::LeftButton) {
            m_swarm->add(new Particle(m_swarm, QPointF((double) e->x(), (double) e->y()), randomizationState));
            emit numberOfParticlesChanged(m_swarm->size());
        }
        else if ((e->button() & Qt::RightButton) == Qt::RightButton) {
            const double scatter = 16;
            const int n = 20;
            QPointF orig((double) e->x(), (double) e->y());
            m_swarm->beginUpdate();
            for (int i = 0; i < n; ++i) {
                double d = r1() * 2 * 3.14159265;
                double r = r1() * scatter;
                QPointF offset(r * sin(d), r * cos(d));
                m_swarm->add(new Particle(m_swarm, orig + offset, randomizationState));
            }
            m_swarm->endUpdate();
            emit numberOfParticlesChanged(m_swarm->size());
        }
        update();
        break;
	case TopoGraphWidget::DoNothing:
            // fall-through
	default:
            qDebug("switch (drawMode) >> Oops! Should've never gone here.");
        break;
    }
}


void TopoGraphWidget::slot_enterBearMode(bool really)
{
    if (really) {
        drawMode = TopoGraphWidget::CreateParticles;
        setCursor(Qt::ArrowCursor);
    }
}


void TopoGraphWidget::slot_enterBuildMode(bool really)
{
    if (really) {
        drawMode = TopoGraphWidget::BuildPeaks;
        setCursor(Qt::CrossCursor);
    }
}


void TopoGraphWidget::slot_enterDestructMode(bool really)
{
    if (really) {
        drawMode = TopoGraphWidget::DestructPeaks;
        peakHasFocus = false;
        setCursor(Qt::CrossCursor);
    }
}


void TopoGraphWidget::generateGraph(void)
{
    if (forceGenerateGraph || width() > prevSize.width() || height() > prevSize.height())
    {
        forceGenerateGraph = false;
        prevSize = size();
        img = QImage(size(), QImage::Format_RGB32);
        const double norm = 256 / m_fitness->estimatedMax();
        const int w = width();
        const int h = height();
#pragma omp parallel for
        for (int x = 0; x < w; ++x) {
            for (int y = 0; y < h; ++y) {
                const double z = m_fitness->f(x, y);
                int c = 255 - (int) (z * norm);
                if (c < 0)   c = 0;
                if (c > 255) c = 255;
                img.setPixel(x, y, c + (c << 8) + (c << 16));
            }
        }
    }
}


void TopoGraphWidget::regenerate(void)
{
    forceGenerateGraph = true;
    update();
}


void TopoGraphWidget::slot_wChanged(double w)
{
    m_swarm->setW(w);
}


void TopoGraphWidget::slot_vMaxChanged(double vmax)
{
    m_swarm->setVmax(vmax);
}


void TopoGraphWidget::slot_c1Changed(double c1)
{
    m_swarm->setC1(c1);
}


void TopoGraphWidget::slot_c2Changed(double c2)
{
    m_swarm->setC2(c2);
}
