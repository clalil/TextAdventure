//
//  game_data.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-19.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "globals.hpp"
#include "game_data.hpp"
#include <chrono>
#include <random>
#include <thread>
#include <algorithm>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <filesystem>
#include <iostream>
using namespace std::chrono_literals;

LocationChoice::LocationChoice(const std::string& choice_id, const std::string& choice_description) {
    next_location_id = choice_id;
    next_location_text = choice_description;
}

Location::Location(const std::string& id, const std::string& descriptive_text) {
    location_id = id;
    location_text = descriptive_text;
}

GameData::GameData() {
    CreateLocations();
    InitializeItems();
}

const void GameData::InitializeItems() {
    std::shared_ptr<BaseItem> scroll01{ new TeleportScroll("scroll01", "Teleport Scroll", "beginGame") };
    std::shared_ptr<BaseItem> fooditem1{ new FoodItem("fooditem1", "Lollipop", 10) };

    items.push_back(scroll01);
    items.push_back(fooditem1);
}

std::shared_ptr<BaseItem> GameData::GetItemsById(const std::string& item_id) {
    for (int i = 0; i < items.size(); ++i) {
        if (items[i]->id == item_id) {

            return items[i];
        }
    }

    return nullptr;
}

int GameData::LoadLocationData(const std::string path) {
    int locations_added = 0;
    std::ifstream location_file(path);
    std::string line;

    if (location_file.is_open() == false) {
        return 0;
    }

    std::shared_ptr<Location> working_location = std::make_shared<Location>("", "");

    while (std::getline(location_file, line)) {
        size_t match_location_comment = line.find("//");
        size_t match_location_id = line.find("#");
        size_t match_location_choice_id = line.find("&");
        size_t match_location_choice_description = line.find(":");
        size_t match_location_endline = line.find("=");
        
        if (match_location_comment != std::string::npos) {
            continue;
        } else if (match_location_id != std::string::npos) {
            working_location->location_id = line.substr(1);
        } else if (match_location_choice_id != std::string::npos) {
            std::shared_ptr<LocationChoice> working_location_choice = std::make_shared<LocationChoice>(line.substr(1, match_location_choice_description - 1), line.substr(match_location_choice_description + 2));
            
            working_location->choices.push_back(working_location_choice);
            
            working_location_choice = std::make_shared<LocationChoice>("", "");
        } else if (match_location_endline != std::string::npos) {
            locations.push_back(working_location);

            working_location = std::make_shared<Location>("", "");
            
        } else {
            working_location->location_text += line;
            working_location->location_text += "\n";
        }
    }
    
    return locations_added;
}

void GameData::CreateLocations(void) {
    namespace fs = std::__fs::filesystem;
    std::string directory_path = "Content/Locations/";
    fs::path path_to_load(directory_path);
    
    if (fs::exists(path_to_load)) {
        for (const auto& entry : fs::directory_iterator(path_to_load)) {
            std::string filename = entry.path().filename();
            std::string file_to_load = directory_path + filename;

            LoadLocationData(file_to_load);
        }
    }
}

std::shared_ptr<Location> GameData::GetStartLocation(void) {
    if (locations.size() != 0) {
        return locations.front();
    }
    
    std::cout << "[ERROR] The game locations did not load properly. \n";

    return nullptr;
}

std::shared_ptr<Location> GameData::GetLocationWithId(const std::string& id) {
    for (int i = 0; i < locations.size(); ++i) {
        if(locations[i]->location_id == id) {
            return locations[i];
        }
    }
    
    return nullptr;
}

const int GameData::ValidateUserInput(int& choice, const std::string& input) {
    // line is not a number, e.g. "abc" or "abc123", or the number is too big
    // to fit in an int, e.g. "11111111111111111111111111111111111"
    try {
        choice = std::stoi(input);
    } catch (std::exception const& exc) {
        std::cout << "You've entered an invalid input. Please try again." << "\n";
        return 1;
    }
    
    return 0;
}

const std::string GameData::GetPlayerName(std::string& user_name) {
    std::cout << "Please enter your name: \n";
    std::getline(std::cin, user_name);
    
    return user_name;
}

const std::string GameData::PersonalizeText(const std::string& player_name, std::string& location_text) {
    size_t match = location_text.find("%%NAME%%");

    if (match != std::string::npos) {
        std::regex name_regex("%%NAME%%");
        std::string personalized_text = std::regex_replace (location_text, name_regex, player_name);
        location_text = personalized_text;

        return location_text;
    }

    return location_text;
}

const void GameData::Introduction(void) {
    std::cout << "*******************\n";
    std::cout << "House of the Haunted\n";
    std::cout << "*******************\n";
}

const void GameData::WaitAMinute(void) {
    std::this_thread::sleep_until(std::chrono::system_clock::now() + 1s);
}

//In this method, you should print a list of the items the user has in their inventory with amount > 0, just like when you are presenting choices for the user to select from at a location.
//If the user enters a valid number for an item they can use - get the item from GameData and call “use()” on that item. After you have used the item, reduce the “amount” of this item in the inventory.
//When done, return to the main game as per usual.

const int GameData::InventoryMenu(void) {
    int choice = 0;
    
    if (Game::InstanceOf().player.inventory.size() == 0) {
        std::cout << "You have no items in your inventory.\n";
        return -1;
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
      ValidateUserInput(choice, line);
    }
    
    if (choice == Game::InstanceOf().player.inventory.size()+1) {
        return -1;
    }

    return choice-1;
}

//Code below only used for debugging purposes

const bool GameData::LocationExistsWithId(const std::string id) {
     for (int i = 0; i < locations.size(); ++i) {
         std::shared_ptr<Location> location = locations[i];
        if (location->location_id == id) {
            return true;
        }
    }

    return false;
}

const void GameData::ReducePlayerSatiety(void) {
    auto hunger_level = Game::InstanceOf().player.satiation;

    if(hunger_level <= 50) {
        std::cout << "You are hungry. You should eat something.\n";
    } else if (hunger_level == 20) {
        std::cout << "You are really hungey. You should really eat something.\n";
    } else if (hunger_level < 20 ) {
        std::cout << "You are starving. You need to eat something to sustain you.\n";
    } else if (hunger_level <= 0) {
        std::cout << "You're about to starve to death and decide to go home and eat something.\n";
    }
    
    unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 random_generator = std::mt19937(seed);
    std::uniform_int_distribution<int> random_hunger_reducer(1, 10);
    
    Game::InstanceOf().player.satiation -= random_hunger_reducer(random_generator);
}

const void GameData::DebugLocations(void) {
    std::vector<std::string> location_ids = {};

    std::cout << "Number of available locations: " << locations.size() << "\n";

    for (int i = 0; i < locations.size(); ++i) {
        std::shared_ptr<Location> location = locations[i];
        location_ids.push_back(location->location_id);

        for (int j = 0; j < location->choices.size(); ++j) {
            std::shared_ptr<LocationChoice> choice = location->choices[j];

            if (LocationExistsWithId(choice->next_location_id) == false) {
                std::cout << "[WARNING] Choice '" << (j+1) << "' on location '" << location->location_id << "' points to '" << choice->next_location_id << "' which doesn't exist.\n";
            }
        }
    }
    
    const auto duplicate = std::adjacent_find(location_ids.begin(), location_ids.end());

    if (duplicate != location_ids.end()) {
        std::cout << "[WARNING] An ID named '" << *duplicate << "' is a duplicate." << "\n";
    }
}
