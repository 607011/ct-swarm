// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef __ABSTRACTFITNESS_H_
#define __ABSTRACTFITNESS_H_

#include <QObject>
#include <QPointF>

#include "defines.h"

template <typename INPUTT, typename OUTPUTT>
        class AbstractFitness
{
public:
    AbstractFitness(void) { /* ... */ }
    virtual ~AbstractFitness(void) { /* ... */ }
    virtual OUTPUTT f(const INPUTT& p) const = 0;
    virtual OUTPUTT operator()(const INPUTT& p) const = 0;
};

#endif
