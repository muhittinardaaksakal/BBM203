#include "RealmShapers.h"
#include <cmath>
#include <algorithm>

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    // TODO: Free any dynamically allocated memory if necessary
    for (RealmShaper *shaper : realmShapers)
    {

        delete shaper;
    }
    realmShapers.clear();
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    // TODO: Insert innitial shapers to the tree
    realmShapers.clear();
    for (RealmShaper *shaper : shapers)
    {

        realmShapers.push_back(shaper);
    }
}

int ShaperTree::getSize()
{
    // TODO: Return number of shapers in the tree
    return realmShapers.size();
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;

    // TODO: Check if the index is valin in the tree
    if (index >= 0 && index < getSize()) {
        isValid = true;
    }
    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper)
{
    // TODO: Insert shaper to the tree
    realmShapers.push_back(shaper);
}

int ShaperTree::remove(RealmShaper *shaper)
{
    // TODO: Remove the player from tree if it exists

    int index = findIndex(shaper);
    if (index == -1)
    {
        std::cout << "Error: Shaper not found in the tree." << std::endl;
        // else
        return -1;
    }

    delete realmShapers[index];

    // Make sure tree protects its form (complate binary tree) after deletion of a node
    for (int i = index; i < realmShapers.size() - 1; ++i)
    {
        realmShapers[i] = realmShapers[i + 1];
    }

    realmShapers.pop_back();
    // return index if found and removed
    return index;

}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    // return index in the tree if found
    for (int i = 0; i < realmShapers.size(); ++i)
    {
        if (realmShapers[i] == shaper)
        {
            return i;
        }
    }
    // else
    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    // return depth of the node in the tree if found

    int index = findIndex(shaper);
    if (index == -1)
    {   // else
        return -1;
    }

    return static_cast<int>(std::floor(std::log2(index + 1)));

}

int ShaperTree::getDepth()
{
    // return total|max depth|height of the tree
    if (realmShapers.empty())
    {
        return 0;
    }

    return static_cast<int>(std::floor(std::log2(realmShapers.size())) + 1);

}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    // TODO: Implement duel logic, return the victor
    // Use   std::cout << "[Duel] " << victorName << " won the duel" << std::endl;
    // Use   std::cout << "[Honour] " << "New honour points: ";
    // Use   std::cout << challengerName << "-" << challengerHonour << " ";
    // Use   std::cout << opponentName << "-" << opponentHonour << std::endl;
    // Use   std::cout << "[Duel] " << loserName << " lost all honour, delete" << std::endl;


    RealmShaper *opponent = getParent(challenger);


    if (result)
    {
        std::cout << "[Duel] " << challenger->getName() << " won the duel" << std::endl;

        challenger->gainHonour();
        opponent->loseHonour();

        std::cout << "[Honour] New honour points: " << challenger->getName() << "-" << challenger->getHonour()
                  << " " << opponent->getName() << "-" << opponent->getHonour() << std::endl;

        // Remove opponent if honour drops to 0
        if (opponent->getHonour() <= 0)
        {
            std::cout << "[Duel] " << opponent->getName() << " lost all honour, delete" << std::endl;
            remove(opponent);
        }
        replace(challenger,opponent);
        return *challenger;
    }

    std::cout << "[Duel] " << challenger->getName() << " lost the duel" << std::endl;

    opponent->gainHonour();
    challenger->loseHonour();

    std::cout << "[Honour] New honour points: " << challenger->getName() << " - " << challenger->getHonour()
              << " " <<  opponent->getName()<< " - " << opponent->getHonour() << std::endl;

    if (challenger->getHonour() <= 0)
    {
        std::cout << "[Duel] " << challenger->getName() << " lost all honour, delete" << std::endl;
        remove(challenger);
    }

    return *opponent;

}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;

    // TODO: return parent of the shaper
    int index = findIndex(shaper);
    if (index == -1 )
    {
        return nullptr;
    }
    if(index == 0){
        return realmShapers[index];
    }

    int parentIndex = (index - 1) / 2;
    parent = realmShapers[parentIndex];

    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    // TODO: Change player_low and player_high's positions on the tree
    int index_low = findIndex(player_low);
    int index_high = findIndex(player_high);

    if (index_low == -1 || index_high == -1)
    {
        std::cout << "Error: One or both players not found in the tree." << std::endl;
        return;
    }

    std::swap(realmShapers[index_low], realmShapers[index_high]);
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    for (RealmShaper *shaperinVector : realmShapers) {
        if (*shaperinVector == shaper) {
            foundShaper = shaperinVector;
        }

    }

    return foundShaper;
}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by name

    for (RealmShaper *shaper : realmShapers) {
        if (shaper->getName() == name) {
            foundShaper = shaper;
        }
    }


    // Return the shaper if found
    return foundShaper;
}
void ShaperTree::inOrderHelper(int index, std::vector<std::string> &result)
{
    // Check if the index is valid
    if (index >= static_cast<int>(realmShapers.size()) || realmShapers[index] == nullptr)
    {
        return;
    }

    int leftChildIndex = 2 * index + 1;
    inOrderHelper(leftChildIndex, result);

    result.push_back(realmShapers[index]->getName());

    int rightChildIndex = 2 * index + 2;
    inOrderHelper(rightChildIndex, result);
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement inOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    // Note: Since SheperTree is not an binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well

    inOrderHelper(index, result);

    return result;
}
void ShaperTree::preOrderHelper(int index, std::vector<std::string> &result)
{
    if (!isValidIndex(index))
        return;

    result.push_back(realmShapers[index]->getName());

    preOrderHelper(2 * index + 1, result);

    preOrderHelper(2 * index + 2, result);
}
std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector

    // Define and implement as many helper functions as necessary for recursive implementation

    preOrderHelper(index, result);
    // Return the vector
    return result;

}
void ShaperTree :: postOrderHelper(int index, std::vector<std::string> &result)
{
    if (!isValidIndex(index))
        return;

    postOrderHelper(2 * index + 1, result);

    postOrderHelper(2 * index + 2, result);

    result.push_back(realmShapers[index]->getName());
}
std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    postOrderHelper(index, result);
    return result;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file

    // Define and implement as many helper functions as necessary for recursive implementation
    if (!isValidIndex(index))
        return;

    outFile << realmShapers[index]->getName() <<"\n";

    preOrderTraversal(2 * index + 1, outFile);

    preOrderTraversal(2 * index + 2, outFile);
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    // TODO: Implement level-order traversal
    // write nodes to output file

    // Define and implement as many helper functions as necessary
    for (RealmShaper *shaper : realmShapers)
    {
        if (shaper)
        {
            outFile << *shaper << "\t" << shaper->getHonour() << std::endl;
        }
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = 2 * index + 1;  // TODO: Calculate left index
    int right = 2 * index + 2; // TODO: Calculate right index

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cout << "Error: Could not open file " << filename << std::endl;
        return;
    }

    for (size_t i = 0; i < realmShapers.size(); ++i)
    {
        if (realmShapers[i])
        {
            outFile << *realmShapers[i];
            if (i != realmShapers.size() - 1)
                outFile << "\n";
        }
    }
    // Use std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
    std::cout << "[Output] Shapers have been written to " << filename << " according to rankings." << std::endl;
    outFile.close();
}


void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order

    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cout << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    preOrderTraversal(0, outFile);
    // Use std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
    std::cout << "[Output] Tree has been written to " << filename << " in pre-order." << std::endl;

    outFile.close();
}
