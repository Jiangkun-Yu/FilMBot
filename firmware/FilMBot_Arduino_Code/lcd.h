#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

extern int PWM_Coil[4];
extern int coil, FunFlag1, FunFlag2, PIDFlag, control_flag;
extern double Ref_Coil[4];
extern double Ref_I[4],d_I[4];
extern double I_value_1;
extern double PID_Iout[4];
extern int PID_slow_i[4];

void lcd_initial();
void lcd_test();
void lcd_test2();

#endif
