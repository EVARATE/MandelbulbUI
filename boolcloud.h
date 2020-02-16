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

#ifndef BOOLCLOUD_H
#define BOOLCLOUD_H

/* This is a more generalized version of the old pointCloudBool class.
 * Instead of containing a primary and a hull cloud it only contains
 * one cloud.
 * If two clouds are needed one must simply create two objects of this class.
 *
 * The class only contains methods which allow access and alteration of the cloud.
 * Functions like 'calcHull()' are not included. Those must be written outside
 * of the class.
 */

//Includes:
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

//typedefs:
typedef std::vector<int> ivec;
typedef std::vector<double> dvec;
typedef std::vector<bool> bvec;
typedef std::vector<bvec> bmat2d;
typedef std::vector<bmat2d> bmat3d;

class boolCloud
{
public:
    //Constructors:
    boolCloud(){};
    boolCloud(std::string fpath){loadInternal(fpath);}
    boolCloud(dvec &dist, dvec &min, ivec &size){init(dist, min, size);}

    //Methods and functions:
    void init();
    void init(dvec &dist, dvec &min, ivec &size);
    void remove();
    //Element access:
    //Set:
    void setState(ivec &index, bool state);
    void setState(dvec& coord, bool state);
    void setState(int xindex, int yindex, int zindex, bool state);
    //Get:
    bool getState(ivec &index);
    bool getState(dvec &coord);
    bool getState(int xindex, int yindex, int zindex);
    //Load/Save:
    void loadInternal(std::string fpath);
    void saveInternal(std::string fpath);
    void saveCoords(std::string fpath);
    //Converters:
    void convIndexToCoord(ivec &index, dvec &coord);
    void convCoordToIndex(dvec &coord, ivec &index);

    //Parameters:
    double xdistance, ydistance, zdistance;
    double xmin, ymin, zmin;
    double xmax, ymax, zmax;
    int xsize, ysize, zsize;
    bool isInit = false;
private:
    bmat3d cloud;
};

#endif // BOOLCLOUD_H
