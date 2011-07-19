// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#if defined (_OPENMP)
#include <omp.h>
#else
/* disable warning "unknown pragma" */
#pragma warning (disable:4068)
#endif

#include <float.h>

#include "swarm.h"
#include "fitness.h"
#include "particle.h"


void Swarm::iterate(void)
{
    if (swarm.count() < 3)
        return;
    findBest();
    const int n = swarm.count();
#pragma omp parallel for
    for (int i = 0; i < n; ++i) 
        swarm.at(i)->move();
}


void Swarm::findBest(void)
{
    double globalBest;
    if (virgin) {
        globalBest = -DBL_MAX;
        virgin = false;
    }
    else {
        globalBest = f->f(m_best);
    }
    const int n = swarm.count();
    for (int i = 0; i < n; ++i) {
        Particle* const p = swarm.at(i);
        double possibleBest = p->z(); 
        if (possibleBest > globalBest) {
            globalBest = possibleBest;
            m_best = p->pos();
        }
    }
}


void Swarm::add(Particle* p)
{
    swarm.append(p);
    findBest();
}


void Swarm::kill(void)
{
    swarm.clear();
    virgin = true;
}
