//
//  items.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-12-12.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "globals.hpp"
#include "game.hpp"
#include "items.hpp"

BaseItem::BaseItem(const std::string& item_id, const std::string& item_title) {
    id = item_id;
    title = item_title;
}

BaseItem::~BaseItem()
{ }

TeleportItem::TeleportItem(const std::string& item_id, const std::string& item_title, const std::string& location_id) : BaseItem(item_id, item_title) {
    teleport_location_id = location_id;
};

void TeleportItem::UseItem(void) {
    std::shared_ptr<Location> new_location = Game::InstanceOf().gamedata.GetLocationById(teleport_location_id);

    if (new_location != nullptr) {
        Game::InstanceOf().player.current_location = new_location;

        std::cout << "You read the words on the scroll and when you've reached its final word you find yourself back where you started! \n";
        
        Game::InstanceOf().player.RemoveItem(GetItemId(), 1);
    }
}

FoodItem::FoodItem(const std::string& item_id, const std::string& item_title, int satiety_level) : BaseItem(item_id, item_title) {
    food_satiety_level = satiety_level;
}

void FoodItem::UseItem(void) {
    Game::InstanceOf().player.satiation += food_satiety_level;

    std::cout << "You swallow the " << GetItemTitle() << " and feel your satiety level increased by at least " << food_satiety_level << ".\n";

    Game::InstanceOf().player.RemoveItem(GetItemId(), 1);
}

JewelItem::JewelItem(const std::string& item_id, const std::string& item_title, const std::string& jewel_functionality) : BaseItem(item_id, item_title) {
    jewel_magic = jewel_functionality;
}

void JewelItem::UseItem(void) {
    bool player_has_pendant = Game::InstanceOf().player.HasItem("pendant01");
    bool player_has_key = Game::InstanceOf().player.HasItem("pendant02");

    if ((player_has_pendant && (jewel_magic == "key")) || (player_has_key && (jewel_magic == "keyhole"))) {
        std::cout << "You insert the tiny key into the keyhole of the glowing red pendant and hear a faint clicking sound.\n";
        std::cout << "The pendant is open. Inside lies a tiny piece of paper with the words 'Teleport Scroll' written on it.\n";
        std::cout << "You put the scroll back into your pocket and decide to leave the pendant. There is no further use for it.\n";

        Game::InstanceOf().player.RemoveItem("pendant01", 1);
        Game::InstanceOf().player.RemoveItem("pendant02", 1);
        Game::InstanceOf().player.AddItem("scroll01", 1);

    } else if (player_has_pendant && (player_has_key == false)) {
        std::cout << "This pendant is locked. Without its key it is useless to you.\n";

    } else {
        std::cout << "This key belongs to something and is of no use to you on its own.\n";
    }
}
