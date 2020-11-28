//
//  game.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-27.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "game.hpp"
#include <string>
#include <iostream>

void Game::Run() {
    std::cout << "*******************\n";
    std::cout << "House of Illusions\n";
    std::cout << "*******************\n";
    gamedata.DebugLocations();

    player.current_location = gamedata.GetStartLocation();
    is_running = true;
    
    while(is_running) {
        if(player.current_location == nullptr) {
            std::cout << "[ERROR] No such player location. Ending game. \n";
            is_running = false;

        } else if (player.current_location->choices.size() == 0) {
            std::cout << "Game Over.\n";
            std::cout << "You made a total of " << player.moves << " moves this time.\n";
            is_running = false;

        } else {
            bool is_valid_input = false;
            int choice = -1;

            std::cout << "Current location is: " << player.current_location->location_text << "\n";
            std::cout << "Where do you wish to proceed next?\n";
            for(int i = 0; i < player.current_location->choices.size(); ++i) {
                std::cout << "[" << i << "] " << player.current_location->choices[i].next_location_text << "\n";
            }

            while(is_valid_input || choice < 0 || choice >= player.current_location->choices.size()) {
                std::cin >> choice;
                is_valid_input = gamedata.InvalidInput(choice);
            }
            
            const std::string& upcoming_location_id = player.current_location->choices[choice].next_location_id;
            player.current_location = gamedata.GetLocationWithId(upcoming_location_id);
            
            player.moves++;
        }
    }
}
