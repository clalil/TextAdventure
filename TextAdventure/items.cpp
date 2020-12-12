//
//  items.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-12-12.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "game.hpp"
#include "items.hpp"
extern Game game;

TeleportScroll::TeleportScroll(const std::string& item_id, const std::string& item_title, const std::string& location_id) : BaseItem(item_id, item_title) {
    teleport_location_id = location_id;
};

void TeleportScroll::UseItem() {
    std::shared_ptr<Location> new_location = game.gamedata.GetLocationWithId(teleport_location_id);

    if (new_location != nullptr) {
        game.player.current_location = new_location;

        std::cout << "You read the words on the scroll and when you've reached its final word  you find yourself back where you started! \n";
    }
}
