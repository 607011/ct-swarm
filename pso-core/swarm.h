// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef __SWARM_H_
#define __SWARM_H_

#include "defines.h"
#include "particle.h"

#include <QString>
#include <QObject>
#include <QList>
#include <QPointF>

class Fitness;

typedef QList< Particle*> ParticleList;

class Swarm : public QObject
{
private:
    Fitness* f;
    ParticleList swarm;
    bool virgin;
    bool updating;
    double Vmax;     // maximum velocity
    double W;        // inertia weight: determines the influence of the old velocity
    double C1;       // acceleration coefficient: determines the influence of the local best position
    double C2;       // acceleration coefficient: determines the influence of the global best position
    QPointF m_best;  // position of best particle

public:
    Swarm(Fitness* f)
        : f(f), swarm(), virgin(true), updating(false), Vmax(20.0), W(1.0), C1(2.0), C2(2.0) { }

    Swarm(Fitness* f, ParticleList& swarm)	
        : f(f), swarm(swarm), virgin(true), updating(false), Vmax(20.0), W(1.0), C1(2.0), C2(2.0) { }

    void add(Particle* p);

    inline double c1(void) const { return C1; }
    inline double c2(void) const { return C2; }
    inline double w(void) const { return W; }
    inline double vMax(void) const { return Vmax; }

    inline void setC1(double c1) { C1 = c1; }
    inline void setC2(double c2) { C2 = c2; }
    inline void setW(double w) { W = w; }
    inline void setVmax(double vmax) { Vmax = vmax; }

    inline Fitness* fitness(void) const { return f; }
    inline const ParticleList& particles(void) const { return swarm; }
    inline int size(void) const { return swarm.count(); }
    inline const QPointF& best(void) const { return m_best; }
    inline void beginUpdate(void) { updating = true; }
    inline void endUpdate(void) { updating = false; }

    void findBest(void);
    void iterate(void);
    void kill(void);

};

#endif
