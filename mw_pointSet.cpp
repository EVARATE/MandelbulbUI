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


void MainWindow::pointCloudToBoolCloud(std::vector<dvec>& pointCloud, ivec& depth){
    //Check if point set is valid:
    if(pointCloud.size() < 1){return;}

    dvec xmin = pointCloud[lowestPoint(pointCloud, 0)];
    dvec ymin = pointCloud[lowestPoint(pointCloud, 1)];
    dvec zmin = pointCloud[lowestPoint(pointCloud, 2)];
    dvec xmax = pointCloud[highestPoint(pointCloud, 0)];
    dvec ymax = pointCloud[highestPoint(pointCloud, 1)];
    dvec zmax = pointCloud[highestPoint(pointCloud, 2)];
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
    double progdiv = double( depth[0]*depth[1]*depth[2] -3.0 ) / 100.0;

    //Calc distance of each gridpoint to each point; distance = weight.
    //Add all weights. If result is above threshold, set gridpoint to true.
    for(int i = 0; i < depth[0]; ++i){
    for(int j = 0; j < depth[1]; ++j){
    for(int k = 0; k < depth[2]; ++k){
        ivec index = {i,j,k};
        dvec coord(3);
        cloud.convIndexToCoord(index, coord);
        double threshold = 0.2;
        bool derivedState = slotAboveThreshold(coord, pointCloud, threshold);
        cloud.setState(index, derivedState);

        progress += progdiv;
        ui->progressBar->setValue(progress);
    }
    }
    }


    /*
    //Check each point:
    for(int n = 0; n < pointCloud.size(); ++n){
        //Check for each slot of the cloud if the point is near it
        checkPoint(depth, cloud, pointCloud[n], xdist);

        progress += progdiv;
        ui->progressBar->setValue(progress);
    }
    */


    //Create object:
    internalEntity cloudEntity(cloud, "New cloud");
    entityHandler.addEntity(cloudEntity);
    createEntry(cloudEntity.name, cloudEntity.type, cloudEntity.id);
    setStatus("Generated boolCloud");
}
void MainWindow::pointCloudToBoolCloud(){
    int id = getSelectedID();
    internalEntity pointCloudObj;
    entityHandler.getEntityAtID(id, pointCloudObj);
    ivec depth = {50,50,50};
    pointCloudToBoolCloud(pointCloudObj.pointCloud, depth);
}
bool MainWindow::slotAboveThreshold(dvec& slotCoord, std::vector<dvec>& pointCloud, double threshold){
    long double weight = 0;
    for(int i = 0; i < pointCloud.size(); ++i){
        double distance = calcDistance(slotCoord, pointCloud[i]);
        weight += exp(- distance * distance);
    }
    weight = weight / pointCloud.size();
    if(weight >= threshold){
        return true;
    }else{
        return false;
    }
}

void MainWindow::checkPoint(ivec& depth, boolCloud& cloud, dvec& point, double radius){
    dvec slot(3);
    for(int i = 0; i < depth[0]; ++i){
    for(int j = 0; j < depth[1]; ++j){
    for(int k = 0; k < depth[2]; ++k){
        ivec index = {i,j,k};

        cloud.convIndexToCoord(index, slot);
        if(isNear(slot, point, radius / 2.0)){
            cloud.setState(index, true);
        }
    }
    }
    }
}

int MainWindow::lowestPoint(std::vector<dvec>& pointCloud, double index){
    double min = pointCloud[0][index];
    int minIndex = 0;
    for(int i = 0; i < pointCloud.size(); ++i){
        if(pointCloud[i][index] < min){
            min = pointCloud[i][index];
            minIndex = i;
        }
    }
    return minIndex;
}
int MainWindow::highestPoint(std::vector<dvec>& pointCloud, double index){
    double max = pointCloud[0][index];
    int maxIndex = 0;
    for(int i = 0; i < pointCloud.size(); ++i){
        if(pointCloud[i][index] > max){
            max = pointCloud[i][index];
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
