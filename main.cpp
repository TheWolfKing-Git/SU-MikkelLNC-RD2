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
    Enemy gameEnemy;
    std::string heroName;
    int heroPick;
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
                gameHero = Hero(heroName);
                Game.setHero(gameHero);
                GameState = 5;
                break;

            case 2:
                std::cout << "Select a resting hero:" << std::endl;
                Game.printHeros();
                std::cin >> heroPick;
                gameHero = Game.loadHero(heroPick);
                Game.setHero(gameHero);
                GameState = 5;
                break;

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

                //Show enemies!
                if (AdventureState == 1)
                {
                    Game.printEnemies();
                    break;
                }
                else if(AdventureState == 2)
                {
                    std::cout << "---- Current Hero ----" << std::endl;
                    std::cout << "Hero name: " << gameHero.getName() << std::endl
                              << "HP: " << gameHero.getHP() << std::endl
                              << "DMG: " << gameHero.getDMG() << std::endl
                              << "Level: " << gameHero.getLevel() << std::endl
                              << "XP: " << gameHero.getCurrentXP()
                              << std::endl;
                    break;
                }
                else if (AdventureState == 3)
                {
                    Game.saveHero();
                    return 1;

                }
                else if (AdventureState == 4)
                {
                    return 1;
                }
                else
                {

                }


            case 11:
                std::cout << "Exiting..." << std::endl;
                return 0;

            default:
                std::cout << "Invalid option. Please try again." << std::endl;
                break;
        }
    }

    return 1;
}
