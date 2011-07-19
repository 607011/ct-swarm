// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#include "fitness.h"


Fitness::Fitness(void)
{
    /* ... */
}


void Fitness::addPeak(double h, double w, double X, double Y)
{
    param.append(FitnessParam(h, w, X, Y));
}


void Fitness::deletePeak(double h, double w, double X, double Y)
{
    for (int i = 0; i < param.count(); ++i) {
        const FitnessParam& fp = param.at(i);
        if (fp.X == X && fp.Y == Y && fp.h == h && fp.w == w) {
            param.removeAt(i);
            break;
        }
    }
}


double Fitness::f(double x, double y) const
{
    double sum = 0.0;
    for (int i = 0; i < param.count(); ++i) {
        const FitnessParam& fp = param.at(i);
        const double xd = x - fp.X;
        const double yd = y - fp.Y;
        sum += fp.h * exp((xd * xd + yd * yd) / fp.w);
    }
    return sum;
}


double Fitness::estimatedMax(void) const
{
    double maximum = 0.0;
    double peak;
    for (int i = 0; i < param.count(); ++i)
        if ((peak = param.at(i).h) > maximum)
            maximum = peak;
    return 1.1 * maximum;
}


QList<FitnessParam>& Fitness::params(void)
{
    return param;
}
