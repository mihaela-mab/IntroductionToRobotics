const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int pinSW = 1;
const int pinX = A0;
const int pinY = A1;
int yValue = 0;
int xValue = 0;

const int segSize = 8;
const int nOfDigits = 10;
const int nOfDisplays = 4;
int swState = LOW;

bool joyMoved = false;
int digit = 0;
int number = 0;
bool button = false;
int minThreshold= 400;
int maxThreshold= 600;

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
// digits array, to switch between them easily
int digits[nOfDisplays] = {
  pinD1, pinD2, pinD3, pinD4
 };   
 
byte digitMatrix[nOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

int lastNumber[nOfDisplays] = {0, 0, 0, 0};

void displayDp(byte decimalPoint) {
  //Write decimal point to DP pin
  digitalWrite(segments[segSize - 1], decimalPoint);  
}

void displayNumber(byte number) {
  for(int i = 0; i < segSize - 1; i ++) {
      digitalWrite(segments[i], digitMatrix[number][i]); 
   }  
}

//Activate the display number received as parameter
void showDigit(int num) {
  for(int i = 0; i < nOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);
    }  
   digitalWrite(digits[num], LOW);
}

void clearDigitSegment() {
  int i;
  for(i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], LOW);
  }
}

void setup() {
  for(int i = 0; i < segSize; i++) {
     pinMode(segments[i], OUTPUT); 
    }
    
   pinMode(pinSW, INPUT_PULLUP);
   
   for(int i = 0; i < nOfDisplays; i++) {
      pinMode(digits[i], OUTPUT);
    }
    
  Serial.begin(9600);
}

void loop() {
  
  swState = digitalRead(pinSW);
  if (swState == LOW && button == false) {
    button = true;
    showDigit(digit);
    displayDp(LOW);
  }
  else if (swState == LOW){
    button = false;
    showDigit(digit);
    clearDigitSegment();
    displayDp(HIGH);
  }
  if (button == false) {
    xValue = analogRead(pinX);
    if(xValue < minThreshold && joyMoved == false){
      if(digit > 0) {
        digit--;
      }
      else {
        digit = 3;
      }
    
      joyMoved = true;
    }
  
    if (xValue > maxThreshold && joyMoved == false) {
      if (digit < 3) {
        digit++;  
      }
      else {
        digit = 0; 
       } 
       
      joyMoved = true;
    }
  
     if(xValue >= minThreshold && xValue <= maxThreshold) {
      joyMoved = false;
     }
     showDigit(digit);
     displayDp(HIGH);
     number = lastNumber[digit];
  }
  else {
      yValue = analogRead(pinY);
      if(yValue < minThreshold && joyMoved == false) {
        if(number > 0) {
          number--;  
        }
        else {
          number = 9;  
        }
        joyMoved = true;
       }
       
       if(yValue > maxThreshold && joyMoved == false) {
        if(number < 9) {
          number++;  
        }
        else {
          number = 0;  
        }
        joyMoved = true;
        
        
       }
      if(yValue >= minThreshold && yValue <= maxThreshold) {
        joyMoved = false;   
     
      }
      lastNumber[digit] = number;
      showDigit(digit);
      displayNumber(number);
  }
 
}
