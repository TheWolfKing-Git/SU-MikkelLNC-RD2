#ifndef MAGIC_H
#define MAGIC_H
#include <string>


class Magic
{
public:
    Magic();

    //Getters
    int getID() const;
    std::string getName() const;
    int getDMG() const;
    int getSelfDMG() const;
    std::string getElement() const;
    std::string getStrength() const;
    std::string getWeakness() const;
    int getManaCost() const;
    int getGoldCost() const;

    //Setters
    void setID(int newID);
    void setName(std::string newName);
    void setDMG(int newDMG);
    void setSelfDMG(int newSelfDMG);
    void setElement(std::string newElement);
    void setStrength(std::string newStrengt);
    void setWeakness(std::string newWeakness);
    void setManaCost(int newManaCost);
    void setGoldCost(int newGoldCost);

private:
    int mID;
    std::string mName;
    int mDMG;
    int mSelfDMG;
    std::string mElement;
    std::string mStrengt;
    std::string mWeakness;
    int mManaCost;
    int mGoldCost;

};

#endif // MAGIC_H
