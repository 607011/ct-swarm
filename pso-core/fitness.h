// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef __FITNESS_H_
#define __FITNESS_H_

#if defined(__INTEL_COMPILER)
#include <mathimf.h>
#else
#include <math.h>
#endif

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#include <emmintrin.h>
#endif

#include "defines.h"

#include <QObject>
#include <QList>

#include "abstractfitness.h"

struct FitnessParam
{
#ifdef USE_SSE2
    SSE_ALIGNED union {
        __m128d hw;
        struct {
            double h;
            double w;
        };
    };
    SSE_ALIGNED union {
        __m128d xy;
        struct {
            double X;
            double Y;
        };
    };
#else
    double h;
    double w;
    double X;
    double Y;
#endif
    FitnessParam(double H, double W, double x, double y)
        : h(H), w(W), X(x), Y(y)
    {}
    FitnessParam(void)
    {}
};


class Fitness : public AbstractFitness<QPointF, double>
{
private: // variables
    QList<FitnessParam> param;

public: // methods
    /// Constructor.
    Fitness(void);

    /// This function is provided for convenience.
    /// @see f(const QPointF& p)
    inline double operator() (const QPointF& p) const { return f(p); }
    double f(double x, double y) const;
    inline double f(int x, int y) const { return f((double) x, (double) y); }
    inline double f(const QPointF& p) const { return f(p.x(), p.y()); }

    void addPeak(double h, double w, double X, double Y);
    void deletePeak(double h, double w, double X, double Y);
    double estimatedMax(void) const;

    QList<FitnessParam>& params(void);

};

#endif
