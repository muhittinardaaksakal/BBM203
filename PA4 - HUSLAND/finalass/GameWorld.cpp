#include "GameWorld.h"

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    bool hasAccess = false;
    // TODO: Check if the realmShaper has access to explore the isle

    // Get necessary depth values
    // Use mapTree.calculateMinMapDepthAccess
    int minAccessDepth = calculateMinMapDepthAccess(shaperTree.getDepth(realmShaper), shaperTree.getDepth(), mapTree.getDepth());

    if (minAccessDepth <= mapTree.getIsleDepth(isle) ){
        hasAccess = true;
    } else {

        return hasAccess;
    }
    return hasAccess;
}
int GameWorld :: calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}
void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    // TODO:
    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation
    if (!realmShaper || !isle) {
        std::cout << "Error: Invalid RealmShaper or Isle." << std::endl;
        return;
    }

    // Check if realmShaper has access
    if (!hasAccess(realmShaper, isle)) {
        // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
        std::cout << "[Explore Area] " << realmShaper->getName()
                  << " does not have access to explore area "
                  << isle->getName() << std::endl;
        return;
    }
    // If realmShaper has access
    if (realmShaper->getCurrentIsleName()!="") {

        Isle *previousIsle = mapTree.findIsle(realmShaper->getCurrentIsleName());
        if (previousIsle) {

            previousIsle->decreaseShaperCount();

        }
    }

    bool isOvercrowded = isle->increaseShaperCount();


    // check overcrowding for Isle,
    if (!isOvercrowded) {
        // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
        std::cout << "[Explore Area] " << realmShaper->getName()
                  << " visited " << isle->getName() << std::endl;
        // Visit isle,
        // collect item
        realmShaper->collectItem(isle->getItem());
        // Use // std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
        std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
        realmShaper->setCurrentIsleName(isle->getName());

    }
    // delete Isle if necessary
    else {

        std::cout << "[Overcrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;


        mapTree.remove(isle);
    }
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName) {
    // TODO: Check energy and craft new isle if possible

    if (!shaper->hasEnoughEnergy())
    {
        // Use std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;
        return;
    }
    if(shaper->hasEnoughEnergy()){
        shaper->loseEnergy();
        Isle *newIsle = new Isle(isleName);
        // Use std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
        std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;
        mapTree.insert(newIsle); // Add the new Isle to the Map
        // Use std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;
        std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
    }
}

void GameWorld::displayGameState()
{
    //mapTree.displayMap();
    //shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs) {
    // TODO:
    // Read logs
    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);

    std::string line;
    int accessCount = 0;
    std::getline(accessFile, line);
    std::getline(duelFile, line);
    while (std::getline(accessFile, line))
    {
        std::istringstream iss(line);
        std::string shaperName, isleName;
        iss >> shaperName >> isleName;

        RealmShaper *shaper = shaperTree.findPlayer(shaperName);
        Isle *isle = mapTree.findIsle(isleName);

        if (isle != nullptr)
        {
            exploreArea(shaper, isle);
        }
        else
        {
            // This function should call exploreArea and craft functions
            craft(shaper, isleName);
        }

        accessCount++;

        // For every 5 access, 1 duel happens
        if (accessCount % 5 == 0 && std::getline(duelFile, line))
        {
            std::istringstream duelIss(line);
            std::string playerName;
            bool result;
            duelIss >> playerName >> result;

            RealmShaper *challenger = shaperTree.findPlayer(playerName);
            if (challenger)
            {
                shaperTree.duel(challenger, result);
            }
            else
            {   std::cout <<playerName << std::endl;
                std::cout << "Error: Challenger not found for duel." << std::endl;
            }
        }
        // Use displayGameState();
        displayGameState();
    }


    // If there are still duel logs left after every access happens duels happens one after other
    while (std::getline(duelFile, line))
    {
        std::istringstream duelIss(line);
        std::string playerName;
        bool result;
        duelIss >> playerName >> result;

        RealmShaper *challenger = shaperTree.findPlayer(playerName);
        if (challenger)
        {
            shaperTree.duel(challenger, result);
        }
        else
        {
            std::cout << "Error: Challenger not found for duel." << std::endl;
        }

        displayGameState();
    }

    accessFile.close();
    duelFile.close();
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}