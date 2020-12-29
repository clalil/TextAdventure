//
//  utils.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-12-20.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "items.hpp"
#include "game_data.hpp"
#include "utils.hpp"

std::string FindString(const std::string& string_to_split, const std::string& first_separator, int first_index, const std::string& last_separstor, int last_index) {
    size_t first = string_to_split.find(first_separator);
    size_t last = string_to_split.find(last_separstor);
    
    return string_to_split.substr(first + 1, last - first - 1);
}

const int RandomSatietyDrop(void) {
    unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 random_generator = std::mt19937(seed);
    std::uniform_int_distribution<int> satiety_reduced_by(1, 10);

    return satiety_reduced_by(random_generator);
}

std::vector<std::string> SplitString(const std::string& text) {
    std::string separator = " | ";
    std::vector<std::string> result;
    size_t start = 0, end = 0;
    
    while((end = text.find(separator, start)) != std::string::npos) {
        result.push_back(text.substr(start, end - start));
        start = end + 3;
    }
    
    result.push_back(text.substr(start));

    return result;
}

const int StringToEnum(const std::string& string_value) {
    if (string_value == "food") return Food;
    if (string_value == "teleport") return Teleport;
    if (string_value == "jewel") return Jewel;
    
    return 0;
}
