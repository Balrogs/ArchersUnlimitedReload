#ifndef ARCHERSUNLIMITED_DBPLAYER_H
#define ARCHERSUNLIMITED_DBPLAYER_H

#include<string>
#include <GameEngine/Player.h>

using namespace std;


class DBPlayer {

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
    PlayerView* getView();

    void setId(int id);
    void setRoomId(int roomId);
    void setToken(string token);
    void setPassword(string password);
    void setName(string name);
    void setView(PlayerView* view);


    void setCountry(int country);

private:
    int _id;
    string _name;
    string _token;
    string _password;
    int _roomId;
    int _country;

    PlayerView* _view;
};


#endif //ARCHERSUNLIMITED_DBPLAYER_H
