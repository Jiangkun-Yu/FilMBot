#ifndef SERIAL_H
#define SERIAL_H

#include <Arduino.h>

extern int PWM_Coil[4];
extern int mode, ErrorFlag;
extern int control_flag;

extern double Ref_Coil[4];
extern double Ref_Coil_old[4];

void uart_receive();

#endif
