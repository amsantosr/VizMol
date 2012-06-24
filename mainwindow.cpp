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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileInfo>
#include <QMessageBox>
#include <exception>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("VizMol");
    glMolecule = new GLMolecule(this);
    setCentralWidget(glMolecule);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
    connect(ui->actionShowBox, SIGNAL(toggled(bool)), glMolecule, SLOT(showBox(bool)));
    connect(ui->actionShowAxis, SIGNAL(toggled(bool)), glMolecule, SLOT(showAxis(bool)));
    connect(ui->actionCPKSpheres, SIGNAL(triggered(bool)), this, SLOT(cpkSpheresStyle(bool)));
    connect(ui->actionBallsAndSticks, SIGNAL(triggered(bool)), this, SLOT(ballsAndSticksStyle(bool)));
    connect(ui->actionSticks, SIGNAL(triggered(bool)), this, SLOT(sticksStyle(bool)));
    connect(ui->actionWires, SIGNAL(triggered(bool)), this, SLOT(wiresStyle(bool)));

    cpkSpheresStyle(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotOpenFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open PDB File",
                                                    QString(),
                                                    "PDB Files (*.pdb)");

    if (!filename.isNull()) {
        std::string fn = filename.toStdString();
        try {
            molecule.load(fn.c_str());
        } catch (std::exception &ex) {
            QMessageBox::information(this, "Error",
                                     QString::fromStdString(ex.what()),
                                     QMessageBox::Ok);
        }
        glMolecule->setMolecule(&molecule);
        setWindowTitle(tr("%1 - VizMol").arg(QFileInfo(filename).fileName()));
    }
}

void MainWindow::slotExit()
{
    this->close();
}

void MainWindow::slotAbout()
{
    QMessageBox::about(this, tr("About VizMol"),
            tr("<p><b>VizMol version 0.4</b></p>"
               "<p>Build date: %1"
               "<br>This program uses Qt 4.7"
               "<br>Abraham Max Santos Ramos"
               "<br>Email: <a href=\"mailto:amsantosr@gmail.com>\">amsantosr@gmail.com</a>"
               "<br>2011"
               "</p>").arg(__TIMESTAMP__));
}

void MainWindow::cpkSpheresStyle(bool checked)
{
    if (!checked) {
        ui->actionCPKSpheres->setChecked(true);
    } else {
        if (ui->actionBallsAndSticks->isChecked())
            ui->actionBallsAndSticks->setChecked(false);
        if (ui->actionSticks->isChecked())
            ui->actionSticks->setChecked(false);
        if (ui->actionWires->isChecked())
            ui->actionWires->setChecked(false);
        glMolecule->setStyle(GLMolecule::STYLE_CPK_SPHERES);
    }
}

void MainWindow::ballsAndSticksStyle(bool checked)
{
    if (!checked) {
        ui->actionBallsAndSticks->setChecked(true);
    } else {
        if (ui->actionCPKSpheres->isChecked())
            ui->actionCPKSpheres->setChecked(false);
        if (ui->actionSticks->isChecked())
            ui->actionSticks->setChecked(false);
        if (ui->actionWires->isChecked())
            ui->actionWires->setChecked(false);
        glMolecule->setStyle(GLMolecule::STYLE_SPHERES_CYLINDERS);
    }
}

void MainWindow::sticksStyle(bool checked)
{
    if (!checked) {
        ui->actionSticks->setChecked(true);
    } else {
        if (ui->actionCPKSpheres->isChecked())
            ui->actionCPKSpheres->setChecked(false);
        if (ui->actionBallsAndSticks->isChecked())
            ui->actionBallsAndSticks->setChecked(false);
        if (ui->actionWires->isChecked())
            ui->actionWires->setChecked(false);
        glMolecule->setStyle(GLMolecule::STYLE_CYLINDERS);
    }
}

void MainWindow::wiresStyle(bool checked)
{
    if (!checked) {
        ui->actionWires->setChecked(true);
    } else {
        if (ui->actionCPKSpheres->isChecked())
            ui->actionCPKSpheres->setChecked(false);
        if (ui->actionBallsAndSticks->isChecked())
            ui->actionBallsAndSticks->setChecked(false);
        if (ui->actionSticks->isChecked())
            ui->actionSticks->setChecked(false);
        glMolecule->setStyle(GLMolecule::STYLE_WIRES);
    }
}

void MainWindow::on_actionFullscreen_triggered()
{
    glMolecule->setParent(0);
    this->hide();
    this->setParent(glMolecule);
    glMolecule->showFullScreen();
}
