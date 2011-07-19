// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef __3DWIDGET_H_
#define __3DWIDGET_H_

#include <QObject>
#include <QString>
#include <QSize>
#include <QPoint>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QtOpenGL>
#include <QThread>
#include <QCloseEvent>

class Fitness;
class Swarm;
class TopoGraphWidget;

class ThreeDWidget : public QGLWidget
{
    Q_OBJECT

private: // variables
    TopoGraphWidget* topoGraph;
    int xRot;
    int yRot;
    double xTrans;
    double yTrans;
    double zTrans;
    double zoom;
    GLuint landscape;
    GLuint particles;
    QPoint lastPos;
    int landscapeType;
    int particleType;
    QImage textureImg;
    GLuint* textureHandle;
    bool lighting;

    static const int numHandles = 1;

private: // methods
    static void normalizeAngle(int& angle);
    void loadTexture(const QString& name, GLuint handle);

public: // variables
    enum LandscapeType {
        Points    = 0x01,
        Polygons  = 0x02,
        Mesh      = 0x04 };

    enum ParticleType {
        Dotty     = 0x01,
        Plush     = 0x04,
        Octaeder  = 0x08 };


public: // methods
    ThreeDWidget(QWidget* parent = 0);
    ~ThreeDWidget(void);

    void setGraph(TopoGraphWidget* g)
    {
        topoGraph = g;
    }

    void setLandscapeType(int type)
    {
        landscapeType = type;
    }

    void setParticleType(int type)
    {
        particleType = type;
    }

    QSize minimumSizeHint(void) const;
    QSize sizeHint(void) const;
    GLuint makeLandscape(void);
    GLuint makeParticles(void);

    void setXRotation(int);
    void setYRotation(int);

public slots:
    void particlesChanged(void);
    void fitnessChanged(void);
    void graphChanged(void);

protected: // methods
    void initializeGL(void);
    void resizeGL(int, int);
    void paintGL(void);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);
    void keyPressEvent(QKeyEvent*);
    void closeEvent(QCloseEvent*);

signals:
    void closed(void);
    void xRotChanged(int);
    void yRotChanged(int);

};

#endif // __3DWIDGET_H_
