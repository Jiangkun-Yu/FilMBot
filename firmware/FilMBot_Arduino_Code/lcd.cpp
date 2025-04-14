#include "lcd.h"
#include <LiquidCrystal.h>
#include "sensor_read.h"

// LCD pins
const int rs = 32, en = 34, d4 = 36, d5 = 38, d6 = 40, d7 = 42;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int lcd_i = 0;
int lcd_x3 = 0;

void lcd_initial(){
  lcd.begin(16, 2);
  lcd.setCursor(4,0);
//  lcd.print("Welcome!");
}


void lcd_test(){
  lcd_i ++;
  switch(lcd_i)
  {               // 2 space every time
    case 1: 
      // if(mode == 0){lcd.setCursor(0,0);lcd.print("J");lcd.print(coil+1);}
      // else if(mode == 1){lcd.setCursor(0,0);lcd.print("S ");}
      break;
    case 2: lcd.print(FunFlag1);lcd.setCursor(3,0);break;
    case 3: lcd.print("1");break;
    case 4: lcd.setCursor(5,0);lcd.print("  ");break;
    case 5: lcd.print("   ");break;
    case 6: lcd.setCursor(5,0);lcd.print(PWM_Coil[0]);break;
    case 7: lcd.setCursor(10,0);lcd.print("2");break;
    case 8: lcd.setCursor(12,0);lcd.print("  ");break;
    case 9: lcd.print("   ");break;
    case 10: lcd.setCursor(12,0);lcd.print(PWM_Coil[1]);break;
    case 11: lcd.setCursor(0,1);lcd.print(FunFlag1);lcd.print(FunFlag2);break;
    case 12: lcd.print(PIDFlag);lcd.setCursor(3,1);break;
    case 13: lcd.print("3");lcd.setCursor(5,1);break;
    case 14: lcd.print("  ");break;
    case 15: lcd.print("   ");break;
    // case 16: lcd.setCursor(5,1);lcd.print(PWM_Coil[2]);break;
    case 16: lcd.setCursor(5,1);lcd.print(int(I_value_1*100));//,fun5_value*100  fun5_apply_flag  Ref_Coil[0]*10 ran_flag  -Ref_I[0]*10  d_I[0]*1000
    case 17: lcd.setCursor(10,1);lcd.print("4");break;
    case 18: lcd.setCursor(12,1);lcd.print("  ");break;
    case 19: lcd.print("   ");break;
    case 20: lcd.setCursor(12,1);lcd.print(PWM_Coil[3]); lcd_i = 0; break;
    
  }
  
}


void lcd_test2(){
  lcd_i ++;
  switch(lcd_i)
  {               // 1 space every time for saving more time
    case 1: lcd.setCursor(0,0); break;
    case 2: lcd.print(control_flag); break;
    case 3: lcd.print(" ");break;    // empty, print a flag if you want
    case 4: lcd.print(" ");break;
    // print PWM 1, bit by bit
    case 5: lcd.setCursor(3,0); break;
    case 6: lcd.print("1"); break;
    case 7: 
      if (PWM_Coil[0] < 0) {lcd.print("-");}
      else {lcd.print("+");} break;
    case 8:
      if(int(PWM_Coil[0]/1000)!=0) {lcd.print(int(abs(PWM_Coil[0])/1000));} break;
    case 9: lcd.print(int((abs(PWM_Coil[0])%1000)/100));break;
    case 10: lcd.print(int((abs(PWM_Coil[0])%100)/10));break;
    case 11: lcd.print(int((abs(PWM_Coil[0])%10)));break;
    case 12:
      if(int(PWM_Coil[0]/1000)==0) {lcd.print(" ");} break;
    // print PWM 2, bit by bit
    case 13: lcd.setCursor(10,0);break;
    case 14: lcd.print("2");break;
    case 15: 
      if (PWM_Coil[1] < 0) {lcd.print("-");}
      else {lcd.print("+");} break;
    case 16:
      if(int(PWM_Coil[1]/1000)!=0) {lcd.print(int(abs(PWM_Coil[1])/1000));} break;
    case 17: lcd.print(int((abs(PWM_Coil[1])%1000)/100));break;
    case 18: lcd.print(int((abs(PWM_Coil[1])%100)/10));break;
    case 19: lcd.print(int((abs(PWM_Coil[1])%10)));break;
    case 20:
      if(int(PWM_Coil[1]/1000)==0) {lcd.print(" ");} break;
    case 21: lcd.setCursor(0,1); break;
    case 22: 
      if((FunFlag2 == 0) && (control_flag == 2)) { lcd.print(coil+1); }
      else{ lcd.print(" "); }
      break;
    case 23: if((FunFlag2 == 0) && (control_flag == 2)) { lcd.print(coil+3); }
      else{ lcd.print(" "); }
      break;
    case 24: lcd.print(" "); break;
    case 25: lcd.setCursor(3,1);break;
    // print PWM 3, bit by bit
    case 26: lcd.print("3");break;
    case 27: 
      lcd_x3 = PWM_Coil[2];//Ref_I[0] * 10000;// Ref_I[0] fun5_value
      if (lcd_x3 < 0) {lcd.print("-");}
      else {lcd.print("+");} break;
    case 28:
      if(int(lcd_x3/1000)!=0){lcd.print(int(abs(lcd_x3)/1000));} break;
    case 29: lcd.print(int((abs(lcd_x3)%1000)/100));break;
    case 30: lcd.print(int((abs(lcd_x3)%100)/10));break;
    case 31: lcd.print(int((abs(lcd_x3)%10)));break;
    case 32:
      if(int(PWM_Coil[2]/1000)==0){lcd.print(" ");} break;
    case 33: lcd.setCursor(10,1); break;
    // print PWM 4, bit by bit
    case 34: lcd.print("4");break;
    case 35: 
      if (PWM_Coil[3] < 0) {lcd.print("-");}
      else {lcd.print("+");} break;
    case 36:
      if(int(PWM_Coil[3]/1000)!=0){lcd.print(int(abs(PWM_Coil[3])/1000));} break;
    case 37: lcd.print(int((abs(PWM_Coil[3])%1000)/100));break;
    case 38: lcd.print(int((abs(PWM_Coil[3])%100)/10));break;
    case 39: lcd.print(int((abs(PWM_Coil[3])%10)));break;
    case 40:
      if(int(PWM_Coil[3]/1000)==0){lcd.print(" ");} break;
    case 41: lcd_i = 0; break;
    
  }
  
}
