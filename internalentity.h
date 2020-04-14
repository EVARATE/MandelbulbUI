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
#include <string>

typedef struct {
   dvec p[3];
} TRIANGLE;

typedef struct {
    std::string name;
    std::string value;
} internalProperty;

typedef std::vector<internalProperty> propertyVec;
typedef std::vector<TRIANGLE> triVec;


class internalEntity
{
public:
    internalEntity(){}

    internalEntity(boolCloud& entityBCloud, std::string entityName):
        bCloud(entityBCloud), name(entityName) {
        type = 0;
        typeName = getTypeName();
    };

    internalEntity(triVec entityTriMesh, std::string entityName):
        triMesh(entityTriMesh), name(entityName) {
        type = 1;
        typeName = getTypeName();
    };

    internalEntity(std::vector<dvec>& entityPointCloud, std::string entityName):
        pointCloud(entityPointCloud), name(entityName) {
        type = 2;
        typeName = getTypeName();
    };
    internalEntity(std::string entityName, int entityType){
        name = entityName;
        type = entityType;
        typeName = getTypeName();
    }

    //Methods
    void addProperty(std::string name, std::string value);
    void addProperty(std::string name, int value);
    void addProperty(std::string name, double value);
    std::string getTypeName();

    //Data:
    boolCloud bCloud;//Type 0
    triVec triMesh;//Type 1
    std::vector<dvec> pointCloud;//Type 2
    //Properties:
    std::string name;
    int id = -1;
    int type;
    std::string typeName;
    propertyVec properties;

    //Is empty?
    bool isEmpty = false;

};

#endif // INTERNALENTITY_H
