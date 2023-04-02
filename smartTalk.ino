#include "Keyboard.h"

const char CTRL = KEY_LEFT_CTRL;
const char SHIFT = KEY_LEFT_SHIFT;
const char ALT= KEY_LEFT_ALT;
const char WIN = KEY_LEFT_GUI;
const int sensorPin=A2;

#define B1 4
#define B2 3
#define B3 2
#define TP 10

bool pressingB1=false;
bool pressingB2=false;
bool pressingB3=false;

bool autoOn=true;

bool standing=false;
bool internalMute=true;

int forceLevel = 0;

int counter=0;

void setup() {
  // put your setup code here, to run once:
  Keyboard.begin();
  Serial.begin(9600);

  pinMode(B1,INPUT_PULLUP);
  pinMode(B2,INPUT_PULLUP);
  pinMode(B3,INPUT_PULLUP);
  delay(1000);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  forceLevel = analogRead(sensorPin);
  delay(50);

  //detects force on sensor and mutes microphone and video accordingly
  if(autoOn)
  {
    if(forceLevel==0&&!standing)
    {
      Keyboard.press(WIN);
      Keyboard.press(SHIFT);
      Keyboard.press('q');
      delay(50);
      Keyboard.releaseAll();
      standing=true;
      internalMute=!internalMute;
      muteNoise();
    }
    else if(forceLevel>20&&standing)
    {
      Keyboard.press(WIN);
      Keyboard.press(SHIFT);
      Keyboard.press('q');
      delay(50);
      Keyboard.releaseAll();
      standing=false;
      internalMute=!internalMute;
      muteNoise();
    }
  }

//push to talk for audio and video tone at start and end of press
  int b1signal=digitalRead(B1);

  if(b1signal==LOW&&!pressingB1)
  {
    Keyboard.press(WIN);
    Keyboard.press(SHIFT);
    Keyboard.press('q');
    delay(50);
    Keyboard.releaseAll();
    pressingB1=true;
    internalMute=!internalMute;
    tone(TP,400,300);
  }
  else if(b1signal==HIGH&&pressingB1)
  {
    Keyboard.press(WIN);
    Keyboard.press(SHIFT);
    Keyboard.press('q');
    delay(50);
    Keyboard.releaseAll();
    pressingB1=false;
    internalMute=!internalMute;
    tone(TP,500,300);

  }

//toggles mute and video and plays tone to convey action and result
  int b2signal = digitalRead(B2);

  if(b2signal==LOW&&!pressingB2)
  {
    Keyboard.press(WIN);
    Keyboard.press(SHIFT);
    Keyboard.press('q');
    delay(50);
    Keyboard.releaseAll();
    pressingB2=true;
    internalMute=!internalMute;
    tone(TP,600,100);
    delay(125);
    tone(TP,600,100);
    delay(125);
    tone(TP,600,100);
    delay(125);
    muteNoise();
  }
  else if(b2signal==HIGH)
  {
    pressingB2=false;
  }

  int b3signal = digitalRead(B3);

//disables force sensor automatic muting and plays tone to convey on or off
  if(b3signal==LOW&&!pressingB3)
  {
    tone(TP,700,200);
    autoOn=!autoOn;
    pressingB3=true;
    delay(200);
    if(autoOn)
    {
      tone(TP,900,300);
    }
    else
    {
      tone(TP,500,300);
    }

  }
  else if(b3signal==HIGH)
  {
    pressingB3=false;
  }

  if(b3signal==LOW&&b2signal==LOW&&b1signal==LOW)
  {

    //this code plays a song for easter egg purposes if all buttons are held for 5ish seconds
    counter++;
    if(counter>100)
    {
      counter=0;
      //playSong();
    }
  }
  else
  {
    counter=0;
  }

}

void muteNoise()
{
  if(internalMute)
  {
    tone(TP,800,300);
  }
  else
  {
    tone(TP,300,300);
  }
}
