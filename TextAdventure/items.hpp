//
//  items.hpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-12-12.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#pragma once
#include <string>

enum ItemsCode {
    Food, Teleport, Jewel
};

class BaseItem {

public:
    BaseItem(const std::string& item_id, const std::string& item_title);

    virtual ~BaseItem();
    virtual void UseItem(void) = 0;
    const std::string& GetItemTitle() const;
    const std::string& GetItemId() const;

private:
    std::string id;
    std::string title;
};

class TeleportItem : public BaseItem {

public:
    TeleportItem(const std::string& item_id, const std::string& item_title, const std::string& location_id);
    
    virtual void UseItem(void);
    
private:
    std::string teleport_location_id;
};

class FoodItem : public BaseItem {

public:
    FoodItem(const std::string& item_id, const std::string& item_title, int satiety_level);

    virtual void UseItem(void);
    
private:
    int food_satiety_level;
};

class JewelItem : public BaseItem {

public:
    JewelItem(const std::string& item_id, const std::string& item_title, const std::string& jewel_functionality);
    
    virtual void UseItem(void);

private:
    std::string jewel_magic;
};
