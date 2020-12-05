//
//  game.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-27.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "game.hpp"

void Game::Run() {
    is_running = true;
    std::string name;

    gamedata.Introduction();
    gamedata.DebugLocations();
    player.name = gamedata.GetPlayerName(name);
    player.current_location = gamedata.GetStartLocation();
    
    while(is_running) {
        if(player.current_location == nullptr) {
            std::cout << "[ERROR] No such location. Ending game. \n";
            is_running = false;

        } else if (player.current_location->choices.size() == 0) {
            std::cout << "Game Over.\n";
            std::cout << "You made a total of " << player.moves << " moves and visited the following game locations: \n";

            for(int i = 0; i < player.locations_visited.size(); ++i) {
                std::cout << player.locations_visited[i]->location_id << "\n";
            }

            is_running = false;

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
                std::cout << "[" << i+1 << "] " << player.current_location->choices[i].next_location_text << "\n";
            }
            
            gamedata.GameMenu();

            while(is_valid_input || choice < 0 || choice >= player.current_location->choices.size()+1) {
                std::cin >> choice;

                is_valid_input = gamedata.IsInvalidInput(choice);
            }
            
            const std::string& upcoming_location_id = player.current_location->choices[choice-1].next_location_id;
            player.current_location = gamedata.GetLocationWithId(upcoming_location_id);
            
            player.moves++;
        }
    }
}
