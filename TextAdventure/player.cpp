//
//  player.cpp
//  
//
//  Created by Clarissa Liljander on 2020-12-14.
//
#include "globals.hpp"
#include "player.hpp"

void Player::AddItem(const std::string& id, int amount) {
    for (int i = 0; i < inventory.size(); ++i) {
        if (inventory[i].item->id == id) {
            inventory[i].inventory_amount += amount;
            return;
        }
    }

    inventory.push_back(InventoryItem { .item = Game::InstanceOf().gamedata.GetItemById(id), .inventory_amount = amount } );
}

void Player::RemoveItem(const std::string& id, int amount) {
    for (int i = 0; i < inventory.size(); ++i) {
        if (inventory[i].item->id == id) {
            inventory[i].inventory_amount -= amount;

            if (inventory[i].inventory_amount <= 0) {
                inventory[i].inventory_amount = 0;
            }

            return;
        }
    }
}

bool Player::HasVisitedLocation(void) {
    for (size_t i = 0; i < locations_visited.size(); ++i) {
        if (locations_visited[i]->location_id == current_location->location_id) {
            return true;
        }
    }

    return false;
}
