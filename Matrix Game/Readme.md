Pacman

Game description: <br/>
Pacman is a maze arcade game, which has its beginnings in 1980. Pacman, represented by a continuous single dot, navigates through the enclosed maze, filled with pac-dots (fruits) that need to be collected in order to gain points. At any time, one or more ghosts (represented by blinking dots) chase Pacman and, if any of them touches it, a life is lost. The objective is to eat as many dots as possible, each one of them representing a point (or more, depending on the level). When all the dots are collected, the level is completed and the player advances to the next one.

Hardware components: <br/>
a. 1 * 8x8 LED matrix <br/>
b. 1 * LCD <br/>
c. 1 * Joystick<br/>
d. 1 * MAX7219 Driver<br/>
e. 1 * Potentiometer<br/>
f. Wires<br/>
g. Resistors (220 Ohm & 100k Ohm)<br/>
h. Arduino<br/>
i. Connector cable<br/>

How to play:<br/>
In the settings option, you can choose the level you begin with. Note: if you start from a level without completing the previous ones, your score still begins with 0 points. Also, in the settings, you can set the name of the player, which will be associated with the highest score. Default is “Pacman” – 0 points.

Entering the game is done by choosing “Play” option.<br/>
You start from an initial position. The maze is represented by continuous dots, similar to walls that cannot be passed through. Pacman’s direction is controlled with the joystick. If the joystick is not being moved on the X or Y axis, its position remains the same.
Depending on the level, one or more ghosts (represented by blinking dots) constantly chase Pacman. They navigate through the maze until they reach its position, which leads to losing a life. When all the lives are lost, the game ends.
The fruits that need to be collected are represented by single dots, placed all over the maze. One fruit is being collected by simply passing through it. When there are no fruits left, the level is completed and the player can choose to continue or to quit the game.
At every level, there is a bonus fruit (the “golden fruit”), which, if it’s being collected, leads to earning a life. It is placed in a position hard to reach and it is not necessary for completing the level. It is represented by a blinking dot, similar to the ghosts, the difference being that it always stays in the same position.<br/>
At any time, the player can quit the game by pressing the button.

In the “High score” option, the users can find the highest score and also the time required by the player to reach that score (measured in seconds). Initially, the highest score is 0.

In order to get more information about the game, you can choose option “Info”, which contains:<br/>
1. Creator name<br/>
2. GitHub Link<br/>
3. Game name

Game specifications:<br/>
Levels <br/>
Level 1: <br/>
-	4 fruits to get + the “golden” fruit<br/>
-	1 ghost – slow speed<br/>
-	Score = 4 * 1 = 4 + one possible life<br/>
-	Maze complexity: easy<br/>
<br/>
Level 2: <br/>
-	5 fruits to get + the “golden fruit”<br/>
-	2 ghosts – normal speed<br/>
-	Score = Score (level 1) + 5 * 2 + one possible life<br/>
-	Maze complexity: medium<br/>
<br/>
Level 3: <br/>
-	6 fruits to get + the “golden fruit”<br/>
-	3 ghosts – fast speed<br/>
-	Score = Score (level 2) + 6 * 3 + one possible life<br/>
-	Maze complexity: difficult<br/>
<br/>
If a level is not completed, the points added to the current score is 0.<br/>
At the end of the game, the final score is calculated as score(level) + level * number of lives, where level can be 1, 2 or 3.

