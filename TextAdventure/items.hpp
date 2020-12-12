//
//  items.hpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-12-12.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include <string>

class BaseItem {
    std::string id;
    std::string title;
    
public:
    BaseItem(const std::string& item_id, const std::string& item_title);
    virtual void UseItem() = 0;
    
    const std::string& GetId();
    const std::string& GetTitle();
};
