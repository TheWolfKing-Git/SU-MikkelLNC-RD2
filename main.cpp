#include <QCoreApplication>
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include <iostream>

#include "Manager.h"

int main()
{

    //Setup database
    QSqlDatabase DB = QSqlDatabase::addDatabase("QMYSQL");
    DB.setHostName("localhost"); // Currently set to localhost
    DB.setUserName("sammy"); // Change to username
    DB.setPassword("password"); // Change to password

    int GameState = 0;
    int AdventureState = 0;

    Hero gameHero;
    Hero my_Hero;
    Enemy gameEnemy;
    Magic gameMagic;
    std::string heroName;

    int heroPick;
    int enemyPick;
    int cavePick;
    QList<int> cavePickedEnemies;
    int caveGold;
    int pickedMagicForPurchase;
    int enemyCount = 0;

    Manager Game(gameHero, gameEnemy, gameMagic, DB);
    Game.addEnemies();
    Game.addCavesToGame();
    Game.addEnemiesToCaves();
    Game.addMagics();

    while (true) {
        switch (GameState) {
            case 0:
                std::cout << "Select an option:" << std::endl;
                std::cout << "1: Start adventure with a new Hero! " << std::endl;
                std::cout << "2: Continue adventure with a Hero!" << std::endl;
                std::cout << "7: Show Heros ready for adventure!" << std::endl;
                std::cout << "8: Show Enemies for slaying!" << std::endl;
                std::cout << "9: Show The magics of this world!" << std::endl;
                std::cout << "11: Exit" << std::endl;
                std::cout << std::endl;
                std::cin >> GameState;
                break;

            case 1:
                std::cout << "Starting new adventure!" << std::endl;
                std::cout << "Name your Hero: " << std::endl;
                std::cin >> heroName;
                my_Hero = Hero(heroName);
                Game.setHero(my_Hero);
                Game.saveHero();
                GameState = 5;
                break;

            case 2:
                std::cout << "Select a resting hero:" << std::endl;
                Game.printHeros();
                std::cin >> heroPick;

                // Attempt to load the hero
                try
                {
                    gameHero = Game.loadHero(heroPick);
                    Game.setHero(gameHero);
                    GameState = 5;
                    break;
                }
                // Handle the case where no hero is found
                catch (const std::runtime_error& e)
                {
                    std::cerr << "Failed to load hero: " << e.what() << std::endl;
                    GameState = 0;
                    break;
                }

            //Print hero case
            case 7:
                Game.printHeros();
                GameState = 0;
                break;

            case 8:
                Game.printEnemies();
                GameState = 0;
                break;
            case 9:
                Game.printMagics();
                GameState = 0;
                break;

            case 5:
                std::cout << "Adventure options:" << std::endl;
                std::cout << "1: Show available enemies for fighting" << std::endl;
                std::cout << "2: Show caves for conquering" << std::endl;
                std::cout << "3: Show Hero stats" << std::endl;
                std::cout << "4: Buy Magic for this hero" << std::endl;
                std::cout << "5: Show Magics for this hero" << std::endl;
                std::cout << "9: Exit the adventure and save Hero" << std::endl;
                std::cout << "11: Exit the adventure without saving" << std::endl;
                std::cin >> AdventureState;

                //Show and fight an enemy!
                if (AdventureState == 1)
                {
                    std::cout << "Select an enemy to encounter:" << std::endl;
                    Game.printEnemies();
                    std::cin >> enemyPick;
                    // Attempt to load the enemy
                    try
                    {
                        gameEnemy = Game.loadEnemy(enemyPick);
                        Game.setEnemy(gameEnemy);
                        GameState = 100;
                        break;
                    }
                    catch (const std::runtime_error& e)
                    {
                        std::cerr << "Failed to load enemy: " << e.what() << std::endl;
                        GameState = 5;
                        break;
                    }
                    break;
                }
                else if(AdventureState == 2)
                {
                    std::cout << "Select a cave to conquer: " << std::endl;
                    Game.printCaves();
                    std::cin >> cavePick;;
                    std::cout << "These are your foes: " << std::endl;
                    Game.printCaveEnemies(cavePick);
                    cavePickedEnemies = Game.getEnemiesForCave(cavePick);
                    caveGold = Game.getGoldFromCave(cavePick);
                    GameState = 110;
                    break;

                }
                //Print the stats on the hero
                else if(AdventureState == 3)
                {
                    Game.printHeroStats();
                    break;
                }
                //Buy Magics
                else if(AdventureState == 4)
                {
                    std::cout << "Select an magic to purchase:" << std::endl;
                    Game.printMagics();
                    std::cin >> pickedMagicForPurchase;
                    // Attempt buy magic
                    try
                    {
                        Game.buyMagic(pickedMagicForPurchase);
                        GameState = 5;
                        break;
                    }
                    catch (const std::runtime_error& e)
                    {
                        std::cerr << "Failed to buy magic: " << e.what() << std::endl;
                        GameState = 5;
                        break;
                    }
                }
                //Show Magics for hero
                else if(AdventureState == 5)
                {
                    // Attempt show magic
                    try
                    {
                        Game.printHeroMagics();
                        GameState = 5;
                        break;
                    }
                    catch (const std::runtime_error& e)
                    {
                        std::cerr << "Failed to show magics: " << e.what() << std::endl;
                        GameState = 5;
                        break;
                    }
                }
                //Save hero and go to main menu
                else if (AdventureState == 9)
                {
                    Game.saveHero();
                    std::cout << "Your hero takes a rest at the local inn, and is ready when you return!" << std::endl;
                    GameState = 0;
                    break;

                }
                //Exit the program
                else if (AdventureState == 11)
                {
                    return 1;
                }
                //Else just to ensure player can reach menu again if a mistake is made
                else
                {
                    std::cout << "Not a vaild selection, returning..." << std::endl;
                    AdventureState = 0;
                    GameState = 5;
                    break;
                }

            case 11:
                std::cout << "Exiting..." << std::endl;
                return 0;

            case 100:
                //10 = won, 20 = lost, 30 = died casting magic, -1 error;
                GameState = Game.EncounterWithMagic();
                if(GameState == 10){
                    std::cout << "You won the fight!" << std::endl;
                    std::cout << "" << std::endl;
                    GameState = 5;
                    break;
                }
                else if(GameState == 20){
                    std::cout << "You lost the fight... Train against weaker enemies!" << std::endl;
                    std::cout << "" << std::endl;
                    GameState = 5;
                    break;
                }
                else if(GameState == 30){
                    std::cout << "You died while casting a spell..." << std::endl;
                    std::cout << "" << std::endl;
                    GameState = 5;
                    break;
                }
                else if(GameState == -1){
                    std::cout << "An error has occured!" << std::endl;
                    std::cout << "" << std::endl;
                    GameState = 5;
                    break;
                }

            case 110:
                //10 = won, 20 = lost, -1 error,
                enemyCount = 0;
                std::cout << "You arrive in the cave and start the struggle!" << std::endl;
                std::cout << "" << std::endl;
                foreach(int enemyIDs, cavePickedEnemies)
                {
                    enemyCount++;
                    gameEnemy = Game.loadEnemy(enemyIDs);
                    Game.setEnemy(gameEnemy);
                    GameState = Game.EncounterWithMagic();
                    if(GameState == 10)
                    {
                        std::cout << "You won the fight against enemy# " << enemyCount<<  " " << gameEnemy.getName() << std::endl;
                        std::cout << "The fight continues!" << std::endl;
                        std::cout << "" << std::endl;
                    }
                    else if(GameState == 20)
                    {
                        std::cout << "You lost the fight against enemy# " << enemyCount << " " << gameEnemy.getName() << std::endl;
                        std::cout << "Train against weaker enemies!" << std::endl;
                        std::cout << "" << std::endl;
                        GameState = 5;
                        break;
                    }
                    else if(GameState == 30){
                        std::cout << "You died while casting a spell..." << std::endl;
                        std::cout << "" << std::endl;
                        GameState = 5;
                        break;
                    }
                    else if(GameState == -1)
                    {
                        std::cout << "An error has occured in cave fight!" << std::endl;
                        std::cout << "" << std::endl;
                        GameState = 5;
                        break;
                    }
                }
                std::cout << "You won the cave fight! " << caveGold << " gold has been added to your purse!" <<std::endl;
                std::cout << "" << std::endl;
                Game.addGoldFromCave(cavePick);
                GameState = 5;
                break;



            default:
                std::cout << "Invalid option. Please try again." << std::endl;
                GameState = 0;
                break;
        }
    }

    return 1;
}
