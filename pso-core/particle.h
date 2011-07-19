// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef __PARTICLE_H_
#define __PARTICLE_H_

#include <QPointF>

#include "defines.h"

class Swarm;

/// This class describes a Particle.
class Particle
{
private: // variables
    /// pointer to swarm the particle is schooling with
    Swarm* swarm;       
    /// current position
    QPointF currentPos; 
    /// best position
    QPointF bestPos;    
    /// current velocity vector
    QPointF v;          
    /// if true, velocity vector is distracted by a random amount at each iteration
    bool m_randomization; 

private: // methods
    /// return a random number between 0 and 1
    double r(void);

public: // methods
    /// constructor
    Particle(Swarm* swarm, const QPointF& p, bool rState);
    /// move particle to next position
    void move(void);
    /// return particle's current fitness
    double z(void) const;
    /// place particle at a certain position
    void place(const QPointF& p);
    /// return particle's position
    const QPointF& pos(void) const;
    /// enable/disable random distraction (@see m_randomization)
    void setRandomization(bool rState = true)
    {
        m_randomization = rState;
    }
    /// (@see m_randomization)
    bool randomization(void) const { return m_randomization; }
};

#endif
