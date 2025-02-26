#include "RoutePlanner.h"
#include <iostream>
#include <fstream>

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {
    // TO DO:
    // Load map data from file
    map.loadDistanceData(distance_data);
    // Mark all provinces as unvisited initially
    map.resetVisited();
    // Load priority provinces
    loadPriorityProvinces(priority_data);
    // Load restricted provinces
    loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open priority provinces file: " << filename << std::endl;
        return;
    }

    std::string line;
    numPriorityProvinces = 0;

    while (std::getline(file, line) && numPriorityProvinces < MAX_PRIORITY_PROVINCES) {
        int start = -1, end = -1;


        for (int i = 0; i < line.length(); ++i) {
            if (line[i] == '(') {
                start = i;
            } else if (line[i] == ')' && start != -1) {
                end = i;
                break;
            }
        }

        if (start != -1 && end != -1) {
            int province = std::stoi(line.substr(start + 1, end - start - 1));
            priorityProvinces[numPriorityProvinces++] = province;
        }
    }

    file.close();
}

void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open restricted provinces file: " << filename << std::endl;
        return;
    }

    std::string line;
    numWeatherRestrictedProvinces = 0;

    while (std::getline(file, line) && numWeatherRestrictedProvinces < MAX_WEATHER_RESTRICTED_PROVINCES) {
        int start = -1, end = -1;

        for (int i = 0; i < line.length(); ++i) {
            if (line[i] == '(') {
                start = i;
            } else if (line[i] == ')' && start != -1) {
                end = i;
                break;
            }
        }
        if (start != -1 && end != -1) {
            int province = std::stoi(line.substr(start + 1, end - start - 1));
            weatherRestrictedProvinces[numWeatherRestrictedProvinces++] = province;
        }
    }

    file.close();
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    // TODO: Your code here
    for (int i = 0; i < numPriorityProvinces; ++i) {
        if (priorityProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    // TODO: Your code here
    for (int i = 0; i < numWeatherRestrictedProvinces; ++i) {
        if (weatherRestrictedProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int start) {
    // Start exploration from the starting province
    stack.push(start);
    map.markAsVisited(start);
    route.push_back(start);

    while (!stack.isEmpty()) {
        exploreFromProvince(stack.peek());
    }
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    enqueueNeighbors(province);

    if (queue.isEmpty()) {
        backtrack();
    } else {
        int nextProvince = queue.dequeue();

        if ( !isWeatherRestricted(nextProvince) && !map.isVisited(nextProvince)) {
            stack.push(nextProvince);
            map.markAsVisited(nextProvince);
            route.push_back(nextProvince);
        }
    }
}


void RoutePlanner::enqueueNeighbors(int province) {
    for (int neighbor = 0; neighbor < MAX_SIZE; ++neighbor) {
        if (!map.isVisited(neighbor)) {
            if (isWeatherRestricted(neighbor)) {
                std::cout << "Province " << cities[neighbor] << " is weather-restricted. Skipping." << std::endl;
                continue;
            }
            if (map.isWithinRange(province, neighbor, maxDistance)&& !queue.contains(neighbor) ) {
                if (isPriorityProvince(neighbor)) {
                    queue.enqueuePriority(neighbor);
                } else {
                    queue.enqueue(neighbor);
                }
            }
        }
    }
}

void RoutePlanner::backtrack() {
    // TODO: Your code here
    if (stack.isEmpty()) {
        return;
    }
    // If you reach a dead-end province
    stack.pop();

}


bool RoutePlanner::isExplorationComplete() const {
    // TODO: Your code here
    if (stack.isEmpty() && queue.isEmpty()) {
        return true;
    }
    for (int i = 0; i < MAX_SIZE; ++i) {
        if (!map.isVisited(i) && !isWeatherRestricted(i)) {
            return false;
        }
    }
    return true;
}

void RoutePlanner::displayResults() const {
    // TODO: Your code here
    // Display "Journey Completed!" message
    std::cout << "---------------------------" << std::endl;
    std::cout << "Journey Completed!" << std::endl;
    std::cout << "---------------------------" << std::endl;

    // Display the total number of provinces visited
    std::cout << "Total Number of Provinces Visited: " << route.size() << std::endl;

    int calculatedTotalDistance = 0;
    for (int i = 1; i < route.size(); ++i) {
        calculatedTotalDistance += map.getDistance(route[i - 1], route[i]);
    }
    // Display the total distance covered
    std::cout << "Total Distance Covered: " << calculatedTotalDistance << " km" << std::endl;

    // Display the route in the order visited
    std::cout << "Route Taken:" << std::endl;
    for (int i = 0; i < route.size(); ++i) {
        std::cout << cities[route[i]] << " -> ";

    }
    std::cout << "End" << std::endl;

    // Priority Province Summary
    std::cout << "\nPriority Provinces Status:" << std::endl;
    int priorityVisited = 0;
    for (int i = 0; i < numPriorityProvinces; ++i) {
        int province = priorityProvinces[i];
        std::cout << "- " << cities[province] << " (";
        if (map.isVisited(province)) {
            std::cout << "Visited";
            ++priorityVisited;
        } else {
            std::cout << "Not Visited";
        }
        std::cout << ")" << std::endl;
    }

    std::cout << "\nTotal Priority Provinces Visited: " << priorityVisited << " out of " << numPriorityProvinces << std::endl;
    if (priorityVisited == numPriorityProvinces) {
        std::cout << "Success: All priority provinces were visited." << std::endl;
    } else {
        std::cout << "Warning: Not all priority provinces were visited." << std::endl;
    }
}
