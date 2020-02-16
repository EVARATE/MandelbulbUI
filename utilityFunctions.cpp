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

#include <string>

std::string getFileExt(std::string& filePath, std::string defaultExtension){
    std::string revExt;
    bool noDot = false;
    for(int i = filePath.size() -1; i >= 0; --i){
        if(filePath[i] == '.'){
            break;
        }
        else if( filePath[i] == '/' ){
            noDot = true;
            break;
        }
        else{
            revExt.push_back(filePath[i]);
        }
        if(i == 0){
            noDot = true;
        }
    }
    if(!noDot){
        std::string ext;
        for(int i = revExt.size() - 1; i >= 0; --i){
            ext.push_back(revExt[i]);
        }
        return ext;
    }else{
        filePath.append('.' + defaultExtension);
        return defaultExtension;
    }
}

std::string getFileExt(std::string& filePath){
    std::string revExt;
    bool noDot = false;
    for(int i = filePath.size() -1; i >= 0; --i){
        if(filePath[i] == '.'){
            break;
        }
        else if( filePath[i] == '/' ){
            noDot = true;
            break;
        }
        else{
            revExt.push_back(filePath[i]);
        }
        if(i == 0){
            noDot = true;
        }
    }
    if(!noDot){
        std::string ext;
        for(int i = revExt.size() - 1; i >= 0; --i){
            ext.push_back(revExt[i]);
        }
        return ext;
    }else{
        return " ";
    }
}
