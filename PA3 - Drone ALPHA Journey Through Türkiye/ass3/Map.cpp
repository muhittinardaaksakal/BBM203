#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {
    // TODO: Your code here
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            // Initialize all distances to a value representing no direct connection
            distanceMatrix[i][j] = -1;
        }

        // Initialize all provinces as unvisited
        visited[i] = false; // All provinces start as unvisited
    }
}
// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // TODO: Your code here



    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    std::string line;
    int row = 0;
    // Read each line in the CSV file
    while (std::getline(file, line) && row < MAX_SIZE) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;
        // Read each cell separated by a comma
        while (std::getline(ss, cell, ',') && col < MAX_SIZE) {
            // Convert cell to an integer and store in distanceMatrix
            int distance = std::stoi(cell);
            distanceMatrix[row][col] = distance;
            ++col;
        }
        ++row;
    }

    file.close();
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here
    int distance = getDistance(provinceA, provinceB);

    if (distance == -1) {

        return false;
    } else if (distance <= maxDistance) {

        return true;
    } else {

        return false;
    }
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    if (province >= 0 && province < MAX_SIZE) {
        visited[province] = true;
    }
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    // TODO: Your code here
    if (province >= 0 && province < MAX_SIZE) {
        return visited[province];
    }
    return false;
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code here
    for (int i = 0; i < MAX_SIZE; ++i) {
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    // TODO: Your code here
    int count = 0;
    for (int i = 0; i < MAX_SIZE; ++i) {
        if (visited[i]) {
            ++count;
        }
    }
    return count;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    // TODO: Your code here
    if (provinceA >= 0 && provinceA < MAX_SIZE && provinceB >= 0 && provinceB < MAX_SIZE) {
        return distanceMatrix[provinceA][provinceB];
    }
    return -1;
}
