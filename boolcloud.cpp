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

#include "boolcloud.h"

//Methods and functions:
void boolCloud::init(){
    xmax = xmin + xsize*xdistance;
    ymax = ymin + ysize*ydistance;
    zmax = zmin + zsize*zdistance;
    cloud.resize(xsize);
    for(int i = 0;i < xsize; ++i){
        cloud[i].resize(ysize);
        for(int j = 0; j < zsize; ++j){
            cloud[i][j].resize(zsize, false);
        }
    }
    isInit = true;
}
void boolCloud::init(dvec &dist, dvec &min, ivec &size){
    //if(isInit){return;} Allow resizing
    xdistance = dist[0];
    ydistance = dist[1];
    zdistance = dist[2];
    xmin = min[0];
    ymin = min[1];
    zmin = min[2];
    xsize = size[0];
    ysize = size[1];
    zsize = size[2];
    init();
}
void boolCloud::remove(){
    bmat3d().swap(cloud);
    isInit = false;
}
//Element access:
//Set:
void boolCloud::setState(ivec &index, bool state){
    if( (index[0] < 0) || (index[1] < 0) || (index[2] < 0) ||
    (index[0] >= xsize) || (index[1] >= ysize) || (index[2] >= zsize)){
        return;
    }
    cloud[index[0]][index[1]][index[2]] = state;
}
void boolCloud::setState(dvec& coord, bool state){
    ivec index(3);
    convCoordToIndex(coord, index);
    setState(index, state);
}
void boolCloud::setState(int xindex, int yindex, int zindex, bool state){
    ivec index = {xindex, yindex, zindex};
    setState(index, state);
}
//Get:
bool boolCloud::getState(ivec &index){
    if( (index[0] < 0) || (index[1] < 0) || (index[2] < 0) ||
    (index[0] >= xsize) || (index[1] >= ysize) || (index[2] >= zsize)){
        return false;
    }else{
        return cloud[index[0]][index[1]][index[2]];
    }
}
bool boolCloud::getState(dvec &coord){
    ivec index(3);
    convCoordToIndex(coord, index);
    return getState(index);
}
bool boolCloud::getState(int xindex, int yindex, int zindex){
    ivec index = {xindex,yindex,zindex};
    return getState(index);
}
//Load/Save:
void boolCloud::loadInternal(std::string fpath){
    std::ifstream ifile;
    ifile.open(fpath);
    if(!ifile.is_open()){return;}
    ifile >> xdistance >> ydistance >> zdistance;
    ifile >> xmin >> ymin >> zmin;
    ifile >> xmax >> ymax >> zmax;
    ifile >> xsize >> ysize >> zsize;
    if(!isInit){init();}
    for(int i = 0; i < xsize; ++i){
    for(int j = 0; j < ysize; ++j){
    for(int k = 0; k < zsize; ++k){
        std::string binState;
        ifile >> binState;
        ivec index = {i,j,k};
        if(binState[0] == '1'){
            setState(index, true);
        }else{
            setState(index, false);
        }
    }
    }
    }
    ifile.close();
}
void boolCloud::saveInternal(std::string fpath){
    std::ofstream ofile;
    ofile.open(fpath);
    if(!ofile.is_open()){return;}
    ofile << xdistance << " " << ydistance << " " << zdistance << "\n";
    ofile << xmin << " " << ymin << " " << zmin << "\n";
    ofile << xmax << " " << ymax << " " << zmax << "\n";
    ofile << xsize << " " << ysize << " " << zsize << "\n";
    for(int i = 0; i < xsize; ++i){
    for(int j = 0; j < ysize; ++j){
    for(int k = 0; k < zsize; ++k){
        ivec index = {i,j,k};
        if(getState(index)){
            ofile << "1";
        }else{
            ofile << "0";
        }
        ofile << " ";
    }ofile << "\n";
    }ofile << "\n";
    }
    ofile.close();
}
void boolCloud::saveCoords(std::string fpath){
    std::ofstream ofile;
    ofile.open(fpath);
    if(!ofile.is_open()){return;}
    for(int i = 0; i < xsize; ++i){
    for(int j = 0; j < ysize; ++j){
    for(int k = 0; k < zsize; ++k){
        ivec index = {i,j,k};
        dvec coord(3);
        if(getState(index)){
            convIndexToCoord(index, coord);
            ofile << coord[0] << " " << coord[1] << " " << coord[2] << "\n";
        }
    }
    }
    }
    ofile.close();
}
//Converters:
void boolCloud::convIndexToCoord(ivec &index, dvec &coord){
    coord[0] = index[0] * xdistance + xmin;
    coord[1] = index[1] * ydistance + ymin;
    coord[2] = index[2] * zdistance + zmin;
}
void boolCloud::convCoordToIndex(dvec &coord, ivec &index){
    index[0] = (coord[0] - xmin) / xdistance;
    index[1] = (coord[1] - ymin) / ydistance;
    index[2] = (coord[2] - zmin) / zdistance;
}
