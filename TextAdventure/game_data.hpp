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
#include <memory>
#include "items.hpp"

struct LocationChoice {
    LocationChoice(const std::string& choice_id, const std::string& choice_description);

    std::string next_location_id;
    std::string next_location_text;
};

struct Location {
    Location(const std::string& id, const std::string& descriptive_text);

    std::string location_id;
    std::string location_text;
    std::vector<std::shared_ptr<LocationChoice>> choices;
};

class GameData {

public:
    GameData();
    const void Introduction(void);
    const void WaitAMinute(void);
    const int InventoryMenu(void);
    const void DebugLocations(void);
    const int ValidateUserInput(int& choice, const std::string& input);
    const void InitializeItems();
    std::string GetPlayerName(std::string& user_name);
    std::string PersonalizeText(const std::string& player_name, std::string& location_text);

    std::shared_ptr<BaseItem> GetItemsById(const std::string& item_id);
    std::shared_ptr<Location> GetStartLocation(void);
    std::shared_ptr<Location> GetLocationWithId(const std::string& id);
    
private:
    void CreateLocations(void);
    int LoadLocationData(const std::string path);
    const bool LocationExistsWithId(const std::string id);
    
    std::vector<std::shared_ptr<Location>> locations;
    std::vector<std::shared_ptr<BaseItem>> items;
};
