#include "key_joy_read.h"
#include "pin_init.h"

int JoyX_value = 0;
int JoyY_value = 0;
int JoyX_value_old = 0;
int JoyY_value_old = 0;
int JoyK_value = 0;
int Key1_value = 0;
int Key2_value = 0;
int Key3_value = 0;
int Key4_value = 0;

int JoyX_value_0 = 0;
int JoyY_value_0 = 0;

void joy_key_init(){
  pinMode(JoyK_Pin, INPUT_PULLUP);
  pinMode(Key1_Pin, INPUT_PULLUP);
  pinMode(Key2_Pin, INPUT_PULLUP);
  pinMode(Key3_Pin, INPUT_PULLUP);
  pinMode(Key4_Pin, INPUT_PULLUP);
  delay(30);
  JoyX_value_0 = analogRead(JoyX_Pin);
  JoyY_value_0 = analogRead(JoyY_Pin);
  
}

void joy_key_read(){
  //Joystick
  JoyX_value = ( analogRead(JoyX_Pin)-JoyX_value_0 ) * 2.5;
  JoyY_value = ( analogRead(JoyY_Pin)-JoyY_value_0 ) * 3;

  //Anti-shake at zero point
  if( (abs(JoyX_value_old - JoyX_value) < 5 )&& (abs(JoyX_value) < 8) ){
    JoyX_value = 0;
  }
  if( (abs(JoyY_value_old - JoyY_value) < 5 )&& (abs(JoyY_value) < 8) ){
    JoyY_value = 0;
  }

  // mode 0, control by Joystick; mode 1, control by Serial data
  if( (abs(JoyY_value_old - JoyY_value) > 20)||(abs(JoyX_value_old - JoyX_value) > 20) ){
    mode = 0;
  }

  JoyX_value_old = JoyX_value;
  JoyY_value_old = JoyY_value;
  
  //Keys
  JoyK_value = digitalRead(JoyK_Pin);
  Key1_value = digitalRead(Key1_Pin);
  Key2_value = digitalRead(Key2_Pin);
  Key3_value = digitalRead(Key3_Pin);
  Key4_value = digitalRead(Key4_Pin);

  
 
}
