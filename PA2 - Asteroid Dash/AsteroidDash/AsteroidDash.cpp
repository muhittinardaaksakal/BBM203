#include "AsteroidDash.h"
#include <fstream>
#include <sstream>

// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    ifstream file(input_file);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << input_file << endl;
        return;
    }

    string line;

    while (getline(file, line)) {
        vector<int> row;
        stringstream ss(line);
        int cell;


        while (ss >> cell) {
            row.push_back(cell);
        }


        space_grid.push_back(row);
    }

    file.close();
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    ifstream file(player_file_name);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << player_file_name << endl;
        return;
    }

    int position_row, position_col;
    vector<vector<bool>> spacecraft_shape;
    string line;


    if (getline(file, line)) {
        stringstream ss(line);
        ss >> position_row >> position_col;
    }


    while (getline(file, line)) {
        vector<bool> row;
        for (char ch : line) {
            if (ch == '1') {
                row.push_back(true);
            } else if (ch == '0') {
                row.push_back(false);
            }
        }
        spacecraft_shape.push_back(row);
    }

    file.close();


    player = new Player(spacecraft_shape, position_row, position_col, player_name);
}

void AsteroidDash::read_celestial_objects(const string &input_file) {
    ifstream file(input_file);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << input_file << endl;
        return;
    }

    string line;
    CelestialObject *previous_object = nullptr;
    CelestialObject *previous_clockwise_90 = nullptr;
    CelestialObject *previous_clockwise_180 = nullptr;
    CelestialObject *previous_clockwise_270 = nullptr;

    while (getline(file, line)) {
        vector<vector<bool>> shape;
        ObjectType type;
        int start_row = 0;
        int time_of_appearance = 0;
        string effect;


        if (line[0] == '[') {
            type = ASTEROID;
        } else if (line[0] == '{') {
            type = LIFE_UP;
        } else {
            continue;
        }


        while (!line.empty() && line[0] != 's' && line[0] != 't' && line[0] != 'e') {
            vector<bool> row;
            for (char ch : line) {
                if (ch == '1') row.push_back(true);
                else if (ch == '0') row.push_back(false);
            }
            shape.push_back(row);
            if (!getline(file, line)) break;
        }


        while (!line.empty() && (line[0] == 's' || line[0] == 't' || line[0] == 'e')) {
            if (line[0] == 's') {
                start_row = stoi(line.substr(2));
            } else if (line[0] == 't') {
                time_of_appearance = stoi(line.substr(2));
            } else if (line[0] == 'e') {
                effect = line.substr(2);
                if (effect == "ammo") {
                    type = AMMO;
                }
            }
            if (!getline(file, line)) break;
        }
        vector<vector<bool>> shape_90 = CelestialObject::rotate_90_clockwise(shape);
        vector<vector<bool>> shape_180 = CelestialObject::rotate_90_clockwise(shape_90);


        CelestialObject *new_object = new CelestialObject(shape, type, start_row, time_of_appearance);
        CelestialObject *clockwise_90 = nullptr;
        CelestialObject *clockwise_180 = nullptr;
        CelestialObject *clockwise_270 = nullptr;
        if (shape == shape_90) {
            new_object->right_rotation = new_object;
            new_object->left_rotation = new_object;
        }
        else if (shape != shape_90 &&shape == shape_180) {
            CelestialObject *clockwise_90 = new CelestialObject(CelestialObject::rotate_90_clockwise(shape), type, start_row, time_of_appearance);
            new_object->right_rotation = clockwise_90;
            new_object->left_rotation = clockwise_90;
            clockwise_90->right_rotation = new_object;
            clockwise_90->left_rotation = new_object;
        }
        else {
            CelestialObject *clockwise_90 = new CelestialObject(CelestialObject::rotate_90_clockwise(shape), type, start_row, time_of_appearance);

            CelestialObject *clockwise_180 = new CelestialObject(CelestialObject::rotate_90_clockwise(clockwise_90->shape), type, start_row, time_of_appearance);

            CelestialObject *clockwise_270 = new CelestialObject(CelestialObject::rotate_90_clockwise(clockwise_180->shape), type, start_row, time_of_appearance);


            new_object->right_rotation = clockwise_90;
            clockwise_90->right_rotation = clockwise_180;
            clockwise_180->right_rotation = clockwise_270;
            clockwise_270->right_rotation = new_object;

            new_object->left_rotation = clockwise_270;
            clockwise_270->left_rotation = clockwise_180;
            clockwise_180->left_rotation = clockwise_90;
            clockwise_90->left_rotation = new_object;

        }


        if (celestial_objects_list_head == nullptr) {
            celestial_objects_list_head = new_object;
        }
        else {
            previous_object->next_celestial_object = new_object;
            if (previous_clockwise_90) {
                previous_clockwise_90->next_celestial_object = new_object;
            }
            if (previous_clockwise_180) {
                previous_clockwise_180->next_celestial_object = new_object;
            }
            if (previous_clockwise_270) {
                previous_clockwise_270->next_celestial_object = new_object;
            }

        }
        previous_object = new_object;

        if (clockwise_90) {
            previous_clockwise_90 = clockwise_90;
        }
        if (clockwise_180) {
            previous_clockwise_180 = clockwise_180;
        }
        if (clockwise_270) {
            previous_clockwise_270 = clockwise_270;
        }
    }

    file.close();
}

// Function to shoot a projectile
void AsteroidDash::shoot() {
    if (player && player->current_ammo > 0) {
        int player_row = player->position_row;
        int player_col = player->position_col;
        const auto &shape = player->spacecraft_shape;


        int shoot_row = player_row + (shape.size() / 2);
        int shoot_col = player_col + shape[0].size();


        Projectile new_projectile(shoot_row, shoot_col);
        projectiles.push_back(new_projectile);


        player->current_ammo--;
    }
}

void AsteroidDash::print_space_grid() const {

    std::cout << "Tick: " << game_time << "\n";
    if (player != nullptr) {
        std::cout << "Lives: " << player->lives << "\n";
        std::cout << "Ammo: " << player->current_ammo << "\n";
    }
    std::cout << "Score: " << current_score << "\n";

    unsigned long high_score = 0;
    if (leaderboard.head_leaderboard_entry != nullptr) {
        high_score = leaderboard.head_leaderboard_entry->score;
    }


    std::cout << "High Score: " << high_score << "\n";
    if (game_over) {
        cout << "Player: " << player->player_name<< "\n";
    }

    for (const auto &row : space_grid) {
        for (int cell : row) {
            if (cell > 0) {
                std::cout << occupiedCellChar;
            } else {
                std::cout << unoccupiedCellChar;
            }
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}



// Function to update the space grid with player, celestial objects, and projectiles
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {

    for (auto &row : space_grid) {
        for (auto &cell : row) {
            cell = 0;
        }
    }


    bool collision_detected = false;


    if (player) {
        int player_row = player->position_row;
        int player_col = player->position_col;
        const auto &shape = player->spacecraft_shape;

        for (int i = 0; i < shape.size(); ++i) {
            for (int j = 0; j < shape[i].size(); ++j) {
                if (shape[i][j] && player_row + i < space_grid.size() && player_col + j < space_grid[0].size()) {
                    space_grid[player_row + i][player_col + j] = 1;
                }
            }
        }
    }


    CelestialObject *current_object = celestial_objects_list_head;
    while (current_object != nullptr) {
        if (!current_object->is_visible) {
            current_object = current_object->next_celestial_object;
            continue;
        }

        if (game_time >= current_object->time_of_appearance) {
            int object_row = current_object->starting_row;
            const auto &shape = current_object->shape;
            int visible_column = game_time - current_object->time_of_appearance;
            int final_column = space_grid[0].size() - 1 - visible_column;

            for (int i = 0; i < shape.size(); ++i) {
                for (int j = 0; j < shape[i].size(); ++j) {
                    if (shape[i][j] && object_row + i < space_grid.size() && final_column + j < space_grid[0].size()) {

                        if (space_grid[object_row + i][final_column + j] == 1) {
                            collision_detected = true;
                            if (current_object->object_type == ASTEROID) {
                                player->lives--;
                                if (player->lives <= 0) {
                                    game_over = true;
                                }
                            } else if (current_object->object_type == LIFE_UP) {
                                player->lives++;
                            } else if (current_object->object_type == AMMO) {
                                player->current_ammo = player->max_ammo;
                            }
                            current_object->is_visible = false;
                        }
                        space_grid[object_row + i][final_column + j] = 1;
                    }
                }
            }
        }
        current_object = current_object->next_celestial_object;
    }


    for (auto &projectile : projectiles) {
        if (projectile.is_active()) {
            if (projectile.row >= 0 && projectile.row < space_grid.size() && projectile.col < space_grid[0].size()) {
                space_grid[projectile.row][projectile.col] = 1;
                projectile.move();
            } else {
                projectile.deactivate();
            }
        }
    }


    if (!collision_detected && game_time > 0) {
        current_score += 1;
    }
}





// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {

    if (player) {
        delete player;
        player = nullptr;
    }


    CelestialObject *current = celestial_objects_list_head;
    while (current != nullptr) {
        CelestialObject *next = current->next_celestial_object;


        CelestialObject::delete_rotations(current);

        current = next;
    }

    celestial_objects_list_head = nullptr;
}
