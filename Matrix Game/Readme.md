**Pacman** (simplified version)

**Game description:** <br/>
Pacman is a maze arcade game, which has its beginnings in 1980. Pacman, represented by a continuous single dot, navigates through the enclosed maze, filled with pac-dots (fruits) that need to be collected in order to gain points. At any time, one ghost (represented by a blinking dot) chases Pacman and, if the ghost touches it, a life is lost. The objective is to eat as many dots as possible, each one representing a point (or more, depending on the level). When all the dots are collected, the level is completed and the player advances to the next one.

**Hardware components:** <br/>
a. 1 * 8x8 LED matrix <br/>
b. 1 * LCD <br/>
c. 1 * Joystick<br/>
d. 1 * MAX7219 Driver<br/>
e. 1 * Potentiometer<br/>
f. Wires<br/>
g. Resistors (220 Ohm & 100k Ohm)<br/>
h. Arduino<br/>
i. Connector cable<br/>
j. 1 * Breadbord<br/>
k. 2 * Mini Breadbords<br/> 

**How to play:**<br/>
In the settings option, you can choose the level you begin with. Note: if you start from a level without completing the previous ones, your score still begins with 0 points. Also, in the "Play" option, you can set the name of the player, which will be associated with the highest score. Default is “PLAYER” – 0 points.

Entering the game is done by choosing “Play” option.<br/>
You start from an initial position. The maze is represented by continuous dots, similar to walls that cannot be passed through. Pacman’s direction is being controlled with the joystick. If the joystick is not being moved on the X or Y axis, its position remains the same.
One ghost (represented by a blinking dot) constantly chases Pacman. It navigates through the maze until it reaches its position, which leads to losing a life. When all the lives are lost, the game ends and a suggestive message appears on screen. <br/>
The fruits that need to be collected are represented by single dots, placed all over the maze. One fruit is being collected by simply passing through it. When there are no fruits left, the level is completed and the player advances to the next one. If all the 3 levels are completed, the player wins the game and a suggestive message appears on screen. Also, if the score is bigger than the highscore, the highest score is updated and a message is shown on screen.<br/>
At every level, there is a bonus fruit (the “golden fruit”), which, if it’s collected, leads to earning a life. It is placed in a position hard to reach and it is not necessary for completing the level. It is represented by a blinking dot, similar to the ghosts, the difference being that it always stays in the same position.<br/>

The ghosts can plant some "traps", represented by single dots, very similar to the walls or the normal fruits. Pacman can pass through them, but they are empty points, meaning that the player earns 0 points by collecting them. The traps can confuse the player in 2 ways: thinking that they are walls and trying to avoid them; thinking that they are fruits and trying to collect them.

In the “High score” option, the users can find the highest score and also the name of the player which reached that score. Initially, the highest score is 0 and the name is "PLAYER".<br/>

In order to get more information about the game, you can choose option “Info”, which contains:<br/>
1. Game name
2. Creator name<br/>
3. GitHub Link<br/> and more


**Game specifications:**<br/>
Levels <br/>
Level 1: <br/>
-	4 fruits to get + the “golden” fruit<br/>
-	ghost – slow speed<br/>
-	Score = 4 * 1 = 4 + one possible life<br/>
-	Maze complexity: easy<br/>
<br/>
Level 2: <br/>
-	5 fruits to get + the “golden fruit”<br/>
-	ghost – normal speed<br/>
-	Score = Score (level 1) + 5 * 2 + one possible life<br/>
-	Maze complexity: medium<br/>
<br/>
Level 3: <br/>
-	6 fruits to get + the “golden fruit”<br/>
-	ghost – fast speed<br/>
-	Score = Score (level 2) + 6 * 3 + one possible life<br/>
-	Maze complexity: difficult<br/>
<br/>
At the end of the game (but only if the player wins the game) the final score is calculated as: score(level) + level * number of lives.

