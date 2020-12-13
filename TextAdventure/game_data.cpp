//
//  game_data.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-19.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "game_data.hpp"
#include <chrono>
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

    items.push_back(scroll01);
}

std::shared_ptr<BaseItem> GameData::GetItemsById(const std::string& item_id) {
    for(int i = 0; i < items.size(); ++i) {
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

    while(std::getline(location_file, line)) {
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
    
    if(fs::exists(path_to_load)) {
        for (const auto& entry : fs::directory_iterator(path_to_load)) {
            std::string filename = entry.path().filename();
            std::string file_to_load = directory_path + filename;

            LoadLocationData(file_to_load);
        }
    }
}

std::shared_ptr<Location> GameData::GetStartLocation(void) {
    if(locations.size() != 0) {
        return locations.front();
    }
    
    std::cout << "[ERROR] The game locations did not load properly. \n";

    return nullptr;
}

std::shared_ptr<Location> GameData::GetLocationWithId(const std::string& id) {
    for(int i = 0; i < locations.size(); ++i) {
        if(locations[i]->location_id == id) {
            return locations[i];
        }
    }
    
    return nullptr;
}

const int GameData::IsInvalidInput(int& choice, std::string input) {
    // line is not a number, e.g. "abc" or "abc123", or the number is too big
    // to fit in an int, e.g. "11111111111111111111111111111111111"
    try {
        choice = std::stoi(input);
    } catch (std::exception const exc) {
        std::cout << "You've entered an invalid input. Please try again." << "\n";
        return 1;
    }
    
    return 0;
}

std::string GameData::GetPlayerName(std::string& user_name) {
    std::cout << "Please enter your name: \n";
    std::getline(std::cin, user_name);
    
    return user_name;
}

std::string GameData::PersonalizeText(const std::string& player_name, std::string& location_text) {
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

const int GameData::GameMenu(void) {
    int choice = 0;
    
    std::cout << "=================\n";
    std::cout << "[1] Resume game\n";
    std::cout << "[2] Exit game\n";
    std::cout << "=================\n";

    while(choice == 0) {
      std::string line;
      std::getline(std::cin, line);
      IsInvalidInput(choice, line);
    }
    
    if (choice == 1 || choice == 2) {
        return choice;
    }
    
    return 2;
}

//Code below only used for debugging purposes

const bool GameData::LocationExistsWithId(const std::string id) {
     for(int i = 0; i < locations.size(); ++i) {
         std::shared_ptr<Location> location = locations[i];
        if (location->location_id == id) {
            return true;
        }
    }

    return false;
}

const void GameData::DebugLocations(void) {
    std::vector<std::string> location_ids = {};

    std::cout << "Number of available locations: " << locations.size() << "\n";

    for(int i = 0; i < locations.size(); ++i) {
        std::shared_ptr<Location> location = locations[i];
        location_ids.push_back(location->location_id);

        for(int j = 0; j < location->choices.size(); ++j) {
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
