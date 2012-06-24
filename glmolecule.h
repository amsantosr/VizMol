/*
    Copyright (C) 2011 Abraham Max Santos Ramos <amsantosr@gmail.com>

    This file is part of VizMol.

    VizMol is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    VizMol is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with VizMol.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GLMOLECULA_H
#define GLMOLECULA_H

#include <QGLWidget>
#include <QMainWindow>
#include <QVector>
#include "molecule.h"

/**
* Represents an GLWidget for molecules
*
* @author Abraham Max Santos Ramos
*/
class GLMolecule : public QGLWidget
{
    Q_OBJECT

public:
    GLMolecule(QWidget *parent = 0);
    ~GLMolecule();
    void setMolecule(const Molecule *mol);

public:
    static const float cpkTable[16][4];
    enum Style {
        STYLE_CPK_SPHERES, STYLE_SPHERES_CYLINDERS,
        STYLE_CYLINDERS, STYLE_WIRES
    };

    struct Link {
        int         atomId;
        float       height;
        GLfloat     rotVec[3];
        float       angle;
    };

public slots:
    void showBox(bool enabled);
    void showAxis(bool enabled);
    void setStyle(Style estilo);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    float getRotationAngle(const Point3D &coord1, const Point3D &coord2, float *vrot);
    void renderAtoms();
    void renderLinks();
    void renderWires();
    void renderBox();
    void renderAxis();

private:
    GLUquadricObj   *quadric;
    QMainWindow     *mainWindow;
    QPoint          lastPos;
    const Molecule  *molecule;

    int             xRot;
    int             yRot;
    int             zRot;
    float           distance;

    float           xMin, xMax;
    float           yMin, yMax;
    float           zMin, zMax;

    QVector<Link>   links;

    bool            flagBox;
    bool            flagAxis;
    Style           flagStyle;
};

#endif // GLMOLECULA_H
