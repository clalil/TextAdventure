//
//  game.hpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-27.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#pragma once
#include "game_data.hpp"
#include "player.hpp"

class Game {
    bool is_running;

public:
    GameData gamedata;
    Player player;
    
    void Run(void);
    const void SaveGame(void);
    const void LoadGame(void);
    const void GameStart(void);
};
