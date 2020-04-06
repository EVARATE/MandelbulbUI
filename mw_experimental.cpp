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

//This file is for experimental code.

//Improve .obj exporter by detecting identical points
void MainWindow::removeOverlapping(triVec& oldMesh, std::vector<dvec>& newArray, ivec& indexOrder){
    setStatus("Removing overlapping vertices...");
    std::vector<dvec> oldArray(0);
    newArray.resize(0);
    triMeshToVecArray(oldMesh, oldArray);

    newArray.push_back(oldArray[0]);
    int currentIndex = 1;
    int referenceIndex = 1;
    indexOrder.resize(0);
    indexOrder.push_back(currentIndex);
    for(int i = 1; i < oldArray.size(); ++i){
        bool isUnique = true;

        for(int j = 0; j < newArray.size(); ++j){
            if(oldArray[i] == newArray[j]){
                isUnique = false;
                referenceIndex = j + 1;
                break;
            }
        }

        if(isUnique){
            newArray.push_back(oldArray[i]);
            currentIndex++;
            indexOrder.push_back(currentIndex);
        }
        else{
            indexOrder.push_back(referenceIndex);
        }

    }

}
void MainWindow::triMeshToVecArray(triVec& triMesh, std::vector<dvec>& vecArray){
    vecArray.resize(0);
    for(int i = 0; i < triMesh.size(); i++){
        dvec v1 = {triMesh[i].p[0][0], triMesh[i].p[0][1], triMesh[i].p[0][2]};
        dvec v2 = {triMesh[i].p[1][0], triMesh[i].p[1][1], triMesh[i].p[1][2]};
        dvec v3 = {triMesh[i].p[2][0], triMesh[i].p[2][1], triMesh[i].p[2][2]};
        vecArray.push_back(v1);
        vecArray.push_back(v2);
        vecArray.push_back(v3);
    }

}
