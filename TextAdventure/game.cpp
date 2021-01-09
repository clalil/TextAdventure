//
//  game.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-27.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "utils.hpp"
#include "game.hpp"

const std::string GAME_SAVE_FILE = "Content/game_save.txt";

Game& Game::InstanceOf() {
    static Game game;
    return game;
}

void Game::GameStart(void) {
    gamedata.MainScreen();

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

void Game::SaveGame(void) {
    std::ofstream save_file(GAME_SAVE_FILE, std::ios::trunc);

    if (save_file.is_open()) {
        save_file << player.name << "\n";

        if (player.current_location != nullptr) {
            save_file << "#" << player.current_location->location_id << "\n";
        } else {
            save_file << "#beginGame" << "\n";
        }

        save_file << "&" << player.moves << "\n";
        
        for (int i = 0; i < player.inventory.size(); ++i) {
            save_file << "*" << player.inventory[i].item->GetId() << ":" << player.inventory[i].inventory_amount << "\n";
        }
        
        for (int i = 0; i < player.locations_visited.size(); ++i) {
            save_file << "=" << player.locations_visited[i] << "\n";
        }

    } else {
        std::cout << "[ERROR] Could not open game_save.txt file.\n";
    }
}

void Game::LoadGame(void) {
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

void Game::MainMenu(void) {
    std::string line;
    int choice;

    std::cout << "Do you want to:\n";
    std::cout << "[1] Start new game\n";
    std::cout << "[2] Load saved game\n";
    std::cout << "[3] Debug game locations\n";
    std::cout << "[4] Exit game\n";
    std::cout << "> ";

    std::getline(std::cin, line);
    choice = std::stoi(line);

    ValidateUserInput(choice, line);
    
    switch (choice) {
        case 1: {
            std::string name = "";
            player.name = gamedata.GetPlayerName(name);

            player.current_location = gamedata.GetStartLocation();
            player.moves = 0;
            game_mode = GameMode::IsRunning;
            player.AddItem("pendant01", 1);
            player.AddItem("pendant02", 1);

            Run();
            break;
        }
        case 2: {
            game_mode = GameMode::IsRunning;

            LoadGame();
            Run();
            break;
        }
        case 3: {
            gamedata.DebugLocations();
            break;
        }
        default: {
            game_mode = GameMode::Exit;
            break;
        }
    }
}

void Game::GameHints(void) const {
    std::cout << "How to win this game:\n";
    std::cout << "- Save your game progress frequently.\n";
    std::cout << "- Keep an eye on your Food HP & don't let it go below 20.\n";
    std::cout << "- Some locations can only be visited once, so be careful with your choices.\n";
    std::cout << "- If you're stuck, try combining items in your inventory. It may reveal new choices.\n";
}

int Game::InGameMenu(void) {
    int choice = 0;
    
    std::cout << "=================\n";
    std::cout << "[1] Game hints \n";
    std::cout << "[2] Resume game\n";
    std::cout << "[3] Save & resume game\n";
    std::cout << "[4] Exit game\n";
    std::cout << "=================\n";

    while (choice == 0) {
      std::cout << "> ";
      std::string line;
      std::getline(std::cin, line);
      ValidateUserInput(choice, line);
    }
    
    if (choice == 1) {
        GameHints();
        WaitASecond();
        std::cout << "Closing menu... \n";
        return 0;

    } else if (choice == 2) {
        return 0;

    } else if (choice == 3) {
        std::cout << "Saving game...\n";
        WaitASecond();
        SaveGame();
        std::cout << "Your game was saved. \n";
        return 0;

    } else if (choice == 4) {
        std::cout << "Exiting game..\n";
        WaitASecond();
        game_mode = GameMode::Exit;
    }
    
    return 0;
}

int Game::CombineItemsMenu(void) {
    int choice1 = 0;
    int choice2 = 0;
    
    if (player.inventory.size() < 2) {
        std::cout << "You don't have enough items to combine.\n";
        return 0;
    }
    
    while ((choice1 == 0) || (choice2 == 0)) {
      std::cout << "> ";
      std::string item1;
      std::cout << "Enter the number of the first item to combine.\n";
      std::cin >> item1;
      ValidateUserInput(choice1, item1);
        
      std::cout << "> ";
      std::string item2;
      std::cout << "Enter the number of the second item to combine.\n";
      std::cin >> item2;
      ValidateUserInput(choice2, item2);
    }
    
    WaitASecond();
    
    std::string player_choice1 = player.inventory[choice1-1].item->GetId();
    std::string player_choice2 = player.inventory[choice2-1].item->GetId();
    
    if (gamedata.CompatibleItems(player_choice1, player_choice2)) {
        std::string new_item = gamedata.CraftNewItem(player_choice1, player_choice2);

        if (new_item != "itemFail") {
            std::cout << "You've successfully combined" << " [" << player.inventory[choice1-1].item->GetTitle() << "] " << "with [" << player.inventory[choice1-1].item->GetTitle() << "].\n";

            player.RemoveItem(player_choice1, 1);
            player.RemoveItem(player_choice2, 1);
            
            player.AddItem(new_item, 1);
        }

    } else {
        std::cout << "These items cannot be combined.\n";
    }
    
    return 0;
}

int Game::InventoryMenu(void) {
    bool invalid_input = true;
    int choice = 0;
    
    if (Game::InstanceOf().player.inventory.size() == 0) {
        std::cout << "You have no items in your inventory.\n";

        return 0;
    }

    std::cout << "=================\n";
    std::cout << "You have the following items in your inventory:\n\n";

    for (int i = 0; i < Game::InstanceOf().player.inventory.size(); ++i) {
        std::cout << "[" << i+1 << "] " << Game::InstanceOf().player.inventory[i].item->GetTitle() << " (x" << Game::InstanceOf().player.inventory[i].inventory_amount << ")" << "\n";
    }
    
    std::cout << "..................\n";
    std::cout << "[c] " << "Combine items\n";
    std::cout << "[e] " << "Exit inventory\n";

    std::cout << "==================\n";
    
    while (choice == 0 || invalid_input) {
      std::cout << "> ";
      std::string line;
      std::getline(std::cin, line);

        if (line.size() > 0 && (line[0] == 'c' || line[0] == 'C')) {
            CombineItemsMenu();

        } else if (line.size() > 0 && (line[0] == 'e' || line[0] == 'E')) {
            break;

        } else {
            invalid_input = ValidateUserInput(choice, line);
        }
    }

    if (choice != 0 && (choice <= Game::InstanceOf().player.inventory.size())) {
        std::shared_ptr<BaseItem> item = player.inventory[choice-1].item;
        
        if (item != nullptr) {
            item->UseItem();
        }
        
        return 0;
    }

    return 0;
}

void Game::Run(void) {
    WaitASecond();

    while (game_mode == GameMode::IsRunning) {

        if (player.current_location == nullptr) {
            std::cout << "[ERROR] No such location. Ending game. \n";
            game_mode = GameMode::Exit;

        } else if (player.current_location->choices.size() == 0) {
            WaitASecond();
            std::cout << "Game Over.\n";
            std::cout << "You made a total of " << player.moves << " moves and visited the following game locations: \n";

            for(int i = 0; i < player.locations_visited.size(); ++i) {
                std::cout << player.locations_visited[i] << "\n";
            }

            game_mode = GameMode::Exit;

        } else {
            int valid_choices = (int)player.current_location->choices.size();
            bool invalid_input = true;
            int choice = -1;

            WaitASecond();
            std::cout << "---\n";
            if (gamedata.PersonalizeText(player.name, player.current_location->location_text) != "") {
                std::cout << gamedata.PersonalizeText(player.name, player.current_location->location_text) << "\n";
            }
            WaitASecond();
            
            if (player.HasVisitedLocation() == false) {
                gamedata.CheckForLocationItems();
                player.locations_visited.push_back(player.current_location->location_id);
            }
            std::cout << "---\n\n";

            while (invalid_input || choice < 0 || (choice >= valid_choices)) {

                valid_choices = gamedata.ShowChoicesAndMenu(choice);

                std::cout << "> ";
                std::string line;
                std::getline(std::cin, line);
                
                if (line.size() > 0 && (line[0] == 'm' || line[0] == 'M')) {
                    InGameMenu();
                    break;

                } else if (line.size() > 0 && (line[0] == 'i' || line[0] == 'I')) {
                    InventoryMenu();
                    break;

                } else {
                    invalid_input = ValidateUserInput(choice, line);
                }
                
                if (choice < valid_choices) {
                    if ((player.current_location->choices[choice-1]->required_item_id != "") && (player.HasItem(player.current_location->choices[choice-1]->required_item_id) == false)) {
                        std::shared_ptr<BaseItem> item = gamedata.GetItemById(player.current_location->choices[choice-1]->required_item_id);

                        if (item != nullptr) {
                            std::cout << "You need the" << " [" << item->GetTitle() << "] " << "to proceed with this choice.\n\n";
                        }

                        choice = -1;
                    }
                }
            }
            
            if (choice >= 0 && choice < valid_choices) {
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
