//
//  player.cpp
//  
//
//  Created by Clarissa Liljander on 2020-12-14.
//
#include "player.hpp"

void Player::AddItem(const std::string& id, int amount) {
    for(int i = 0; i < inventory.size(); ++i) {
        if (inventory[i].item_id == id) {
            inventory[i].inventory_amount += amount;
            return;
        }
    }

    inventory.push_back(InventoryItem { .item_id = id, .inventory_amount = amount } );
}

void Player::RemoveItem(const std::string& id, int amount) {
    for(int i = 0; i < inventory.size(); ++i) {
        if (inventory[i].item_id == id) {
            inventory[i].inventory_amount -= amount;

            if (inventory[i].inventory_amount <= 0) {
                inventory[i].inventory_amount = 0;
            }

            return;
        }
    }
}
