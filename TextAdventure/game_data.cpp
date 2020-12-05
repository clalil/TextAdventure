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
}

int GameData::LoadLocationData(const std::string path) {
    int locations_added = 0;
    std::ifstream location_file(path);
    std::string line;

    if (location_file.is_open() == false) {
        return 0;
    }

    Location working_location("", "");
    
    while(std::getline(location_file, line)) {
        size_t match_location_comment = line.find("//");
        size_t match_location_id = line.find("#");
        size_t match_location_choice_id = line.find("&");
        size_t match_location_choice_description = line.find(":");
        size_t match_location_endline = line.find("=");
        
        if (match_location_comment != std::string::npos) {
            continue;
        } else if (match_location_id != std::string::npos) {
            working_location.location_id = line.substr(match_location_id + 1);
        } else if (match_location_choice_id != std::string::npos) {
        working_location.choices.push_back(LocationChoice((line.substr((match_location_choice_id + 1), match_location_choice_description - 1)), (line.substr(match_location_choice_description + 2))));
        } else if (match_location_endline != std::string::npos) {
            locations.push_back(working_location);

            working_location.location_id = "";
            working_location.location_text = "";
            working_location.choices.clear();
            
        } else {
            working_location.location_text += line;
            working_location.location_text += "\n";
        }
    }
    
    return locations_added;
}

void GameData::CreateLocations(void) {
    LoadLocationData("Content/game_locations.txt");
}

Location* GameData::GetStartLocation(void) {
    if(locations.size() != 0) {
        return &locations.front();
    }
    
    std::cout << "Sorry, something went wrong & this game will now exit.\n";
    return nullptr;
}

Location* GameData::GetLocationWithId(const std::string& id) {
    for(int i = 0; i < locations.size(); ++i) {
        if(locations[i].location_id == id) {
            return &locations[i];
        }
    }
    
    return nullptr;
}

const int GameData::IsInvalidInput(int input) {
    while(std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        
        std::cout << "You've entered: " << input << "\n";
        std::cout << "That's not an option.\n";

        return 1;
    }
    return 0;
}

std::string GameData::GetPlayerName(std::string& user_name) {
    std::cout << "Please enter your name: \n";
    std::getline(std::cin, user_name);
    
    return user_name;
}

std::string GameData::PersonalizeText(const std::string player_name, std::string& location_text) {
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
    std::string menu;
    std::cout << "[m] Menu\n";
    std::getline(std::cin, menu);
    
    if(menu.length() > 0 && menu[0] == 'm') {
        int input = 0;

        while(input == 0) {
          std::string line;
          std::getline(std::cin, line);
          input = std::stoi(line);
            
            std::cout << "=================\n";
            std::cout << "[r] Resume game\n";
            std::cout << "[e] Exit game\n";
            std::cout << "=================\n";

          if (input < 0 || input > 2) {
              continue;
          } else if(input == 1) {
              std::cout << "Where do you wish to proceed next?\n";
          } else if(input == 2) {
              std::cout << "Exited game.\n";
              break;
          }
        }
    } else {
        return -1;
    }
    
    return -1;
}

//Code below only used for debugging purposes

const bool GameData::LocationExistsWithId(const std::string id) {
     for(int i = 0; i < locations.size(); ++i) {
        Location location = locations[i];
        if (location.location_id == id) {
            return true;
        }
    }
    return false;
}

const void GameData::DebugLocations(void) {
    std::vector<std::string> location_ids = {};

    std::cout << "Number of available locations: " << locations.size() << "\n";

    for(int i = 0; i < locations.size(); ++i) {
        Location location = locations[i];
        location_ids.push_back(location.location_id);

        for(int j = 0; j < location.choices.size(); ++j) {
            LocationChoice choice = location.choices[j];

            if (LocationExistsWithId(choice.next_location_id) == false) {
                std::cout << "[WARNING] Choice '" << (j+1) << "' on location '" << location.location_id << "' points to '" << choice.next_location_id << "' which doesn't exist.\n";
            }
        }
    }
    
    const auto duplicate = std::adjacent_find(location_ids.begin(), location_ids.end());

    if (duplicate != location_ids.end()) {
        std::cout << "[WARNING] An ID named '" << *duplicate << "' is a duplicate." << "\n";
    }
}
