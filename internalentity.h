/*
Copyright 2020 Sebastian Motzet

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

#ifndef INTERNALENTITY_H
#define INTERNALENTITY_H

#include <iostream>
#include "boolcloud.h"

typedef struct {
   dvec p[3];
} TRIANGLE;



class internalEntity
{
public:
    internalEntity(){}

    internalEntity(boolCloud& entityBCloud, std::string entityName):
        bCloud(entityBCloud), name(entityName) {type = 0;};

    internalEntity(std::vector<TRIANGLE> entityTriMesh, std::string entityName):
        triMesh(entityTriMesh), name(entityName) {type = 1;};

    internalEntity(std::vector<dvec>& entityPointCloud, std::string entityName):
        pointCloud(entityPointCloud), name(entityName) {type = 2;};



    //Type slots:
    boolCloud bCloud;//Type 0
    std::vector<TRIANGLE> triMesh;//Type 1
    std::vector<dvec> pointCloud;//Type 2
    //Data:
    std::string name = "Item";
    int id = -1;//Must be changed to a positive integer
    int type = 0;
    int data0 = 0;//Custom data.
    int data1 = 0;//Dependent on object and not always
    int data2 = 0;//available

};

#endif // INTERNALENTITY_H
