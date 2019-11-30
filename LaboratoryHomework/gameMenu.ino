#include <LiquidCrystal.h>

const int rsPin = 12;
const int ePin = 11;
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

int arrow  = 0;
struct arrowPosition {
  int x;
  int y;
} pos[3];

int level = 1;
bool settingsExit = false;
bool exitPlay = false;
bool exitScore = false;
bool endGameExit = false;

int score = 0;
int maxScore = 0;

const int endGameTime = 10000;
const int levelUpTime = 5000;
unsigned long prevLevelMillis = 0;
unsigned long prevGameMillis = 0;
unsigned long currentMillis = 0;
unsigned long startTime = 0;

LiquidCrystal lcd(rsPin, ePin, d4Pin, d5Pin, d6Pin, d7Pin);

void initArrowPositions() {
  pos[0].x = 10;
  pos[0].y = 0;
  
  pos[1].x = 0;
  pos[1].y = 1;
  
  pos[2].x = 10;
  pos[2].y = 1;
}

void showArrow(int arr) {
  for(int i = 0; i < 3; i++) {
    lcd.setCursor(pos[i].x, pos[i].y);
    lcd.print(" ");
  }
   lcd.setCursor(pos[arr].x, pos[arr].y);
   lcd.print(">");
}

void settings() {
  lcd.clear();
  delay(500);
  
  while(settingsExit == false){
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
        
  swState = digitalRead(pinSw);
  if(swState == LOW && button == false) {
     button = true;
     settingsExit = true;
  }
  else 
    if (swState == LOW) {
      button = false;
    }
  }
  
  arrow = 0;
  button = false;
  delay(500);
}

void endGame() {
  lcd.clear();
  delay(500);
  
  while(endGameExit == false) {
    lcd.setCursor(0, 0);
    lcd.print("Congratulations,");
    
    lcd.setCursor(0, 1);
    lcd.print("you won!    Exit");
    
    swState = digitalRead(pinSw);
    if(swState == LOW && button == false) {
       button = true;
       endGameExit = true;
       exitPlay = true;
    }
    else 
      if (swState == LOW) {
        button = false;
      }
  }
  
  arrow = 0;
  button = false;
  delay(500);
}

void play() {
  
  lcd.clear();
  delay(500);
  
  startTime = millis();
  Serial.println(startTime);

  while(exitPlay == false) {
    lcd.setCursor(5, 0);
    lcd.print("Lives: 3");
    
    lcd.setCursor(0, 1);
    lcd.print("Level: ");
  
    lcd.setCursor(6, 1);
    lcd.print(level);
  
    lcd.setCursor(9, 1);
    lcd.print("Score: ");

    score = level*3;
    lcd.setCursor(15, 1);
    lcd.print(score);
    
    swState = digitalRead(pinSw);
    if(swState == LOW && button == false) {
       button = true;
       exitPlay = true;
    }
    else 
      if (swState == LOW) {
        button = false;
      }

    currentMillis = millis() - startTime;
    lcd.setCursor(14, 0);
    lcd.print(currentMillis/1000);
    
    if((currentMillis - prevLevelMillis >= levelUpTime)) {
      prevLevelMillis = currentMillis;
      if (level < 3) {
        level++;
      }
      delay(50);
    }
    
    if((currentMillis - prevGameMillis >= endGameTime) && level == 3) {
      prevGameMillis = currentMillis;
      lcd.setCursor(6, 1);
      lcd.print(level);

      lcd.setCursor(15, 1); // if these lines were missing, the last level wouldn't be desplayed
      score = level*3;
      lcd.print(score);
      
      delay(500);
      endGame();
    } 
  }
    
   arrow = 0;
   button = false;
   delay(500);
}

void highScore() {
  lcd.clear();
  delay(500);
  
  while(exitScore == false) {
    lcd.setCursor(0, 0);
    lcd.print("Highest score: ");

    maxScore = (score > maxScore) ? score : maxScore;
    lcd.setCursor(2, 1);
    lcd.print(maxScore);

    lcd.setCursor(10, 1);
    lcd.print("Exit");

    swState = digitalRead(pinSw);
    if(swState == LOW && button == false) {
       button = true;
       exitScore = true;
       
    }
    else 
      if (swState == LOW) {
        button = false;
      }
    }
    
    arrow = 0;
    button = false;
    delay(500);
}

void setup() {
  // set up the LCD's number of columns and rows
 lcd.begin(16, 2);

 pinMode(pinSw, INPUT_PULLUP);

 // initialize arrow positions
 initArrowPositions();

 Serial.begin(9600);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Main Menu ");

  lcd.setCursor(11, 0);
  lcd.print("Play ");

  lcd.setCursor(1, 1);
  lcd.print("HScore   ");

  lcd.setCursor(11, 1);
  lcd.print("Setts");
  
  xValue = analogRead(pinX);
  if(xValue < minThreshold && joyMoved == false) {
    if(arrow > 0) {
      arrow--;
    }
    else {
      arrow = 2;
    }
    joyMoved = true;
  }

  if (xValue > maxThreshold && joyMoved == false) {
      if (arrow < 2) {
       arrow++;  
      }
      else {
        arrow = 0; 
       } 
       
      joyMoved = true;
    }
  
     if(xValue >= minThreshold && xValue <= maxThreshold) {
      joyMoved = false;
     }

  showArrow(arrow);

  swState = digitalRead(pinSw);
  if(swState == LOW && button ==false)
     button = true;
  else 
    if (swState == LOW) {
      button = false;
    }
  
  settingsExit = false;
  exitPlay = false;
  exitScore = false;
  endGameExit = false;
  
  if(button == true) {
    if (arrow == 0) {
      play();
    }
    else
      if (arrow == 1) {
        highScore();
      }
      else
        if(arrow == 2) {
          settings();
        }
        
  }
  
}
