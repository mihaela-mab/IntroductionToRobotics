analogRGBcontrol

This project implies controlling an RGB led using:
a. 1 x RGB LED (it consists of 3 separate LEDs - red, green and blue - in a single case; it has 4 leads, one for each color and a common cathode one) <br/>
b. 3 potentiometers  <br/>
c. 3 resistors (330 ohm)  <br/>
d. wires  <br/>
e. Arduino  <br/>
f. Connector cable  <br/>
I used the pins number 11, 10 and 9 (in the setup function, they are defined as output), respectively A2, A1, A0 (in the setup function, they are defined as input). In the loop I repeatedly read the pot values and map them. After calling the setColor function, the led values are given as output.

knockDetectorBuzzer

This project required the following hardware components: <br/>
a. 1 passive buzzer for knock detection <br/>
b. 1 active buzzer for sound <br/>
c. 1 pushbutton <br/>
d. 2 resistors (100ohm), one for each buzzer <br/>
e. Wires <br/>
f. Arduino <br/>
g. Connector cable <br/>
The passive buzzer detects a knock and, after 5 seconds (time is measured with millis() function), the actvive one emits a sound with tone  of 2500. The sound stops after pushing the button.
