
#include <fstream>
#include "cocos2d.h"
#include "LocalizedStrings.h"

static LocalizedStrings *_instance = nullptr;

LocalizedStrings *LocalizedStrings::getInstance() {
    if(_instance == nullptr){
        _instance = new LocalizedStrings();
    }
    return _instance;
}


void LocalizedStrings::setLanguage(string language) {
    _language = language;
    string filepath = cocos2d::StringUtils::format("Localized_%s", _language.c_str());
    _document = cocos2d::FileUtils::getInstance()->getStringFromFile(filepath);
}

const char *LocalizedStrings::getString(const char *message) {
    if (_document.length() == 0) {
        setLanguage("ENGLISH");
    }
    Document document;
    document.Parse(_document.c_str());
    if (document.HasMember(message) && document[message].IsString()) {
        return document[message].GetString();
    } else {
        return "";
    }

}

