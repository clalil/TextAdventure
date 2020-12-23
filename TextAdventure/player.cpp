//
//  player.cpp
//  
//
//  Created by Clarissa Liljander on 2020-12-14.
//
#include "globals.hpp"
#include "player.hpp"

const void Player::ShowChoicesAndMenu(const int choice) const {
    std::cout << "Where do you wish to proceed next?\n";

    for(int i = 0; i < current_location->choices.size(); ++i) {
        std::cout << "[" << i+1 << "] " << current_location->choices[i]->next_location_text << "\n";
    }
    
    std::cout << ".............\n";
    std::cout << "[i] Inventory\n";
    std::cout << "[m] Menu\n";
    std::cout << "(Food HP): " << satiation << "\n";

}

const void Player::AddItem(const std::string& id, int amount) {
    for (int i = 0; i < inventory.size(); ++i) {
        if (inventory[i].item->id == id) {
            inventory[i].inventory_amount += amount;
            return;
        }
    }
    
    InventoryItem new_item;
    new_item.item = Game::InstanceOf().gamedata.GetItemById(id);
    new_item.inventory_amount = amount;
    
    if (new_item.item != nullptr) {
        inventory.push_back(new_item);
    }
}

const void Player::RemoveItem(const std::string& id, int amount) {
    for (int i = 0; i < inventory.size(); ++i) {
        if (inventory[i].item->id == id) {
            inventory[i].inventory_amount -= amount;
            
            if (inventory[i].inventory_amount <= 0) {
                inventory[i].inventory_amount = 0;
                
                auto remove_item = (inventory.begin() + i);
                inventory.erase(remove_item);
            }

            return;
        }
    }
}

const bool Player::HasVisitedLocation(void) const {
    for (size_t i = 0; i < locations_visited.size(); ++i) {
        if (locations_visited[i] == current_location->location_id) {
            return true;
        }
    }

    return false;
}
