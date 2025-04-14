#ifndef KEY_JOY_READ_H
#define KEY_JOY_READ_H

#include <Arduino.h>

extern int JoyX_value, JoyY_value, JoyK_value, Key1_value, Key2_value, Key3_value, Key4_value;
extern int JoyX_value_old, JoyY_value_old;
extern int mode;


void joy_key_init();
void joy_key_read();

#endif
