#include "sensor_read.h"
#include "pin_init.h"

int Tem_AD_1 = 0,Tem_AD_2 = 0,Tem_AD_3 = 0,Tem_AD_4 = 0;
double Tem_value_1 = 0,Tem_value_2 = 0,Tem_value_3 = 0,Tem_value_4 = 0;
double Tem_offset_1 = 0,Tem_offset_2 = 0,Tem_offset_3 = 0,Tem_offset_4 = 0;

//Current related varibles
int I_AD_1 = 0, I_AD_2 = 0, I_AD_3 = 0, I_AD_4 = 0;
double I_value_1 = 0, I_offset_1 = 0.0, I_amp_1 = 1;
double I_value_11 = 0;
double I_value_2 = 0, I_offset_2 = 0.0, I_amp_2 = 1;
double I_value_3 = 0, I_offset_3 = 0.0, I_amp_3 = 1;
double I_value_4 = 0, I_offset_4 = 0.0, I_amp_4 = 1;

int FilBuf_I1[8]  ={0}, FilBuf_I2[8]  ={0}, FilBuf_I3[8]  ={0}, FilBuf_I4[8]  ={0};
double Fil_I1 = 0, Fil_I2 = 0, Fil_I3 = 0, Fil_I4 = 0;


void sensor_init(){
  OCR1A = 0;
  OCR2A = 0;
  OCR3A = 0;
  OCR4A = 0;
  delay(300);
  
  for(int i = 0; i < 10; i++){
    delay(20);
    I_AD_1 = analogRead(I_Pin_1);
    I_AD_2 = analogRead(I_Pin_2);
    I_AD_3 = analogRead(I_Pin_3);
    I_AD_4 = analogRead(I_Pin_4);

    I_offset_1 += ( -I_AD_1 * 0.001691358901341 + 0.910755504848910 )* I_amp_1;
    I_offset_2 += ( -I_AD_2 * 0.001701865599424 + 0.912337950070264 )* I_amp_2;
    I_offset_3 += ( I_AD_3 * 0.001694428848601 - 0.910198662019227 )* I_amp_3;
    I_offset_4 += ( I_AD_4 * 0.001688722587740 - 0.911320472952832 )* I_amp_4;

  }
  
  I_offset_1 = I_offset_1 / 10;
  I_offset_2 = I_offset_2 / 10;
  I_offset_3 = I_offset_3 / 10;
  I_offset_4 = I_offset_4 / 10;

}

void sensor_read(){
   //Temperature
   Tem_AD_1 = analogRead(Tem_Pin_1);
   Tem_AD_2 = analogRead(Tem_Pin_2);
   Tem_AD_3 = analogRead(Tem_Pin_3);
   Tem_AD_4 = analogRead(Tem_Pin_4);
   
   //the coeffecient are from current calibrition
   Tem_value_1 = -1.1863 + 0.11366 * Tem_AD_1;
   Tem_value_2 = -1.1863 + 0.11366 * Tem_AD_2;
   Tem_value_3 = -1.1863 + 0.11366 * Tem_AD_3;
   Tem_value_4 = -1.1863 + 0.11366 * Tem_AD_4;

  //Current
  I_AD_1 = analogRead(I_Pin_1);
  I_AD_2 = analogRead(I_Pin_2);
  I_AD_3 = analogRead(I_Pin_3);
  I_AD_4 = analogRead(I_Pin_4);

  // the coeffecient are from current calibrition
  I_value_1 = ( -I1_Ave(I_AD_1) * 0.001691358901341 + 0.910755504848910 )* I_amp_1 - I_offset_1;
  I_value_2 = ( -I2_Ave(I_AD_2) * 0.001701865599424 + 0.912337950070264 )* I_amp_2 - I_offset_2;
  I_value_3 = ( I3_Ave(I_AD_3) * 0.001694428848601 - 0.910198662019227 )* I_amp_3 - I_offset_3;
  I_value_4 = ( I4_Ave(I_AD_4) * 0.001688722587740 - 0.911320472952832 )* I_amp_4 - I_offset_4;
  
}


int I1_Ave( int I1_AD){

  FilBuf_I1[0] = I1_AD;

  Fil_I1 = (FilBuf_I1[0]+FilBuf_I1[1]+FilBuf_I1[2]+FilBuf_I1[3]+FilBuf_I1[4]+FilBuf_I1[5]+FilBuf_I1[6]+FilBuf_I1[7])*0.125; //) * 0.2; //
//  FilBuf_I1[15] = FilBuf_I1[14];
//  FilBuf_I1[14] = FilBuf_I1[13];
//  FilBuf_I1[13] = FilBuf_I1[12];
//  FilBuf_I1[12] = FilBuf_I1[11];
//  FilBuf_I1[11] = FilBuf_I1[10];
//  FilBuf_I1[10] = FilBuf_I1[9];
//  FilBuf_I1[9] = FilBuf_I1[8];
//  FilBuf_I1[8] = FilBuf_I1[7];
  FilBuf_I1[7] = FilBuf_I1[6];
  FilBuf_I1[6] = FilBuf_I1[5];
  FilBuf_I1[5] = FilBuf_I1[4];
  FilBuf_I1[4] = FilBuf_I1[3];
  FilBuf_I1[3] = FilBuf_I1[2];
  FilBuf_I1[2] = FilBuf_I1[1];
  FilBuf_I1[1] = FilBuf_I1[0];

  return Fil_I1 ;
  
}


int I2_Ave( int I2_AD){
  
  FilBuf_I2[0] = I2_AD;

  Fil_I2 = (FilBuf_I2[0]+FilBuf_I2[1]+FilBuf_I2[2]+FilBuf_I2[3]+FilBuf_I2[4]+FilBuf_I2[5]+FilBuf_I2[6]+FilBuf_I2[7])*0.125;
//  FilBuf_I2[15] = FilBuf_I2[14];
//  FilBuf_I2[14] = FilBuf_I2[13];
//  FilBuf_I2[13] = FilBuf_I2[12];
//  FilBuf_I2[12] = FilBuf_I2[11];
//  FilBuf_I2[11] = FilBuf_I2[10];
//  FilBuf_I2[10] = FilBuf_I2[9];
//  FilBuf_I2[9] = FilBuf_I2[8];
//  FilBuf_I2[8] = FilBuf_I2[7];
  FilBuf_I2[7] = FilBuf_I2[6];
  FilBuf_I2[6] = FilBuf_I2[5];
  FilBuf_I2[5] = FilBuf_I2[4];
  FilBuf_I2[4] = FilBuf_I2[3];
  FilBuf_I2[3] = FilBuf_I2[2];
  FilBuf_I2[2] = FilBuf_I2[1];
  FilBuf_I2[1] = FilBuf_I2[0];

  return Fil_I2 ;
  
}

int I3_Ave( int I3_AD){
  
  FilBuf_I3[0] = I3_AD;

  Fil_I3 = (FilBuf_I3[0]+FilBuf_I3[1]+FilBuf_I3[2]+FilBuf_I3[3]+FilBuf_I3[4]+FilBuf_I3[5]+FilBuf_I3[6]+FilBuf_I3[7])*0.125;
//  FilBuf_I3[15] = FilBuf_I3[14];
//  FilBuf_I3[14] = FilBuf_I3[13];
//  FilBuf_I3[13] = FilBuf_I3[12];
//  FilBuf_I3[12] = FilBuf_I3[11];
//  FilBuf_I3[11] = FilBuf_I3[10];
//  FilBuf_I3[10] = FilBuf_I3[9];
//  FilBuf_I3[9] = FilBuf_I3[8];
//  FilBuf_I3[8] = FilBuf_I3[7];
  FilBuf_I3[7] = FilBuf_I3[6];
  FilBuf_I3[6] = FilBuf_I3[5];
  FilBuf_I3[5] = FilBuf_I3[4];
  FilBuf_I3[4] = FilBuf_I3[3];
  FilBuf_I3[3] = FilBuf_I3[2];
  FilBuf_I3[2] = FilBuf_I3[1];
  FilBuf_I3[1] = FilBuf_I3[0];

  return Fil_I3 ;

  
}

int I4_Ave( int I4_AD){
  
  FilBuf_I4[0] = I4_AD;

  Fil_I4 = (FilBuf_I4[0]+FilBuf_I4[1]+FilBuf_I4[2]+FilBuf_I4[3]+FilBuf_I4[4]+FilBuf_I4[5]+FilBuf_I4[6]+FilBuf_I4[7])*0.125;
//  FilBuf_I4[15] = FilBuf_I4[14];
//  FilBuf_I4[14] = FilBuf_I4[13];
//  FilBuf_I4[13] = FilBuf_I4[12];
//  FilBuf_I4[12] = FilBuf_I4[11];
//  FilBuf_I4[11] = FilBuf_I4[10];
//  FilBuf_I4[10] = FilBuf_I4[9];
//  FilBuf_I4[9] = FilBuf_I4[8];
//  FilBuf_I4[8] = FilBuf_I4[7];
  FilBuf_I4[7] = FilBuf_I4[6];
  FilBuf_I4[6] = FilBuf_I4[5];
  FilBuf_I4[5] = FilBuf_I4[4];
  FilBuf_I4[4] = FilBuf_I4[3];
  FilBuf_I4[3] = FilBuf_I4[2];
  FilBuf_I4[2] = FilBuf_I4[1];
  FilBuf_I4[1] = FilBuf_I4[0];

  return Fil_I4 ;
  
}
