//
//  items.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-12-12.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "game.hpp"
#include "utils.hpp"
#include "items.hpp"

BaseItem::BaseItem(const std::string& item_id, const std::string& item_title) {
    id = item_id;
    title = item_title;
}

BaseItem::~BaseItem()
{ }

const std::string& BaseItem::GetId() const {
    return id;
}

const std::string& BaseItem::GetTitle() const {
    return title;
}

TeleportItem::TeleportItem(const std::string& item_id, const std::string& item_title, const std::string& location_id) : BaseItem(item_id, item_title) {
    teleport_location_id = location_id;
};

void TeleportItem::UseItem(void) {
    std::shared_ptr<Location> new_location = Game::InstanceOf().gamedata.GetLocationById(teleport_location_id);

    if (new_location != nullptr) {
        Game::InstanceOf().player.current_location = new_location;

        std::cout << "You read the words on the scroll and when you've reached its final word you find yourself back where you started! \n";
        
        Game::InstanceOf().player.RemoveItem(GetId(), 1);
    }
}

FoodItem::FoodItem(const std::string& item_id, const std::string& item_title, int satiety_level) : BaseItem(item_id, item_title) {
    food_satiety_level = satiety_level;
}

void FoodItem::UseItem(void) {
    Game::InstanceOf().player.satiation += food_satiety_level;

    std::cout << "You swallow the " << GetTitle() << " and feel your satiety level increased by at least " << food_satiety_level << ".\n";

    Game::InstanceOf().player.RemoveItem(GetId(), 1);
}

JewelItem::JewelItem(const std::string& item_id, const std::string& item_title, const std::string& jewel_functionality) : BaseItem(item_id, item_title) {
    jewel_magic = jewel_functionality;
}

void JewelItem::UseItem(void) {
    if (combined) {
        std::cout << "You insert the tiny key into the keyhole of the glowing red pendant and hear a faint clicking sound.\n";
        std::cout << "The pendant is open. Inside lies a tiny piece of paper with the words: '" << Game::InstanceOf().player.name << " : Teleport Scroll' written on it.\n";
        std::cout << "You put the scroll back inside of the pocket and decide to leave the pendant. There is no further use for it.\n\n";

    } else {
        std::cout << "This item belongs to something and is of no use to you on its own.\n";
    }
}

ExpendableItem::ExpendableItem(const std::string& item_id, const std::string& item_title, int item_power_amount) : BaseItem(item_id, item_title) {
    item_power = item_power_amount;
}

void ExpendableItem::UseItem(void) {
    if (item_power == 0) {
        item_power = RandomNumbers();
    }
    
    if (this->GetId() == "rock01" && (item_power > 5)) {
        std::cout << "You use the " << GetTitle() << " to smash the window and it shatters.\n\n";
        Game::InstanceOf().player.AddItem("brokenGlass01", 1);
        Game::InstanceOf().player.RemoveItem("rock01", 1);
    } else {
        std::cout << "This rock seems to be broken because this window sure ain't.\n\n";
        Game::InstanceOf().player.RemoveItem("rock01", 1);
        Game::InstanceOf().player.AddItem("brokenRock01", 1);
    }
}
