#ifndef ARCHERSUNLIMITED_DBPLAYER_H
#define ARCHERSUNLIMITED_DBPLAYER_H

#include<string>

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
    bool canLogin();

    void setId(int id);
    void setRoomId(int roomId);
    void setToken(string token);
    void setPassword(string password);
    void setName(string name);
};

#endif //ARCHERSUNLIMITED_DBPLAYER_H
