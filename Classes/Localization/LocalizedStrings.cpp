
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
    string filepath = "Localized_" + language;
    auto str = cocos2d::FileUtils::getInstance()->getStringFromFile(filepath);
    _document = nullptr;
    _document.Parse(str.c_str());

    if(_document.HasParseError()){
        setLanguage("ENGLISH");
    }
}

const char *LocalizedStrings::getString(const char *message) {

    if (_document.HasMember(message) && _document[message].IsString()) {
        return _document[message].GetString();
    } else {
        //CCLOG(message);

        return message;
    }


}

