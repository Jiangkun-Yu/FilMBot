#ifndef SENSOR_READ_H
#define SENSOR_READ_H

#include <Arduino.h>

extern int I_AD_1, I_AD_2, I_AD_3, I_AD_4;
extern double I_value_1 , I_value_2, I_value_3, I_value_4, I_offset_1;
extern double Tem_value_1,Tem_value_2,Tem_value_3,Tem_value_4;

void sensor_init();
void sensor_read();

int I1_Ave( int I1_AD);
int I2_Ave( int I2_AD);
int I3_Ave( int I3_AD);
int I4_Ave( int I4_AD);

#endif
