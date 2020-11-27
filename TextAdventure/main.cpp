//
//  main.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-18.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "game.hpp"
#include "game_data.hpp"
#include <iostream>
Game game;

int main() {
    GameData gamedata;
    game.run();
    
    Location* start_location = gamedata.GetStartLocation();
    
    if(start_location == nullptr) {
        std::cout << "No start location found - something went wrong? \n";
        return 1;
    }
    
    std::cout << "Start Location is " << start_location->location_id << ": " << start_location->location_text << "\n";
    
    if (start_location->choices.size() > 0) {
        const std::string& second_location_id = start_location->choices[0].next_location_id;
        Location* second_location = gamedata.GetLocationWithId(second_location_id);

        if (second_location != nullptr) {
            std::cout << "Possible Second Location: " << second_location->location_text << "\n";
        }
    }
}
