//
//  game.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-27.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "game.hpp"
#include <fstream>
const std::string GAME_SAVE_FILE = "Content/game_save.txt";

Game& Game::InstanceOf() {
    static Game game;
    return game;
}

const void Game::GameStart(void) {
    std::string name;
    gamedata.Introduction();

    player.name = gamedata.GetPlayerName(name);
    
    while (game_mode != GameMode::Exit) {
        switch(game_mode) {
            case GameMode::Menu:
                MainMenu();
                break;
            case GameMode::IsRunning:
                Run();
                break;
            default:
                break;
        }
    }
    
    std::cout << "Hope you enjoyed the game!\n";
    
}

const void Game::SaveGame(void) {
    std::ofstream save_file(GAME_SAVE_FILE, std::ios::trunc);

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
    std::ifstream load_file(GAME_SAVE_FILE);
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

const void Game::MainMenu(void) {
    std::string line;
    int choice;

    std::cout << "Do you want to:\n";
    std::cout << "[1] Start new game\n";
    std::cout << "[2] Load saved game\n";
    std::cout << "[3] Debug game locations\n";
    std::cout << "[4] Exit game\n";

    std::getline(std::cin, line);
    choice = std::stoi(line);

    gamedata.ValidateUserInput(choice, line);
    
    switch(choice) {
        case 1:
            player.current_location = gamedata.GetStartLocation();
            player.moves = 0;
            player.AddItem("scroll01", 1);
            game_mode = GameMode::IsRunning;

            Run();
            break;
        case 2:
            game_mode = GameMode::IsRunning;

            LoadGame();
            Run();
            break;
        case 3:
            gamedata.DebugLocations();
            break;
        default:
            game_mode = GameMode::Exit;
            break;
    }
}

const int Game::InGameMenu(void) {
    int choice = 0;
    
    std::cout << "=================\n";
    std::cout << "[1] Resume game\n";
    std::cout << "[2] Save and Exit game\n";
    std::cout << "=================\n";

    while (choice == 0) {
      std::string line;
      std::getline(std::cin, line);
      gamedata.ValidateUserInput(choice, line);
    }
    
    if (choice == 1 || choice == 2) {
        return choice;
    }
    
    return 2;
}

const void Game::Run(void) {
    while (game_mode == GameMode::IsRunning) {
        if (player.current_location == nullptr) {
            std::cout << "[ERROR] No such location. Ending game. \n";
            game_mode = GameMode::Exit;

        } else if (player.current_location->choices.size() == 0) {
            std::cout << "Game Over.\n";
            std::cout << "You made a total of " << player.moves << " moves and visited the following game locations: \n";

            for(int i = 0; i < player.locations_visited.size(); ++i) {
                std::cout << player.locations_visited[i]->location_id << "\n";
            }

            game_mode = GameMode::Exit;

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
            
            std::cout << "[i] Inventory\n";
            std::cout << "[m] Menu\n";
            std::cout << "Player satiety: " << player.satiation << "\n";

            while (is_valid_input || choice < 0 || choice >= player.current_location->choices.size()+1) {
                std::string line;
                std::getline(std::cin, line);
                
                if (line.size() > 0 && line[0] == 'm') {
                    int input = InGameMenu();

                    if (input == 1) {
                        std::cout << player.current_location->location_text << "\n";
                        std::cout << "Where do you wish to proceed next?\n";
                        std::getline(std::cin, line);
                        is_valid_input = gamedata.ValidateUserInput(choice, line);

                    } else if (input == 2) {
                        std::cout << "Exiting game\n";
                        SaveGame();
                        game_mode = GameMode::Exit;
                        break;
                    }
                } else if (line.size() > 0 && line[0] == 'i') {
                    int input = gamedata.InventoryMenu();

                    if (input != -1) {
                        std::shared_ptr<BaseItem> item = player.inventory[input].item;
                        
                        if(item != nullptr) {
                            item->UseItem();
                            break;
                        }
                    } else {
                        std::cout << player.current_location->location_text << "\n";
                        std::cout << "Where do you wish to proceed next?\n";
                        std::getline(std::cin, line);
                        is_valid_input = gamedata.ValidateUserInput(choice, line);
                    }
                } else {
                    is_valid_input = gamedata.ValidateUserInput(choice, line);
                }
            }
            
            if (choice >= 0 && choice < player.current_location->choices.size()+1) {
                const std::string& upcoming_location_id = player.current_location->choices[choice-1]->next_location_id;
                player.current_location = gamedata.GetLocationWithId(upcoming_location_id);
                
                player.moves++;
                gamedata.ReducePlayerSatiety();
            }
            
            if (player.satiation <= 0) {
                game_mode = GameMode::Exit;
            }
        }
    }
}
