Pacman

Game description: 
Pacman is a maze arcade game, which has its beginnings in 1980. Pacman, represented by a continuous single dot, navigates through the enclosed maze, filled with pac-dots (fruits) that need to be collected in order to gain points. At any time, one or more ghosts (represented by blinking dots) chase Pacman and, if any of them touches it, a life is lost. The objective is to eat as many dots as possible, each one of them representing a point (or more, depending on the level). When all the dots are collected, the level is completed and the player advances to the next one.

Hardware components: 
a. 1 * 8x8 LED matrix
b. 1 * LCD
c. 1 * Joystick
d. 1 * MAX7219 Driver
e. 1 * Potentiometer
f. Wires
g. Resistors (220 Ohm & 100k Ohm)
h. Arduino
i. Connector cable

How to play:
In the settings option, you can choose the level you begin with. Note: if you start from a level without completing the previous ones, your score still begins with 0 points. Also, in the settings, you can set the name of the player, which will be associated with the highest score. Default is “Pacman” – 0 points.

Entering the game is done by choosing “Play” option.
You start from an initial position. The maze is represented by continuous dots, similar to walls that cannot be passed through. Pacman’s direction is controlled with the joystick. If the joystick is not being moved on the X or Y axis, its position remains the same.
Depending on the level, one or more ghosts (represented by blinking dots) constantly chase Pacman. They navigate through the maze until they reach its position, which leads to losing a life. When all the lives are lost, the game ends.
The fruits that need to be collected are represented by single dots, placed all over the maze. One fruit is being collected by simply passing through it. When there are no fruits left, the level is completed and the player can choose to continue or to quit the game.
At every level, there is a bonus fruit (the “golden fruit”), which, if it’s being collected, leads to earning a life. It is placed in a position hard to reach and it is not necessary for completing the level. It is represented by a blinking dot, similar to the ghosts, the difference being that it always stays in the same position.
At any time, the player can quit the game by pressing the button.

In the “High score” option, the users can find the highest score and also the time required by the player to reach that score (measured in seconds). Initially, the highest score is 0.

In order to get more information about the game, you can choose option “Info”, which contains:
1. Creator name
2. GitHub Link
3. Game name

Game specifications:
Levels:
Level 1: 
-	4 fruits to get + the “golden” fruit
-	1 ghost – slow speed
-	Score = 4 * 1 = 4 + one possible life
-	Maze complexity: easy
Level 2:
-	5 fruits to get + the “golden fruit”
-	2 ghosts – normal speed
-	Score = Score (level 1) + 5 * 2 + one possible life
-	Maze complexity: medium
-	
Level 3:
-	6 fruits to get + the “golden fruit”
-	3 ghosts – fast speed
-	Score = Score (level 2) + 6 * 3 + one possible life
-	Maze complexity: difficult

If a level is not completed, the points added to the current score is 0.
At the end of the game, the final score is calculated as score(level) + level * number of lives, where level can be 1, 2 or 3.

