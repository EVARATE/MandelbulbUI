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

#ifndef CUBEMAT_H
#define CUBEMAT_H

#include <pointcloudbool.h>

//To avoid undefined behaviour when accessing a 'deleted' face
//it will not actually be deleted. Instead the vertices of the face
//will be set to dummy values (0,0,0,0)

//Mesh Generation:
typedef std::vector<ivec> face;  //Has 4 elements
typedef std::vector<face> cube; //Has at most 6 elements
typedef std::vector<std::vector<cube>> cubeMat2d;
typedef std::vector<cubeMat2d> cubeMat3d;

class cubeMat
{
public:
    //Constructor(s):
    cubeMat(pointCloudBool& boolCloud): bCloud(boolCloud){initCubeMat();}

    //Methods:
    void initCubeMat();
    void placeCubes();
    void placeCubeAt(ivec& index, cube& localCube);
    void genLocalVerts(ivec& index, std::vector<ivec>& localVerts);
    void genLocalCube(ivec& index, cube& localCube);

    void remOverlapping(cube& cubeA, cube& cubeB);
    void remAllOverlapping();
    void saveObj(std::string fileName);

    bool isEmpty(cube& checkCube);

private:
    pointCloudBool bCloud;
    cubeMat3d cubeSlots;  //Map of all cubes

    bool isInit = false;

    const face emptyFace = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
    const cube emptyCube = {
        {{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
        {{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
        {{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
        {{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
        {{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
        {{0,0,0},{0,0,0},{0,0,0},{0,0,0}}
    };
};

#endif // CUBEMAT_H
