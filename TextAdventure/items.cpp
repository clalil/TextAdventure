//
//  items.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-12-12.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//

#include "items.hpp"

const std::string& BaseItem::GetId() {
    return id;
}

const std::string& BaseItem::GetTitle() {
    return title;
}
