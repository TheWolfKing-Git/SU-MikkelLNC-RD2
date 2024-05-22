#ifndef HERO_H
#define HERO_H
#include "Character.h"

class Hero : public Character
{
public:
    Hero();

    //Getters
    int getLevel() const;
    int getCurrentXP() const;

    //Setters
    void setLevel(int newLevel);
    void setXP(int newXP);

    //Functions
    void addXP(int addXP);

private:
    int mLevel;
    int mCurrentXP;
};

#endif // HERO_H
