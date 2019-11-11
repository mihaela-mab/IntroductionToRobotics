const int speakerPin = A0;

int speakerValue = 0;

//sensitivity
const int threshold = 1;

unsigned long previousMillis = 0;
const long interval = 5000;

const int buzzerPin = 11;
int buzzerTone = 2500;
int buzzerValue = 0;

const int pushButton = 2;
int pushButtonState = 0;

int knockValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(speakerPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  
}

void loop() {
  unsigned long currentMillis = millis();
  speakerValue = analogRead(speakerPin);
  
  if (speakerValue != 0)
      Serial.println(speakerValue);
      
  if (speakerValue > threshold) {
      Serial.println("Knock");
      knockValue = 1;
  }
     
  if ((currentMillis - previousMillis >= interval) && knockValue == 1) {
          previousMillis = currentMillis;
          tone (buzzerPin, buzzerTone);
  }
  pushButtonState = digitalRead(pushButton);
  
  if (!pushButtonState) {
          noTone(buzzerPin);
          knockValue = 0;
  }
  
}
