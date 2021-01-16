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
    std::shared_ptr<BaseItem> item;
    int inventory_amount = 1;
};

struct Player {
    int moves = 0;
    int satiation = 100;
    std::string name;

    std::vector<std::string> locations_visited;
    std::vector<InventoryItem> inventory;
    std::shared_ptr<Location> current_location = nullptr;

    void AddItem(const std::string& id, int amount);
    bool CanVisitLocation(const std::string& upcoming_location) const;
    void RemoveItem(const std::string& id, int amount);
    bool HasVisitedLocation(void) const;
    bool HasItem(const std::string& id) const;
};
