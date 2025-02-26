#include "GameController.h"
#include <iostream>
#include <fstream>
// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
    // TODO: Your code here, if you want to perform extra initializations
}

void GameController::play(const string &commands_file) {
    std::ifstream file(commands_file);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << commands_file << std::endl;
        return;
    }

    std::string command;
    while (std::getline(file, command)) {
        if (game->player->lives <= 0) {
            // Game ends if the player loses all lives
            std::cout << "GAME OVER!" << std::endl;
            for (int i = 0; i < game->player->spacecraft_shape.size(); ++i) {
                for (int j = 0; j < game->player->spacecraft_shape[i].size(); ++j) {
                    game->player->spacecraft_shape[i][j] = false;
                }
            }
            game->game_over = true;
            game->update_space_grid();
            break;
        }

        game->update_space_grid();



        if (command == "PRINT_GRID") {
            game->print_space_grid();
        }
        else if (command == "MOVE_UP") {
            game->player->move_up();
        }
        else if (command == "MOVE_DOWN") {
            game->player->move_down(game->space_grid.size());
        }
        else if (command == "MOVE_RIGHT") {
            game->player->move_right(game->space_grid[0].size());
        }
        else if (command == "MOVE_LEFT") {
            game->player->move_left();
        }
        else if (command == "SHOOT") {
            game->shoot();
        }
        else if (command == "NOP") {

        }

        else {
            std::cerr << "Unknown command: " << command << std::endl;
        }
        if(!game->game_over){
        game->game_time++;
        }
    }


    file.close();

    if (game->player->lives > 0 && game->game_time > 0) {
      	game->update_space_grid();
        game->game_time--;
        std::cout << "GAME FINISHED! No more commands!" << std::endl;
        game->game_over = true;
        for (int i = 0; i < game->player->spacecraft_shape.size(); ++i) {
        	for (int j = 0; j < game->player->spacecraft_shape[i].size(); ++j) {
                    game->player->spacecraft_shape[i][j] = false;
            }
        }
    }



    game->print_space_grid();



    LeaderboardEntry *new_entry = new LeaderboardEntry(
        game->current_score,
        time(nullptr),
        game->player->player_name
    );
    game->leaderboard.insert(new_entry);


    game->leaderboard.write_to_file(game->leaderboard_file_name);

}


// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    delete game;
}

