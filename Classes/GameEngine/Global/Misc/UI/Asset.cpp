#include "Asset.h"


AssetInfo::AssetInfo(int id, std::string n, std::string p, bool a) {
    _id = id;
    _name = n;
    _path = p;
    _available = a;
}

std::string AssetInfo::Name() {
    return _name;
}

std::string AssetInfo::Path() {
    return _path;
}

bool AssetInfo::isAvailable() {
    return _available;
}

int AssetInfo::Id() {
    return _id;
}
