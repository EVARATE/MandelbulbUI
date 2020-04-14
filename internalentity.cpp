/*
Copyright 2020 Sebastian Motzet

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

#include "internalentity.h"

void internalEntity::addProperty(std::string name, std::string value){
    internalProperty prop;
    prop.name = name;
    prop.value = value;
    properties.push_back(prop);
}
void internalEntity::addProperty(std::string name, int value){
    std::string val = std::to_string(value);
    addProperty(name, val);
}
void internalEntity::addProperty(std::string name, double value){
    std::string val = std::to_string(value);
    addProperty(name, val);
}
std::string internalEntity::getTypeName(){
    switch(type){
    case 0:
        return "boolCloud";
    case 1:
        return "triMesh";
    case 2:
        return "pointCloud";
    default:
        return "unknown";
    }
}
