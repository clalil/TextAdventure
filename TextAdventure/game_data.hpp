//
//  game_data.hpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-19.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#pragma once
#include <chrono>
#include <random>
#include <thread>
#include <algorithm>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <map>
#include "items.hpp"

struct LocationChoice {
    LocationChoice(const std::string& choice_id, const std::string& choice_description);

    std::string next_location_id;
    std::string next_location_text;
    std::string hidden_item_id;
    std::string required_item_id;
};

struct Location {
    Location(const std::string& id, const std::string& descriptive_text);

    std::string location_id;
    std::string location_text;
    bool can_only_visit_once = false;
    
    std::vector<std::shared_ptr<LocationChoice>> choices;
    std::vector<std::string> location_items;
};

class GameData {

public:
    GameData();

    void CheckForLocationItems(void);
    bool CompatibleItems(const std::string& item1, const std::string& item2);
    void DebugLocations(void) const;
    std::string GetPlayerName(std::string& user_name) const;
    void MainScreen(void) const;
    std::map<std::string, std::string> MapPairedItems(void) const;
    std::string PersonalizeText(const std::string& player_name, std::string& location_text) const;
    void ReducePlayerSatiety(void);
    int ShowChoicesAndMenu(const int choice) const;
    bool ValidateUserInput(int& choice, const std::string& input) const;
    void WaitASecond(void) const;

    std::shared_ptr<BaseItem> GetItemById(const std::string& item_id) const;
    std::shared_ptr<Location> GetLocationById(const std::string& id);
    std::shared_ptr<Location> GetStartLocation(void) const;
    
private:
    void InitializeItems(void);
    void InitializeLocations(void);
    int LoadItemData(void);
    bool LocationExistsWithId(const std::string id) const;
    int LoadLocationData(const std::string path);
    
    std::vector<std::shared_ptr<BaseItem>> items;
    std::vector<std::shared_ptr<Location>> locations;
    std::unordered_map<std::string, std::shared_ptr<Location>> location_index;
    std::map<std::string, std::string> pairs;
};
