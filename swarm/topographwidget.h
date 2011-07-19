// Copyright (c) 2005-2008 Oliver Lau <ola@ctmagazin.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef __TOPOGRAPH_H_
#define __TOPOGRAPH_H_

#include <QObject>
#include <QWidget>
#include <QImage>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QSize>
#include <QTimer>

#include <fitness.h>

class Swarm;
class ZFinder;


class TopoGraphWidget : public QWidget
{
	Q_OBJECT

private: // variables
	static const int ParticleRadius = 4;
	QImage img;
	Swarm* m_swarm;
	Fitness* m_fitness;
	QSize prevSize;
	QTimer* timer;
	ZFinder* zfinder;
    bool randomizationState;
	bool forceGenerateGraph;
	void generateGraph(void);
	int drawMode;
	QPointF newPeak;
	QPointF newPeakRadius;
	FitnessParam snappedPeak;
	bool peakHasFocus;
	enum BuildPeaksMode
    {
		Pending,
		CenterSelected,
		RadiusSelected
	} buildPeaksMode;

private: // methods
    double r1(void)
    {
        return (double) rand() / (double) RAND_MAX;
    }

protected: // methods
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	void resizeEvent(QResizeEvent*);

public: // variables
	enum DrawMode
    {
		DoNothing,
		BuildPeaks,
		DestructPeaks,
		CreateParticles
	};


public: // methods
	TopoGraphWidget(QWidget* parent = 0);
    ~TopoGraphWidget();

    Swarm* swarm(void) const { return m_swarm; }
	Fitness* fitness(void) const { return m_fitness; }

    void setRandomizationState(bool rState)
    {
        randomizationState = rState;
    }
	
    void regenerate(void);
    QSize minimumSizeHint() const { return QSize(150, 150); }
    QSize sizeHint() const { return QSize(400, 400); }

public slots:
	void slot_wChanged(double);
	void slot_vMaxChanged(double);
	void slot_c1Changed(double);
	void slot_c2Changed(double);
	void slot_enterBearMode(bool);
	void slot_enterBuildMode(bool);
	void slot_enterDestructMode(bool);

signals:
	void graphChanged(void);
    void numberOfParticlesChanged(int);
};

#endif // __TOPOGRAPH_H_
