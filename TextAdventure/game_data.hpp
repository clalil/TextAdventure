//
//  game_data.hpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-19.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#pragma once
#include <iostream>
#include <vector>
#include <string>

struct LocationChoice {
    LocationChoice(const std::string& choice_id, const std::string& choice_description);
    std::string next_location_id;
    std::string next_location_text;
};

struct Location {
    Location(const std::string& id, const std::string& descriptive_text);
    std::string location_id;
    std::string location_text;
    std::vector<LocationChoice> choices;
};

class GameData {
    std::vector<Location> locations;
    void CreateLocations();
    bool LocationExistsWithId(const std::string& id);

public:
    GameData();
    void DebugLocations();
    std::string GetPlayerName(std::string& user_name);
    int IsInvalidInput(int input);
    std::string AddUserName(const std::string player_name, std::string& location_text);

    Location* GetStartLocation();
    Location* GetLocationWithId(const std::string& id);
};
