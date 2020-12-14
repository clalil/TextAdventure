//
//  player.h
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-27.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#pragma once
#include "game_data.hpp"

struct InventoryItem {
    std::string item_id;
    int inventory_amount = 1;
};

struct Player {
public:
    int moves = 0;
    std::string name;
    std::vector<std::shared_ptr<Location>> locations_visited;
    
    void AddItem(const std::string& id, int amount);
    void RemoveItem(const std::string& id, int amount);
    
    std::vector<InventoryItem> inventory;
    std::shared_ptr<Location> current_location = nullptr;
};
