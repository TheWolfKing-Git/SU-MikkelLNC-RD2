#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"


class Enemy : public Character
{
public:
    //Constructors
    Enemy();
    Enemy(std::string newName);

    //Getters
    int getXPReward() const;
    std::string getName() const;

    //Setters
    void setXPReward(int newXPReward);

protected:
    int mXPReward;
};

#endif // ENEMY_H
