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

#include "internalentityhandler.h"

void internalEntityHandler::addEntity(internalEntity& entity){
    entity.id = getID();
    entityList.push_back(entity);
}
void internalEntityHandler::deleteEntity(int id){
    auto it = entityList.begin();
    do{
        if(it->id == id){
            entityList.erase(it);
            break;
        }
        ++it;
    }while(it != entityList.end());
}
internalEntity internalEntityHandler::returnEmptyEntity(){
    internalEntity entity;
    entity.isEmpty = true;
    entity.type = -1;
    entity.name = "nan";
    return entity;
}

void internalEntityHandler::getEntityAtID(int id, internalEntity &entity){
    auto it = entityList.begin();
    do{
        if(it->id == id){
            entity = *it;
            break;
        }
        ++it;
    }while(it != entityList.end());
}
int internalEntityHandler::getID(){
    nextID++;
    return nextID - 1;
}
