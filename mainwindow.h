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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "molecule.h"
#include "glmolecule.h"

namespace Ui {
    class MainWindow;
}

/**
* @author Abraham Max Santos Ramos
*/
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionFullscreen_triggered();
    void slotOpenFile();
    void slotExit();
    void slotAbout();
    void cpkSpheresStyle(bool checked);
    void ballsAndSticksStyle(bool checked);
    void sticksStyle(bool checked);
    void wiresStyle(bool checked);

private:
    Ui::MainWindow *ui;
    Molecule        molecule;
    GLMolecule      *glMolecule;
};

#endif // MAINWINDOW_H
