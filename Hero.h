#ifndef HERO_H
#define HERO_H
#include "Character.h"

class Hero : public Character
{
public:
    //Constructors
    Hero();
    Hero(std::string newName);

    //Getters
    int getLevel() const;
    int getCurrentXP() const;

    //Setters
    void setLevel(int newLevel);
    void setXP(int newXP);

    //Functions
    void addXP(int addedXP);
    void addLevel(int addedLevel);

protected:
    int mLevel;
    int mCurrentXP;
};

#endif // HERO_H
