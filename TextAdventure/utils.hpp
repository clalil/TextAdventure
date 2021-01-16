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
#include <chrono>
#include <random>
#include <thread>
#include <iostream>
using namespace std::chrono_literals;

std::string FindString(const std::string& string_to_split, const std::string& first_separator, int first_index, const std::string& last_separstor, int last_index);
const int RandomNumbers(void);
std::vector<std::string> SplitString(const std::string& text, const std::string& separator);
bool ValidateUserInput(int& choice, const std::string& input);
void WaitASecond(void);
