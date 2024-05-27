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
    std::string getElement() const;

    //Setters
    void setXPReward(int newXPReward);
    void setElement(std::string newElement);

protected:
    int mXPReward;
    std::string mElement;
};

#endif // ENEMY_H
