#ifndef BOOLCLOUD_H
#define BOOLCLOUD_H

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
    void loadInternal(const std::string& fpath);
    void saveInternal(const std::string& fpath);
    void saveCoords(const std::string& fpath);
    //Converters:
    void convIndexToCoord(ivec &index, dvec &coord);
    void convCoordToIndex(dvec &coord, ivec &index);

    //Parameters:
    double xdistance, ydistance, zdistance;
    double xmin, ymin, zmin;
    double xmax, ymax, zmax;
    int xsize, ysize, zsize;
    int pointCount = 0;
    bool isInit = false;
private:
    bmat3d cloud;
};


#endif // BOOLCLOUD_H
