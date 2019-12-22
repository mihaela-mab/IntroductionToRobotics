#include <LiquidCrystal.h>
#include "LedControl.h"
#include <EEPROM.h>

const int rsPin = 7;
const int ePin = 6;
const int d4Pin = 5;
const int d5Pin = 4;
const int d6Pin = 3;
const int d7Pin = 2;

const int pinSw = 13;
const int pinX = A0;
const int pinY = A1;
int switchValue;
int xValue = 0;
int yValue = 0;
int swState = LOW;

bool joyMoved = false;
bool button = false;
int minThreshold = 400;
int maxThreshold = 600;

const int brightness = 2;
const int dinPin = 12;
const int clkPin = 11;
const int loadPin = 10;

const int noOfDrivers = 1;

int arrow  = 0;
struct arrowPosition {
 int x;
 int y;
 } pos[9];  
int startPos = 0;
int endPos = 0;

int option = 0; /* 0 = Main Menu
                   1 = Play
                   2 = HighScore
                   3 = More
                   4 = Settins
                   5 = Info*/
bool exitMore = false;
bool exitInfo = false;
bool exitHighScore = false;
bool exitPlay = false;
bool exitSettings = false;
bool exitNext = false;
bool exitGameOver = false;
bool exitWin = false;
bool exitChangeName = false;

int hScore = 0;
char hScorePlayer[7] = "PLAYER";
char playerName[7] = "PLAYER";

int level = 1;
int lives = 3;
int score = 0;

unsigned long prevMillis = 0;
unsigned long prevMillisWin = 0;

bool matrix[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 1, 0, 1, 0, 0},
  {0, 1, 0, 1, 0, 1, 0, 0},
  {0, 1, 1, 1, 0, 1, 0, 0},
  {0, 1, 0, 1, 0, 1, 0, 0},
  {0, 1, 0, 1, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}

};

struct {
  int x, y;
}ghost[6], prevGhost[6], pacman[3], prevPacman[3], fruits[15], gFruits[3];

struct Distance{
  int distance, index;
} dist[4]; 

const int MAX = 1000;

bool takenFruit[15] = {0};
bool takenGoldenFruit[3] = {0};
int nOfFruitsTaken = 0;

int letter = 0;
int prevLetter = 0;

int eeAddress = 0;

LedControl lc = LedControl(dinPin, clkPin, loadPin, noOfDrivers);

LiquidCrystal lcd(rsPin, ePin, d4Pin, d5Pin, d6Pin, d7Pin);

// Note: all the delays used at the beginning and at the end of functions "win", "gameOver", "play", "next", "highScore",
// "more", "settings", "info", "level1", "level2" and "level 3" have the purpose to make a slower and visually more pleasant
// transition from a state to another.

// initialize arrow positions
void initArrowPositions() {
  pos[0].x = 0;
  pos[0].y = 1;
  
  pos[1].x = 6;
  pos[1].y = 1;
  
  pos[2].x = 10;
  pos[2].y = 1;

  pos[3].x = 0;
  pos[3].y = 1;

  pos[4].x = 10;
  pos[4].y = 1;

  pos[5].x = 14;
  pos[5].y = 0;

  pos[6].x = 0;
  pos[6].y = 0;

  pos[7].x = 2;
  pos[7].y = 1;

  pos[8].x = 9;
  pos[8].y = 1;  
}

void clearMatrix() {
  for(int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++) {
      matrix[row][col] = 0;
      lc.setLed(0, col, row, false);
    }
  }
}

void initPacmanPositions() {
  if (level == 1 || level == 2) {
    pacman[0].x = pacman[0].y = prevPacman[0].x = prevPacman[0].y = 0;
  }
  else {
    pacman[0].x = pacman[0].y = prevPacman[0].x = prevPacman[0].y = 1;
  }
}

void initGhostPositions() {
  if (level == 1 || level == 2) {
    ghost[0].x = ghost[0].y = 7; prevGhost[0].x = prevGhost[0].y = 0;
  }
  else {
    ghost[0].x = ghost[0].y = 6; prevGhost[0].x = prevGhost[0].y = 0;
  }
}

void initFruitsPositions(int level) {
  if (level == 1) {
    fruits[0].x = 0; fruits[0].y = 4;
    fruits[1].x = 4; fruits[1].y = 7;
    fruits[2].x = 3; fruits[2].y = 1;
    fruits[3].x = 7; fruits[3].y = 2;
    for (int i = 0; i < 4; i++) {
      if(!takenFruit[i]) {
        lc.setLed(0, fruits[i].x, fruits[i].y, true);
      } 
    }
    // golden fruit
    if (!takenGoldenFruit[0]) { // level 1
      gFruits[0].x = 7; gFruits [0].y = 7; 
      lc.setLed(0, gFruits[0].x, gFruits[0].y, true); delay(25);
      lc.setLed(0, gFruits[0].x, gFruits[0].y, false);
    }
  }

  if (level == 2) {
    fruits[4].x = 4; fruits[4].y = 1;
    fruits[5].x = 1; fruits[5].y = 7;
    fruits[6].x = 5; fruits[6].y = 7;
    fruits[7].x = 5; fruits[7].y = 3;
    fruits[8].x = 0; fruits[8].y = 4;

    for (int i = 4; i < 9; i++) {
      if(!takenFruit[i]) {
        lc.setLed(0, fruits[i].x, fruits[i].y, true);
      } 
    }
    // golden fruit
    if (!takenGoldenFruit[1]) { 
      gFruits[1].x = 7; gFruits [1].y = 7; 
      lc.setLed(0, gFruits[1].x, gFruits[1].y, true); delay(25);
      lc.setLed(0, gFruits[1].x, gFruits[1].y, false);
    }
  }

  if (level == 3) {    
    fruits[9].x = 4; fruits[9].y = 2;
    fruits[10].x = 5; fruits[10].y = 3;
    fruits[11].x = 6; fruits[11].y = 1;
    fruits[12].x = 1; fruits[12].y = 6;
    fruits[13].x = 3; fruits[13].y = 5;
    fruits[14].x = 4; fruits[14].y = 4;

    for (int i = 9; i < 15; i++) {
      if(!takenFruit[i]) {
        lc.setLed(0, fruits[i].x, fruits[i].y, true);
      } 
    }
    // golden fruit
    if (!takenGoldenFruit[2]) { 
      gFruits[2].x = 6; gFruits[2].y = 6; 
      lc.setLed(0, gFruits[2].x, gFruits[2].y, true); delay(25);
      lc.setLed(0, gFruits[2].x, gFruits[2].y, false);
    }
  }
}

// for a certain position arr, show the arrow; every function in which arrows appear have some positions allocated for them
// for example, the mainMenu function has the positions from 0 to 2, the More function has the postions
// 3 and 4 (here, StartPos = 3, endPos = 4) etc.
void showArrow(int arr, int startPos, int endPos) {
  //disable all the arrows
  for (int i = startPos; i <= endPos; i++) {
     lcd.setCursor(pos[i].x, pos[i].y);
     lcd.print(" ");
  }

  //enable the arrow at the position arr
  lcd.setCursor(pos[arr].x, pos[arr].y);
  lcd.print(">");
}

// read joystick's movements (only on X-axis) and display the arrow properly
void arrowMoved(int startPos, int endPos) {
  xValue = analogRead(pinX);
  if(xValue < minThreshold && joyMoved == false) {
    if(arrow > startPos) {
      arrow--;
    }
    else {
      arrow = endPos;
    }
    joyMoved = true;
  }

  if (xValue > maxThreshold && joyMoved == false) {
      if (arrow < endPos) {
       arrow++;  
      }
      else {
        arrow = startPos; 
       } 
       
      joyMoved = true;
    }
  
     if(xValue >= minThreshold && xValue <= maxThreshold) {
      joyMoved = false;
     }

  showArrow(arrow, startPos, endPos);
}

// read button state
void buttonState() {
  swState = digitalRead(pinSw);
  //Serial.println(swState);
  if(swState == LOW && button == false)
     button = true;
  else 
    if (swState == LOW) {
      button = false;
    }
}

void mainMenu() {
  lcd.setCursor(5, 0);
  lcd.print("Pacman");

  lcd.setCursor(1, 1);
  lcd.print("Play");

  lcd.setCursor(7, 1);
  lcd.print("HS");

  lcd.setCursor(11, 1);
  lcd.print("More");

  arrowMoved(0, 2); 
  
  clearMatrix();
  
  // Pacman animation on the matrix
  matrix[2][1] = matrix[3][1] = matrix[4][1] = matrix[5][1] = matrix[1][2] = matrix[1][3] = matrix[1][4] = matrix[1][5] =
  matrix[6][2] = matrix[6][3] = matrix[6][4] = matrix[6][5] = matrix[5][4] = matrix[4][3] = matrix[3][4] = 
  matrix[2][5] = matrix[1][4] = matrix[1][4] = matrix[1][3] = matrix[2][3] = 1;

  for(int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
  
  buttonState();

  // if the player chooses an option and presses the button
  if (button == true) {
    option = arrow + 1; // arrows start from 0 and options from 1, this is the reason why we have arrow + 1
  }
}

void saveOnEeprom() {
  eeAddress = 0;
  EEPROM.put(eeAddress, hScore);
  eeAddress += sizeof(int);
  EEPROM.put(eeAddress, hScorePlayer);
}

void updateScore() {
  lcd.setCursor(6, 1);
  lcd.print(score);
}

void updateLives() {
  lcd.setCursor(15, 0);
  lcd.print(lives);
}

// a function that reads and displays on the matrix Pacman's movements
void movePacman(int who) {
  // on the X-axis
  xValue = analogRead(pinX);
  if(xValue < minThreshold && joyMoved == false) {
    if(pacman[who].x > 0) { 
      if (!matrix[pacman[who].x - 1][pacman[who].y]) {
        prevPacman[who].x = pacman[who].x;
        pacman[who].x--; 
        prevPacman[who].y = pacman[who].y;
      }
    }
    else if(!matrix[7][pacman[who].y]){
      prevPacman[who].x = pacman[who].x;
      pacman[who].x = 7;
      prevPacman[who].y = pacman[who].y;
    }
    joyMoved = true;
  }

  if (xValue > maxThreshold && joyMoved == false) {
      if (pacman[who].x < 7) {
        if (!matrix[pacman[who].x + 1][pacman[who].y]) {
         prevPacman[who].x = pacman[who].x;
         pacman[who].x++;  
         prevPacman[who].y = pacman[who].y;
        }
      }
      else if (!matrix[0][pacman[who].y]){
        prevPacman[who].x = pacman[who].x;
        pacman[who].x = 0; 
        prevPacman[who].y = pacman[who].y;
       } 
       
      joyMoved = true;
    }
  
     if(xValue >= minThreshold && xValue <= maxThreshold) {
      joyMoved = false;
     }

  // on the Y-axis
  yValue = analogRead(pinY);
  if(yValue < minThreshold && joyMoved == false) {
    if(pacman[who].y > 0) {
      if (!matrix[pacman[who].x][pacman[who].y - 1]) {
        prevPacman[who].y = pacman[who].y;
        pacman[who].y--;
        prevPacman[who].x = pacman[who].x;
      }
    }
    else if (!matrix[pacman[who].x][7]){
      prevPacman[who].y = pacman[who].y;
      pacman[who].y = 7;
      prevPacman[who].x = pacman[who].x;
    }
    joyMoved = true;
  }

  if (yValue > maxThreshold && joyMoved == false) {
      if (pacman[who].y < 7) {
        if (!matrix[pacman[who].x][pacman[who].y + 1]) {
         prevPacman[who].y = pacman[who].y;
         pacman[who].y++;  
         prevPacman[who].x = pacman[who].x;
        }
      }
      else if (!matrix[pacman[who].x][0]){
        prevPacman[who].y = pacman[who].y;
        pacman[who].y = 0; 
        prevPacman[who].x = pacman[who].x;
       } 
       
      joyMoved = true;
    }
  
    if(yValue >= minThreshold && yValue <= maxThreshold) {
     joyMoved = false;
    }

 // display Pacman's position on the matrix, but first remove the previous one
 lc.setLed(0, prevPacman[who].x, prevPacman[who].y, false); 
 lc.setLed(0, pacman[who].x, pacman[who].y, true); 

 // depending on the level, fruits will start and end from a certain position to another
 int startPos, endPos;
 if (level == 1) {
  startPos = 0; endPos = 4;
 }
 else
  if (level == 2) {
    startPos = 4; endPos = 9;
  }
  else {
    startPos = 9; endPos = 15;
  }

 // here, check if Pacman passed through a fruit and, if so, remove it from the matrix
 for (int i = startPos; i < endPos; i++) {
  if (prevPacman[who].x == fruits[i].x && prevPacman[who].y == fruits[i].y && !takenFruit[i]) {
    score += level;
    updateScore();
    takenFruit[i] = 1;
    nOfFruitsTaken++;
    break;
  }
 }

  // here, check if Pacman passed through the golden fruit and, if so, remove it from the matrix and increase the number of lives
  if (prevPacman[who].x == gFruits[level - 1].x && prevPacman[who].y == gFruits[level - 1].y && !takenGoldenFruit[level - 1]) {
    lives++;
    updateLives();
    takenGoldenFruit[level - 1] = 1;
  }
  
}

// change ghost's coordinates on the matrix
void changeCoord(int index) {
  if (index == 0) {
    prevGhost[0].x = ghost[0].x;
    ghost[0].x--;
    prevGhost[0].y = ghost[0].y;
  }
  else
    if (index == 1) {
      prevGhost[0].y = ghost[0].y;
      ghost[0].y--;
      prevGhost[0].x = ghost[0].x;
    }
    else
      if (index == 2) {
        prevGhost[0].x = ghost[0].x;
        ghost[0].x++;
        prevGhost[0].y = ghost[0].y;
      }
      else {
        prevGhost[3].y = ghost[0].y;
        ghost[0].y++;
        prevGhost[0].x = ghost[0].x;
      }
}

// check if the previous position is different from the current one, in order to make that movement possible
bool checkPrevPosition (int who) {
  if(who == 0) {
    if (ghost[0].x - 1 == prevGhost[0].x && ghost[0].y == prevGhost[0].y) {
      return true;
    }
    return false;
  }

  if(who == 1) {
    if (ghost[0].y - 1 == prevGhost[0].y && ghost[0].x == prevGhost[0].x) {
      return true;
    }
    return false;
  }

  if(who == 2) {
    if (ghost[0].x + 1 == prevGhost[0].x && ghost[0].y == prevGhost[0].y) {
      return true;
    }
    return false;
  }

  if(who == 3) {
    if (ghost[0].y + 1 == prevGhost[0].y && ghost[0].x == prevGhost[0].x) {
      return true;
    }
    return false;
  }
}

// here, check if the ghost can move to a certain position: avoid a wall and the end of the maze
bool possibleMovement(int who) {
  if (who == 0) {
    if (ghost[0].x > 0 && !matrix[ghost[0].x - 1][ghost[0].y] && !checkPrevPosition(0)) {
      return true;
    }
    else {
      return false;
    }
  }
  
  if (who == 1) { 
    if (ghost[0].y > 0 && !matrix[ghost[0].x][ghost[0].y - 1] && !checkPrevPosition(1)) {
      return true;
    }
    else {
      return false;
    }
  }
  
  if (who == 2) {
    if (ghost[0].x < 7 && !matrix[ghost[0].x + 1][ghost[0].y] && !checkPrevPosition(2)) {
      return true;
    }
    else {
      return false;
    }
  }
  
  if (who == 3) {
    if (ghost[0].y < 7 && !matrix[ghost[0].x][ghost[0].y + 1] && !checkPrevPosition(3)) {
      return true;
    }
    else {
      return false;
    }
  }
}

// check if Pacman and the ghost are in the same position
bool samePosition() {
  if (pacman[0].x == ghost[0].x && pacman[0].y == ghost[0].y) {
    return true;
  }
  else {
    return false;
  }
}

// this function sorts the 4 possible distances from the ghost to Pacman, in order to select the minimum one
void sortPositions(struct Distance dist[]) {
  for (int i = 0; i < 4; i++) {
    for (int j = i + 1; j < 4; j++) {
      if (dist[i].distance > dist[j].distance) {
        int auxD = dist[i].distance;
        int auxI = dist[i].index;

        dist[i].distance = dist[j].distance;
        dist[i].index = dist[j].index;

        dist[j].distance = auxD;
        dist[j].index = auxI;
      }
    }
  }
    
}

void moveGhost(int who) {
  while (!samePosition() && nOfFruitsTaken < level + 3) {
    movePacman(0);

    dist[0].distance = abs(ghost[who].x - 1 - pacman[0].x); // south
    dist[1].distance = abs(ghost[who].y - 1 - pacman[0].y); // west
    dist[2].distance = abs(ghost[who].x + 1 - pacman[0].x); // north
    dist[3].distance = abs(ghost[who].y + 1 - pacman[0].y); // east
    for(int i = 0; i < 4; i++) {
      dist[i].index = i;
    }
    
    sortPositions(dist);
    
    for (int k = 0; k < 4; k++) {
      if(possibleMovement(dist[k].index)) {
        changeCoord(dist[k].index);
        break;
      }
    }

   // depending on the level, the ghost moves with a certain speed
   int delayPeriod;
   if (level == 1) {
    delayPeriod = 500;
   }
   else 
    if (level == 2) {
      delayPeriod = 300;
    }
    else {
      delayPeriod = 150;
    } 

   lc.setLed(0, prevGhost[who].x, prevGhost[who].y, false); delay(25);
   lc.setLed(0, ghost[who].x, ghost[who].y, true); delay(delayPeriod);
 
   int startPos, endPos;
   if (level == 1) {
    startPos = 0; endPos = 4;
   }
   else
    if (level == 2) {
      startPos = 4; endPos = 9;
    }
    else {
      startPos = 9; endPos = 15;
    }

   // if the ghost passes through a fruit or the golden fruit, nothing should happen
   for(int i = startPos; i < endPos; i++) {
      if (prevGhost[who].x == fruits[i].x && prevGhost[who].y == fruits[i].y && !takenFruit[i]) {
         lc.setLed(0, prevGhost[who].x, prevGhost[who].y, true);
         break;
      }
   }
   if(!takenGoldenFruit[level - 1]) {
     lc.setLed(0, gFruits[level - 1].x, gFruits[level - 1].y, true); delay(50);
     lc.setLed(0, gFruits[level - 1].x, gFruits[level - 1].y, false);
   }
  }

  // if the ghost ate Pacman, decrease the number of lives
  if (nOfFruitsTaken < level + 3) {
    lives--;
    clearMatrix();
    delay(1000);
  }
  // if Pacman collected all the fruits, move to the next level
  else {
    level++;
    if(level == 2) {
      level2();
    }
    else 
      if (level == 3) {
      level3();
    }
    else {
      score += 3*lives;
      win();
    }
  }
}

// reset the collected fruits status, score and number of lives
void resetGame() {
  for (int i = 0; i < 15; i++) {
    takenFruit[i] = 0;
  }

  takenGoldenFruit[0] = takenGoldenFruit[1] = takenGoldenFruit[2] = 0;
  score = 0;
  lives = 3;
}

void gameOver () {
  lcd.clear(); delay(500);
  clearMatrix();
  button = false;

  eeAddress = 0;
  EEPROM.get(eeAddress, hScore);

  // if the score is higher than the previous one, store it and also the player who achieved it
  if (score > hScore) {
    hScore = score;
    strcpy(hScorePlayer, playerName);
    saveOnEeprom();
    
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("You have the");
    
    lcd.setCursor(0, 1);
    lcd.print("hscore, ");
    
    lcd.setCursor(8, 1);
    lcd.print(hScorePlayer);
    delay(2000);
    
  }
  
  while(exitGameOver == false) {
    // display a sad face on the matrix
    matrix[2][2] = matrix[2][5] = matrix[6][1] = 
    matrix[5][2] = matrix[5][3]= matrix[5][4] = matrix[5][5] = matrix[6][6] =  1;
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
          lc.setLed(0, row, col, matrix[row][col]);
        }
      }
      
    if (millis() - prevMillis >= 5000) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Press button to");
  
      lcd.setCursor(5, 1);
      lcd.print("EXIT");

      prevMillis = millis();
    }

    buttonState();
    // if the player presses the button, he exits the game and goes to the main menu
    if(button == true) {
      option = 0;
      arrow = 0;
      exitGameOver = true;
      exitNext = true;
      exitPlay = true;
    }
  }

  button = false;
  delay(500);
  lcd.clear();
  clearMatrix();
  
}

// this function is called only if the player completes all the levels
void win() {
  lcd.clear(); delay(500);
  clearMatrix();
  button = false;
  
  while(exitWin == false) {
    lcd.setCursor(0, 0);
    lcd.print("Congrats, you ");

    lcd.setCursor(0, 1);
    lcd.print("won! Score ");

    lcd.setCursor(11, 1);
    lcd.print(score);

    // display a smiley face on the matrix
    matrix[2][2] = matrix[2][4] = matrix[4][1] = matrix[5][2] = matrix[5][3] = matrix[4][5] = matrix[5][4] = 1;
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
          lc.setLed(0, row, col, matrix[row][col]);
        }
      }
    delay(2000);
    
    // if the score is higher than the highest score, update the highest score and also the player's name
    eeAddress = 0;
    EEPROM.get(eeAddress, hScore);
    
    if (score > hScore) {
      lcd.clear();
      hScore = score;

      strcpy(hScorePlayer, playerName);

      saveOnEeprom();
      
      lcd.setCursor(0, 0);
      lcd.print("You have the");
   
      lcd.setCursor(0, 1);
      lcd.print("highest score,");

      delay(2000);
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print(playerName);
      
      lcd.setCursor(0, 1);
      lcd.print("Congratulations!");
      delay(2000);
    }

    // after 5 seconds, exit the function and return to the main menu
    delay(5000);
    option = 0;
    arrow = 0;
    exitWin = true;
    exitNext = true;
    exitPlay = true;
    lcd.clear();
  }

  button = false;
  delay(500);
  lcd.clear();
  clearMatrix();
  
}

void updateLevel() {
  lcd.setCursor(6, 0);
  lcd.print(level);
}

void level1() {
  nOfFruitsTaken = 0;
  updateLevel();
  
  while (lives > 0) {
    clearMatrix();
    updateLives();

    // maze
    matrix[1][0] = matrix[1][1] = matrix[1][2] = matrix[1][3] = matrix[3][3] = 
    matrix[4][3] = matrix[5][3] = matrix[6][5] = matrix[6][6] = matrix[6][7] = 1;
    
    for(int row = 0; row < 8; row++) {
      for(int col = 0; col < 8; col++) {
        lc.setLed(0, row, col, matrix[row][col]);
      }
    }
    
    initPacmanPositions();
    initGhostPositions();
    initFruitsPositions(1);
    
    lc.setLed(0, pacman[0].x, pacman[0].y, true);
    lc.setLed(0, ghost[0].x, ghost[0].y, true);
    
    delay(10);
    lc.setLed(0, ghost[0].x, ghost[0].y, false); 

    movePacman(0); 
    moveGhost(0);
  }

  // if the number of lives is 0, the player loses the game
  gameOver();
}

void level2() {
  delay(1000);
  nOfFruitsTaken = 0;
  updateLevel();
  
  while (lives > 0) {
    clearMatrix();
    updateLives();

    // maze
    matrix[0][2] = matrix[1][2] = matrix[2][2] = matrix[3][2] = matrix[3][3] = matrix[3][4] = matrix[4][4] = matrix[3][6] = matrix[3][7] = 
    matrix[6][0] = matrix[6][1] = matrix[6][2] = matrix[7][5] = matrix[7][6] = 1;
    
    for(int row = 0; row < 8; row++) {
      for(int col = 0; col < 8; col++) {
        lc.setLed(0, row, col, matrix[row][col]);
      }
    }
    
    initPacmanPositions();
    initGhostPositions();
    initFruitsPositions(2);
    
    lc.setLed(0, pacman[0].x, pacman[0].y, true);
    lc.setLed(0, ghost[0].x, ghost[0].y, true);
    
    delay(10);
    lc.setLed(0, ghost[0].x, ghost[0].y, false); 

    movePacman(0); 
    moveGhost(0);
  }

  // if the number of lives is 0, the player loses the game
  gameOver(); 
}

void level3() {
  delay(1000);
  nOfFruitsTaken = 0;
  updateLevel();
  
  while (lives > 0) {
    clearMatrix();
    updateLives();

    // maze
    for (int i = 0; i < 8; i++) {
      matrix[0][i] = matrix[i][0] = matrix[7][i] = matrix[i][7] = 1;
    }
    matrix[1][3] = matrix[2][3] = matrix[3][3] = matrix[5][5] = matrix[6][5] = 1;
    
    for(int row = 0; row < 8; row++) {
      for(int col = 0; col < 8; col++) {
        lc.setLed(0, row, col, matrix[row][col]);
      }
    }
    
    initPacmanPositions();
    initGhostPositions();
    initFruitsPositions(3);
    
    lc.setLed(0, pacman[0].x, pacman[0].y, true);
    lc.setLed(0, ghost[0].x, ghost[0].y, true);
    
    delay(10);
    lc.setLed(0, ghost[0].x, ghost[0].y, false); 

    movePacman(0); 
    moveGhost(0);
  }

  // if the number of lives is 0, the player loses the game
  gameOver();   
}

void next() {
  lcd.clear(); delay(500);
  clearMatrix();

  // display a "HI" message
  bool matrix[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 1, 0, 1, 0, 0},
  {0, 1, 0, 1, 0, 1, 0, 0},
  {0, 1, 1, 1, 0, 1, 0, 0},
  {0, 1, 0, 1, 0, 1, 0, 0},
  {0, 1, 0, 1, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}

  };

  for(int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++) {
      lc.setLed(0, col, row, matrix[row][col]);
      delay(25);
    }
  }

  initPacmanPositions();
  initGhostPositions();
  
  while(exitNext == false) {
    lcd.setCursor(0, 0);
    lcd.print("Level");
  
    lcd.setCursor(6, 0);
    lcd.print(level);

    lcd.setCursor(9, 0);
    lcd.print("Lives");

    lcd.setCursor(15, 0);
    lcd.print(lives);

    lcd.setCursor(0, 1);
    lcd.print("Score");

    lcd.setCursor(6, 1);
    lcd.print(score);

    lives = 3;
    if (level == 1) {
      level1();
    }

    if (level == 2) {
      level2();
    }

    if (level == 3) {
      level3();
    }
  }
  
}

// a function to change player's name; the maximum length is 6; the player can choose a shorter name
void changeName() {
  lcd.clear(); delay(500);
  button = false;
  while(exitChangeName == false) {
    lcd.setCursor(5, 0);
    lcd.print(playerName);

    // on the X-axis, choose which letter you want to modify
    xValue = analogRead(pinX);
    if(xValue < minThreshold && joyMoved == false) {
      if(letter > 0) {
        prevLetter = letter;
        letter--;
      }
      else {
        prevLetter = letter;
        letter = 5;
      }
      joyMoved = true;
    }
  
    if (xValue > maxThreshold && joyMoved == false) {
        if (letter < 5) {
         prevLetter = letter;
         letter++;  
        }
        else {
          prevLetter = letter;
          letter = 0; 
         } 
         
        joyMoved = true;
      }
    
    if(xValue >= minThreshold && xValue <= maxThreshold) {
     joyMoved = false;
    } 

    // on the Y-axis, change the letter
    yValue = analogRead(pinY);
    if(yValue < minThreshold && joyMoved == false) {
      if (playerName[letter] != 'A') {
        if (playerName[letter] == ' ') {
          playerName[letter] = 'Z';
        }
        else {
          playerName[letter]--;
        }
      }
      else {
        playerName[letter] = ' ';
      }
      joyMoved = true;
    }

    if (yValue > maxThreshold && joyMoved == false) {
      if (playerName[letter] != 'Z') {
        if (playerName[letter] != ' ') {
          playerName[letter]++;
        }
        else {
          playerName[letter] = 'A';
        }
      }
      else {
        playerName[letter] = ' ';
      }
      joyMoved = true;
    }
  
    if(yValue >= minThreshold && yValue <= maxThreshold) {
     joyMoved = false;
    }

    lcd.setCursor(5, 0);
    lcd.print(playerName);
    
    lcd.setCursor(prevLetter + 5, 1);
    lcd.print(" ");
    
    lcd.setCursor(letter + 5, 1);
    lcd.print("^"); delay(500);
    
    buttonState();
     if (button == true) {
       option = 1;
       arrow = 6;
       exitChangeName = true;
     }
  }
  lcd.clear(); delay(500);
  button = false;
}

void play() {
  arrow = 6;
  lcd.clear(); delay(500);
  button = false;
  while(exitPlay == false) {
    lcd.setCursor(1, 0);
    lcd.print("Name: ");
    
    lcd.setCursor(7, 0);
    lcd.print(playerName);

    lcd.setCursor(3, 1);
    lcd.print("Next");

    lcd.setCursor(10, 1);
    lcd.print("Exit");

    arrowMoved(6, 8);

    buttonState();
    
    if (button == true) {
      if (arrow == 6) {
        exitChangeName = false;
        changeName();
      }
      else
        if(arrow == 7) {
          resetGame();
          next();
        }
      else
        if (arrow == 8) {
          option = 0;
          arrow = 0;
          exitPlay = true;
        }
    }
  }
  lcd.clear(); delay(500);
  button = false;
}

void highScore() {
  lcd.clear(); delay(500);
  button = false;
  while (exitHighScore == false) {
    lcd.setCursor(0, 0);
    lcd.print("Highest score: ");

    eeAddress = 0;
    EEPROM.get(eeAddress, hScore);

    lcd.setCursor(1, 1);
    lcd.print(hScore);

    eeAddress += sizeof(int);
    EEPROM.get(eeAddress, hScorePlayer);

    lcd.setCursor(5, 1);
    lcd.print(hScorePlayer);

    buttonState();
    if(button == true) {
      option = 0;
      arrow = 0;
      exitHighScore = true;
    }
  }

  button = false;
  delay(500);
  lcd.clear();
}

void more() {
  lcd.clear(); delay(500);
  arrow = 3;
  button = false;
  while(exitMore == false) {
    lcd.setCursor(15,0);
    lcd.print("E");
    
    lcd.setCursor(5,0);
    lcd.print("Pacman");
  
    lcd.setCursor(1, 1);
    lcd.print("Settings");
  
    lcd.setCursor(11, 1);
    lcd.print("Info");
  
    arrowMoved(3, 5);

    buttonState();
    if (button == true) {
      if (arrow == 3) {
        option = 4;
        exitMore = true;
      }
      else
        if (arrow == 4) {
          option = 5;
          arrow = 3;
          exitMore = true; 
        }
        else
          if (arrow == 5) {
            option = 0;
            arrow = 0;
            exitMore = true;
          }
    }
  }
  
  button = false;
  delay(500);
  lcd.clear();
}

void settings() {
  lcd.clear(); delay(500);
  button = false;
  while (exitSettings == false) {
    lcd.setCursor(0, 0);
    lcd.print("Choose level: ");
  
    lcd.setCursor(11, 1);
    lcd.print("Exit");
  
    yValue = analogRead(pinY);
    if(yValue < minThreshold && joyMoved == false) {
      if(level > 1) {
        level--;  
      }
      else {
        level = 3;  
      }
      joyMoved = true;
      lcd.setCursor(14, 0);
      lcd.print(level);
      }
         
      if(yValue > maxThreshold && joyMoved == false) {
       if(level < 3) {
         level++;  
       }
       else {
         level = 1;  
       }
       joyMoved = true;
  
       lcd.setCursor(14, 0);
       lcd.print(level);
          
      }
     if(yValue >= minThreshold && yValue <= maxThreshold) {
       joyMoved = false; 
     }

     buttonState();
     if (button == true) {
       option = 3;
       arrow = 3;
       exitSettings = true;
     }
  }
  lcd.clear(); delay(500);
  button = false;
} 
  
void info() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game: Pacman");

  lcd.setCursor(0, 1);
  lcd.print("Creator: Mihaela");
    
  delay(2000);
    
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("GitHub link: ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("https://github.c");

  lcd.setCursor(0, 1);
  lcd.print("om/mihaela-mab");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("@ UnibucRobotics");
  delay(2000);

  option = 3;
}

void setup() {
  // set up the LCD's number of columns and rows
 lcd.begin(16, 2);

 pinMode(pinSw, INPUT_PULLUP);

 // initialize arrow positions
 initArrowPositions();

 lc.shutdown(0, false); //turns off power saving, enables display
 lc.setIntensity(0, brightness); //sets brightness (between 0 and 15)
 lc.clearDisplay(0);
 
 Serial.begin(9600);
}

void loop() {
 exitInfo = false;
 exitMore = false;
 exitHighScore = false;
 exitPlay = false;
 exitSettings = false;
 exitNext = false;
 exitGameOver = false;
 exitWin = false;
 exitChangeName = false;
 
 if (option == 0) {
  mainMenu(); 
 }
 else
  if (option == 1) {
    play();
  }
  else
    if (option == 2) {
      highScore();
    }
    else
      if (option == 3) {
        more();
      }
      else
       if (option == 4) {
        button = true;
        settings();
       }
       else
        if (option == 5) {
          button = true;
          info();
        }
        
}
