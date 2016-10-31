//
// Created by igor on 05.10.16.
//

#ifndef ARCUNLIM_PLAYER_H
#define ARCUNLIM_PLAYER_H


class Player {
public:
    Player(int id, const char *name);

private:
    int _id;
    const char *_name;
    int _score;
    int _shotsCount;
public:

    int getShotsCount() const;

    void addShotsCount();

    int getId() const;

    const char * getName() const;

    int getScore() const;

    void addScore(int _score);

};


#endif //ARCUNLIM_PLAYER_H
