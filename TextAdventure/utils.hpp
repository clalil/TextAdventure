//
//  utils.hpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-12-20.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#pragma once
#include <string>
#include <vector>
#include <iostream>
using namespace std::chrono_literals;

const int RandomSatietyDrop(void);
const int StringToEnum(const std::string& string_value);
std::vector<std::string> SplitString(const std::string& text);
