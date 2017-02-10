//
// Created by igor on 22.11.16.
//

#ifndef ARCHERSUNLIMITED_JSONPARSER_H
#define ARCHERSUNLIMITED_JSONPARSER_H

#include "rapidjson/document.h"

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
};


#endif //ARCHERSUNLIMITED_JSONPARSER_H
