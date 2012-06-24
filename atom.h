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

#ifndef ATOM_H
#define ATOM_H

#include "point3d.h"
#include <QString>

#define MAXELEMNO       104 // total number of atoms

/**
* Represents an Atom with 3D coordinates
*
* @author Abraham Max Santos Ramos
*/
class Atom : public Point3D {
public:
    Atom() {}
    int code() const;
    void setCode(int code);
    void setCode(const QString &strCode);

public:
    struct Info {
        char symbol[2];
        int covalentRadio;
        int vanDerWaalsRadio;
        int cpkColor;
        const char *name;
    };

    static Info Table[MAXELEMNO];

protected:
    int     m_code;
};

inline int Atom::code() const
{
    return m_code;
}

inline void Atom::setCode(int code)
{
    m_code = code;
}

#endif // ATOM_H
