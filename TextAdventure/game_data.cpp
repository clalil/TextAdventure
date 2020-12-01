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

void GameData::CreateLocations(void) {
    Location room1("start", "Hello %%NAME%%! You are about to embark on a great adventure.");
    room1.choices.push_back(LocationChoice("begin", "I'm psyched! Let's play 'House of the Haunted'."));
    room1.choices.push_back(LocationChoice("exit", "Exit"));
    locations.push_back(room1);

    Location room2("begin", "You wake up in the middle of a forest, not remembering how you got there in the first place. You are wearing your favourite clothes: a t-shirt and shorts. Perhaps you shouldn't be - it's about 10 degrees celcius in the woods. You feel kind of cold...and hungry, so hungry.");
    room2.choices.push_back(LocationChoice("exit", "Exit"));
    locations.push_back(room2);

    Location exit("exit", "This is the exit! :o");
    locations.push_back(exit);
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

int GameData::IsInvalidInput(int input) {
    while(std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        
        std::cout << "Please enter a valid choice." << "\n";
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

void GameData::Introduction(void) {
    std::cout << "*******************\n";
    std::cout << "House of the Haunted\n";
    std::cout << "*******************\n";
}

void GameData::WaitAMinute(void) {
    std::this_thread::sleep_until(std::chrono::system_clock::now() + 1.5s);
}

//Code below only used for debugging purposes

bool GameData::LocationExistsWithId(const std::string& id) {
     for(int i = 0; i < locations.size(); ++i) {
        Location location = locations[i];
        if (location.location_id == id) {
            return true;
        }
    }
    return false;
}

void GameData::DebugLocations(void) {
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
