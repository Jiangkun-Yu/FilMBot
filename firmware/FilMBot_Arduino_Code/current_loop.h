#ifndef CURRENT_LOOP_H
#define CURRENT_LOOP_H

#include <Arduino.h>

extern double Ref_I[4], PID_Iout[4];

void current_loop_init();
double current_pid_setRef(double ref1, double ref2, double ref3, double ref4);
void current_pid_start();
void current_pid_compute();



#endif
