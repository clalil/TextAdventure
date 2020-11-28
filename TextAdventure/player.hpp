//
//  player.h
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-27.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#pragma once
#include "game_data.hpp"

struct Player {
public:
    std::vector<Location*> locations_visited;
    Location* current_location = nullptr;
    int moves = 0;
};
