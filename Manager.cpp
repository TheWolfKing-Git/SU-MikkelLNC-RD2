#include "Manager.h"

Manager::Manager(Hero hero, Enemy enemy, QSqlDatabase gameDatabase)
{

        mHero = hero;
        mEnemy = enemy;
        mDB = gameDatabase;

        //Check open connection
        if (!mDB.open()) {
            qDebug() << "Failed to connect to database:";
            qDebug() << mDB.lastError().text();
        }

        // Ensure the connection is set to the "Game" database
        if (mDB.databaseName() != "Game") {
            if (!mDB.isOpen()) {
                qDebug() << "Failed to select database:";
                qDebug() << mDB.lastError().text();
            } else {
                mDB.setDatabaseName("Game");
            }
        }

        //Create main database for game
        QString createDB = "CREATE DATABASE IF NOT EXISTS Game";
        if (!mGameQuery.exec(createDB)) {
            qDebug() << "Failed to create database:";
            qDebug() << mGameQuery.lastError().text();

        }

        //Change the current database to "Game"
        mDB.setDatabaseName("Game");
        if (!mDB.open()) {
            qDebug() << "Failed to select database:";
            qDebug() << mDB.lastError().text();
        }

        //Create table for Hero
        QString createTB1 = "CREATE TABLE IF NOT EXISTS Hero ("
                "ID INT AUTO_INCREMENT PRIMARY KEY,"
                "Name VARCHAR(255) UNIQUE,"
                "HP INT,"
                "DMG INT,"
                "Level INT,"
                "XP INT,"
                "Gold INT"
                ")";
        if (!mGameQuery.exec(createTB1)) {
            qDebug() << "Failed to create Hero table:";
            qDebug() << mGameQuery.lastError().text();
        }

        //Create table for Enemy
        QString createTB2 = "CREATE TABLE IF NOT EXISTS Enemy ("
                "ID INT AUTO_INCREMENT PRIMARY KEY,"
                "Name VARCHAR(255) UNIQUE,"
                "HP INT,"
                "DMG INT,"
                "XPReward INT"
                ")";
        if (!mGameQuery.exec(createTB2)) {
            qDebug() << "Failed to create Enemy table:";
            qDebug() << mGameQuery.lastError().text();
        }

        // Create table for Caves
        QString createTB3 = "CREATE TABLE IF NOT EXISTS Caves ("
                            "CaveID INT AUTO_INCREMENT PRIMARY KEY,"
                            "Name VARCHAR(255) UNIQUE,"
                            "GoldReward INT"
                            ")";
        if (!mGameQuery.exec(createTB3)) {
            qDebug() << "Failed to create Caves table:";
            qDebug() << mGameQuery.lastError().text();
        }


        //Drop CaveEnemies, to ensure fresh table. Create after drop.
        QString dropTB4 = "DROP TABLE IF EXISTS CaveEnemies";
        if (!mGameQuery.exec(dropTB4)) {
            qDebug() << "Failed to drop CaveEnemies table:";
            qDebug() << mGameQuery.lastError().text();
        }
        QString createTB4 = "CREATE TABLE IF NOT EXISTS CaveEnemies ("
                            "ID INT AUTO_INCREMENT PRIMARY KEY,"
                            "CaveID INT,"
                            "EnemyID INT,"
                            "FOREIGN KEY (CaveID) REFERENCES Caves(CaveID),"
                            "FOREIGN KEY (EnemyID) REFERENCES Enemy(ID)"
                            ")";
        if (!mGameQuery.exec(createTB4)) {
            qDebug() << "Failed to create CaveEnemies table:";
            qDebug() << mGameQuery.lastError().text();
        }

        QString dropTB5 = "DROP TABLE IF EXISTS Magic";
        if (!mGameQuery.exec(dropTB5)) {
            qDebug() << "Failed to drop Magic table:";
            qDebug() << mGameQuery.lastError().text();
        }
        // Create table for Magic
        QString createTB5 = "CREATE TABLE IF NOT EXISTS Magic ("
                            "ID INT AUTO_INCREMENT PRIMARY KEY,"
                            "Name VARCHAR(255) UNIQUE,"
                            "DMG INT,"
                            "SelfDMG INT,"
                            "Element VARCHAR(255),"
                            "Strengt VARCHAR(255),"
                            "Weakness VARCHAR(255),"
                            "ManaCost INT,"
                            "GoldCost INT"
                            ")";
        if (!mGameQuery.exec(createTB5)) {
            qDebug() << "Failed to create Magic table:";
            qDebug() << mGameQuery.lastError().text();
        }
}
//------------------------------------------------ Enemy Handling --------------------------------------------------------
void Manager::setEnemy(Enemy newEnemy)
{
    mEnemy = newEnemy;
}

void Manager::addEnemies()
{
    //After DB creation, insert enemies
    QList<QVariantList> enemies = {
        {"Weak", 2, 1, 500},
        {"Medium", 5, 2, 1000},
        {"Strong", 10, 5, 5000},
        {"Very Strong", 50, 20, 10000},
        {"DEATH", 1000, 1000, 1}
    };


    for (const auto& enemy : enemies) {
        mGameQuery.prepare("INSERT INTO Enemy (Name, HP, DMG, XPReward) VALUES (?, ?, ?, ?)");
        mGameQuery.addBindValue(enemy[0]); // Name
        mGameQuery.addBindValue(enemy[1]); // HP
        mGameQuery.addBindValue(enemy[2]); // DMG
        mGameQuery.addBindValue(enemy[3]); // XPReward

        //Execute the query
        if (!mGameQuery.exec()) {

            //Skip duplicate entires (Database and table already exsists)
            if (mGameQuery.lastError().number() == 1062) // MySQL error code for duplicate entry
            {
                //Output MSG for duplicates
                //qDebug() << "TEST - Skipping duplicate entry for enemy:" << enemy[0].toString();
                mGameQuery.clear(); // Clear the query object for the next iteration
                continue; // Skip to the next iteration
            }
            else
            {
                //Other error than 1062???
                qDebug() << "Failed to insert enemy:";
                qDebug() << mGameQuery.lastError().text();
            }
        }
        mGameQuery.clear(); // Clear the query object for the next iteration
    }
}

Enemy Manager::loadEnemy(int enemyID) {

    // Prepare the SELECT query for retrieving the enemy from the database based on ID
    QString selectEnemyQuery = "SELECT * FROM Enemy WHERE ID = ?";
    mGameQuery.prepare(selectEnemyQuery);
    mGameQuery.addBindValue(enemyID); // Bind the enemy's ID as a parameter

    // Execute the query
    if (!mGameQuery.exec()) {
        qDebug() << "Failed to execute select query:";
        qDebug() << mGameQuery.lastError().text();
        throw std::runtime_error("Failed to execute select query");
    }

    // Check if any results were returned
    if (mGameQuery.next())
    {
        // Extract hero's attributes from the query result
        QString name = mGameQuery.value("Name").toString();
        int hp = mGameQuery.value("HP").toInt();
        int dmg = mGameQuery.value("DMG").toInt();
        int xpreward = mGameQuery.value("XPReward").toInt();


        // Construct a new instance of the hero using the retrieved attributes
        Enemy loadedEnemy(name.toStdString());
        loadedEnemy.setHP(hp);
        loadedEnemy.setDMG(dmg);
        loadedEnemy.setXPReward(xpreward);
        return loadedEnemy;

    }
    else
    {
        throw std::runtime_error("No enemy found with the specified ID");
    }

}

void Manager::printEnemies()
{
    if (!mGameQuery.exec("SELECT * FROM Enemy"))
    {
        qDebug() << "Failed to execute query:" << mGameQuery.lastError().text();

    }

    // Print enemy vars
    while (mGameQuery.next())
    {
        int id = mGameQuery.value("ID").toInt();
        QString name = mGameQuery.value("Name").toString();
        int hp = mGameQuery.value("HP").toInt();
        int dmg = mGameQuery.value("DMG").toInt();
        int xpReward = mGameQuery.value("XPReward").toInt();
        qDebug()
        << "ID:" << id
        << "Enemy:" << name
        << "HP:" << hp
        << "DMG:" << dmg
        << "XP Reward:" << xpReward;
    }
}

void Manager::printEnemyStats()
{
    std::cout << "---- Current Enemy ----" << std::endl;
    std::cout << "Enemy name: " << mEnemy.getName() << std::endl
              << "HP: " << mEnemy.getHP() << std::endl
              << "DMG: " << mEnemy.getDMG() << std::endl
              << "XP Reward: " << mEnemy.getXPReward() << std::endl;
}

//------------------------------------------------ Hero handling --------------------------------------------------------
void Manager::setHero(Hero newHero)
{
    mHero = newHero;
}

void Manager::saveHero(){

    // Prepare the INSERT query for the Hero table
    QString insertHeroQuery = "INSERT INTO Hero (Name, HP, DMG, Level, XP, Gold) "
                              "VALUES (:Name, :HP, :DMG, :Level, :XP, :Gold)"
                              "ON DUPLICATE KEY UPDATE "
                              "HP = :HP,"
                              "DMG = :DMG,"
                              "Level = :Level,"
                              "XP = :XP,"
                              "Gold = :Gold";
    mGameQuery.prepare(insertHeroQuery);

    // Bind values to the parameters
    mGameQuery.bindValue(":Name", QString::fromStdString(mHero.getName()));
    mGameQuery.bindValue(":HP", mHero.getHP());
    mGameQuery.bindValue(":DMG", mHero.getDMG());
    mGameQuery.bindValue(":Level", mHero.getLevel());
    mGameQuery.bindValue(":XP", mHero.getCurrentXP());
    mGameQuery.bindValue(":Gold", mHero.getGold());

    // Execute the query
    if (!mGameQuery.exec()) {
        qDebug() << "Failed to insert hero:";
        qDebug() << mGameQuery.lastError().text();
    }
    // Success message
    qDebug() << "Your hero takes a rest at the local inn, and is ready when you return!";
}

Hero Manager::loadHero(int heroID) {
    // Prepare the SELECT query for retrieving the hero from the database based on ID
    QString selectHeroQuery = "SELECT * FROM Hero WHERE ID = ?";
    mGameQuery.prepare(selectHeroQuery);
    mGameQuery.addBindValue(heroID); // Bind the hero's ID as a parameter

    // Execute the query
    if (!mGameQuery.exec()) {
        qDebug() << "Failed to execute select query:";
        qDebug() << mGameQuery.lastError().text();
        throw std::runtime_error("Failed to execute select query");
    }

    // Check if any results were returned
    if (mGameQuery.next())
    {
        // Extract hero's attributes from the query result
        QString name = mGameQuery.value("Name").toString();
        int hp = mGameQuery.value("HP").toInt();
        int dmg = mGameQuery.value("DMG").toInt();
        int level = mGameQuery.value("Level").toInt();
        int xp = mGameQuery.value("XP").toInt();
        int gold = mGameQuery.value("Gold").toInt();

        // Construct a new instance of the hero using the retrieved attributes
        Hero loadedHero(name.toStdString());
        loadedHero.setHP(hp);
        loadedHero.setDMG(dmg);
        loadedHero.setLevel(level);
        loadedHero.setXP(xp);
        loadedHero.setGold(gold);
        return loadedHero;

    }
    else
    {
        throw std::runtime_error("No hero found with the specified ID");
    }
}

void Manager::printHeros()
{
     // Execute the query
    if (!mGameQuery.exec("SELECT * FROM Hero")) {
        qDebug() << "Failed to execute select query:";
        qDebug() << mGameQuery.lastError().text();
    }

    // Iterate over the results and print each hero's attributes
    while (mGameQuery.next()) {
        int id = mGameQuery.value("ID").toInt();
        QString name = mGameQuery.value("Name").toString();
        int hp = mGameQuery.value("HP").toInt();
        int dmg = mGameQuery.value("DMG").toInt();
        int level = mGameQuery.value("Level").toInt();
        int xp = mGameQuery.value("XP").toInt();
        int gold = mGameQuery.value("Gold").toInt();

        qDebug()
        << "ID:" << id
        << "Name:" << name
        << "HP:" << hp
        << "DMG:" << dmg
        << "Level:" << level
        << "XP:" << xp
        << "Gold:" << gold;
    }
}

void Manager::printHeroStats()
{
    std::cout << "---- Current Hero ----" << std::endl;
    std::cout << "Enemy name: " << mHero.getName() << std::endl
              << "HP: " << mHero.getHP() << std::endl
              << "DMG: " << mHero.getDMG() << std::endl
              << "Level: " << mHero.getLevel() << std::endl
              << "XP: " << mHero.getCurrentXP() << std::endl
              << "Gold: " << mHero.getGold() << std::endl;
}

//------------------------------------------------ Cave handling -------------------------------------------------------------

void Manager::addCavesToGame() {
    // List of caves with name and gold reward
    QList<QVariantList> caves = {
        {"Peaceful clearing", 1000},
        {"Harsh Garden", 2000},
        {"Dark Grotto", 3000},
        {"Biting Glacier", 4000},
        {"Mount Drago", 5000}
    };

    // Prepare the INSERT query for the Caves table
    QString insertCaveQuery = "INSERT INTO Caves (Name, GoldReward) VALUES (:Name, :GoldReward)";
    mGameQuery.prepare(insertCaveQuery);

    // Loop through each cave and add it to the database
    for (const auto& cave : caves) {
        mGameQuery.bindValue(":Name", cave[0].toString()); // Name
        mGameQuery.bindValue(":GoldReward", cave[1].toInt()); // Gold reward

        // Execute the query
        if (!mGameQuery.exec()) {
            //qDebug() << "Failed to insert cave:";
            //qDebug() << mGameQuery.lastError().text();
            return;
        }
    }

    //qDebug() << "Caves added to the game successfully.";
}

void Manager::addEnemiesToCaves()
{
    // List of caves with CaveID and 3 enemies
    QList<QVariantList> rosters = {
        {1, 1, 1, 2},
        {2, 1, 2, 2},
        {3, 1, 2, 2},
        {4, 2, 2, 2},
        {5, 2, 2, 3},
    };

    // Prepare the INSERT query for the CaveEnemies table
    QString insertCaveEnemiesQuery = "INSERT INTO CaveEnemies (CaveID, EnemyID) "
                                     "VALUES (:CaveID, :EnemyID)";
    mGameQuery.prepare(insertCaveEnemiesQuery);

    // Loop through each cave and its associated enemies
    for (const auto& roster : rosters) {
        int caveID = roster[0].toInt();
        for (int i = 1; i < roster.size(); ++i) {
            int enemyID = roster[i].toInt();

            // Bind values to the parameters for the current cave and enemy
            mGameQuery.bindValue(":CaveID", caveID);
            mGameQuery.bindValue(":EnemyID", enemyID);

            // Execute the query
            if (!mGameQuery.exec()) {
                qDebug() << "Failed to insert data into CaveEnemies table for Cave" << caveID << "and Enemy" << enemyID << ":";
                qDebug() << mGameQuery.lastError().text();
            }
        }

        //qDebug() << "Data inserted into CaveEnemies table for Cave" << caveID << "successfully.";
    }
}

void Manager::printCaves()
{
    if (!mGameQuery.exec("SELECT * FROM Caves"))
    {
        qDebug() << "Failed to execute query:" << mGameQuery.lastError().text();
        return;

    }

    // Print cave values.
    while (mGameQuery.next())
    {
        int id = mGameQuery.value("CaveID").toInt();
        QString name = mGameQuery.value("Name").toString();
        int goldreward = mGameQuery.value("GoldReward").toInt();
        qDebug()
        << "ID:" << id
        << "Enemy:" << name
        << "Gold Reward:" << goldreward;
    }
}

void Manager::printCaveEnemies(int caveID)
{
    // Prepare the SELECT query to get the specific cave by CaveID
    QString selectCaveQuery = "SELECT CaveID, Name FROM Caves WHERE CaveID = :CaveID";
    mGameQuery.prepare(selectCaveQuery);
    mGameQuery.bindValue(":CaveID", caveID);

    // Execute the query
    if (!mGameQuery.exec()) {
        qDebug() << "Failed to retrieve data from Caves table for Cave ID" << caveID << ":";
        qDebug() << mGameQuery.lastError().text();
        return;
    }

    // Check if the cave exists
    if (mGameQuery.next()) {
        QString name = mGameQuery.value("Name").toString();

        std::cout << "Cave ID: " << caveID << "\n";
        std::cout << "Name: " << name.toStdString() << "\n";
        std::cout << "Enemies: ";

        // Prepare the SELECT query to get enemies for the specific cave
        QString selectEnemiesQuery = "SELECT Enemy.Name FROM Enemy "
                                     "INNER JOIN CaveEnemies ON Enemy.ID = CaveEnemies.EnemyID "
                                     "WHERE CaveEnemies.CaveID = :CaveID";
        QSqlQuery enemyQuery;
        enemyQuery.prepare(selectEnemiesQuery);
        enemyQuery.bindValue(":CaveID", caveID);

        // Execute the query to get enemies for the specific cave
        if (!enemyQuery.exec()) {
            qDebug() << "Failed to retrieve data from CaveEnemies table for Cave ID" << caveID << ":";
            qDebug() << enemyQuery.lastError().text();
            return;
        }

        // Fetch and print each enemy's name for the specific cave
        while (enemyQuery.next()) {
            QString enemyName = enemyQuery.value("Name").toString();
            std::cout << enemyName.toStdString() << ", ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Cave ID " << caveID << " does not exist.\n";
    }
}

QList<int> Manager::getEnemiesForCave(int caveID){

    QList<int> enemyIDs;

    QString getEnemiesFromCaveQuery = "SELECT EnemyID FROM CaveEnemies WHERE CaveID = :CaveID";
    mGameQuery.prepare(getEnemiesFromCaveQuery);
    mGameQuery.bindValue(":CaveID", caveID);

    if (mGameQuery.exec())
    {
        while (mGameQuery.next())
        {
            enemyIDs.append(mGameQuery.value(0).toInt());
        }
    }
    else
    {
        qDebug() << "Failed to retrieve enemies for Cave" << caveID << ":";
        qDebug() << mGameQuery.lastError().text();
    }

    return enemyIDs;
}

int Manager::getGoldFromCave(int caveID)
{
    // Prepare the SELECT query to fetch the gold reward for the specified cave ID
    QString selectGoldQuery = "SELECT GoldReward FROM Caves WHERE CaveID = :CaveID";
    mGameQuery.prepare(selectGoldQuery);
    mGameQuery.bindValue(":CaveID", caveID);

    // Execute the query
    if (mGameQuery.exec() && mGameQuery.next())
    {
        // Retrieve the gold reward from the query result
        int goldReward = mGameQuery.value(0).toInt();
        return goldReward;
    }
    else
    {
        // Failed to execute the query or fetch the result
        qDebug() << "Failed to fetch gold reward for cave" << caveID << ":";
        qDebug() << mGameQuery.lastError().text();
        return -1; // Return a default value indicating failure
    }
}

void Manager::addGoldFromCave(int caveID)
{
    // Prepare the SELECT query to fetch the gold reward for the specified cave ID and add it to the player.
    QString selectGoldQuery = "SELECT GoldReward FROM Caves WHERE CaveID = :CaveID";
    mGameQuery.prepare(selectGoldQuery);
    mGameQuery.bindValue(":CaveID", caveID);

    // Execute the query
    if (mGameQuery.exec() && mGameQuery.next())
    {
        //Add the gold to the hero
        mHero.addGold(mGameQuery.value(0).toInt());

    }
    else
    {
        // Failed to execute the query or fetch the result
        qDebug() << "Failed to fetch gold reward for cave" << caveID << ":";
        qDebug() << mGameQuery.lastError().text();
    }
}

//------------------------------------------------ Fighting -------------------------------------------------------------
void Manager::nextPhase()
{
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

int Manager::Encounter()
{

    //Return for state handling:
    //Hero won: 10
    //Enemy won: 20
    //Error: -1
    //std::cout << "Test: Try encounter" << std::endl;

    while(mHero.isAlive() && mEnemy.isAlive()){

        //std::cout << "Test: Enter fight loop" << std::endl;
        printHeroStats();
        printEnemyStats();

        nextPhase();
        mEnemy.takeDMG(mHero.getDMG());
        if(!mEnemy.isAlive()){
            //std::cout << "Test: Enemy dead" << std::endl;
            mHero.resetHero();
            mHero.addXP(mEnemy.getXPReward());
            return 10;
        }
        mHero.takeDMG(mEnemy.getDMG());
        if(!mHero.isAlive()){
            mHero.resetHero();
            return 20;
        }

    }
    return -1;
}

//------------------------------------------------ Magic -------------------------------------------------------------
void Manager::addMagics()
{
    // After DB creation, insert magics
    QList<QVariantList> magics = {
        {1, "Fireball", 5, 3, "Fire", "Metal", "Water", 10, 100},
        {2, "Rocksmash", 5, 3, "Earth", "Water", "Forest", 10, 100},
        {3, "Shard Rain", 5, 3, "Metal", "Forest", "Fire", 10, 100},
        {4, "Tsunami", 5, 3, "Water", "Fire", "Earth", 10, 100},
        {5, "Root Crush", 5, 3, "Forest", "Earth", "Metal", 10, 100}
    };

    for (const auto& magic : magics) {
        mGameQuery.prepare("INSERT INTO Magic (ID, Name, DMG, SelfDMG, Element, Strengt, Weakness, ManaCost, GoldCost) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
        mGameQuery.addBindValue(magic[0]); // ID
        mGameQuery.addBindValue(magic[1]); // Name
        mGameQuery.addBindValue(magic[2]); // DMG
        mGameQuery.addBindValue(magic[3]); // SelfDMG
        mGameQuery.addBindValue(magic[4]); // Element
        mGameQuery.addBindValue(magic[5]); // Strength
        mGameQuery.addBindValue(magic[6]); // Weakness
        mGameQuery.addBindValue(magic[7]); // ManaCost
        mGameQuery.addBindValue(magic[8]); // GoldCost

        // Execute the query
        if (!mGameQuery.exec()) {
            // Skip duplicate entries
            if (mGameQuery.lastError().number() == 1062) // MySQL error code for duplicate entry
            {
                // Skip to the next iteration
                mGameQuery.clear(); // Clear the query object for the next iteration
                continue;
            }
            else
            {
                qDebug() << "Failed to insert magic:";
                qDebug() << mGameQuery.lastError().text();
            }
        }
        mGameQuery.clear(); // Clear the query object for the next iteration
    }
}

Magic Manager::loadMagic(int magicID)
{
    // Prepare the SELECT query for retrieving the magic from the database based on ID
    QString selectMagicQuery = "SELECT * FROM Magic WHERE ID = ?";
    mGameQuery.prepare(selectMagicQuery);
    mGameQuery.addBindValue(magicID); // Bind the magic's ID as a parameter

    // Execute the query
    if (!mGameQuery.exec()) {
        qDebug() << "Failed to execute select query:";
        qDebug() << mGameQuery.lastError().text();
        throw std::runtime_error("Failed to execute select query");
    }

    // Check if any results were returned
    if (mGameQuery.next())
    {
        // Extract magic's attributes from the query result
        int id = mGameQuery.value("ID").toInt();
        QString name = mGameQuery.value("Name").toString();
        int dmg = mGameQuery.value("DMG").toInt();
        int selfDmg = mGameQuery.value("SelfDMG").toInt();
        QString element = mGameQuery.value("Element").toString();
        QString strengt = mGameQuery.value("Strengt").toString();
        QString weakness = mGameQuery.value("Weakness").toString();
        int manaCost = mGameQuery.value("ManaCost").toInt();
        int goldCost = mGameQuery.value("GoldCost").toInt();

        // Construct a new instance of the magic using the retrieved attributes
        Magic loadedMagic;
        loadedMagic.setID(id);
        loadedMagic.setName(name.toStdString());
        loadedMagic.setDMG(dmg);
        loadedMagic.setSelfDMG(selfDmg);
        loadedMagic.setElement(element.toStdString());
        loadedMagic.setStrength(strengt.toStdString());
        loadedMagic.setWeakness(weakness.toStdString());
        loadedMagic.setManaCost(manaCost);
        loadedMagic.setGoldCost(goldCost);
        return loadedMagic;
    }
    else
    {
        throw std::runtime_error("No magic found with the specified ID");
    }
}

void Manager::printMagics()
{
    if (!mGameQuery.exec("SELECT * FROM Magic"))
    {
        qDebug() << "Failed to execute query:" << mGameQuery.lastError().text();
    }

    // Print magic vars
    while (mGameQuery.next())
    {
        int id = mGameQuery.value("ID").toInt();
        QString name = mGameQuery.value("Name").toString();
        int dmg = mGameQuery.value("DMG").toInt();
        int selfDmg = mGameQuery.value("SelfDMG").toInt();
        QString element = mGameQuery.value("Element").toString();
        QString strengt = mGameQuery.value("Strengt").toString();
        QString weakness = mGameQuery.value("Weakness").toString();
        int manaCost = mGameQuery.value("ManaCost").toInt();
        int goldCost = mGameQuery.value("GoldCost").toInt();

        qDebug()
        << "ID:" << id
        << "Name:" << name
        << "DMG:" << dmg
        << "SelfDMG:" << selfDmg
        << "Element:" << element
        << "Strengt:" << strengt
        << "Weakness:" << weakness
        << "Mana Cost:" << manaCost
        << "Gold Cost:" << goldCost;
    }
}
