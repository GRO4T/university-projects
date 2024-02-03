# Documentation of the project "simple platformer in box2d"

## Introduction

Main goal of the project was to Create a simple game using box2d graphics engine.
During development few other libraries came useful. <br>
These are:
* rapidJSON - JSON parser. Used for parsing JSON config files.
* SFML - for displaying sprites, text and playing audio.

## Game mechanics

The goal of the game is to gather enough points to complete currently played level either by collecting the coins or slaying enemies.
After you score enough points you are given an opportunity to advance to the next stage.
In order to defend himself kPlayer is equipped with two weapons: sword and bow.
Sword deals more damage but keeps you more vulnerable to attacks. On the other hand
bow is slower and has lower damage output but allows you to keep distance from your enemies.

## Controls

arrows -> movement<br>
z -> melee attack<br>
Space -> shoot an arrow<br>
q -> quit back to menu<br>

(Debug options)<br>
s -> switch into spectate mode<br>
o -> zoom out<br>
p -> zoom in

## Implementation

### Main modules

Main part of the project is class **Game**. It communicates with smaller modules like **AudioController** calling their update() and Draw() methods. Second most important module is class **Level**. It's main purpose is to generate level and update it's state until either kPlayer dies or collects all the coins. After that it's destroyed and game switches back to menu.

### Object structure
Base class for all physical objects which take part in collision or simply detect them is **Entity**. It derives from sf::Drawable. Main purpose of this is to be able to use SFML notation for displaying objects
~~~~~~~~~~cpp
window->Draw(object);
~~~~~~~~~~

Entity is an interface but instance of it still can be created. That's because I wanted to use virtual methods mechanics. <br>
Another important class is **Creature**. It defines any character that has animated movement.

### Asset management
Assets are managed through **Assets** class which loads and stores them.
Then anytime a class needs a certain assets I simply give her a pointer to Assets object or to one of it's
members.

### Wrong design decisions
Inside **Creature** we have an **Animator** class whose main purpose is to store and update sprites and animation states.
I've written this class at the early stage of the project and made a mistake implementing class **Animation**.
I've known already class sf::Clock but I doubted it's precision and decided not to use it, implementing timer by myself.
This and the fact that I was't limiting framerate in any way made my game framerate dependant. Game simply ran slower on weaker machines.
I've tried to fix it in many different ways from sleeping the thread which resulted in box2d physics refusing to function,
to making everything depend on elapsed time. And until today I haven't figured out why this second method didn't work.
I've finally fixed it after executing
~~~~~cpp
mainWindow.setFramerateLimit(240.0f);
~~~~~
In execution of the **setFrameLimit** function another problem can be seen. Game has to run at 240 fps.
It's still better than unlocked 1000fps but when I was creating this game I wanted it to run on 60.
The issue came from the way box2d calculates physics. Box2d simulates step using metres not pixels.
That's to avoid huge floating point errors. Because of that box2d limits maximum velocity.
It can be omitted by simply changing the value in box. But according to box2d manual it can cause
undesirable effects like tunneling (objects going through eachother).
Fortunately I found out that this limit is dependant on framerate and at 240fps is at 480 m/s^2
which was enough for my needs.

### Project requirements

#### Templates
One instance of template implementation is **RemoveFromVector** function. Which gets vector of objects of any type and a function returning bool
to decide whether remove an element. Game and Creature were also templates but I've decided that it served no purpose while cluttering most
often visited part of the code.

#### Pointer to implementation
Gui and AudioController are implemented using **PIMPL**. It was very beneficial as I noticed improvements in compilation time.

#### Exception handling
In order to handle exception I use:
* one main try-catch in main to catch any unhandled exception from previous modules
* bunch of different local try-catches many of which in assets.cpp when loading game assets

#### Standard containers
From variety of std containers in my project I use only std::vector. F.e. storing enemies or sprites for kPlayer's animation.
Iterators are only directly used when removing elements from these vectors. When accessing elements I use **auto** directive.

#### Virtual functions
Example of virtual function from the project can be **OnCollision** function which a function called by class **ContactListener** when object
starts collision.

#### Unit tests
It's the only requirement which is not fully met. The only unit tests I have are in python script generating map.
They check whether map generation works properly on simple, well-known data. But still it's up to a programmer whether
test failed or not as algorithm is undeterministic.

