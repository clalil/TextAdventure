//
//  game_data.cpp
//  TextAdventure
//
//  Created by Clarissa Liljander on 2020-11-19.
//  Copyright © 2020 Clarissa Liljander. All rights reserved.
//
#include "utils.hpp"
#include "game.hpp"
#include "game_data.hpp"

LocationChoice::LocationChoice(const std::string& choice_id, const std::string& choice_description) {
    next_location_id = choice_id;
    next_location_text = choice_description;
}

Location::Location(const std::string& id, const std::string& descriptive_text) {
    location_id = id;
    location_text = descriptive_text;
}

GameData::GameData() {
    InitializeLocations();
    InitializeItems();
}

const void GameData::Introduction(void) const {
    std::cout << "*******************\n";
    std::cout << "House of the Haunted\n";
    std::cout << "*******************\n";
}

const int GameData::ShowChoicesAndMenu(const int choice) const {
    std::vector<std::string> valid_choices{};

    std::cout << "Where do you wish to proceed next?\n\n";

    for (int i = 0; i < Game::InstanceOf().player.current_location->choices.size(); ++i) {
        bool can_visit_next_location = Game::InstanceOf().player.CanVisitLocation(Game::InstanceOf().player.current_location->choices[i]->next_location_id);

        if (can_visit_next_location) {
            valid_choices.push_back(Game::InstanceOf().player.current_location->choices[i]->next_location_text);

            //if hidden_item_id is not empty, but player the does not have that item yet it will not be displayed as a choice
            if ((Game::InstanceOf().player.current_location->choices[i]->hidden_item_id != "") && (Game::InstanceOf().player.HasItem(Game::InstanceOf().player.current_location->choices[i]->hidden_item_id) == false)) {
                valid_choices.pop_back();
            }
        }
    }
    
    for (int i = 0; i < valid_choices.size(); ++i) {
        std::cout << "[" << i+1 << "] " << valid_choices[i] << "\n";
    }
    
    std::cout << ".............\n";
    std::cout << "[i] Inventory\n";
    std::cout << "[m] Menu\n";
    std::cout << "(Food HP): " << Game::InstanceOf().player.satiation << "\n";
    
    return ((int)valid_choices.size()+1);

}

const void GameData::WaitAMinute(void) const {
    std::this_thread::sleep_until(std::chrono::system_clock::now() + 1s);
}

const int GameData::ValidateUserInput(int& choice, const std::string& input) const {
    // line is not a number, e.g. "abc" or "abc123", or the number is too big
    // to fit in an int, e.g. "11111111111111111111111111111111111"
    try {
        choice = std::stoi(input);

    } catch (std::exception const& exc) {
        std::cout << "\n";
        std::cout << "You've entered an invalid input. Please try again." << "\n";
        std::cout << "\n";

        return 1;
    }
    
    return 0;
}

const void GameData::ReducePlayerSatiety(void) {
    auto hunger_level = Game::InstanceOf().player.satiation;

    if(hunger_level <= 50) {
        std::cout << "You are hungry. You should eat something.\n";
    } else if (hunger_level == 20) {
        std::cout << "You are really hungey. You should really eat something.\n";
    } else if (hunger_level < 20 ) {
        std::cout << "You are starving. You need to eat something to sustain you.\n";
    } else if (hunger_level <= 0) {
        std::cout << "You're about to starve to death and decide to go home and eat something.\n";
    }

    Game::InstanceOf().player.satiation -= RandomSatietyDrop();
}

std::shared_ptr<BaseItem> GameData::GetItemById(const std::string& item_id) const {
    for (int i = 0; i < items.size(); ++i) {
        if (items[i]->GetId() == item_id) {

            return items[i];
        }
    }

    return nullptr;
}

const void GameData::CheckForLocationItems(void) {
    for (size_t i = 0; i < Game::InstanceOf().player.current_location->location_items.size(); ++i) {
        std::shared_ptr<BaseItem> item = GetItemById(Game::InstanceOf().player.current_location->location_items[i]);

        if (item != nullptr) {
            Game::InstanceOf().player.AddItem(item->GetId(), 1);
        }
    }
}

std::shared_ptr<Location> GameData::GetStartLocation(void) const {
    if (locations.size() != 0) {
        return locations.front();
    }
    
    std::cout << "[ERROR] The game locations did not load properly. \n";

    return nullptr;
}

std::shared_ptr<Location> GameData::GetLocationById(const std::string& id) {
    if (location_index.find(id) != location_index.end()) {
        return location_index[id];
    }
    
    return nullptr;
}

const std::string GameData::GetPlayerName(std::string& user_name) const {
    std::cout << "Please enter your name: \n";
    std::getline(std::cin, user_name);
    
    return user_name;
}

const std::string GameData::PersonalizeText(const std::string& player_name, std::string& location_text) const {
    size_t match = location_text.find("%%NAME%%");

    if (match != std::string::npos) {
        std::regex name_regex("%%NAME%%");
        std::string personalized_text = std::regex_replace (location_text, name_regex, player_name);
        location_text = personalized_text;

        return location_text;
    }

    return location_text;
}

const void GameData::InitializeLocations(void) {
    namespace fs = std::__fs::filesystem;
    std::string directory_path = "Content/Locations/";
    fs::path path_to_load(directory_path);
    
    if (fs::exists(path_to_load)) {
        for (const auto& entry : fs::directory_iterator(path_to_load)) {
            std::string filename = entry.path().filename();
            std::string file_to_load = directory_path + filename;

            LoadLocationData(file_to_load);
        }

    } else {
        std::cout << "File(s) does not exist. If you're a Mac user, the path to the working directory might be incorrect.\n";
    }
}

const int GameData::LoadLocationData(const std::string path) {
    int locations_added = 0;
    std::ifstream location_file(path);
    std::string line;

    if (location_file.is_open() == false) {
        std::cout << "File " << path << " could not open.\n";
        return 0;
    }

    std::shared_ptr<Location> current_location = std::make_shared<Location>("", "");

    while (std::getline(location_file, line)) {
        size_t loc_comment = line.find("//");
        size_t loc_id = line.find("#");
        size_t item_id = line.find("^");
        size_t loc_choice_id = line.find("&");
        size_t loc_only_visit_once = line.find("@");
        size_t loc_endline = line.find("=");
        
        if (loc_comment != std::string::npos) {
            continue;

        } else if (loc_id != std::string::npos) {
            current_location->location_id = line.substr(1);

        } else if (item_id != std::string::npos) {
            current_location->location_items.push_back(line.substr(1));

        } else if (loc_choice_id != std::string::npos) {
            std::shared_ptr<LocationChoice> current_location_choice = std::make_shared<LocationChoice>(FindString(line, "", 1, "|", 1), FindString(line, ":", 2, "", 1));

            current_location_choice->hidden_item_id = FindString(line, "|", 1, "+", 1);
            current_location_choice->required_item_id = FindString(line, "+", 1, ":", 1);

            current_location->choices.push_back(current_location_choice);
            
            current_location_choice = std::make_shared<LocationChoice>("", "");

        } else if (loc_only_visit_once != std::string::npos) {
            current_location->can_only_visit_once = true;

        } else if (loc_endline != std::string::npos) {
            locations.push_back(current_location);
            location_index[current_location->location_id] = current_location;

            current_location = std::make_shared<Location>("", "");
            locations_added++;
            
        } else {
            current_location->location_text += line;
            current_location->location_text += "\n";
        }
    }
    
    return locations_added;
}

const void GameData::InitializeItems() {
    LoadItemData();
    pairs = MapPairedItems();
}

const int GameData::LoadItemData(void) {
    int items_added = 0;
    std::ifstream file("Content/Items/game_items.txt");
    std::string line;
    
    if (file.is_open() == false) {
        std::cout << "Items file could not open.\n";
        return 0;
    }
    
        while(std::getline(file, line)) {
            size_t match_location_comment = line.find("//");

            if (match_location_comment != std::string::npos) {
                continue;
            }

            std::vector<std::string> tokens = SplitString(line);
            
            switch (StringToEnum(tokens[0])) {
                case Food: {
                    std::shared_ptr<FoodItem> food = std::make_shared<FoodItem>(tokens[1], tokens[2], std::stoi(tokens[3]));
                    items.push_back(food);
                    items_added++;
                    break;
                }
                case Teleport: {
                    std::shared_ptr<TeleportItem> scroll = std::make_shared<TeleportItem>(tokens[1], tokens[2], tokens[3]);
                    items.push_back(scroll);
                    items_added++;
                    break;
                }
                case Jewel: {
                    std::shared_ptr<JewelItem> jewel = std::make_shared<JewelItem>(tokens[1], tokens[2], tokens[3]);
                    items.push_back(jewel);
                    items_added++;
                }
                default: {
                    break;
                }
            }
        }

    return items_added;
}

const std::map<std::string, std::string> GameData::MapPairedItems(void) const {
    std::ifstream file("Content/Items/pairs.txt");
    std::map<std::string, std::string> paired_items;
    
    if (file.is_open() == false) {
        std::cout << "Pairs file could not open.\n";
    }

    if (file.is_open()) {
        std::string key, val;
        
        while (std::getline(std::getline(file, key, ':') >> std::ws, val)) {
            paired_items[key] = val;
        }
    }
    
    return paired_items;
}

const bool GameData::CompatibleItems(const std::string& item1, const std::string& item2) {
    std::map<std::string, std::string>::iterator key_or_value1;
    std::map<std::string, std::string>::iterator key_or_value2;

    key_or_value1 = pairs.find(item1);
    key_or_value2 = pairs.find(item2);

    //returns true if input1 or input2 are valid keys in the 'pairs' map
    if ((key_or_value1 != pairs.end()) || (key_or_value2 != pairs.end())) {

        if ( key_or_value1->second == item2 ) {
            //input2 is the value of input1 key
            return true;

            } else if (key_or_value2->second == item1) {
                //input1 is the value of input2 key
                return true;

            } else {
                //one or both values exists as keys but are not key-value pairs
                return false;
            }
    
    } else {
        //not a key-value pair and may not exist in the map
        return false;
    }
}

//Code below only used for debugging purposes

const bool GameData::LocationExistsWithId(const std::string id) const {
    if (location_index.find(id) != location_index.end()) {
        return true;
    }

    return false;
}

const void GameData::DebugLocations(void) const {
    std::vector<std::string> location_ids = {};

    std::cout << "Number of available locations: " << locations.size() << "\n";

    for (int i = 0; i < locations.size(); ++i) {
        std::shared_ptr<Location> location = locations[i];
        location_ids.push_back(location->location_id);

        for (int j = 0; j < location->choices.size(); ++j) {
            std::shared_ptr<LocationChoice> choice = location->choices[j];

            if (LocationExistsWithId(choice->next_location_id) == false) {
                std::cout << "[WARNING] Choice '" << (j+1) << "' on location '" << location->location_id << "' points to '" << choice->next_location_id << "' which doesn't exist.\n";
            }
        }
    }

    const auto duplicate = std::adjacent_find(location_ids.begin(), location_ids.end());

    if (duplicate != location_ids.end()) {
        std::cout << "[WARNING] An ID named '" << *duplicate << "' is a duplicate." << "\n";
    }
}
