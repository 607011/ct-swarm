// Copyright (c) 2005-2008 Oliver Lau <ola@ctmagazin.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#if defined (_OPENMP)
#include <omp.h>
#else
#pragma warning (disable:4068)
#endif

#if defined(__INTEL_COMPILER)
#include <mathimf.h>
#else
#include <math.h>
#endif

#include <float.h>

#include <QtDebug>
#include <QPainter>
#include <QColor>
#include <QRect>
#include <QPointF>

#include "fitnessgraph.h"


FitnessGraphWidget::FitnessGraphWidget(QWidget* parent) : QWidget(parent)
{
    current = -DBL_MAX;
    firstResult = DBL_MAX;
    firstDiff = DBL_MAX;
    percentage = 1e-10;
}


void FitnessGraphWidget::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setPen(Qt::black);
	p.setBrush(QColor(0, 0, 0, 144));
	p.drawRect(QRect(0, 0, width() - 1, height() - 1));
	p.setPen(QColor(32, 224, 32));
    const double h = (double) height();
	for (int x = 1; x < results.count() - 1; ++x)
	{
        int y = (int)(firstResult * h / results.at(x));
		p.drawLine(x, height() - 2, x, height() - y - 2);
	}
	p.setPen(QColor(255, 255, 32, 240));
	p.drawText(4, 4,
        width() - 8, 20,
        Qt::TextSingleLine | Qt::AlignRight,
        QString("%L0").arg(current, 0, 'g', 15));
}
 

void FitnessGraphWidget::add(double v)
{
    current = v;
	results.enqueue(v);
    if (results.count() > width()) {
		results.dequeue();
    }
    else if (results.count() == 2) {
        firstDiff = results.last() - results.first();
    }
    else if (results.count() == 1) {
        firstResult = v;
    }
	update();
}


void FitnessGraphWidget::reset(void)
{
	results.clear();
	update();
}


double FitnessGraphWidget::error(void)
{
    if (results.count() < 2)
        return DBL_MAX;
    double diff = results.last() - results.at(results.count() - 2);
    return fabs(diff / (results.last() - firstResult));
}


bool FitnessGraphWidget::stagnating(void)
{
    if (results.count() < 5)
        return false;
    const double cur = results.last();
    const double prev = results.at(results.count() - 2);
    const double currDiff = cur - prev;
    const double maxDiff = (cur - firstResult) * percentage * 1e-2;
    return currDiff < maxDiff;
}
