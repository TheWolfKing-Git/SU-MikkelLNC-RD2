#ifndef HERO_H
#define HERO_H
#include "Character.h"
#include "Magic.h"

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
    int getMana() const;

    //Setters
    void setLevel(int newLevel);
    void setXP(int newXP);
    void setName(std::string newName);
    void setGold(int newGold);
    void setMana(int newMana);

    //Functions
    void addXP(int addedXP);
    void addLevel(int addedLevel);
    void resetHero();
    void addGold(int addedGold);
    void subGold(int subedGold);
    void addMana(int addedMana);
    void subMana(int subedMana);
    void healHP(int healedHP);
    void healMana(int healedMana);

protected:
    int mLevel;
    int mCurrentXP;
    int mGold;
    int mMana;
    Magic mMagic;
};

#endif // HERO_H
