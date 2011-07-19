// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#if defined (_OPENMP)
#include <omp.h>
#endif

#if defined(__INTEL_COMPILER)
#include <mathimf.h>
#else
#include <math.h>
#endif

#include <QColor>
#include <QImage>
#include <QPixmap>

#include <fitness.h>
#include <particle.h>
#include <swarm.h>

#include "3dwidget.h"
#include "topographwidget.h"

static const float light0_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
static const float light0_pos[]     = {0.0f, 1.0f, 1.0f, 0.0f};
static const float mat_shininess[]  = {8.0f};
static const float mat_specular[]   = {1.0f, 1.0f, 1.0f, 1.0f};


ThreeDWidget::ThreeDWidget(QWidget* parent) : QGLWidget(parent)
{
    topoGraph = NULL;
    zoom = -2.0;
    xRot = 0;
    yRot = 0;
    xTrans = 0.0;
    yTrans = 0.0;
    zTrans = 0.0;
    landscape = 0;
    particles = 0;
    landscapeType = Polygons;
    particleType  = Plush;
    textureHandle = new GLuint[numHandles];
    lighting = false;
}


ThreeDWidget::~ThreeDWidget(void)
{
    glDeleteLists(landscape, 1);
    glDeleteLists(particles, 1);
    glDeleteTextures(numHandles, textureHandle);
    delete [] textureHandle;
}


QSize ThreeDWidget::minimumSizeHint(void) const
{
    return QSize(200, 200);
}


QSize ThreeDWidget::sizeHint(void) const
{
    return QSize(750, 750);
}


void ThreeDWidget::loadTexture(const QString& name, GLuint handle)
{
    QPixmap bitmap(name);
    textureImg = bitmap.toImage();
    glBindTexture(GL_TEXTURE_2D, handle);
    switch (textureImg.depth()) {
    case 24:
        glTexImage2D(GL_TEXTURE_2D, 0, 3, textureImg.width(), textureImg.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, (const GLvoid *) textureImg.scanLine(0));
        break;
    case 32:
        glTexImage2D(GL_TEXTURE_2D, 0, 4, textureImg.width(), textureImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid *) textureImg.scanLine(0));
        break;
    default:
        qFatal("texture image has invalid depth: %d", textureImg.depth());
        break;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}


void ThreeDWidget::initializeGL(void)
{
    landscape = makeLandscape();
    particles = makeParticles();
    glGenTextures(numHandles, textureHandle);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);   
    loadTexture(":/Resources/Particle.bmp", textureHandle[0]);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glDisable(GL_TEXTURE_2D);
    glCullFace(GL_BACK);
}


void ThreeDWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.01, 5000.0);
    glMatrixMode(GL_MODELVIEW);
}


void ThreeDWidget::paintGL(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    const double nsqrt = sqrt((double) topoGraph->width() * (double) topoGraph->height());
    glTranslated(-0.0, -0.0, zoom * nsqrt);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glTranslated(- (double) topoGraph->width() / 2 + xTrans, - nsqrt / 2 + yTrans, - (double) topoGraph->height() / 2 + zTrans);
    if (lighting) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }
    else {
        glDisable(GL_LIGHTING);
    }
    glDisable(GL_BLEND);
    if (landscapeType == Mesh)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glCallList(landscape);
    if (landscapeType == Mesh)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (particleType == Plush) {
        glEnable(GL_TEXTURE_2D);
        particles = makeParticles();
        glCallList(particles);
        glDisable(GL_TEXTURE_2D);
    }
    else {
        glCallList(particles);
    }
}


void ThreeDWidget::keyPressEvent(QKeyEvent* e)
{
    double incrTrans = ((e->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier)? 1 : 10;
    switch(e->key()) {
    case Qt::Key_1:
        landscapeType = Points;
        landscape = makeLandscape();
        updateGL();
        break;
    case Qt::Key_2:
        landscapeType = Polygons;
        landscape = makeLandscape();
        updateGL();
        break;
    case Qt::Key_3:
        landscapeType = Mesh;
        landscape = makeLandscape();
        updateGL();
        break;
    case Qt::Key_Q:
        particleType = Dotty;
        particles = makeParticles();
        updateGL();
        break;
    case Qt::Key_W:
        particleType = Plush;
        particles = makeParticles();
        updateGL();
        break;
    case Qt::Key_E:
        particleType = Octaeder;
        particles = makeParticles();
        updateGL();
        break;
    case Qt::Key_Left:
        xTrans -= incrTrans;
        updateGL();
        break;
    case Qt::Key_Right:
        xTrans += incrTrans;
        updateGL();
        break;
    case Qt::Key_Down:
        yTrans -= incrTrans;
        updateGL();
        break;
    case Qt::Key_Up:
        yTrans += incrTrans;
        updateGL();
        break;
    case Qt::Key_PageDown:
        zTrans -= incrTrans;
        updateGL();
        break;
    case Qt::Key_PageUp:
        zTrans += incrTrans;
        updateGL();
        break;
    case Qt::Key_Escape:
        zoom = -2.0;
        xRot = 0;
        yRot = 0;
        xTrans = 0.0;
        yTrans = 0.0;
        zTrans = 0.0;
        updateGL();
        break;
    case Qt::Key_Return:
        lighting = !lighting;
        updateGL();
        break;
    default:
        break;
    }
}


void ThreeDWidget::mousePressEvent(QMouseEvent* e)
{
    lastPos = e->pos();
}


void ThreeDWidget::wheelEvent(QWheelEvent* e)
{
    double increment = ((e->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier)
                       ? 0.025 : 0.2;
    if (e->delta() < 0)
        zoom -= increment;
    else
        zoom += increment;
    updateGL();
}


void ThreeDWidget::mouseMoveEvent(QMouseEvent* e)
{
    int factor = ((e->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier)
                 ? 1 : 8;
    int dx = e->x() - lastPos.x();
    int dy = e->y() - lastPos.y();
    if ((e->buttons() & Qt::LeftButton) == Qt::LeftButton) {
        setXRotation(xRot + factor * dy);
        setYRotation(yRot + factor * dx);
    }
    lastPos = e->pos();
}


void ThreeDWidget::setXRotation(int angle)
{
    normalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotChanged(angle);
        updateGL();
    }
}


void ThreeDWidget::setYRotation(int angle)
{
    normalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotChanged(angle);
        updateGL();
    }
}


void ThreeDWidget::normalizeAngle(int& angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}


GLuint ThreeDWidget::makeLandscape(void)
{
    glDeleteLists(landscape, 1);
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    if (topoGraph) {
        const Fitness* f = topoGraph->fitness();
        const int width = topoGraph->width();
        const int height = topoGraph->height();
        const double nsqrt = sqrt((double) (width * height));
        const double estimatedMax = 1.1 * (f->estimatedMax() + 1);
        const double redshift = 0.0;
        switch (landscapeType) {
        case Points:
            glBegin(GL_POINTS);
            for (int x = 0; x < width; ++x) {
                for (int z = 0; z < height; ++z) {
                    double y = f->f(x, z) / estimatedMax;
                    glColor3d(0.0, y + 0.12, 0.0);
                    glVertex3d((double) x, y * nsqrt, (double) z);
                }
            }
            glEnd();
            break;
        case Mesh:
            /* fall-through */
        case Polygons:
            {
                const int xincr = 2;
                const int zincr = 2;
                for (int x = 0; x < width; x += xincr) {
                    int xi = x + xincr;
                    glBegin(GL_TRIANGLE_STRIP);
                    for (int z = 0; z < height; z += zincr) {
                        double y1 = f->f(x, z) / estimatedMax;
                        double y2 = f->f(xi, z) / estimatedMax;
                        QColor c1 = QColor::fromHsvF(y1 + redshift, 1, 1);
                        QColor c2 = QColor::fromHsvF(y2 + redshift, 1, 1);
                        glColor3d(c1.redF(), c1.greenF(), c1.blueF());
                        glVertex3d((double) x, y1 * nsqrt, (double) z);
                        glColor3d(c2.redF(), c2.greenF(), c2.blueF());
                        glVertex3d((double) xi, y2 * nsqrt, (double) z);
                    }
                    glEnd();
                }
            }
            break;
        default:
            qDebug("Oops! Should've never gone here.");
            break;
        }
    }
    glEndList();
    return list;
}


GLuint ThreeDWidget::makeParticles(void)
{
    glDeleteLists(particles, 1);
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    if (topoGraph) {
        const Fitness* f = topoGraph->fitness();
        const double estimatedMax = 1.1 * (f->estimatedMax() + 1);
        const ParticleList& particles = topoGraph->swarm()->particles();
        const double nsqrt = sqrt((double) topoGraph->width() * (double) topoGraph->height());
        const double hoverHeight = 1.0 / estimatedMax * nsqrt;
        int n = particles.count(); 
        switch (particleType) {
        case Plush:
            {
                glBindTexture(GL_TEXTURE_2D, textureHandle[0]);
                glEnable(GL_BLEND);
                glDepthMask(GL_FALSE);
                const float PARTICLESIZE = 4.0f / 2;
                for (int i = 0; i < n; ++i) {
                    const Particle *p = particles.at(i);
                    double x = p->pos().x();
                    double y = p->z() * hoverHeight + 9.0;
                    double z = p->pos().y();
                    glPushMatrix();
                    glTranslated(x, y, z);
                    glColor3f(1.0f, 1.0f, 1.0f);
                    glRotatef((float) -yRot / 16.0f, 0.0f, 1.0f, 0.0f);
                    glRotatef((float) -xRot / 16.0f, 1.0f, 0.0f, 0.0f);
                    glBegin(GL_QUADS);
                    glTexCoord2i(0, 0); glVertex3f(- PARTICLESIZE,   PARTICLESIZE, 0.0f);
                    glTexCoord2i(0, 1); glVertex3f(- PARTICLESIZE, - PARTICLESIZE, 0.0f);
                    glTexCoord2i(1, 1); glVertex3f(  PARTICLESIZE, - PARTICLESIZE, 0.0f);
                    glTexCoord2i(1, 0); glVertex3f(  PARTICLESIZE,   PARTICLESIZE, 0.0f);
                    glEnd();
                    glPopMatrix();
                }
                glDepthMask(GL_TRUE);
            }
            break;
        case Dotty:
            {
                glBegin(GL_POINTS);
                glColor3f(0.92f, 0.1f, 0.1f);
                for (int i = 0; i < n; ++i) {
                    const Particle* p = particles.at(i);
                    glVertex3d(p->pos().x(), p->z() * hoverHeight + 9.0, p->pos().y());
                }
                glEnd();
            }
            break;
        case Octaeder:
            {
                const double PARTICLESIZE = 3.0 / 2;
                for (int i = 0; i < n; ++i) {
                    glBegin(GL_TRIANGLE_FAN);
                    const Particle* p = particles.at(i);
                    double x = p->pos().x();
                    double y = p->z() * hoverHeight + 8.0;
                    double z = p->pos().y();
                    glBegin(GL_TRIANGLE_FAN);
                    glColor3f(1.0f, 1.0f, 1.0f);
                    glVertex3d(x, y + PARTICLESIZE * 1.4142135623730950488016887242097, z);
                    glColor3f(0.15f, 0.15f, 0.90f);
                    glVertex3d(x - PARTICLESIZE, y, z - PARTICLESIZE);
                    glVertex3d(x + PARTICLESIZE, y, z - PARTICLESIZE);
                    glVertex3d(x + PARTICLESIZE, y, z + PARTICLESIZE);
                    glVertex3d(x - PARTICLESIZE, y, z + PARTICLESIZE);
                    glVertex3d(x - PARTICLESIZE, y, z - PARTICLESIZE);
                    glEnd();
                    glBegin(GL_TRIANGLE_FAN);
                    glColor3f(1.0f, 1.0f, 1.0f);
                    glVertex3d(x, y - PARTICLESIZE * 1.4142135623730950488016887242097, z);
                    glColor3f(0.15f, 0.15f, 0.90f);
                    glVertex3d(x - PARTICLESIZE, y, z - PARTICLESIZE);
                    glVertex3d(x + PARTICLESIZE, y, z - PARTICLESIZE);
                    glVertex3d(x + PARTICLESIZE, y, z + PARTICLESIZE);
                    glVertex3d(x - PARTICLESIZE, y, z + PARTICLESIZE);
                    glVertex3d(x - PARTICLESIZE, y, z - PARTICLESIZE);
                    glEnd();
                }
            }
            break;
        }
    }
    glEndList();
    return list;
}


void ThreeDWidget::particlesChanged(void)
{
    particles = makeParticles();
    updateGL();
}


void ThreeDWidget::fitnessChanged(void)
{
    landscape = makeLandscape();
    updateGL();
}


void ThreeDWidget::graphChanged(void)
{
    landscape = makeLandscape();
    updateGL();
}


void ThreeDWidget::closeEvent(QCloseEvent*)
{
    emit closed();
}
