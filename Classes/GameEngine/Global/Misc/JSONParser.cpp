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
