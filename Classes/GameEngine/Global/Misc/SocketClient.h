#ifndef ARCHERSUNLIMITED_SOCKETCLIENT_H
#define ARCHERSUNLIMITED_SOCKETCLIENT_H

#include<string.h>
#include<string>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include "cocos2d.h"

using namespace std;

class DBPlayer {

private:
    int _id;
    string _name;
    string _token;
    string _password;
    int _roomId;
    int _country;

public:
    DBPlayer();
    DBPlayer(int id, string password, string name, int country);

    int getId();
    string getToken() ;
    string getName() ;
    string getPassword() ;
    int getRoomId() ;
    int getCountry() ;

    void setId(int id);
    void setRoomId(int roomId);
    void setToken(string token);
    void setPassword(string password);
    void setName(string name);
};


class SocketClient {

private:
    SocketClient();
    int _sock;
    string _address;
    int _port;
    struct sockaddr_in _server;
    bool _isConnected;
    DBPlayer* _player;

    std::vector<std::string> _buffer;

    bool _conn(string, int);
    bool sendMessage(string data);

    string _parseError(int error);
    void _parseReply(string reply);

    void receive();

public:

    static SocketClient* getInstance();
    DBPlayer* getDBPlayer();

    bool connected();

    // {"id":1,"password":"wqwf","playerView":{"color":{"red":0,"green":0,"blue":0},"helmet":0,"hood":0, "bow":0},"code":1}
    void login();
    // {"name":"some username12","country":1,"password":"1234","code":2}
    void registerUser(string name, int country, string password);
    //{"player_name":"123","player_rank":1,"room_id":55,"token":{"id":-1,"token":"server"}}
    void enterRoom();
    // {"player_id":0,"room_id":1,"angle":180.0, "power":100.0, "arrow":"nikita lox", "token":{"id":0,"token":"⥿貧ဵට觱鞻㊆ꢖ풶꿘儐➧責鄆읽领ྯք뤘櫨"}, "code":6}
    void action(float angle, float power, int type);
    // {"player_id":1, "room_id":1,"token":{"id":1,"token":"顥鵰泏茙퍨ꅑꯛ೜㑓Ꮖ軼쭏籱ƹ槞⃥뚭薿咂튑"},"code":4}
    void invite(int playerId, int roomId);
    // {"id":0, "friend_id":1, "code":11}
    void addToFriends(int friendId);
    //{"s_type":3, "name":"some other username","code":9}
    void getPlayerInfo(int s_type, string playerName);

};


#endif //ARCHERSUNLIMITED_SOCKETCLIENT_H
