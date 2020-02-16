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

#ifndef TRIMESH_H
#define TRIMESH_H

#include <vector>

//Overall class for storing a 3d mesh made of ngons

//typedefs:
typedef struct {
    //Vertex has x,y,z coordinates and stores a value (for now)
    double xpos;
    double ypos;
    double zpos;
    double val;
} vertex;
typedef struct {
    vertex vert[3];
    double normal;
} triangle;



class triMesh
{
public:
    //Constructor:
    triMesh(){};

    //Methods:
    unsigned int triCount() {return mesh.size();}

    void addTriangle(triangle tri);


private:
    double pivot[3] = {0.0, 0.0, 0.0};//Pivot point
    std::vector<triangle> mesh;
};

#endif // TRIMESH_H
