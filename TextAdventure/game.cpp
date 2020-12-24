//
//  game.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-27.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "game.hpp"

const std::string GAME_SAVE_FILE = "Content/game_save.txt";

Game& Game::InstanceOf() {
    static Game game;
    return game;
}

const void Game::GameStart(void) {
    gamedata.Introduction();

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
        save_file << player.name << "\n";

        if(player.current_location != nullptr) {
            save_file << "#" << player.current_location->location_id << "\n";
        } else {
            save_file << "#beginGame" << "\n";
        }

        save_file << "&" << player.moves << "\n";
        
        for(int i = 0; i < player.inventory.size(); ++i) {
            save_file << "*" << player.inventory[i].item->id << ":" << player.inventory[i].inventory_amount << "\n";
        }
        
        for(int i = 0; i < player.locations_visited.size(); ++i) {
            save_file << "=" << player.locations_visited[i] << "\n";
        }

    } else {
        std::cout << "[ERROR] Could not open game_save.txt file.\n";
    }
}

const void Game::LoadGame(void) {
    std::ifstream load_file(GAME_SAVE_FILE);
    std::string line;
    
    if (load_file.is_open()) {

        while (std::getline(load_file, line)) {
            size_t match_start_location = line.find("#");
            size_t match_player_moves = line.find("&");
            size_t match_locations_visited = line.find("=");
            size_t match_item_type = line.find("*");
            size_t match_item_amount = line.find(":");
            
            if (match_start_location != std::string::npos) {
                player.current_location = gamedata.GetLocationById(line.substr(1));

            } else if (match_player_moves != std::string::npos) {
                player.moves = std::stoi(line.substr(1));

            } else if (match_locations_visited != std::string::npos) {
                player.locations_visited.push_back(line.substr(1));

            } else if (match_item_type != std::string::npos) {
                std::string item = line.substr(1, match_item_amount - 1);
                int amount = std::stoi(line.substr(match_item_amount + 1));

                player.AddItem(item, amount);

            } else {
                player.name = line;
            }
        }

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
    std::cout << "[2] Save & resume game\n";
    std::cout << "[3] Exit game\n";
    std::cout << "=================\n";

    while (choice == 0) {
      std::string line;
      std::getline(std::cin, line);
      gamedata.ValidateUserInput(choice, line);
    }
    
    if (choice == 1 ) {
        return 0;

    } else if (choice == 2) {
        std::cout << "Saving game...\n";
        gamedata.WaitAMinute();
        SaveGame();
        std::cout << "Your game was saved. \n";
        return 0;

    } else if (choice == 3) {
        std::cout << "Exiting game..\n";
        gamedata.WaitAMinute();
        game_mode = GameMode::Exit;
    }
    
    return 0;
}

const int Game::InventoryMenu(void) {
    int choice = 0;
    
    if (Game::InstanceOf().player.inventory.size() == 0) {
        std::cout << "You have no items in your inventory.\n";

        return 0;
    }

    std::cout << "=================\n";
    std::cout << "You have the following items in your inventory:\n";

    for (int i = 0; i < Game::InstanceOf().player.inventory.size(); ++i) {
        std::cout << "[" << i+1 << "] " << Game::InstanceOf().player.inventory[i].item->title << " (x" << Game::InstanceOf().player.inventory[i].inventory_amount << ")" << "\n";
    }

    std::cout << "[" << Game::InstanceOf().player.inventory.size()+1 << "] " << "Exit inventory\n";
    std::cout << "=================\n";
    
    while (choice == 0) {
      std::string line;
      std::getline(std::cin, line);
      gamedata.ValidateUserInput(choice, line);
    }
    
    if (choice == Game::InstanceOf().player.inventory.size()+1) {
        return 0;
    }

    if (choice != 0 && (choice <= Game::InstanceOf().player.inventory.size())) {
        std::shared_ptr<BaseItem> item = player.inventory[choice-1].item;
        
        if(item != nullptr) {
            item->UseItem();
        }

    }

    return 0;
}

const void Game::Run(void) {
    std::string name = "";

    while (game_mode == GameMode::IsRunning) {
        
        if (player.name == "") {
            player.name = gamedata.GetPlayerName(name);
        }

        if (player.current_location == nullptr) {
            std::cout << "[ERROR] No such location. Ending game. \n";
            game_mode = GameMode::Exit;

        } else if (player.current_location->choices.size() == 0) {
            std::cout << "Game Over.\n";
            std::cout << "You made a total of " << player.moves << " moves and visited the following game locations: \n";

            for(int i = 0; i < player.locations_visited.size(); ++i) {
                std::cout << player.locations_visited[i] << "\n";
            }

            game_mode = GameMode::Exit;

        } else {
            bool is_valid_input = false;
            int choice = -1;

            gamedata.WaitAMinute();
            std::cout << "---\n";
            std::cout << "-> " << gamedata.PersonalizeText(player.name, player.current_location->location_text) << "\n";
            
            if (player.HasVisitedLocation() == false) {
                gamedata.CheckForLocationItems();
                player.locations_visited.push_back(player.current_location->location_id);
            }
            std::cout << "---\n";

            while (is_valid_input || choice < 0 || choice >= player.current_location->choices.size()+1) {

                player.ShowChoicesAndMenu(choice);

                std::string line;
                std::getline(std::cin, line);
                
                if (line.size() > 0 && (line[0] == 'm' || line[0] == 'M')) {
                    InGameMenu();
                    return;

                } else if (line.size() > 0 && (line[0] == 'i' || line[0] == 'I')) {
                    InventoryMenu();
                    return;

                } else {
                    is_valid_input = gamedata.ValidateUserInput(choice, line);
                }
            }
            
            if (choice >= 0 && choice < player.current_location->choices.size()+1) {
                const std::string& upcoming_location_id = player.current_location->choices[choice-1]->next_location_id;
                player.current_location = gamedata.GetLocationById(upcoming_location_id);
                
                player.moves++;
                gamedata.ReducePlayerSatiety();
            }
            
            if (player.satiation <= 0) {
                game_mode = GameMode::Exit;
            }
        }
    }
}
