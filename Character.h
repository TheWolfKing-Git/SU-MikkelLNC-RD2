#ifndef CHARACTER_H
#define CHARACTER_H
#include <string>
#include <iostream>


class Character
{
public:
    //Constructor
    Character();
    Character(int InitialHP);

    //Getters
    int getHP() const;
    int getDMG() const;

    //Setters
    void setHP(int newHP);
    void setDMG(int newDMG);

    //Is alive?
    bool isAlive() const;

    //Take DMG
    void takeDMG(int incDMG);

protected:
    int mHP;
    int mDMG;
    std::string mName;
};

#endif // CHARACTER_H
