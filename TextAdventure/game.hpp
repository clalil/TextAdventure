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
#include <fstream>

enum class GameMode {
    Menu, IsRunning, Exit
};

class Game {

public:
    static Game& InstanceOf();

    GameData gamedata;
    Player player;
    
    const void GameStart(void);
    
private:
    GameMode game_mode = GameMode::Menu;

    const int InGameMenu(void);
    const int InventoryMenu(void);
    const void LoadGame(void);
    const void MainMenu(void);
    const void SaveGame(void);
    const void Run(void);
};
