#include <platform/CCFileUtils.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "JSONParser.h"

string JSONParser::parse(string message, string key) {
    Document document;
    document.Parse(message.c_str());
    if (!document.HasParseError() && document.HasMember("answer") && document["answer"].IsString()) {
        document.Parse(document["answer"].GetString());
        if (!document.HasParseError() && document.HasMember(key.c_str()) && document[key.c_str()].IsString()) {
            return document[key.c_str()].GetString();
        }
    }
    return "";
}

int JSONParser::parseInt(string message, string key) {
    Document document;
    document.Parse(message.c_str());
    if (!document.HasParseError() && document.HasMember("answer") && document["answer"].IsString()) {
        document.Parse(document["answer"].GetString());
        if (!document.HasParseError() && document.HasMember(key.c_str()) && document[key.c_str()].IsInt()) {
            return document[key.c_str()].GetInt();
        }
    }
    return 0;
}


float JSONParser::parseFloat(string message, string key) {
    Document document;
    document.Parse(message.c_str());
    if (!document.HasParseError() && document.HasMember(key.c_str()) && document[key.c_str()].IsFloat()) {
        return document[key.c_str()].GetFloat();
    }
    return 0;
}


bool JSONParser::isError(string message) {
    if(message.empty()){
        return false;
    }

    Document document;
    document.Parse(message.c_str());
    if (!document.HasParseError() && document.HasMember("answer") && document["answer"].IsString()) {
        return atoi(document["answer"].GetString()) < 0;
    }
    return false;
}

string JSONParser::parseError(string message, string key) {
    Document document;
    document.Parse(message.c_str());
    if (!document.HasParseError() && document.HasMember("answer") && document["answer"].IsString()) {
        return document[key.c_str()].GetString();
    }
    return "";
}

string JSONParser::parseAnswer(string message, string key) {
    Document document;
    document.Parse(message.c_str());
    if (!document.HasParseError() && document.HasMember(key.c_str()) && document[key.c_str()].IsString()) {
        return document[key.c_str()].GetString();
    }
    return "";
}

int JSONParser::parseIntAnswer(string message, string key) {
    Document document;
    document.Parse(message.c_str());
    if (!document.HasParseError() && document.HasMember(key.c_str()) && document[key.c_str()].IsInt()) {
        return document[key.c_str()].GetInt();
    }
    return 0;
}

bool JSONParser::isValid(string message) {
    Document document;
    document.Parse(message.c_str());
    return !document.HasParseError();
}

std::vector<AssetInfo *> JSONParser::parseAsset(string key) {
    auto content = cocos2d::FileUtils::getInstance()->getStringFromFile("assets/assets.json");
    Document document;
    document.Parse(content.c_str());
    std::vector<AssetInfo*> assets;

    const Value& b = document[key.c_str()];

    for (rapidjson::SizeType i = 0; i < b.Size(); i++)
    {
        const Value& c = b[i];

        assets.push_back(new AssetInfo(c["id"].GetInt(), c["name"].GetString(), c["path"].GetString(), c["available"].GetBool()));
    }

    return assets;
}

AssetInfo *JSONParser::parseAsset(string key, int id) {
    auto assets = parseAsset(key);
    for(auto asset : assets){
        if(asset->Id() == id){
            return asset;
        }
    }
    return nullptr;
}

//TODO test this
void JSONParser::setAssetAvailable(string key, int id) {
    auto content = cocos2d::FileUtils::getInstance()->getStringFromFile("assets/assets.json");
    Document document;
    document.Parse(content.c_str());
    std::vector<AssetInfo*> assets;

    Value& parent = document[key.c_str()];
    Value& asset = parent[id];
    asset["available"].SetBool(true);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);
    auto fullpath = cocos2d::FileUtils::getInstance()->fullPathForFilename("assets/assets.json");
    cocos2d::FileUtils::getInstance()->writeStringToFile(buffer.GetString(), fullpath);

}
