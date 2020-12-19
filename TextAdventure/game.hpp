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

enum class GameMode {
    Menu, InGameMenu, IsRunning, Exit
};

class Game {

public:
    static Game& InstanceOf();
    GameData gamedata;
    Player player;
    
    const void GameStart(void);
    
private:
    GameMode game_mode = GameMode::Menu;

    void Run(void);
    const void MainMenu(void);
    const int InGameMenu(void);
    const void SaveGame(void);
    const void LoadGame(void);
};
