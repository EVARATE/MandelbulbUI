#ifndef MARCHINGCUBE_H
#define MARCHINGCUBE_H

#include <pointcloudbool.h>

typedef struct {
   dvec p[3];
} TRIANGLE;

typedef struct {
   dvec p[8];
   double val[8];
} GRIDCELL;

class MarchingCube
{
public:
    /*
    MarchingCube(pointCloudBool& boolCloud): bCloud(boolCloud){}
    void march();
    void getEdgeCoord(dvec& coord, ivec& index, int edgeIndex);
    void getEdgeCenter(dvec& outCoord, ivec& indexA, ivec& indexB);
    */
    MarchingCube(pointCloudBool& boolCloud): bCloud(boolCloud){};

    void cubeMarch();
    void getGridCell(GRIDCELL& grid, ivec& index);

    int Polygonise(GRIDCELL grid,double isolevel,TRIANGLE *triangles);
    dvec VertexInterp(double isolevel, dvec& p1, dvec& p2, double valp1, double valp2);

    std::vector<TRIANGLE> triBuffer;
    std::vector<int> triCountBuffer;
    pointCloudBool bCloud;
};

#endif // MARCHINGCUBE_H
