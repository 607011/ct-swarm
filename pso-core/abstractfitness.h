// Copyright (c) 2005-2008 Oliver Lau <ola@ctmagazin.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef _ABSTRACTFITNESS_H_
#define _ABSTRACTFITNESS_H_

#include <QObject>
#include <QPointF>

#include "defines.h"

template <typename INPUTT, typename OUTPUTT>
class AbstractFitness
{
public:
	AbstractFitness(void) {};
	virtual ~AbstractFitness(void) {};
	virtual OUTPUTT f(const INPUTT& p) const = 0;
    virtual OUTPUTT operator()(const INPUTT& p) const = 0;
};

#endif
