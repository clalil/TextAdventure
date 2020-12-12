//
//  items.hpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-12-12.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include <string>

class BaseItem {
public:
    BaseItem(const std::string& item_id, const std::string& item_title);
    virtual void UseItem() = 0;
    
protected:
    std::string id;
    std::string title;
};

class TeleportScroll : public BaseItem {
    std::string teleport_location_id;
    
public:
    TeleportScroll(const std::string& item_id, const std::string& item_title, const std::string& location_id);
    
    virtual void UseItem();
};
