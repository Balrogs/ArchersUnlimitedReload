#ifndef ARCHERSUNLIMITED_JSONPARSER_H
#define ARCHERSUNLIMITED_JSONPARSER_H

#include <vector>
#include "rapidjson/document.h"
#include "GameEngine/Global/Misc/UI/Asset.h"
#include "EventInfo.h"
#include <GameEngine/Global/Misc/UI/Views.h>

using namespace rapidjson;
using namespace std;



class JSONParser {
public:
    static string parse(string message, string key);
    static string parseAnswer(string message, string key);
    static int parseInt(string message, string key);
    static int parseIntAnswer(string message, string key);
    static bool isError(string message);
    static bool isValid(string message);
    static string parseError(string message, string key);
    static float parseFloat(string message, string key);

    static std::vector<AssetInfo*> parseAsset(string key);
    static AssetInfo* parseAsset(string key, int id);
    static void setAssetAvailable(string key, int id);

    static int getListSize(string message);
    static vector<RankView *> parseRankings(string message);
    static std::vector<FriendView *> parseFriends(string message);

    static EventInfo* parseEvent(string message);

    static PlayerView *parsePlayerView(string message);

private:
    static std::vector<InfoButton*> parseInfo(string message);
    static std::vector<Reward*> parseRewards(string message);
    static std::vector<EventScore*> parseEventScore(string message);

};


#endif //ARCHERSUNLIMITED_JSONPARSER_H
