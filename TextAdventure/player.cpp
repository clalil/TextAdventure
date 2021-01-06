//
//  player.cpp
//  
//
//  Created by Clarissa Liljander on 2020-12-14.
//
#include "game.hpp"
#include "player.hpp"

bool Player::CanVisitLocation(const std::string& upcoming_location) const {
    auto new_location = Game::InstanceOf().gamedata.GetLocationById(upcoming_location);

    if (new_location->can_only_visit_once) {
        if (std::find(locations_visited.begin(), locations_visited.end(), new_location->location_id) != locations_visited.end()) {
            return false;

        } else {
            return true;
        }
    }
    
    return true;
}

void Player::AddItem(const std::string& id, int amount) {
    for (int i = 0; i < inventory.size(); ++i) {
        if (inventory[i].item->GetId() == id) {
            inventory[i].inventory_amount += amount;
            return;
        }
    }
    
    InventoryItem new_item;
    new_item.item = Game::InstanceOf().gamedata.GetItemById(id);
    new_item.inventory_amount = amount;
    
    if (new_item.item != nullptr) {
        std::cout << "[" << new_item.item->GetTitle() << "] was added to your inventory.\n";
        inventory.push_back(new_item);
    }
}

void Player::RemoveItem(const std::string& id, int amount) {
    for (int i = 0; i < inventory.size(); ++i) {
        if (inventory[i].item->GetId() == id) {
            inventory[i].inventory_amount -= amount;
            
            if (inventory[i].inventory_amount <= 0) {
                inventory[i].inventory_amount = 0;
                
                auto remove_item = (inventory.begin() + i);
                inventory.erase(remove_item);
            }
            
            std::cout << "[" << inventory[i].item->GetTitle() << "] was removed from your inventory.\n";

            return;
        }
    }
}

bool Player::HasVisitedLocation(void) const {
    for (size_t i = 0; i < locations_visited.size(); ++i) {
        if (locations_visited[i] == current_location->location_id) {
            return true;
        }
    }

    return false;
}

bool Player::HasItem(const std::string& id) const {
    for (size_t i = 0; i < inventory.size(); ++i) {
        if (inventory[i].item->GetId() == id) {
            return true;
        }
    }

    return false;
}
