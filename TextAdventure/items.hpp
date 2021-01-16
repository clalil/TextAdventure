//
//  items.hpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-12-12.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#pragma once
#include <string>

enum StringToItemsCode {
    Food,
    Teleport,
    Jewel,
    Expendable,
    Note
};

class BaseItem {

public:
    BaseItem(const std::string& item_id, const std::string& item_title, const std::string& item_description);

    virtual ~BaseItem();
    virtual void UseItem(void) = 0;

    const std::string& GetTitle() const;
    const std::string& GetId() const;
    const std::string& GetDescription() const;
    static StringToItemsCode StringToEnum(const std::string& str);

private:
    std::string id;
    std::string description;
    std::string title;
};

class TeleportItem : public BaseItem {

public:
    TeleportItem(const std::string& item_id, const std::string& item_title, const std::string& item_description, const std::string& location_id);
    
    virtual void UseItem(void);
    
private:
    std::string teleport_location_id;
};

class FoodItem : public BaseItem {

public:
    FoodItem(const std::string& item_id, const std::string& item_title, const std::string& item_description, int satiety_level);

    virtual void UseItem(void);
    
private:
    int food_satiety_level = 0;
};

class JewelItem : public BaseItem {

public:
    JewelItem(const std::string& item_id, const std::string& item_title, const std::string& item_description, const std::string& jewel_functionality);
    
    virtual void UseItem(void);

private:
    std::string jewel_magic;
};

class ExpendableItem : public BaseItem {
    
public:
    ExpendableItem(const std::string& item_id, const std::string& item_title, const std::string& item_description, int item_power);
    
    virtual void UseItem(void);
    
private:
    int item_power = 0;
};

class NoteItem : public BaseItem {

public:
    NoteItem(const std::string& item_id, const std::string& item_title, const std::string& item_description, const std::string& item_text);
    
    virtual void UseItem(void);
    
private:
    std::string text;
};
