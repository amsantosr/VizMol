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

#include <QtAlgorithms>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <exception>
#include "util.h"
#include "molecule.h"

void Molecule::load(const QString &filename)
{
    QString line;

    m_Atoms.clear();
    m_Links.clear();

    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        throw std::bad_exception();
    }
    QTextStream input(&file);

    while (line = input.readLine(), !line.isNull()) {
        if (line.startsWith("ATOM") || line.startsWith("HETATM")) {
            Atom atom;
            QString codeStr = QStringRef(&line, 12, 4).toString();

            atom.setX(Util::parseFloat(line, 30, 8));
            atom.setY(Util::parseFloat(line, 38, 8));
            atom.setZ(Util::parseFloat(line, 46, 8));
            atom.setCode(codeStr.trimmed());

            m_Atoms.push_back(atom);
        }
        else if (line.startsWith("CONECT")) {
            int id1 = Util::parseInt(line, 6, 5);
            int id2 = Util::parseInt(line, 11, 5);
            int id3 = Util::parseInt(line, 16, 5);
            int id4 = Util::parseInt(line, 21, 5);
            int id5 = Util::parseInt(line, 26, 5);
            if (id1 != -1) {
                if (id2 != -1)
                    addLink(id1 - 1, id2 - 1);
                if (id3 != -1)
                    addLink(id1 - 1, id3 - 1);
                if (id4 != -1)
                    addLink(id1 - 1, id4 - 1);
                if (id5 != -1)
                    addLink(id1 - 1, id5 - 1);
            }
        }
    }
}

void Molecule::addLink(int id1, int id2)
{
    if (id1 > id2)
        qSwap(id1, id2);
    Link newEnlace(id1, id2);
    QVector<Link>::const_iterator first, last;
    first = m_Links.begin();
    last = m_Links.end();

    int numAtomos = this->numAtoms();
    if (0 <= id1 && id1 < numAtomos && 0 <= id2 && id2 < numAtomos)
        if (qFind(first, last, newEnlace) == last)
            m_Links.push_back(Link(id1, id2));
}
