//
//  player.cpp
//  
//
//  Created by Clarissa Liljander on 2020-12-14.
//
#include "game.hpp"
#include "player.hpp"

const bool Player::CanVisitLocation(const std::string& upcoming_location) const {
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

const void Player::ShowChoicesAndMenu(const int choice) const {
    std::vector<std::string> valid_choices{};

    std::cout << "Where do you wish to proceed next?\n";

    for (int i = 0; i < current_location->choices.size(); ++i) {
        bool can_visit_next_location = CanVisitLocation(current_location->choices[i]->next_location_id);

        if (can_visit_next_location) {
            valid_choices.push_back(current_location->choices[i]->next_location_text);
        }
    }
    
    for (int i = 0; i < valid_choices.size(); ++i) {
        std::cout << "[" << i+1 << "] " << valid_choices[i] << "\n";
    }
    
    std::cout << ".............\n";
    std::cout << "[i] Inventory\n";
    std::cout << "[m] Menu\n";
    std::cout << "(Food HP): " << satiation << "\n";

}

const void Player::AddItem(const std::string& id, int amount) {
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
        inventory.push_back(new_item);
    }
}

const void Player::RemoveItem(const std::string& id, int amount) {
    for (int i = 0; i < inventory.size(); ++i) {
        if (inventory[i].item->GetId() == id) {
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

const bool Player::HasItem(const std::string& id) const {
    for (size_t i = 0; i < inventory.size(); ++i) {
        if (inventory[i].item->GetId() == id) {
            return true;
        }
    }

    return false;
}
