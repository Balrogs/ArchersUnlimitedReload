#include <platform/CCFileUtils.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <GameEngine/Global/Misc/UI/Views.h>
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

void JSONParser::setAssetAvailable(string key, int id) {
    auto content = cocos2d::FileUtils::getInstance()->getStringFromFile("assets/assets.json");
    Document document;
    document.Parse(content.c_str());

    Value& parent = document[key.c_str()];
    Value& asset = parent[id];
    asset["available"].SetBool(true);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);
    auto fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("assets/assets.json");
    cocos2d::FileUtils::getInstance()->writeStringToFile(buffer.GetString(), fullPath);

}

EventInfo *JSONParser::parseEvent(string message) {

    Document document;
    document.Parse(message.c_str());

    if (document.HasParseError() &&
            !document.HasMember("id") &&
            !document.HasMember("type") &&
            !document.HasMember("date_end") &&
            !document.HasMember("infos") &&
            !document.HasMember("rewards")) {
        return nullptr;
    }

    auto id = document["id"].GetInt();
    auto type = document["description"]["e_type"].GetInt();

    struct tm tm;
    auto time = document["date_end"].GetString();
    strptime(time, "%Y-%m-%dT%H:%M:%S", &tm);
    auto endTime = mktime(&tm);

    auto infos = parseInfo(message);
    auto rewards = parseRewards(message);
    auto scores = parseEventScore(message);

    return new EventInfo(id, type, endTime, infos, rewards, scores);
}

std::vector<InfoButton *> JSONParser::parseInfo(string message) {
    std::vector<InfoButton*> infos;
    Document document;
    document.Parse(message.c_str());
    Value& b = document["description"]["infos"];

    for (rapidjson::SizeType i = 0; i < b.Size(); i++)
    {
        const Value& c = b[i];

        infos.push_back(InfoButtonTime::create());
    }

    return infos;
}

std::vector<Reward *> JSONParser::parseRewards(string message) {
    std::vector<Reward*> rewards;
    Document document;
    document.Parse(message.c_str());
    Value& b = document["description"]["rewards"];

    for (rapidjson::SizeType i = 0; i < b.Size(); i++)
    {
        const Value& c = b[i];
        switch(c.GetInt()){
            case 1:
                rewards.push_back(RewardArrow::create());
                break;
            case 2:
                rewards.push_back(RewardBow::create());
                break;
            case 3:
                rewards.push_back(RewardHat::create());
                break;
            case 4:
                rewards.push_back(RewardCoin::create(100));
                break;
            case 5:
                rewards.push_back(RewardCoin::create(250));
                break;
            case 6:
                rewards.push_back(RewardCoin::create(500));
                break;
            case 7:
                rewards.push_back(RewardCoin::create(750));
                break;
            case 8:
                rewards.push_back(RewardCoin::create(1000));
                break;
            default:
                break;
        }
    }
    return rewards;
}

std::vector<EventScore *> JSONParser::parseEventScore(string message) {
    std::vector<EventScore*> scores;
//    Document document;
//    document.Parse(message.c_str());
//    Value& b = document["scores"];
//
//    for (rapidjson::SizeType i = 0; i < b.Size(); i++)
//    {
//        const Value& c = b[i];
//
//        scores.push_back(new EventScore(c["name"].GetString(), c["score"].GetInt()));
//    }

    return scores;
}

std::vector<RankView *> JSONParser::parseRankings(string message) {
    std::vector<RankView *> ranks;
    Document document;
    document.Parse(message.c_str());
    Value& b = document["list"];

    for (rapidjson::SizeType i = 0; i < b.Size(); i++)
    {
        const Value& c = b[i];

        ranks.push_back(new RankView(c["player_name"].GetString(), c["country"].GetInt(), c["rank"].GetInt()));
    }

    return  ranks;
}

std::vector<FriendView *> JSONParser::parseFriends(string message) {
    std::vector<FriendView *> ranks;
    Document document;
    document.Parse(message.c_str());
    Value& b = document["friends"];

    for (rapidjson::SizeType i = 0; i < b.Size(); i++)
    {
        const Value& c = b[i];

        ranks.push_back(new FriendView(c["player_name"].GetString(), c["player_id"].GetInt(), c["online"].GetBool()));
    }

    return  ranks;
}


int JSONParser::getListSize(string message) {
    Document document;
    document.Parse(message.c_str());
    return document["list"].GetArray().Size();
}

//TODO
PlayerView *JSONParser::parsePlayerView(string message) {
    return new PlayerView(2, 2, 2);
}
