/*
 * Art 385 Project 2
 * Light Focus
 * by Ashley Woon
 * 
 * Components: Adafruit ESP32 feather, breadboard, connecting wires, (1)RGB LED, (1) push button, 
 * (1) potentiometer, and (4) resistors.
 * 
 * Project 2 is an Arduino sketch incorporating hardware and software 
 * to create an interactive focus breathing tool. 
 * 
 * Toggling between the four states can be done through using the push button. 
 * The default state allows the user the change the RGB LED output by twisting the potentiometer. 

 */

#include <analogWrite.h>
//RGB LED pins and variables
const int redLED = 12;
const int greenLED = 27;
const int blueLED = 33;
int red = 0;
int green = 0;
int blue = 0;

//Potentiometer pin and variables
const int POT = A0;
int potVal = 0;
int colorPOT;

//Push button pin and variables
const int button = A1;
int buttonState;
int lastButtonState;
int pushCount = 0;

//State machine
int state = 0;
int stateStressed = 1;
int stateMeh = 2;
int stateCalm = 3;

//Timer variables
unsigned long currentMillis;
unsigned long prevMillis;
int i;
long colorTimer; 

void setup() {
  Serial.begin(9600);
  //initializing inputs
  pinMode(POT, INPUT);
  pinMode(button, INPUT);
  //initializing outputs
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  prevMillis = millis();
}

void loop() {
  currentMillis = millis();
  buttonState = digitalRead(button);
  potVal = analogRead(POT);
  colorPOT = map(potVal, 0, 4095, 0, 255);
  colorPOT = constrain(colorPOT, 0, 255);
  colorTimer = random(0, 5);
  
  if(buttonState != lastButtonState) 
    {
      if(buttonState == HIGH)
      {
        pushCount++;
      }
      if(pushCount == 4)
      {
        pushCount = 0;
      }
    }
  lastButtonState = buttonState;
  
  if(currentMillis >= prevMillis)
  {
    changeStates(pushCount);
  }
    
}

void changeStates(int count) {
  //switching states based on button push
  if (pushCount == 0)
  {
    state = 0;
    colorful();
  }
  else if (pushCount == 1)
  {
    state = stateStressed;
    analogWrite(greenLED, 0);
    analogWrite(blueLED, 0);
    stressed();
  }
  else if (pushCount == 2)
  {
    state = stateMeh;
    analogWrite(blueLED, 0);
    meh();
  }
  else if (pushCount == 3)
  {
   state = stateCalm; 
   analogWrite(greenLED, 0);
   analogWrite(redLED, 0);
   calm();
  }
}

void colorful() {
  i++;
  if(i < colorTimer)
  {
    red+=colorPOT;
    green+=255-colorPOT;
    blue+=colorPOT;
  }
  else if(i < colorTimer+2)
  {
    red++;
    green--;
    blue+=colorPOT;
  }
  else if(i < colorTimer+5)
  {
    red+=255-colorPOT;
    green+=colorPOT;
    blue+=255-colorPOT;
  }
  else if(i >= colorTimer+5)
  {
    i = -1;
    red = 0;
    blue = 0;
    green = 0;
  }
  analogWrite(redLED, red);
  analogWrite(greenLED, green);
  analogWrite(blueLED, blue);
  prevMillis = millis() + 1000;
  
}

void calm() {
  i ++;
  //inhale for 5
  if(i < 5)
  {
    blue++;
    inhale();
  }
  //hold for 7
  else if(i > 4 && i < 11)
  {
    blue = 80;
    hold();
  }
  //exhale for 8
  else if(i > 11 && i < 20)
  {
    blue-=10;
    exhale();
  }
  else if(i >=20)
  {
    i = -1;
    blue = 0;
  }
  analogWrite(blueLED, blue);
  prevMillis = millis() + 1000;
}

void meh() {
  i ++;
  //inhale for 4
  if(i < 4)
  {
    red++;
    green++;
    inhale();
  }
  //hold for 3
  else if(i > 3 && i < 7 )
  {
    red = 50;
    green = 50;
    hold();
  }
  //exhale for 5
  else if(i > 6 && i < 12)
  {
    red-=10;
    green-=10;
    exhale();
  }
  //repeat
  else if(i >= 12)
  {
    i = -1;
    red = 0;
    green = 0;
  }
  analogWrite(redLED, red);
  analogWrite(greenLED, green);
  prevMillis = millis() + 1000;
}

void stressed() {
    i ++;
  //inhale for 3
  if(i < 3)
  {
    red++;
    inhale();
  }
  //hold for 3
  else if(i > 2 && i < 6 )
  {
    red = 30;
    hold();
  }
  //exhale for 4
  else if(i > 5 && i < 10)
  {
    red-=8;
    exhale();
  }
  //repeat
  else if(i >= 10)
  {
    i = -1;
    red = 0;
  }
  analogWrite(redLED, red);
  prevMillis = millis() + 1000;
}

//Serial printing text on the serial monitor to let the use know what action to take
void inhale() {
  Serial.print("Inhale");
  Serial.println();
}
void hold() {
  Serial.print("Hold");
  Serial.println();
}
void exhale() {
  Serial.print("Exhale");
  Serial.println();
}
