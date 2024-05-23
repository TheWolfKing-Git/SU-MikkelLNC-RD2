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
    std::string getName() const;
    int getGold() const;

    //Setters
    void setLevel(int newLevel);
    void setXP(int newXP);
    void setName(std::string newName);
    void setGold(int newGold);

    //Functions
    void addXP(int addedXP);
    void addLevel(int addedLevel);
    void resetHero();
    void addGold(int addedGold);

protected:
    int mLevel;
    int mCurrentXP;
    int mGold;
};

#endif // HERO_H
