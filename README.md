# [WIP] A text based adventure game

## Built with  
**Programming language:** C++  
**Testing framework:** N/A

## Authors  
[Clarissa Liljander](https://github.com/clalil)  

## The code  
...

## Dependencies  
* Xcode  

## Setup  
...

## Instructions  
The game instructions are provided within the game itself.  

## Contribute to the adventure!  
I'm happy to accept pull requests to improve the story of this game. In order not to crash the game as it loads, or further down the gameplay road the following rules apply:

__All game locations must begin with a '#'sign and are written in camelCase:__
```
#locationOne
```
__Choices belonging to the locations are worded in camelCase and preceeded by a '&' sign, unless the choices have hidden keys or required keys they must be added in the following format:__
```
&locationChoice|+
```
__**Hidden items** are items are used to hide locationChoices from the player until he/she has the **required items** to proceed while required items are needed in order to proceed, but the player can still see the locationChoice it belongs to. The complete syntax for valid locationChoices with keys are:__
```
&validLocationChoice|hiddenKeyItem+requiredKeyItem: This locationChoice's text.
```
__Locations which may only be visited once should contain the '@' sign.__
```
#locationOne
&oneLocationChoice|+: Once you leave locationOne you never come here again.
@
```
__Items are added to a location by using the '^' sign:__
```
^anItemToPickUp
```
__An example of a possible valid location syntax:__
```
===
#locationOne
^anItemToPickUp
&validLocationChoice|hiddenKeyItem+requiredKeyItem: This locationChoice's text.
@
===
```
**IMPORTANT!**
Due to how the code is currently written, all of the hidden choices (i.e. those with hiddenKeyItems) must be at the bottom of the locationChoice list or else the user's input will not work properly on some occasions.  

You can add __game items__ inside of the game_items.txt file like this for it to parse correctly:
```
itemType | itemId | Item title | power (string or int)
```
Items can also be combined, if they are paired up inside of the pairs.txt file and have valid 'combination' options inside of the items.cpp, for example:
```
itemToPairWith1:itemToPairWith2
```

## Acknowledgement  
Thank you to Paradox Interactive's [GamesByHer](https://gamedevcourse.paradoxinteractive.com/) initiative for creating this challenge.
