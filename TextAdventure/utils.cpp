//
//  utils.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-12-20.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//

#include "utils.hpp"

std::vector<std::string> SplitString(const std::string& text, char separator) {
    std::vector<std::string> result;
    size_t start = 0, end = 0;
    
    while((end = text.find(separator, start)) != std::string::npos) {
        result.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    
    result.push_back(text.substr(start));

    return result;
}
