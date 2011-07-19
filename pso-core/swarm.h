// Copyright (c) 2005-2008 Oliver Lau <ola@ctmagazin.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef _SWARM_H_
#define _SWARM_H_

#include "defines.h"
#include "particle.h"

#include <QString>
#include <QObject>
#include <QList>
#include <QPointF>

class Fitness;

typedef QList< Particle* > ParticleList;

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

    ~Swarm() { }

	void add(Particle* p)
	{
		swarm.append(p);
		findBest();
	}

    double c1(void) { return C1; }
    double c2(void) { return C2; }
    double w(void) { return W; }
    double vMax(void) { return Vmax; }

    void setC1(double c1) { C1 = c1; }
    void setC2(double c2) { C2 = c2; }
    void setW(double w) { W = w; }
    void setVmax(double vmax) { Vmax = vmax; }

	Fitness* fitness(void) const
	{
		return f;
	}

	const ParticleList& particles(void) const
	{
		return swarm;
	}

    int size(void) const
    {
        return swarm.count();
    }

	const QPointF& best(void) const
	{
		return m_best;
	}

	void beginUpdate(void)
	{
		updating = true;
	}

	void endUpdate(void)
	{
		updating = false;
	}

	void findBest(void);
	void iterate(void);
	void kill(void);

};

#endif
