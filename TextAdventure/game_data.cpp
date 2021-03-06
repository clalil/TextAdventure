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

void GameData::MainScreen(void) const {
    std::cout << "*******************\n";
    std::cout << "House of the Haunted\n";
    std::cout << "*******************\n";
}

int GameData::ShowChoicesAndMenu(const int choice) const {
    std::vector<std::string> valid_choices{};

    std::cout << "What are you going to do?\n\n";

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
    std::cout << "(Food HP: " << Game::InstanceOf().player.satiation << ")\n";
    
    return ((int)valid_choices.size()+1);

}

void GameData::ReducePlayerSatiety(void) {
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

    Game::InstanceOf().player.satiation -= RandomNumbers();
}

std::shared_ptr<BaseItem> GameData::GetItemById(const std::string& item_id) const {
    for (int i = 0; i < items.size(); ++i) {
        if (items[i]->GetId() == item_id) {

            return items[i];
        }
    }

    return nullptr;
}

void GameData::CheckForLocationItems(void) {
    for (size_t i = 0; i < Game::InstanceOf().player.current_location->location_items.size(); ++i) {
        std::shared_ptr<BaseItem> item = GetItemById(Game::InstanceOf().player.current_location->location_items[i]);

        if (item != nullptr) {
            Game::InstanceOf().player.AddItem(item->GetId(), 1);
        }
    }
}

std::shared_ptr<Location> GameData::GetStartLocation(void) const {
    if (locations.size() != 0) {
        return locations.at("start");
    }
    
    std::cout << "[ERROR] The game locations did not load properly. \n";

    return nullptr;
}

std::shared_ptr<Location> GameData::GetLocationById(const std::string& id) {
    if (locations.find(id) != locations.end()) {
        return locations[id];
    }
    
    return nullptr;
}

std::string GameData::GetPlayerName(std::string& user_name) const {
    std::cout << "Please enter your name: \n";
    std::cout << "> ";
    std::getline(std::cin, user_name);
    
    return user_name;
}

std::string GameData::PersonalizeText(const std::string& player_name, std::string& location_text) const {
    size_t match = location_text.find("%%NAME%%");

    if (match != std::string::npos) {
        std::regex name_regex("%%NAME%%");
        std::string personalized_text = std::regex_replace (location_text, name_regex, player_name);
        location_text = personalized_text;

        return location_text;
    }

    return location_text;
}

void GameData::InitializeLocations(void) {
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

int GameData::LoadLocationData(const std::string path) {
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
            debug_location_ids.push_back(line.substr(1));

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
            locations[current_location->location_id] = current_location;

            current_location = std::make_shared<Location>("", "");
            locations_added++;
            
        } else {
            current_location->location_text += line;
            current_location->location_text += "\n";
        }
    }
    
    return locations_added;
}

void GameData::InitializeItems() {
    LoadItemData();
    MapPairedItems();
}

int GameData::LoadItemData(void) {
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

            std::vector<std::string> tokens = SplitString(line, " | ");
            
            switch (BaseItem::StringToEnum(tokens[0])) {
                case Food: {
                    std::shared_ptr<FoodItem> food = std::make_shared<FoodItem>(tokens[1], tokens[2], tokens[4], std::stoi(tokens[3]));
                    items.push_back(food);
                    items_added++;
                    break;
                }
                case Teleport: {
                    std::shared_ptr<TeleportItem> scroll = std::make_shared<TeleportItem>(tokens[1], tokens[2], tokens[4], tokens[3]);
                    items.push_back(scroll);
                    items_added++;
                    break;
                }
                case Jewel: {
                    std::shared_ptr<JewelItem> jewel = std::make_shared<JewelItem>(tokens[1], tokens[2], tokens[4], tokens[3]);
                    items.push_back(jewel);
                    items_added++;
                    break;
                }
                case Expendable: {
                    std::shared_ptr<ExpendableItem> expendable = std::make_shared<ExpendableItem>(tokens[1], tokens[2], tokens[4], std::stoi(tokens[3]));
                    items.push_back(expendable);
                    items_added++;
                    break;
                }
                case Note: {
                    std::shared_ptr<NoteItem> expendable = std::make_shared<NoteItem>(tokens[1], tokens[2], tokens[4], tokens[3]);
                    items.push_back(expendable);
                    items_added++;
                    break;
                    }
                default: {
                    break;
                }
            }
        }

    return items_added;
}

void GameData::MapPairedItems(void) {
    std::ifstream file("Content/Items/pairs.txt");
    
    if (file.is_open() == false) {
        std::cout << "Pairs file could not open.\n";
    }

    if (file.is_open()) {
        std::string line;
        
        while (std::getline(file, line)) {
            std::vector<std::string> tokens = SplitString(line, " : ");

            item_pairs.insert({ std::make_pair(tokens[0], tokens[1]), tokens[2]});
        }
    }
}

std::string GameData::CraftNewItem(std::string input1, std::string input2) {
    //find pairs & return value aka new item
    for (const auto &entry: item_pairs) {
        auto key_pair = entry.first;
        
        if ((key_pair.first == input1 && key_pair.second == input2) || (key_pair.first == input2 && key_pair.second == input1)) {
            return entry.second;
        }
    }

    //if for some random reason code reaches this point although this function should not be called unless pairs exists
    return "itemFail";
}

bool GameData::CompatibleItems(const std::string& item1, const std::string& item2) {
    //if the items suggested by player are indeed valid key-value pairs
        if (item_pairs.count(std::make_pair(item2, item1)) || item_pairs.count(std::make_pair(item1, item2))) {
            return true;
        }

    return false;
}

void GameData::CheckCompatibility(const std::string& player_choice1, const std::string& player_choice2) {
    if (CompatibleItems(player_choice1, player_choice2)) {
        std::string new_item = CraftNewItem(player_choice1, player_choice2);

        if (new_item != "itemFail") {
            Game::InstanceOf().player.RemoveItem(player_choice1, 1);
            Game::InstanceOf().player.RemoveItem(player_choice2, 1);
            
            Game::InstanceOf().player.AddItem(new_item, 1);

            int n = (int)Game::InstanceOf().player.inventory.size();
            std::cout << "\n" << Game::InstanceOf().player.inventory[n-1].item->GetDescription() << "\n";
        }

    } else {
        std::cout << "These items cannot be combined.\n";
    }
}

//Code below only used for debugging purposes

bool GameData::LocationExistsWithId(const std::string id) const {
    if (locations.find(id) != locations.end()) {
        return true;
    }

    return false;
}

bool GameData::CheckForDuplicateLocations(const std::string& location_id) {
    if (std::count (debug_location_ids.begin(), debug_location_ids.end(), location_id) > 1) {
        return true;
    }
    
    return false;
}

void GameData::DebugLocations(void) {
    std::cout << "Number of available locations: " << locations.size() << "\n";

    for (std::pair<std::string, std::shared_ptr<Location>> map_entry : locations) {
        if (CheckForDuplicateLocations(map_entry.second->location_id)) {
            std::cout << "[WARNING] An ID named '" << map_entry.second->location_id << "' is a duplicate." << "\n";
        }

        for (int j = 0; j < map_entry.second->choices.size(); ++j) {
            std::shared_ptr<LocationChoice> choice = map_entry.second->choices[j];

            if (LocationExistsWithId(choice->next_location_id) == false) {
                std::cout << "[WARNING] Choice '" << (j+1) << "' on location '" << map_entry.second->location_id << "' points to '" << choice->next_location_id << "' which doesn't exist (or has an invalid format).\n";
            }
        }
    }
}
