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

#include "pointcloudbool.h"

//pointCloudBool implementation:


//Primary cloud:
void pointCloudBool::initPrimary(){
    xmax = xmin + xsize*xdistance;
    ymax = ymin + ysize*ydistance;
    zmax = zmin + zsize*zdistance;

    primCloud.resize(xsize);
    for(int i = 0;i < xsize; ++i){
        primCloud[i].resize(ysize);
        for(int j = 0; j < zsize; ++j){
            primCloud[i][j].resize(zsize, false);
        }
    }

    primIsInit = true;
}
void pointCloudBool::initPrimary(dvec& dist, dvec& min, ivec& size){
    if(primIsInit){return;}
    xdistance = dist[0];
    ydistance = dist[1];
    zdistance = dist[2];
    xmin = min[0];
    ymin = min[1];
    zmin = min[2];
    xsize = size[0];
    ysize = size[1];
    zsize = size[2];
    initPrimary();
}
void pointCloudBool::deletePrimary(){
    bmat().swap(primCloud);
    primIsInit = false;
}
//Access primCloud:
void pointCloudBool::setPState(ivec& index, bool state){
    primCloud[index[0]][index[1]][index[2]] = state;
}
void pointCloudBool::setPState(dvec& coord, bool state){
    ivec index(3);
    convCoordToIndex(coord, index);
    setPState(index, state);
}
void pointCloudBool::setPState(int xindex, int yindex, int zindex, bool state){
    ivec index = {xindex,yindex,zindex};
    setPState(index, state);
}
bool pointCloudBool::getPState(ivec& index){
    if( (index[0] < 0) || (index[1] < 0) || (index[2] < 0) ){
        return false;
    }
    else if( (index[0] >= xsize) || (index[1] >= ysize) || (index[2] >= zsize) ){
        return false;
    }else{
        return primCloud[index[0]][index[1]][index[2]];
    }
}
bool pointCloudBool::getPState(dvec& coord){
    ivec index(3);
    convCoordToIndex(coord, index);
    return getPState(index);
}
bool pointCloudBool::getPState(int xindex, int yindex, int zindex){
    ivec index = {xindex,yindex,zindex};
    return getPState(index);
}
//Load/save:
void pointCloudBool::loadPrimary(std::string filename){
    std::ifstream ifile;
    ifile.open(filename);
    if(!ifile.is_open()){return;}

    ifile >> xdistance >> ydistance >> zdistance;
    ifile >> xmin >> ymin >> zmin;
    ifile >> xmax >> ymax >> zmax;
    ifile >> xsize >> ysize >> zsize;

    if(!primIsInit){initPrimary();}

    std::string numstate;
    for(int i = 0; i < xsize; ++i){
        for(int j = 0; j < ysize; ++j){
            for(int k = 0; k < zsize; ++k){
                ifile >> numstate;
                ivec index = {i,j,k};
                if(numstate[0] == '1'){
                    setPState(index,true);
                }else{
                    setPState(index,false);
                }
            }
        }
    }
    ifile.close();
}
void pointCloudBool::savePrimary(std::string filename){
    std::ofstream ofile;
    ofile.open(filename);
    if(!ofile.is_open()){return;}

    ofile << xdistance << " " << ydistance << " " << zdistance << "\n";
    ofile << xmin << " " << ymin << " " << zmin << "\n";
    ofile << xmax << " " << ymax << " " << zmax << "\n";
    ofile << xsize << " " << ysize << " " << zsize << "\n";

    for(int i = 0; i < xsize; ++i){
        for(int j = 0; j < ysize; ++j){
            for(int k = 0; k < zsize; ++k){
                ivec index = {i,j,k};
                if(getPState(index)){
                    ofile << "1";
                }else{
                    ofile << "0";
                }
                ofile << " ";
            } ofile << "\n";
        } ofile << "\n";
    }
    ofile.close();
}
void pointCloudBool::savePrimCoords(std::string filename){
std::ofstream ofile;
ofile.open(filename);
if(!ofile.is_open()){return;}
dvec coord(3);
for(int i = 0; i < xsize; ++i){
    for(int j = 0; j < ysize; ++j){
        for(int k = 0; k < zsize; ++k){
            ivec index = {i,j,k};
            if(getPState(index)){
                convIndexToCoord(index,coord);
                ofile << coord[0] << " " << coord[1] << " " << coord[2] << "\n";
            }
        }
    }
}
ofile.close();
}

//Hull cloud:
void pointCloudBool::initHull(){
    xmax = xmin + xsize*xdistance;
    ymax = ymin + ysize*ydistance;
    zmax = zmin + zsize*zdistance;

    hullCloud.resize(xsize);
    for(int i = 0;i < xsize; ++i){
        hullCloud[i].resize(ysize);
        for(int j = 0; j < zsize; ++j){
            hullCloud[i][j].resize(zsize, false);
        }
    }

    hullIsInit = true;
}
void pointCloudBool::initHull(dvec& dist, dvec& min, ivec& size){
    if(primIsInit){return;}
    xdistance = dist[0];
    ydistance = dist[1];
    zdistance = dist[2];
    xmin = min[0];
    ymin = min[1];
    zmin = min[2];
    xsize = size[0];
    ysize = size[1];
    zsize = size[2];
    initHull();
}
void pointCloudBool::deleteHull(){
    bmat().swap(hullCloud);
    hullIsInit = false;
}
//Access hullCloud:
void pointCloudBool::setHState(ivec& index, bool state){
    hullCloud[index[0]][index[1]][index[2]] = state;
}
void pointCloudBool::setHState(dvec& coord, bool state){
    ivec index(3);
    convCoordToIndex(coord, index);
    setHState(index, state);
}
void pointCloudBool::setHState(int xindex, int yindex, int zindex, bool state){
    ivec index = {xindex,yindex,zindex};
    setHState(index, state);
}
bool pointCloudBool::getHState(ivec& index){
    if( (index[0] < 0) || (index[1] < 0) || (index[2] < 0) ){
            return false;
        }
        else if( (index[0] >= xsize) || (index[1] >= ysize) || (index[2] >= zsize) ){
            return false;
        }else{
    return hullCloud[index[0]][index[1]][index[2]];
        }
}
bool pointCloudBool::getHState(dvec& coord){
    ivec index(3);
    convCoordToIndex(coord, index);
    return getHState(index);
}
bool pointCloudBool::getHState(int xindex, int yindex, int zindex){
    ivec index = {xindex,yindex,zindex};
    return getHState(index);
}

//Load/save:
void pointCloudBool::loadHull(std::string filename){
    std::ifstream ifile;
    ifile.open(filename);
    if(!ifile.is_open()){return;}

    ifile >> xdistance >> ydistance >> zdistance;
    ifile >> xmin >> ymin >> zmin;
    ifile >> xmax >> ymax >> zmax;
    ifile >> xsize >> ysize >> zsize;

    if(!hullIsInit){initHull();}

    std::string numstate;
    for(int i = 0; i < xsize; ++i){
        for(int j = 0; j < ysize; ++j){
            for(int k = 0; k < zsize; ++k){
                ifile >> numstate;
                ivec index = {i,j,k};
                if(numstate[0] == '1'){
                    setHState(index,true);
                }else{
                    setHState(index,false);
                }
            }
        }
    }
    ifile.close();
}
void pointCloudBool::saveHull(std::string filename){
    std::ofstream ofile;
    ofile.open(filename);
    if(!ofile.is_open()){return;}

    ofile << xdistance << " " << ydistance << " " << zdistance << "\n";
    ofile << xmin << " " << ymin << " " << zmin << "\n";
    ofile << xmax << " " << ymax << " " << zmax << "\n";
    ofile << xsize << " " << ysize << " " << zsize << "\n";

    for(int i = 0; i < xsize; ++i){
        for(int j = 0; j < ysize; ++j){
            for(int k = 0; k < zsize; ++k){
                ivec index = {i,j,k};
                if(getHState(index)){
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
void pointCloudBool::saveHullCoords(std::string filename){
    std::ofstream ofile;
    ofile.open(filename);
    if(!ofile.is_open()){return;}
    dvec coord(3);
    for(int i = 0; i < xsize; ++i){
        for(int j = 0; j < ysize; ++j){
            for(int k = 0; k < zsize; ++k){
                ivec index = {i,j,k};
                if(getHState(index)){
                    convIndexToCoord(index,coord);
                    ofile << coord[0] << " " << coord[1] << " " << coord[2] << "\n";
                }
            }
        }
    }
}
//Generate Hull:
void pointCloudBool::calcHull(){
    if(!hullIsInit){initHull();}

    ivec neighbour(3);
    //Check all points:
    for(int i = 0; i < xsize; ++i){
    for(int j = 0; j < ysize; ++j){
    for(int k = 0; k < zsize; ++k){
        ivec active = {i,j,k};

        if(getPState(active)){
                 if(!getPState(i-1,j-1,k-1)){setHState(active,true);}
            else if(!getPState(i-1,j-1,k  )){setHState(active,true);}
            else if(!getPState(i-1,j-1,k+1)){setHState(active,true);}
            else if(!getPState(i-1,j  ,k-1)){setHState(active,true);}
            else if(!getPState(i-1,j  ,k  )){setHState(active,true);}
            else if(!getPState(i-1,j  ,k+1)){setHState(active,true);}
            else if(!getPState(i-1,j-1,k-1)){setHState(active,true);}
            else if(!getPState(i-1,j-1,k  )){setHState(active,true);}
            else if(!getPState(i-1,j-1,k+1)){setHState(active,true);}

            else if(!getPState(i  ,j-1,k-1)){setHState(active,true);}
            else if(!getPState(i  ,j-1,k  )){setHState(active,true);}
            else if(!getPState(i  ,j-1,k+1)){setHState(active,true);}
            else if(!getPState(i  ,j  ,k-1)){setHState(active,true);}

            else if(!getPState(i  ,j  ,k+1)){setHState(active,true);}
            else if(!getPState(i  ,j-1,k-1)){setHState(active,true);}
            else if(!getPState(i  ,j-1,k  )){setHState(active,true);}
            else if(!getPState(i  ,j-1,k+1)){setHState(active,true);}

            else if(!getPState(i+1,j-1,k-1)){setHState(active,true);}
            else if(!getPState(i+1,j-1,k  )){setHState(active,true);}
            else if(!getPState(i+1,j-1,k+1)){setHState(active,true);}
            else if(!getPState(i+1,j  ,k-1)){setHState(active,true);}
            else if(!getPState(i+1,j  ,k  )){setHState(active,true);}
            else if(!getPState(i+1,j  ,k+1)){setHState(active,true);}
            else if(!getPState(i+1,j-1,k-1)){setHState(active,true);}
            else if(!getPState(i+1,j-1,k  )){setHState(active,true);}
            else if(!getPState(i+1,j-1,k+1)){setHState(active,true);}

        }

    }
    }
    }
}


//Mandelbulb:
void pointCloudBool::calcMandelBulb(int iter, int res, double power, double maxlength, double min, double max){
    double distance = std::abs(max - min) / res;
    dvec dist(3, distance);
    dvec minimum(3, min);
    ivec size(3, res);
    primIsInit = false;
    initPrimary(dist, minimum, size);

    double density = 0;
    double xpos, ypos, zpos;
    double xpoint, ypoint, zpoint;
    double cx, cy, cz;
    double r, phi, theta;

    //double progress = 0.0;
    //double progdiv = 100.0 / std::pow(double(res - 3), 3.0);

    for(xpos = min; xpos <= max; xpos += distance){
        for(ypos = min; ypos <= max; ypos += distance){
        for(zpos = min; zpos <= max; zpos += distance){

            //Display progress in console:
            //progress += progdiv;
            //ui->progressBar->setValue(progress);

            //reset for next point:
            xpoint = xpos;
            ypoint = ypos;
            zpoint = zpos;
            cx = xpos;
            cy = ypos;
            cz = zpos;

            //Sequence loop:
            for(int i = 0; i <= iter; i++)
            {
                r = sqrt(xpoint*xpoint + ypoint*ypoint + zpoint*zpoint);
                phi = atan(ypoint/xpoint);
                theta = acos(zpoint/r);

                xpoint = pow(r, power) * sin(power * theta) * cos(power * phi) + cx;
                ypoint = pow(r, power) * sin(power * theta) * sin(power * phi) + cy;
                zpoint = pow(r, power) * cos(power * theta) + cz;

                if(r >= maxlength)
                {
                    density = 0.0;
                    break;
                }
                density = 1.0;

            }

            if(density == 1.0)
            {
                dvec coord = {xpos,ypos,zpos};
                setPState(coord, true);
            }

        }//zpos loop end
        }//ypos loop end
        }//xpos loop end
}

void pointCloudBool::convIndexToCoord(ivec &index, dvec &coord){
    coord[0] = index[0] * xdistance + xmin;
    coord[1] = index[1] * ydistance + ymin;
    coord[2] = index[2] * zdistance + zmin;
}
void pointCloudBool::convCoordToIndex(dvec &coord, ivec &index){
    index[0] = (coord[0] - xmin) / xdistance;
    index[1] = (coord[1] - ymin) / ydistance;
    index[2] = (coord[2] - zmin) / zdistance;
}

