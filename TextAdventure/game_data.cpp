//
//  game_data.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-19.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//

#include "game_data.hpp"
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

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

void GameData::CreateLocations() {
    Location room1("start", "You are in the first room of this story.");
    room1.choices.push_back(LocationChoice("room2", "Go North"));
    room1.choices.push_back(LocationChoice("exit", "Exit"));
    locations.push_back(room1);

    Location room2("room2", "You are in the second room of this story.");
    room2.choices.push_back(LocationChoice("exit", "Exit"));
    locations.push_back(room2);

    Location exit("exit", "This is the exit! :o");
    locations.push_back(exit);
}

Location* GameData::GetStartLocation() {
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

bool GameData::LocationExistsWithId(const std::string& id) {
     for(int i = 0; i < locations.size(); ++i) {
        Location location = locations[i];
        if (location.location_id == id) {
            return true;
        }
    }
    return false;
}

void GameData::DebugLocations() {
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
