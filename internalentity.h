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

};

#endif // INTERNALENTITY_H
