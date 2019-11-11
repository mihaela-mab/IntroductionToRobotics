analogRGBcontrol

This project implies controlling an RGB led using:
a. 1 x RGB LED (it consists of 3 separate LEDs - red, green and blue - in a single case; it has 4 leads, one for each color and a common cathode one)
b. 3 potentiometers 
c. 3 resistors (330 ohm) 
d. wires 
e. Arduino 
f. Connector cable 
I used the pins number 11, 10 and 9 (in the setup function, they are defined as output), respectively A2, A1, A0 (in the setup function, they are defined as input). In the loop I repeatedly read the pot values and map them. After calling the setColor function, the led values are given as output.
