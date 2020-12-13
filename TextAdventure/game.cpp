//
//  game.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-27.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "game.hpp"
#include <fstream>

const void Game::SaveGame(void) {
    std::ofstream save_file("Content/game_save.txt", std::ios::trunc);

    if (save_file.is_open()) {
        if(player.current_location != nullptr) {
            save_file << player.current_location->location_id << "\n";
        } else {
            save_file << "beginGame" << "\n";
        }

        save_file << player.moves << "\n";
    } else {
        std::cout << "[ERROR] Could not open game_save.txt file.\n";
    }
}

const void Game::LoadGame(void) {
    std::ifstream load_file("Content/game_save.txt");
    std::string line;

    if (load_file.is_open()) {
        std::getline(load_file, line);
            player.current_location = gamedata.GetLocationWithId(line);
        std::getline(load_file, line);
            player.moves = std::stoi(line);
    } else {
        std::cout << "[ERROR] Could not open game_save.txt file.\n";
    }
}

const void Game::GameStart(void) {
    std::string line;
    int choice;

    std::cout << "Do you want to:\n";
    std::cout << "[1] Start new game\n";
    std::cout << "[2] Load saved game\n";
    std::getline(std::cin, line);
    choice = std::stoi(line);

    gamedata.IsInvalidInput(choice, line);
    
    if (choice == 1) {
        player.current_location = gamedata.GetStartLocation();
        player.moves = 0;
        Run();
    } else if (choice == 2) {
        LoadGame();
        Run();
    }
}

void Game::Run(void) {
    is_running = true;
    std::string name;

    gamedata.Introduction();
    gamedata.DebugLocations();
    player.name = gamedata.GetPlayerName(name);
    
    while(is_running) {
        if(player.current_location == nullptr) {
            std::cout << "[ERROR] No such location. Ending game. \n";
            is_running = false;

        } else if (player.current_location->choices.size() == 0) {
            std::cout << "Game Over.\n";
            std::cout << "You made a total of " << player.moves << " moves and visited the following game locations: \n";

            for(int i = 0; i < player.locations_visited.size(); ++i) {
                std::cout << player.locations_visited[i]->location_id << "\n";
            }

            is_running = false;

        } else {
            bool is_valid_input = false;
            int choice = -1;
            player.locations_visited.push_back(player.current_location);

            gamedata.WaitAMinute();
            std::cout << "---\n";
            std::cout << "-> " << gamedata.PersonalizeText(player.name, player.current_location->location_text) << "\n";
            std::cout << "---\n";
            std::cout << "Where do you wish to proceed next?\n";

            for(int i = 0; i < player.current_location->choices.size(); ++i) {
                std::cout << "[" << i+1 << "] " << player.current_location->choices[i]->next_location_text << "\n";
            }
            
            std::cout << "[m] Menu\n";

            while(is_valid_input || choice < 0 || choice >= player.current_location->choices.size()+1) {
                std::string line;
                std::getline(std::cin, line);
                
                if (line.size() > 0 && line[0] == 'm') {
                    int input = gamedata.GameMenu();

                    if (input == 1) {
                        std::cout << "Where do you wish to proceed next?\n";
                        std::getline(std::cin, line);
                        is_valid_input = gamedata.IsInvalidInput(choice, line);

                    } else if (input == 2) {
                        std::cout << "Exiting game\n";
                        SaveGame();
                        is_running = false;
                        break;
                    }
                } else {
                    is_valid_input = gamedata.IsInvalidInput(choice, line);
                }
            }
            
            if (choice >= 0 && choice < player.current_location->choices.size()+1) {
                const std::string& upcoming_location_id = player.current_location->choices[choice-1]->next_location_id;
                player.current_location = gamedata.GetLocationWithId(upcoming_location_id);
                
                player.moves++;
            }
        }
    }
}
