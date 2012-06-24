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

#ifndef MOLECULE_H
#define MOLECULE_H

#include <QVector>
#include <QPair>
#include "point3d.h"
#include "atom.h"

/**
* Represents a molecule given its atoms and links
*
* @author Abraham Max Santos Ramos
*/
class Molecule : QObject
{
public:
    typedef QPair<int, int>     Link;

    Molecule() {}
    ~Molecule() {}

    void load(const QString &filename);
    int numAtoms() const { return m_Atoms.size(); }
    int numLinks() const { return m_Links.size(); }
    const Atom &getAtom(int id) const { return m_Atoms[id]; }
    const Link &getLink(int id) const { return m_Links[id]; }

private:
    void addLink(int id1, int id2);

private:
    QVector<Atom>       m_Atoms;
    QVector<Link>       m_Links;
};

#endif // MOLECULE_H
