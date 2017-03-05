//
// Created by igor on 01.03.17.
//

#ifndef ARCHERSUNLIMITED_ASSET_H
#define ARCHERSUNLIMITED_ASSET_H


#include <string>

class AssetInfo {
public:
    AssetInfo(int id, std::string n, std::string p, bool a);
    int Id();
    std::string Name();
    std::string Path();
    bool isAvailable();
private:
    int _id;
    std::string _name;
    std::string _path;
    bool _available;
};


#endif //ARCHERSUNLIMITED_ASSET_H
