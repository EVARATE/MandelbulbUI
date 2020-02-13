/*
Copyright 2019, 2020 Sebastian Motzet

This file is part of MandelbulbUI_V2.

MandelbulbUI_V2 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

MandelbulbUI_V2 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with MandelbulbUI_V2.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <Q3DScatter>
#include "pointcloudbool.h"
#include "cubemat.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //pointCloudBool objects:
    pointCloudBool mBulb;
    //Scatter graph:
    QtDataVisualization::QScatter3DSeries scatterSeries;
    QtDataVisualization::Q3DScatter scatterGraph;
private slots:
    //Saving:
    void actionSaveMBulb();
    void actionSaveHull();
    void saveCubeModel();
    //Loading:
    void actionLoadMBulb();
    void actionLoadHull();
    //Info/About Dialog:
    void actionInfo();
    void actionAbout();

    //Output:
    void updateOutput();
    //Scatter graph:
    void toggleScatterGraph();
    void MBulbToGraph();
    void HullToGraph();

    //Main functions:
    void calcMBulbUI();
    void calcHullUI();


};
#endif // MAINWINDOW_H
