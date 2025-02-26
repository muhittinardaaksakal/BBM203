#include "RealmShaper.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define NECESSARY_ENERGY 2000 // Necessary energy to craft an Isle

RealmShaper::RealmShaper(std::string name, int honour) : name(name)
{
    this->honour = honour;
}

const std::string &RealmShaper::getName() const
{
    return this->name;
}

int RealmShaper::getEnergyLevel()
{
    return this->collectedEnergyPoints;
}

int RealmShaper::getHonour()
{
    return this->honour;
}

void RealmShaper::gainHonour()
{
    this->honour += 150;
}

void RealmShaper::loseHonour()
{
    this->honour -= 150;
}

void RealmShaper::collectItem(Item item)
{
    this->collectedEnergyPoints += item;
}

void RealmShaper::loseEnergy()
{
    this->collectedEnergyPoints -= NECESSARY_ENERGY;
}

bool RealmShaper::hasEnoughEnergy()
{
    return this->collectedEnergyPoints >= NECESSARY_ENERGY;
}

std::vector<RealmShaper *> RealmShaper::readFromFile(const std::string &filename)
{
    std::vector<RealmShaper *> players;
    // TODO: Read RealmShapers from the file,
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return players;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        std::string playerName;
        int honourPoints;

        if (lineStream >> playerName >> honourPoints)
        {
            // add them to vector
            RealmShaper *player = new RealmShaper(playerName, honourPoints);

            players.push_back(player);
        }
        else
        {
            // Input format: playerName[tab]honourPoints
            std::cout << "Error: Invalid line format: " << line << std::endl;
        }
    }

    file.close();
    // return the vector
    return players;
}


bool RealmShaper::operator==(const RealmShaper &other) const
{
    // TODO: Compare by name, return true if same
    if (this->getName() == other.getName()) {
        return true;
    }
    return false;

}

std::ostream &operator<<(std::ostream &os, const RealmShaper &p)
{
    // Outs the player name
    return (os << p.name); // << std::endl);
}

const std::string& RealmShaper::getCurrentIsleName() const {
    return this->currentIsleName;
}

void RealmShaper::setCurrentIsleName(const std::string& isleName) {
    this->currentIsleName = isleName;
}
