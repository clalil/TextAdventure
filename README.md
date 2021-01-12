# A text based adventure game

## Built with  
**Programming language:** C++  
**Testing framework:** N/A

## Authors  
[Clarissa Liljander](https://github.com/clalil)  

## The code  
This is my second coding experience in C++, taking advantage of it's manual memory management with an OOP approach. The coding styles in C++ are several however I've chosen to write the code in a way that is hopefully straightforward to you, the reader.

## Dependencies  
* Xcode  

## Setup  
In order to successfully load the game files you need to tell Xcode to use the Project Directory as the working directory. To update the default settings, follow these steps:
```
1. Open your project and go to “Product -> Scheme -> Edit Scheme”.
2. Select “Run” on the left hand side. This is where you can change the settings that Xcode will use when it runs your application directly from the Play button.
3. Go to the [Options] tab and check “Working Directory: Use Custom Working Directory”
4. In the text box below, type ${PROJECT_DIR}

(Instructions originally provided by GamesByHer)
```

## Instructions  
The game instructions are provided within the game itself.  

## Contribute to the adventure!  
I'm happy to accept pull requests to improve the story of this game. In order not to crash the game as it loads, or further down the gameplay road the following rules apply:

__All game locations must begin with a '#' sign. All locations, choices, items & keys are written in camelCase.__  
An example of a possible valid location syntax:
```
===
#locationOne
^anItemToPickUp
&validLocationChoice|hiddenKeyItem+requiredKeyItem: This locationChoice's text.
@
===
```
A location can have multiple choices. Choices are preceeded by a '&' sign. Unless the choice have __hidden__ or __required__ keys they are written in the following format:
```
&locationChoice|+
```
**A hidden item** is an item used to hide choices from the player until he/she has obtained it. **Required items** are needed in order to proceed with that particular choice. Locations may also have regular **items** for the player to pick up, to either be used alone or combined with another item.

**IMPORTANT!**
Due to how the code is currently written, all of the hidden choices (i.e. those with hiddenKeyItems) must be at the bottom of the locationChoice list or else the user's input will not work properly on some occasions.  

__Locations which may only be visited once should contain the '@' sign:__
```
#locationOne
&locationChoice|+: Once you leave 'locationOne' you can never come back.
@
```
You can add __game items__ inside of the game_items.txt file like this for it to parse correctly:
```
itemType | itemId | Item title | power (string or int)
```
Items can be combined if they are paired up inside of the pairs.txt file and have valid 'combination' options inside of the items.cpp, for example:
```
itemToPairWith1:itemToPairWith2
```

## Acknowledgement  
Thank you to Paradox Interactive's [GamesByHer](https://gamedevcourse.paradoxinteractive.com/) initiative for creating this challenge.
