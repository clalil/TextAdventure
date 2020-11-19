//
//  location_choice.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-19.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//

#include "game_data.hpp"
#include <string>
#include <iostream>

LocationChoice::LocationChoice(const std::string& next_id, const std::string& a_text) {
    location_id = next_id;
    text = a_text;
}

Location::Location(const std::string& an_id, const std::string& a_text) {
    id = an_id;
    text = a_text;
}

void GameData::createLocations() {
    Location room1("start", "You are in the first room of this story.");
    room1.choices.push_back(LocationChoice("room2", "Go North"));
    room1.choices.push_back(LocationChoice("exit", "Exit"));
    locations.push_back(room1);

    Location exit("exit", "This is the exit! :o");
    locations.push_back(exit);
}

GameData::GameData() {
    createLocations();
}

void GameData::debugLocations() {
    for(int i = 0; i < locations.size(); ++i) {
        Location location = locations[i];
        std::cout << location.id << location.text << "\n";
    }
}
