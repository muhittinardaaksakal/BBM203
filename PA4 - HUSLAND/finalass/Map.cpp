#include "Map.h"

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    // TODO: Free any dynamically allocated memory if necessary
    freeMap(root);
}

void Map::initializeMap(std::vector<Isle *> isles)
{
    // TODO: Insert innitial isles to the tree

    for (Isle *isle : isles)
    {
        insert(isle);
    }
    // Then populate with Goldium and Einstainium items
    populateWithItems();
    is_initialize_finished = true;
}

MapNode *Map::rotateRight(MapNode *current)
{
    // TODO: Perform right rotation according to AVL


    if (!current)
    {
        // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
        std::cerr << "[Right Rotation] Called on invalid node!" << std::endl;
        return nullptr;
    }

    MapNode *newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    if(is_initialize_finished) {
        rebalanceCounter++;
        if (rebalanceCounter % 3 == 0)
            dropItemBFS();
    }


    return newRoot;
}

MapNode *Map::rotateLeft(MapNode *current)
{
    // TODO: Perform left rotation according to AVL
    if (!current)
    {
        // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
        std::cout << "[Left Rotation] Called on invalid node!" << std::endl;
        return nullptr;
    }

    MapNode *newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    if(is_initialize_finished) {
        rebalanceCounter++;
        if (rebalanceCounter % 3 == 0)
            dropItemBFS();
    }

    // return necessary new root
    return newRoot;
}

int calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    // TODO: Return height of the node
    if (!node)
        return 0;
    return node->height;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{
    // TODO: Recursively insert isle to the tree
    if (!node)
    {
        return new MapNode(isle);
    }


    if (isle->getName() < node->isle->getName()) {
        node->left = insert(node->left, isle);
    }

    else if (isle->getName() > node->isle->getName()) {
        node->right = insert(node->right, isle);
    }

    else if (isle->getName() == node->isle->getName()) {
        return node;
    }



    node->height = 1 + std::max(height(node->left), height(node->right));

    int balanceFactor = height(node->left) - height(node->right);


    if (balanceFactor > 1)
    {
        if (isle->getName() < node->left->isle->getName()) {
            return rotateRight(node);
        }
        else
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }

    if (balanceFactor < -1)
    {
        if (isle->getName() > node->right->isle->getName()) {
            return rotateLeft(node);
        }
        else
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }
    // returns inserted node
    return node;

}

void Map::insert(Isle *isle)
{
    // you might need to insert some checks / functions here depending on your implementation
    root = insert((root), isle);
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    // TODO: Recursively delete isle from the tree
    // Will be called if there is overcrowding


    if (!node) {
        // Use std::cout << "[Remove] " << "Tree is Empty" << std::endl;
        std::cout << "[Remove] Tree is Empty" << std::endl;
        return nullptr;
    }

    if (isle->getName() < node->isle->getName()) {
        node->left = remove(node->left, isle);
    } else if (isle->getName() > node->isle->getName()) {
        node->right = remove(node->right, isle);
    } else {

        if (!node->left || !node->right) {


            if (!node->left&& !node->right) {
                delete node->isle;
                delete node;
                return nullptr;
            }
            else if(!node->left&&node->right) {
                delete node->isle;
                delete node;
                return node->right;

            }
            else if(node->left&&!node->right) {

                // One child
                delete node->isle;
                delete node;
                return node->left;

            }
        } else {
            MapNode* temp = node->left;
            while (temp->right) {
                temp = temp->right;
            }

            node->isle = new Isle(*temp->isle);

            node->left = remove(node->left, temp->isle);
        }
    }

    node->height = 1 + std::max(height(node->left), height(node->right));

    int balanceFactor = height(node->left) - height(node->right);

    if (balanceFactor > 1) {
        if (height(node->left->left) >= height(node->left->right))
            return rotateRight(node);
        else {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }

    if (balanceFactor < -1) {
        if (height(node->right->right) >= height(node->right->left))
            return rotateLeft(node);
        else {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }
    // returns node
    return node;


}

void Map::remove(Isle *isle)
{
    // you might need to insert some checks / functions here depending on your implementation
    root = remove((root), isle);
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop EINSTEINIUM according to rules
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    if (!current)
        return;

    count++;
    if (current->isle->getItem() == EMPTY && count % 5 == 0)
    {
        current->isle->setItem(EINSTEINIUM);
        std::cout << "[Item Drop] EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }

    preOrderItemDrop(current->left, count);
    preOrderItemDrop(current->right, count);
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop GOLDIUM according to rules

    if (!current)
        return;

    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);

    count++;
    if (current->isle->getItem() == EMPTY && count % 3 == 0)
    {
        current->isle->setItem(GOLDIUM);
        // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
        std::cout << "[Item Drop] GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    // TODO: Find first Isle with no item
    if (!node)
        return nullptr;

    std::vector<MapNode *> nodes;
    nodes.push_back(node);

    size_t front = 0;

    while (front < nodes.size())
    {
        MapNode *current = nodes[front];
        front++;

        if (current->isle->getItem() == EMPTY)
        {
            return current;
        }

        if (current->left)
            nodes.push_back(current->left);
        if (current->right)
            nodes.push_back(current->right);
    }

    return nullptr;
}


void Map::dropItemBFS()
{
    // TODO: Drop AMAZONITE according to rules


    MapNode *first_empty_node = findFirstEmptyIsle(root);

    if (first_empty_node && first_empty_node->isle->getItem() == EMPTY)
    {
        // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
        first_empty_node->isle->setItem(AMAZONITE);
        std::cout << "[BFS Drop] AMAZONITE dropped on Isle: " << first_empty_node->isle->getName() << std::endl;
    }
    else
    {
        // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
        std::cout << "[BFS Drop] No eligible Isle found for AMAZONITE drop." << std::endl;
    }
}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    // TODO: Return node depth if found, else
    if (!node)
        return 0;
    return 1 + std::max(getDepth(node->left), getDepth(node->right));

}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    // TODO: Return node depth by isle if found, else
    MapNode *current = root;
    int depth = 0;

    while (current)
    {
        if (current->isle->getName() == isle->getName())
        {
            return depth;
        }
        else if (isle->getName() < current->isle->getName())
        {
            current = current->left;
        }
        else if (isle->getName() > current->isle->getName())
        {

            current = current->right;
        }

        depth++;
    }
    return -1;

}

int Map::getDepth()
{
    // TODO: Return max|total depth of tree
    return getDepth(root);
}

void Map::populateWithItems()
{
    // TODO: Distribute fist GOLDIUM than EINSTEINIUM

    if (!root)
    {
        std::cout << "[Error] Tree is empty. Cannot populate items." << std::endl;
        return;
    }

    int preOrderCount = 0;
    int postOrderCount = 0;

    preOrderItemDrop(root, preOrderCount);

    postOrderItemDrop(root, postOrderCount);

}

Isle *Map::findIsle(Isle isle)
{
    // TODO: Find isle by value
    MapNode *current = root;

    while (current)
    {
        if (current->isle->getName() == isle.getName())
        {
            return current->isle;
        }
        else if (isle.getName() < current->isle->getName())
        {
            current = current->left;
        }
        else if (isle.getName() > current->isle->getName())
        {
            current = current->right;
        }
    }

    return nullptr;


}

Isle *Map::findIsle(std::string name)
{
    // TODO: Find isle by name
    MapNode *current = root;

    while (current)
    {
        if (current->isle->getName() == name)
        {
            return current->isle;
        }
        else if (name < current->isle->getName())
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    // TODO: Find node by value
    MapNode *current = root;

    while (current)
    {
        if (current->isle->getName() == isle.getName())
        {
            return current;
        }
        else if (isle.getName() < current->isle->getName())
        {
            current = current->left;
        }
        else if (isle.getName() >    current->isle->getName())
        {
            current = current->right;
        }
    }

    return nullptr;

}

MapNode *Map::findNode(std::string name)
{
    // TODO: Find node by name
    MapNode *current = root;

    while (current)
    {
        if (current->isle->getName() == name)
        {
            return current;
        }
        else if (name < current->isle->getName())
        {
            current = current->left;
        }
        else if (name > current->isle->getName())
        {
            current = current->right;
        }
    }

    return nullptr;

}

void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output level by level
    if (!root)
    {
        std::cout << "Error: Tree is empty. Nothing to write." << std::endl;
        return;
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    int treeHeight = height(root);

    for (int level = 0; level < treeHeight; ++level)
    {
        writeLevel(root, level, outFile);
        outFile << std::endl;
    }

    std::cout << "[Output] Tree has been written to " << filename << " level-by-level." << std::endl;
    outFile.close();
}
void Map::writeLevel(MapNode *node, int level, std::ofstream &outFile)
{
    if (!node)
    {
        outFile << "NULL ";
        return;
    }

    if (level == 0)
    {
        outFile << node->isle->getName() << " ";
    }
    else
    {
        writeLevel(node->left, level - 1, outFile);  // Sol alt ağaç için seviye azalt
        writeLevel(node->right, level - 1, outFile); // Sağ alt ağaç için seviye azalt
    }
}
void Map:: writeIslesInOrder(MapNode *node, std::ofstream &outFile)
{
    if (node == nullptr)
        return;

    writeIslesInOrder(node->left, outFile);

    if (node->isle) // isle kontrolü
        outFile << node->isle->getName() << std::endl;
    writeIslesInOrder(node->right, outFile);
}
void Map::writeIslesToFile(const std::string &filename)
{
    // TODO: Write Isles to output file in alphabetical order

    std::ofstream outFile(filename);
    if (!outFile)
    {
        std::cerr << "[Error] Unable to open file: " << filename << std::endl;
        return;
    }

    writeIslesInOrder(root, outFile);

    outFile.close();
    // Use std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
    std::cout << "[Output] Isles have been written to " << filename << " in alphabetical order." << std::endl;
}

void Map::freeMap(MapNode *node)
{
    if (!node)
        return;

    freeMap(node->left);
    freeMap(node->right);

    delete node->isle;

    delete node;
}