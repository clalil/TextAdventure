//
//  location_choice.hpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-19.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#pragma once
#include <string>
#include <vector>

struct LocationChoice {
    LocationChoice(const std::string& next_id, const std::string& a_text);
    std::string location_id;
    std::string text;
};

struct Location {
    Location(const std::string& an_id, const std::string& a_text);
    std::string id;
    std::string text;
    std::vector<LocationChoice> choices;
};

class GameData {
public:
    GameData();
    void debugLocations();

private:
    void createLocations();
    std::vector<Location> locations;
};
