#include "internalentityhandler.h"

void internalEntityHandler::addEntity(internalEntity& entity){
    entity.id = getID();
    entityList.push_back(entity);
}
void internalEntityHandler::deleteEntity(int id){
    std::list<internalEntity>::iterator it = entityList.begin();
    do{
        if(it->id == id){
            entityList.erase(it);
            break;
        }
        ++it;
    }while(it != entityList.end());
}

void internalEntityHandler::getEntityAtID(int id, internalEntity &entity){
    std::list<internalEntity>::iterator it = entityList.begin();
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
