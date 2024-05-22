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
    std::string heroName;

    int heroPick;
    int enemyPick;

    Manager Game(gameHero, gameEnemy, DB);
    Game.addEnemies();

    while (true) {
        switch (GameState) {
            case 0:
                std::cout << "Select an option:" << std::endl;
                std::cout << "1: Start adventure with a new Hero! " << std::endl;
                std::cout << "2: Continue adventure with a Hero!" << std::endl;
                std::cout << "3: Show Heros ready for adventure!" << std::endl;
                std::cout << "4: Show Enemies for slaying!" << std::endl;
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

            case 3:
                Game.printHeros();
                GameState = 0;
                break;

            case 4:
                Game.printEnemies();
                GameState = 0;
                break;

            case 5:
                std::cout << "Adventure options:" << std::endl;
                std::cout << "1: Show available enemies for fighting: " << std::endl;
                std::cout << "2: Show Hero stats" << std::endl;
                std::cout << "3: Exit the adventure and save Hero" << std::endl;
                std::cout << "4: Exit the adventure without saving" << std::endl;
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
                        GameState = 6;
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
                //Print the stats on the hero
                else if(AdventureState == 2)
                {
                    Game.printHeroStats();
                    Game.printEnemyStats();
                    break;
                }
                //Save hero and go to main menu
                else if (AdventureState == 3)
                {
                    Game.saveHero();
                    GameState = 0;
                    break;

                }
                //Exit the program
                else if (AdventureState == 4)
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
            case 6:
                //10 = won, 20 = lost, -1 error;
                GameState = Game.Encounter();
                if(GameState == 10){
                    std::cout << "You won the fight!" << std::endl;
                    GameState = 5;
                    break;
                }
                else if(GameState == 20){
                    std::cout << "You lost the fight... Train against weaker enemies!" << std::endl;
                    GameState = 5;
                    break;
                }
                else if(GameState == -1){
                    std::cout << "An error has occured!" << std::endl;
                    GameState = 5;
                    break;
                }

            case 11:
                std::cout << "Exiting..." << std::endl;
                return 0;

            default:
                std::cout << "Invalid option. Please try again." << std::endl;
                GameState = 0;
                break;
        }
    }

    return 1;
}
