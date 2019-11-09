const int potRedPin = A0;
const int potGreenPin = A1;
const int potBluePin = A2;

const int ledRedPin = 11;
const int ledGreenPin = 10;
const int ledBluePin = 9;

int redLedValue = 0;
int blueLedValue = 0;
int greenLedValue = 0;

int potRedValue = 0;
int potGreenValue = 0;
int potBlueValue = 0;

void setup() {
  pinMode(potRedPin, INPUT);
  pinMode(potGreenPin, INPUT);
  pinMode(potBluePin, INPUT);

  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  potRedValue = analogRead(potRedPin);
  potGreenValue = analogRead(potGreenPin);
  potBlueValue = analogRead(potBluePin);
  // we can also use redLedValue = map(redPotValue, 0, 1023, 0, 255);
  redLedValue = potRedValue / 4;
  greenLedValue = potGreenValue / 4;
  blueLedValue = potBlueValue / 4;
  setColor(redLedValue, greenLedValue, blueLedValue);
}

void setColor(int red, int green, int blue){
  analogWrite(ledRedPin, red);
  analogWrite(ledGreenPin, green);
  analogWrite(ledBluePin, blue);
}
