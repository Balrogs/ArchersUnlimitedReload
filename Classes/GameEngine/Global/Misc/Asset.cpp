#include "Asset.h"


AssetInfo::AssetInfo(int id, std::string n, std::string p, bool a) {
    _id = id;
    _name = n;
    _path = p;
    _available = a;
}

std::string AssetInfo::Name() {
    if(isAvailable()){
        return _name;
    }
    return std::string();
}

std::string AssetInfo::Path() {
    if(isAvailable()){
        return _path;
    }
    return std::string();
}

bool AssetInfo::isAvailable() {
    return _available;
}

int AssetInfo::Id() {
    return _id;
}
