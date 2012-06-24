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

#include "glmolecule.h"
#include "util.h"
#include <QMouseEvent>
#include <cfloat>

#define ATOM_RADIO    0.5f
#define LINK_RADIO    0.18f

const float GLMolecule::cpkTable[16][4] = {
    { 200/255.0f, 200/255.0f, 200/255.0f, 1.0f }, /* Light Grey */
    { 143/255.0f, 143/255.0f, 255/255.0f, 1.0f }, /* Sky Blue */
    { 240/255.0f,   0/255.0f,   0/255.0f, 1.0f }, /* Red */
    { 255/255.0f, 200/255.0f,   0/255.0f, 1.0f }, /* Yellow */
    { 255/255.0f, 255/255.0f, 255/255.0f, 1.0f }, /* White */
    { 255/255.0f, 192/255.0f, 203/255.0f, 1.0f }, /* Pink */
    { 218/255.0f, 165/255.0f,  32/255.0f, 1.0f }, /* Golden Rod */
    {   0/255.0f,   0/255.0f, 255/255.0f, 1.0f }, /* Blue */
    { 255/255.0f, 165/255.0f,   0/255.0f, 1.0f }, /* Orange */
    { 128/255.0f, 128/255.0f, 144/255.0f, 1.0f }, /* Dark Grey */
    { 165/255.0f,  42/255.0f,  42/255.0f, 1.0f }, /* Brown */
    { 160/255.0f,  32/255.0f, 240/255.0f, 1.0f }, /* Purple */
    { 255/255.0f,  20/255.0f, 147/255.0f, 1.0f }, /* Deep Pink */
    {   0/255.0f, 255/255.0f,   0/255.0f, 1.0f }, /* Green */
    { 178/255.0f,  34/255.0f,  34/255.0f, 1.0f }, /* Fire Brick */
    {  34/255.0f, 139/255.0f,  34/255.0f, 1.0f }, /* Forest Green */
};

GLMolecule::GLMolecule(QWidget *parent)
    : QGLWidget(parent)
{
    distance = -40.0f;
    molecule = 0;

    flagBox = false;
    flagAxis = false;
    flagStyle = STYLE_CPK_SPHERES;
    xRot = yRot = zRot = 0;

    setCursor(Qt::OpenHandCursor);

    mainWindow = (QMainWindow*)parent;
    quadric = gluNewQuadric();
}

GLMolecule::~GLMolecule()
{
    gluDeleteQuadric(quadric);
}

void GLMolecule::setMolecule(const Molecule *molecule) {
    this->molecule = molecule;
    if (this->molecule == 0)
        return;

    this->links.clear();
    xMin = yMin = zMin = FLT_MAX;
    xMax = yMax = zMax = FLT_MIN;

    for (int i = 0; i < molecule->numAtoms(); i++) {
        const Atom &atom = molecule->getAtom(i);
        if (atom.x() < xMin)
            xMin = atom.x();
        if (atom.y() < yMin)
            yMin = atom.y();
        if (atom.z() < zMin)
            zMin = atom.z();
        if (atom.x() > xMax)
            xMax = atom.x();
        if (atom.y() > yMax)
            yMax = atom.y();
        if (atom.z() > zMax)
            zMax = atom.z();
    }

    for (int i = 0; i < molecule->numLinks(); i++) {
        int id1 = molecule->getLink(i).first;
        int id2 = molecule->getLink(i).second;
        float distance = Util::distance(
                molecule->getAtom(id1).values(),
                molecule->getAtom(id2).values());

        const Atom &atom1 = molecule->getAtom(id1);
        const Atom &atom2 = molecule->getAtom(id2);
        int colorId1 = Atom::Table[atom1.code()].cpkColor;
        int colorId2 = Atom::Table[atom2.code()].cpkColor;
        Link link1, link2;

        if (colorId1 == colorId2) {
            link1.atomId = id1;
            link1.height = distance;
            link1.angle = getRotationAngle(atom1, atom2, link1.rotVec);
            this->links.push_back(link1);
        }
        else {
            link1.atomId = id1;
            link1.height = distance / 2.0f;
            link1.angle = getRotationAngle(atom1, atom2, link1.rotVec);
            this->links.push_back(link1);

            link2.atomId = id2;
            link2.height = distance / 2.0f;
            link2.angle = getRotationAngle(atom2, atom1, link2.rotVec);
            this->links.push_back(link2);
        }
    }

    updateGL();
}

void GLMolecule::showBox(bool enabled)
{
    flagBox = enabled;
    updateGL();
}

void GLMolecule::showAxis(bool enabled)
{
    flagAxis = enabled;
    updateGL();
}

void GLMolecule::initializeGL()
{
    /*static const GLfloat lightPos[4] = { 5.0f, 5.0f, 10.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);*/
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, distance);
}

void GLMolecule::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (molecule == 0) return;

    glPushMatrix();
    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(yRot, 0.0, 1.0, 0.0);

    if (flagStyle != STYLE_WIRES) {
        renderAtoms();
        if (flagStyle != STYLE_CPK_SPHERES)
            renderLinks();
    } else {
        renderWires();
    }

    if (flagBox)
        renderBox();
    if (flagAxis)
        renderAxis();

    glPopMatrix();
}

void GLMolecule::resizeGL(int width, int height)
{
    double side = qMin(width, height);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-width/side, width/side, -height/side, height/side, 5.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

float GLMolecule::getRotationAngle(const Point3D &coord1,
                                   const Point3D &coord2,
                                   float *vrot)
{
    static const float zAxis[3] = { 0.0, 0.0, 1.0 };
    static float diff[3];

    diff[0] = coord2.x() - coord1.x();
    diff[1] = coord2.y() - coord1.y();
    diff[2] = coord2.z() - coord1.z();

    float angle = Util::vectorialAngle(zAxis, diff);
    Util::vectorialProduct(zAxis, diff, vrot);

    return angle;
}

void GLMolecule::renderAtoms()
{
    for (int i = 0; i < molecule->numAtoms(); i++) {
        const Atom &atom = molecule->getAtom(i);
        int colorId = Atom::Table[atom.code()].cpkColor;
        float radio = Atom::Table[atom.code()].vanDerWaalsRadio;

        switch (flagStyle) {
        case STYLE_CPK_SPHERES:
            radio /= 250.0f;
            break;
        case STYLE_SPHERES_CYLINDERS:
            radio /= 1000.0f;
            break;
        case STYLE_CYLINDERS:
            radio = LINK_RADIO;
            break;
        case STYLE_WIRES:
            break;
        }

        glPushMatrix();
        glTranslatef(atom.x(), atom.y(), atom.z());
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cpkTable[colorId]);
        gluSphere(quadric, radio, 20, 20);
        glPopMatrix();
    }
}

void GLMolecule::renderLinks()
{
    for (int i = 0; i < links.size(); i++) {
        Link &link = links[i];
        const Atom &atom = molecule->getAtom(link.atomId);
        int colorId = Atom::Table[atom.code()].cpkColor;

        glPushMatrix();
        glTranslatef(atom.x(), atom.y(), atom.z());
        glRotatef(link.angle, link.rotVec[0], link.rotVec[1], link.rotVec[2]);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cpkTable[colorId]);
        gluCylinder(quadric, LINK_RADIO, LINK_RADIO, link.height, 10, 10);
        glPopMatrix();
    }
}

void GLMolecule::renderWires()
{
    glDisable(GL_LIGHTING);
    //glBegin(GL_LINES);

    for (int i = 0; i < links.size(); i++) {
        Link &link = links[i];
        const Atom &atom = molecule->getAtom(link.atomId);
        int colorId = Atom::Table[atom.code()].cpkColor;

        glPushMatrix();
        glTranslatef(atom.x(), atom.y(), atom.z());
        glRotatef(link.angle, link.rotVec[0], link.rotVec[1], link.rotVec[2]);
        glBegin(GL_LINES);
        glColor3fv(cpkTable[colorId]);
        glVertex3d(0, 0, 0);
        glVertex3f(0, 0, link.height);
        glEnd();
        glPopMatrix();
    }

    //glEnd();
    glEnable(GL_LIGHTING);
}

void GLMolecule::renderBox()
{
    glDisable(GL_LIGHTING);

    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);
    glVertex3f(xMin, yMin, zMin);
    glVertex3f(xMin, yMax, zMin);
    glVertex3f(xMin, yMax, zMin);
    glVertex3f(xMax, yMax, zMin);
    glVertex3f(xMax, yMax, zMin);
    glVertex3f(xMax, yMin, zMin);
    glVertex3f(xMax, yMin, zMin);
    glVertex3f(xMin, yMin, zMin);

    glVertex3f(xMax, yMax, zMax);
    glVertex3f(xMax, yMin, zMax);
    glVertex3f(xMax, yMin, zMax);
    glVertex3f(xMin, yMin, zMax);
    glVertex3f(xMin, yMin, zMax);
    glVertex3f(xMin, yMax, zMax);
    glVertex3f(xMin, yMax, zMax);
    glVertex3f(xMax, yMax, zMax);

    glVertex3f(xMin, yMin, zMin);
    glVertex3f(xMin, yMin, zMax);
    glVertex3f(xMin, yMax, zMin);
    glVertex3f(xMin, yMax, zMax);
    glVertex3f(xMax, yMax, zMin);
    glVertex3f(xMax, yMax, zMax);
    glVertex3f(xMax, yMin, zMin);
    glVertex3f(xMax, yMin, zMax);
    glEnd();

    glEnable(GL_LIGHTING);
}

void GLMolecule::renderAxis()
{
    double xMid = (xMin + xMax) / 2;
    double yMid = (yMin + yMax) / 2;
    double zMid = (zMin + zMax) / 2;

    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    glColor3ub(255, 0, 0);
    glVertex3f(xMin, yMid, zMid);
    glVertex3f(xMax, yMid, zMid);

    glColor3ub(0, 255, 0);
    glVertex3f(xMid, yMin, zMid);
    glVertex3f(xMid, yMax, zMid);

    glColor3ub(0, 0, 255);
    glVertex3f(xMid, yMid, zMin);
    glVertex3f(xMid, yMid, zMax);
    glEnd();

    glEnable(GL_LIGHTING);
}

void GLMolecule::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        setCursor(Qt::ClosedHandCursor);
        lastPos = event->pos();
    }
}

void GLMolecule::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();
        (xRot += dy + 360) %= 360;
        (yRot += dx + 360) %= 360;
        lastPos = event->pos();
        updateGL();
    }
}

void GLMolecule::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setCursor(Qt::OpenHandCursor);
    }
}

void GLMolecule::wheelEvent(QWheelEvent *event)
{
    distance -= event->delta() / 50.0f;
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, distance);
    updateGL();
}

void GLMolecule::setStyle(Style style)
{
    if (flagStyle != style) {
        flagStyle = style;
        updateGL();
    }
}

void GLMolecule::keyPressEvent(QKeyEvent *event)
{
    if (this->parent() == 0 && event->key() == Qt::Key_Escape) {
        mainWindow->setParent(0);
        mainWindow->setCentralWidget(this);
        mainWindow->show();
    }
}
