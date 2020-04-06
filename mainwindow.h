/*
Copyright 2019, 2020 Sebastian Motzet

This file is part of MandelbulbUI.

MandelbulbUI is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

MandelbulbUI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with MandelbulbUI.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <algorithm>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>
#include <Q3DScatter>
#include <QAbstract3DSeries>
#include <Q3DTheme>
#include <QMenu>
#include "boolcloud.h"
#include "internalentityhandler.h"
#include "utilityFunctions.cpp"


//Structs:
typedef struct {
   dvec p[8];
   double val[8];
} GRIDCELL;


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

    //Methods:
    int getSelectedID();
    int getSelectedType();

    //Scatter graph:
    QtDataVisualization::QScatter3DSeries scatterSeries;
    QtDataVisualization::Q3DScatter scatterGraph;

    //Input data:
    struct {
        int res = 50;
        int iter = 10;
        double min = -1.75;
        double max = 1.75;
        double power = 8.0;
        double maxLength = 2.0;
    } inputValues;

    internalEntityHandler entityHandler;//Contains and manages all internal Objects

private slots:
    //Saving/Loading:
    void actionSaveBoolCloud(boolCloud& cloud);
    void actionSaveBoolCloud();
    void actionLoadBoolCloud();
    void actionExportMesh(triVec& triMesh);
    void actionExportMesh();
    void actionSaveTriMesh(triVec& triMesh);
    void actionSaveTriMesh();
    void actionLoadpointCloud();
    //Info/About Dialog:
    void actionInfo();
    void actionAbout();

    //Output:
    void updateOutput();
    void updatePropertyViewer();
    void setPropertyAtRow(const int row, const std::string& name, const std::string& value);
    void setStatus(const std::string& statusMessage);
    //Scatter graph:
    void toggleScatterGraph();
    void selectedToGraph();
    void boolCloudToGraph(boolCloud& cloud);
    void pointCloudToGraph(std::vector<dvec>& pointCloud);

    //Main functions:
    void calcMBulb();
    void calcHull(boolCloud& cloud);
    void calcHull();

    //Marching Cubes:
    void generateMesh(boolCloud& cloud);
    void generateMesh();

    //.obj processing
    void removeOverlapping(triVec& triMesh, std::vector<dvec>& newArray, ivec& indexOrder);
    void triMeshToVecArray(triVec& triMesh, std::vector<dvec>& vecArray);

    //Point set:
    void pointCloudToBoolCloud(std::vector<dvec>& pointCloud, ivec& depth);
    void pointCloudToBoolCloud();
    bool slotAboveThreshold(dvec& slotCoord, std::vector<dvec>& pointCloud, double threshold);
    void checkPoint(ivec& depth, boolCloud& cloud, dvec& point, double xdist);
    int lowestPoint(std::vector<dvec>& pointCloud, double index);
    int highestPoint(std::vector<dvec>& pointCloud, double index);
    bool isNear(dvec& pointA, dvec& pointB, double radius);

    //Entity list management
    void createEntry(const std::string& name, int type, int id);
    void deleteEntry();
    void updateActionAvailability();

    //Entity list context menu
    void createContextMenu(const QPoint& pos);

};
#endif // MAINWINDOW_H
