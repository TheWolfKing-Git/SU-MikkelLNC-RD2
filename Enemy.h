#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"


class Enemy : public Character
{
public:
    Enemy();

    //Getters
    int getXPReward() const;

    //Setters
    void setXPReward(int newXPReward);

private:
    int mXPReward;
};

#endif // ENEMY_H
