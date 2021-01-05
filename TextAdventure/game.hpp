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
    
    void GameStart(void);
    
private:
    GameMode game_mode = GameMode::Menu;

    int CombineItemsMenu(void);
    void GameHints(void) const;
    int InGameMenu(void);
    int InventoryMenu(void);
    void LoadGame(void);
    void MainMenu(void);
    void SaveGame(void);
    void Run(void);
};
