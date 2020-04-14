#include "boolcloud.h"
#include <string>
#include <cmath>

//inline reason: Compiler puts these functions into several files and
//then complains that there are multiple definitions of it. Inline fixes that.

inline std::string getFileExt(std::string& filePath, std::string defaultExtension){
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

inline std::string getFileExt(std::string& filePath){
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

inline void setFileExt(std::string &filePath, std::string defaultExtension){
    std::string extension = getFileExt(filePath, defaultExtension);
    if(extension != defaultExtension){
        for(int i = 0; i < extension.size(); ++i){
            filePath.pop_back();
        }
        filePath.append(defaultExtension);
    }
}

inline double calcDistance(dvec& pointA, dvec& pointB){
    return sqrt( pow(pointA[0] - pointB[0], 2.0) + pow(pointA[1] - pointB[1], 2.0) + pow(pointA[2] - pointB[2], 2.0) );
}

inline bool equalPoints(dvec& pointA, dvec& pointB){
    for(int i = 0; i < pointA.size(); ++i){
        if(pointA[i] != pointB[i]){
            return false;
        }
    }
    return true;
}

inline void lineToWords(const std::string& line, std::vector<std::string>& stringVec){
    std::string word;
    stringVec.resize(0);
    for(int i = 0; i < line.size(); ++i){
        if(line[i] == ' '){
            stringVec.push_back(word);
            word.resize(0);
        }else{
            word.push_back(line[i]);
        }
    }


}
