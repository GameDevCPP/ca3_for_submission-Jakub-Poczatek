# Second Thoughts - GDD #
### by Jakub Poczatek ###
# # 
# #

# GDD #

## Introduction ##
This is the game design document for the game, "Second Thoughts". Second Thoughts is a 2D side-scroller platformer action game. The game utilizes simple shape graphics with a greater focus on systems and mechanics. It contains 3 levels, each level introducing new mechanics to the player. Second Thoughts places great emphasis on the "Entity Componenet System", AI, memory management, different design patterns and data driven functionality. 

## Technology ##
Seconds Thoughts will be developed for the Windows Platofrm using SFML and a mix of my own personal libraries as well as an external library. see last section for further inspection of technologies.

### My personal libraries include: ###
- A* star library, implemented using the following guide: https://www.redblobgames.com/pathfinding/a-star/implementation.html . Used for controlling the AI of flying enemies.
- audio library, implemented following documentation on SFML-Audio. Used for all audio in the game.
- bmp loader library, implemented using the following guide: https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/ . Used for reading in bmp images to later be processed into levels.
- rgb tile loader libarry, a refactored version of the previously used text based tile loader library. Utilises the bmp loader library to read in coloured pixels and design a level based on them.

### External Library ###
- json library, single include file from the following url: https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp . Used for processing game data into and from json files.

## Backstory ##
You play as a nameless character trapped in the depths of his own mind. Can you make it out without causing even more damage? Or will you give in to fear?

## Objective ##
Second Thoughts is composed of 3 distinct levels that the player must progress through. On each level, the player will need to avoid enemies and platform around the level to find the key item to unlock passage to the next level. The player will have a set amount of health that will decrease everytime an enemy gets close to them. If the player's health reaches 0, they are restarted to the start of the level, with the amount of health they started the level with. 

## Gameplay ##
Second Thoughts will be a side scrolling platformer. The player will be able to run left and right as well as jump up on platforms and around enemies. The player will have no combat abilities and as such will need to be cautious around the enemies who will cause damage to them when touched. The player starts off the game with 3 health. As the level's progress, the player will be introduced to new mechanics. 

## Levels ##
All the levels are designed in bitmap images and later processed into actual levels in the game. The level breakdown is as such:

### Level 1 ###
Basic platforming, the player will be able to get feel for the level's controls without any danger. Once the player makes their way to the upper level they will need to jump up on a platform to cross a deadly fall. On this platform the player will be introduced to the yellow key object that they need to collect to be able to leave the level. 

### Level 2 ###
Level 2 will build on the previous level by including enemies and health pickups. The level will include 2 types of enemies.
- Ground Enemy, this is a rolling ball that depending on difficulty will either track the player and progress towards them or will go left and right, changing direction when they hit a wall.
- Turrets, these are enemies on top of ceilings/bottom of platforms that shoot down balls that can hurt the player. 
The health pickup is a small floating ball, when the player interacts with it, they gain 1 health.

### Level 3 ###
Depending on difficulty level 3 will either introduce the final enemy or will be a rehash of level 2. If the player chooses hard difficulty (which is also the default difficulty), level 3 will contain flying enemies. These enemies use my personal implementation of A* to track the player and pathfind towards them. Besides that, the level will contain all prior tropes. 

## Controls ##
The game will be fully playable using the keyboard. Here are the controls:
- **A:** move left
- **D:** move right
- **W:** jump

## Menu ##
The game starts up into the main menu scene here the player has the following functionality.
- Difficulty: difficulty can be set to either hard using "Keyboard::Num1" or easy using "Keyboard::Num0".
- New/Continue: The player can either start a new game using "Keyboard::Space" or continue their last saved game using "Keyboard::Enter". Choosing to continue the game will discard any difficulty choice the player made, instead using the difficulty of the save game.

After choosing to start the game, the player will be shown a loading screen for level 1, all further levels do not have a loading screen. 

## Artwork ##
The game utilizes a simple art style using simple shapes and colours.

## Audio ##
There is 3 audio files total, a background music track and 2 sounds for taking damage and collecting pickups. Sounds are managed by the audio lib that I wrote using SFML::Audio documentation. 

# Externals #

## Repository Usage ##
Git was used as the version control system used to manage the project. Branches were created for each specific set of features and later merged into main when their functionality was fully implemented. Git Submodules were not used as there wasn't a need for them since no outside GitHub repos were used.

## Game/Project Management ##
Trello was used to manage the project progress and assign the next set of tasks.
Trellow URL: https://trello.com/b/5lj21LzW/c-assignment 

## Video Walkthrough ##
Youtube URL: https://youtu.be/SKgmpb-Q_bE 

# Technology #
In this section I will go through the technological aspects of the game that I designed and/or implemented.

## Libraries ##

### lib_aStar : Personal ###
lib_aStar is a library for handling the A* AI algorithm. It is a heavily refactored version of the one present in the guide listed above. The library had any non-A* functions removed and the once single file containing the full functionality has been split into the standard .cpp/.h split.  

Certain functions such as aStarSearch() are static functions and as such needed to be defined in the .h instead of the .cpp as otherwise the code wouldn't compile. The library was also improved to use 8 directional neighbours instead of the prior 4, this leads to smooth AI paths as they can now travel in diagonals.  

This library is used inside the EnemyAStarComponent class.

### lib_audio : Personal ###
lib_audio is a small library implemented to provide static functions for playing music and sounds. Instead of defining new instances of sf::Sound and sf::Music everytime audio needs to be played, this library allows for those components to only be instantiated once and then statically called with custom paths passed in, these paths are controlled through the jsonData.h file. The library only contains 2 functions:
- static void start(): to be called in the main.cpp to initialize certain variables and play the background music, loops audio.
- static void play(): to be called whenever an audio track needs to be played once.

### lib_bmp_loader: Personal ###
lib_bmp is a library for reading in bitmap files and returning them as a vector of sf::Color. This implementation is a heavily refactored and improved version of the one present in the guide above. Functions such as writing and creating of in-memory bitmaps have been stripped as they were deemed redundant and new functions were added. These functions give the ability for bitmaps to be read in, each pixel analysed for color and saved in an vector of sf::Color.  

This library is heavily utilized using the lib_tile_level_loader_rgb library.

### lib_json: Third Party ###
lib_json is an implementation of the single-include file found in the github above. This library handles the reading in, parsing and writing of json files. Used heavily for the data driven functionality of the game.

Utilized in my personal jsonData class.

### lib_tile_level_loader_rgb: Personal ###
lib_tile_level_loader_rgb is a library responsible for taking in vectors of sf::Color objects and processing them into level layouts. This is a heavily refacted version of the supplied lib_tile_level_loader library. It completely overhauls the previous functionality, favouring image based level design over text based. This provides a more intuitive design process as levels can be visualizes much easier. 

The Tiles enum used for tile identification supports a wide variety of colours and their assigned usages, see lib_tile_level_loader_rgb/LevelSystem.h for more details. 

## Components ##
a wide variety of additional functionality has been implemented in the form of new or refactored compoenents.

### BulletComponent: Refactored ###
Turrets now utilize an object pool for handling their bullets and as such the lifetime functionality in bullets has become redundant and has been removed.

### EnemyTargetedAIComponent: New ###
This component is an improved version of the EnemyAIComponent that is used in it's place when difficulty:hard is used instead of difficulty:easy. It maintains all the core functionality of the previous component while also adding player tracking by changing its _direction based on it's position to the player.

### EnemyAStarComponent: New ###
This component contains a substantial amount of functionality, handling the full implementation of the lib_aStar library. On startup it initializes a _grid containing the layout of the whole level, letting the AI know where the walls are so it can avoid them. Once a path is calculating, every frame the AI will come closer to its next path point, when it get's close enough, the _pathCounter is incremented and the next path point is sought out. To save on performance, the path to player calculation is only done every half a second using delta time and a simple counter.

### EnemyTurretComponent: Refactored ###
An object pool for bullets has been created. Now instead of creating new bullets everytime one is fired and then that bullet deleting itself after a specific amount of time. The turrets get assigned a bullet pool of size 5 and cycles through it's bullets, changing their position and applying a physics impule when fired. 

### EntityHealth: New ###
EntityHealth is primarly used for the player, with future plans of implementing it to tougher enemies. The EntityHealth gives the player a set amount of life that the enemies need to eat through before the player dies and the level is reset. The health in this component can also be increased by the player collecting health pickups. There is a cooldown of 1second before the player can take damage again. If the adjustHealth function identifies the _health modifier as negative, it will play a sound to let the player know that they took damage.

### HealthPickup: New ###
Part of the new "PickupComponent" family. The health pickup adds functionality to the base component by adding health to the player's EntityHealth object when its desctructor is called (when the base PickupComponent's update() function detects collision with player and set's itself for deletion).

### HurtComponent: Refactored ###
The hurt component no longer destroys the entity it's attached to, allowing for enemies to stay alive after dealing damage. Instead of deleting the player when collision is detect, the hurt component now simply damages them by adjusting their EntityHealth component.

### PickupComponent Family: New ###
The PickupComponent is the base component for a family of components all utilizing it's player collision detection capability. It is also directly use by the key item in each level, as that pickup doesn't need any further functionality. The component now also plays a sound when collision is detected. 

### PlayerPhysicsComponent: Refactored ###
Json data functionality implemented for certain aspects. The player's jump height, run speed and restitution have all been adjusted.

## AI ##
The game now features 3 types of AI.
- The base dumb AI, as supplied in EntityAIComponent.
- A smarter targeted AI. Still only moves left and right, but can target the player.
- A* AI, flying enemies only, is always seeking the player.

## Data Driven Functionality ##
a JsonData class was implemented to handle all JSON data functionality in the game. This includes handling the paths for all audio in the game. The main use of this class is to handle the setting up of each level, containing data such as player/enemy sizes, tile sizes, view sizes etc. This allows repeating data to be changed in the JSON file and it taking effect throughout the project. 

The JsonData object is also the core of the save/load functionality. The playerData.json file contains variables for the current difficulty, level and player health. These variables are written to and fetched from through all the levels and some of the components in the project. 

## Sound and Music ##
A seperate library was introduced to handle the playing of all music and sound instances throughout the game. Audio file paths are handles by the JsonData class. 3 Distinct Audio's present throughout the game: background music, pickup audio, player taking damage audio.

## Memory Management ##
Smart pointers and memory allocation used throughout the project. Examples include:
- shared pointers
- unique pointers
- weak pointers

## Management of Game Resources ##
The following guidelines have been used:
- Flyweight is used througout the project in components and scenes.
- Using JsonData as a resource manager for audio.

## Advanced C++ ##
This is some of the C++ functionality is used througout:
- unordered maps
- vectors
- lambda conditionals
- smart pointers: shared, unique, weak
- [[ maybe unused ]] tag used for class members in the bmp library. Used to tell the compiler to disregard the fact that these variables are unused. C++17 feature.
- [[ likely ]] tags used in conditionals to inform the compiler which option is more likely than others. C++20 feature.
- [[ nodiscard ]] tag used to inform the compiler that a function or has has its attribute and its return value is discarded.

## Design Patterns ##
The following design patterns are utilized:
- Flyweight used for components and scenes.
- Object pools used for the turret component.