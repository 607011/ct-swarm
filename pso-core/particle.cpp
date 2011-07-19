// Copyright (c) 2005-2008 Oliver Lau <ola@ctmagazin.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#if defined(__INTEL_COMPILER)
#include <mathimf.h>
#else
#include "math.h"
#endif

#include <QPointF>

#include "particle.h"
#include "fitness.h"
#include "swarm.h"


Particle::Particle(Swarm* swarm, const QPointF& p, bool rState)
: swarm(swarm), currentPos(p), bestPos(p), v(0, 0), m_randomization(rState)
{ }


void Particle::move(void)
{
    QPointF distraction =
        swarm->c1() * r() * (bestPos - currentPos) +
        swarm->c2() * r() * (swarm->best() - currentPos);
    v += distraction;
    double distance = sqrt(v.x() * v.x() + v.y() * v.y());
    if (distance > swarm->vMax())
        v = v / distance * swarm->vMax();
    if (z() > swarm->fitness()->f(bestPos))
        bestPos = currentPos;
    currentPos += swarm->w() * v;
}


double Particle::z(void) const
{
    return swarm->fitness()->f(currentPos);
}


void Particle::place(const QPointF& p)
{
    currentPos = p;
}


const QPointF& Particle::pos(void) const
{
    return currentPos;
}


double Particle::r(void)
{
    return (m_randomization)
        ? (double) rand() / (double) RAND_MAX
        : 1.0;
}
