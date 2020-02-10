#ifndef POINTCLOUDBOOL_H
#define POINTCLOUDBOOL_H


#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

//not using namespace std

//Typedefs:
typedef std::vector<std::vector<bool>> bmat2d;
typedef std::vector<bmat2d> bmat;
typedef std::vector<int> ivec;
typedef std::vector<double> dvec;

/* CHANGES:
 *
 * TODO:
 * Make pointCloudBool more accessible:
 * -More constructer variations (overloading)
 * -Work without 0-border
 * -Don't use 'using namespace std;'
 * -Don't use cout
 *
 * Add exception handling (I'll do it tomorrow for sure)
 *
 * Add Functions:
 * -getPrimLayer(...);
 * -getHullLayer(...);
 * -calcMandelBulb();
 *
 * The whole chain detection algorithm
 */


//Classes:
class pointCloudBool{

public:
    //Constructors (overloaded):
    pointCloudBool(){}
    pointCloudBool(std::string fname){loadPrimary(fname);}
    pointCloudBool(dvec& dist, dvec& min, ivec& size){initPrimary(dist, min, size);}

    //Methods and functions:
    //Primary cloud:
    void initPrimary();
    void initPrimary(dvec& dist, dvec& min, ivec& size);
    void deletePrimary();
    //Access primCloud:
    void setPState(ivec& index, bool state);	//Access by index
    void setPState(dvec& coord, bool state);	//Access by coordinate
    void setPState(int xindex, int yindex, int zindex, bool state);
    bool getPState(ivec& index);
    bool getPState(dvec& coord);
    bool getPState(int xindex, int yindex, int zindex);
    //Load/save:
    void loadPrimary(std::string filename);
    void savePrimary(std::string filename);
    void savePrimCoords(std::string filename);


    //Hull cloud:
    void initHull();
    void initHull(dvec& dist, dvec& min, ivec& size);
    void deleteHull();
    //Access primCloud:
    void setHState(ivec& index, bool state);	//Access by index
    void setHState(dvec& coord, bool state);	//Access by coordinate
    void setHState(int xindex, int yindex, int zindex, bool state);
    bool getHState(ivec& index);
    bool getHState(dvec& coord);
    bool getHState(int xindex, int yindex, int zindex);
    //Load/save:
    void loadHull(std::string filename);
    void saveHull(std::string filename);
    void saveHullCoords(std::string filename);
    //Generate hull:
    void calcHull();


    //Mandelbulb:
    void calcMandelBulb(int iter, int res, double power, double maxlength, double min, double max);

    //Other:
    void convIndexToCoord(ivec& index, dvec& coord);
    void convCoordToIndex(dvec& coord, ivec& index);

    double xdistance, ydistance, zdistance;
    double xmin, ymin, zmin;
    double xmax, ymax, zmax;
    double xsize, ysize, zsize;

    bool primIsInit = false;
    bool hullIsInit = false;

private:
    bmat primCloud;
    bmat hullCloud;
};

#endif // POINTCLOUDBOOL_H
