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

#ifndef POINT3D_H
#define POINT3D_H

/**
* Represents a 3D point in the space
*
* @author Abraham Max Santos Ramos
*/
class Point3D {
public:
    Point3D() {}
    Point3D(float x, float y, float z) {
        m_Values[0] = x;
        m_Values[1] = y;
        m_Values[2] = z;
    }

    float x() const { return m_Values[0]; }
    float y() const { return m_Values[1]; }
    float z() const { return m_Values[2]; }
    const float *values() const { return m_Values; }

    void setX(float x) { m_Values[0] = x; }
    void setY(float y) { m_Values[1] = y; }
    void setZ(float z) { m_Values[2] = z; }

protected:
    float   m_Values[3];
};

#endif // POINT3D_H
