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
#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::pointSetToBoolCloud(std::vector<dvec>& pointSet, ivec& depth){
    //Check if point set is valid:
    if(pointSet.size() < 1){return;}

    dvec xmin = pointSet[lowestPoint(pointSet, 0)];
    dvec ymin = pointSet[lowestPoint(pointSet, 1)];
    dvec zmin = pointSet[lowestPoint(pointSet, 2)];
    dvec xmax = pointSet[highestPoint(pointSet, 0)];
    dvec ymax = pointSet[highestPoint(pointSet, 1)];
    dvec zmax = pointSet[highestPoint(pointSet, 2)];
    double xminBorder = xmin[0];
    double xmaxBorder = xmax[0];
    double yminBorder = xmin[1];
    double ymaxBorder = xmax[1];
    double zminBorder = xmin[2];
    double zmaxBorder = xmax[2];
    double xdist = (xmaxBorder - xminBorder) / double(depth[0]);
    double ydist = ymaxBorder - yminBorder / double(depth[1]);
    double zdist = zmaxBorder - zminBorder / double(depth[2]);

    dvec dist = {xdist, ydist, zdist};
    dvec min = {xminBorder, yminBorder, zminBorder};
    boolCloud cloud(dist, min, depth);

    double progress = 0.0;
    double progdiv = double(pointSet.size()) / 100.0;

    //Check each point:
    for(int n = 0; n < pointSet.size(); ++n){
        //Check for each slot of the cloud if the point is near it
        checkPoint(depth, cloud, pointSet[n], xdist);

        progress += progdiv;
        ui->progressBar->setValue(progress);
    }

    //Create object:
    createAbstrObj(cloud, "New cloud");
    ui->label_infoText->setText("Generated boolcloud.");
}
void MainWindow::pointSetToBoolCloud(){
    int id = getSelectedID();
    abstrItem pointSetObj;
    getObjAtID(id, pointSetObj);
    ivec depth = {100,100,100};
    pointSetToBoolCloud(pointSetObj.pointSet, depth);
}
void MainWindow::checkPoint(ivec& depth, boolCloud& cloud, dvec& point, double xdist){
    dvec slot(3);
    for(int i = 0; i < depth[0]; ++i){
    for(int j = 0; j < depth[1]; ++j){
    for(int k = 0; k < depth[2]; ++k){
        ivec index = {i,j,k};

        cloud.convIndexToCoord(index, slot);
        if(isNear(slot, point, xdist / 2.0)){
            cloud.setState(index, true);
        }
    }
    }
    }
}

int MainWindow::lowestPoint(std::vector<dvec>& pointSet, double index){
    double min = pointSet[0][index];
    int minIndex = 0;
    for(int i = 0; i < pointSet.size(); ++i){
        if(pointSet[i][index] < min){
            min = pointSet[i][index];
            minIndex = i;
        }
    }
    return minIndex;
}
int MainWindow::highestPoint(std::vector<dvec>& pointSet, double index){
    double max = pointSet[0][index];
    int maxIndex = 0;
    for(int i = 0; i < pointSet.size(); ++i){
        if(pointSet[i][index] > max){
            max = pointSet[i][index];
            maxIndex = i;
        }
    }
    return maxIndex;
}
bool MainWindow::isNear(dvec& pointA, dvec& pointB, double radius){
    bool xnear = false;
    bool ynear = false;
    bool znear = false;
    if( std::abs(pointA[0] - pointB[0]) <= radius ){xnear = true;}
    if( std::abs(pointA[1] - pointB[1]) <= radius ){ynear = true;}
    if( std::abs(pointA[2] - pointB[2]) <= radius ){znear = true;}

    if(xnear && ynear && znear){
        return true;
    }else{return false;}
}
