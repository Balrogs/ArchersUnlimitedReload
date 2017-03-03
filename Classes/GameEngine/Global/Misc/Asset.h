//
// Created by igor on 01.03.17.
//

#ifndef ARCHERSUNLIMITED_ASSET_H
#define ARCHERSUNLIMITED_ASSET_H


#include <string>

class AssetInfo {
public:
    AssetInfo(std::string n, std::string p, bool a);
    std::string name;
    std::string path;
    bool available;
};


#endif //ARCHERSUNLIMITED_ASSET_H
