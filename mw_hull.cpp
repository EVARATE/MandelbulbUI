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


//Algorithm:
void MainWindow::calcHull(boolCloud& cloud){
    //Get input values:
    bool autoMesh = ui->checkBox_autoMesh->isChecked();
    int res = cloud.xsize;
    double min = cloud.xmin;
    double max = cloud.xmax;
    double distance = std::abs(max - min) / res;
    dvec vDist(3,distance);
    dvec vMin(3,min);
    ivec vSize(3,res);

    //Create cloud object:
    boolCloud hull(vDist, vMin, vSize);
    //Reset UI:
    setStatus("Generating Hull...");
    //Progress:
        double progress = 0.0;
        double progdiv = 100.0 / pow(double(hull.xsize - 3), 3.0);
        int pC = 0;

        //Check all points:
        for(int i = 0; i < hull.xsize; ++i){
        for(int j = 0; j < hull.ysize; ++j){
        for(int k = 0; k < hull.zsize; ++k){
            ivec active = {i,j,k};
            dvec coord(3);
            hull.convIndexToCoord(active,coord);
            if(cloud.getState(active)){
                     if(!cloud.getState(i-1,j-1,k-1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i-1,j-1,k  )){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i-1,j-1,k+1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i-1,j  ,k-1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i-1,j  ,k  )){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i-1,j  ,k+1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i-1,j+1,k-1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i-1,j+1,k  )){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i-1,j+1,k+1)){hull.setState(active,true);pC++;}

                else if(!cloud.getState(i  ,j-1,k-1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i  ,j-1,k  )){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i  ,j-1,k+1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i  ,j  ,k-1)){hull.setState(active,true);pC++;}

                else if(!cloud.getState(i  ,j  ,k+1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i  ,j+1,k-1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i  ,j+1,k  )){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i  ,j+1,k+1)){hull.setState(active,true);pC++;}

                else if(!cloud.getState(i+1,j-1,k-1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i+1,j-1,k  )){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i+1,j-1,k+1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i+1,j  ,k-1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i+1,j  ,k  )){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i+1,j  ,k+1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i+1,j+1,k-1)){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i+1,j+1,k  )){hull.setState(active,true);pC++;}
                else if(!cloud.getState(i+1,j+1,k+1)){hull.setState(active,true);pC++;}
            }


            //Display progress:
            progress += progdiv;
            ui->progressBar->setValue(progress);

        }
        }
        }
        ui->checkBox_autoHull->setEnabled(true);
        setStatus("Generated Hull");

        std::string name = "Hull";
        hull.pointCount = pC;

        internalEntity hullEntity(hull,name);
        entityHandler.addEntity(hullEntity);
        createEntry(hullEntity.name, hullEntity.type, hullEntity.id);

        if(autoMesh){
            generateMesh(hull);
        }
}

void MainWindow::calcHull(){
    int id = ui->treeWidget_objects->currentItem()->text(2).toInt();
    internalEntity entity;
    entityHandler.getEntityAtID(id, entity);
    calcHull(entity.bCloud);
}
