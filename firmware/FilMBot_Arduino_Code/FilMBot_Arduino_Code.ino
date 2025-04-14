//////*******************FilMBot Arduino Code*****************//////
// 
// 2025.04.13
// 
// Hardware:
//    Arduino Mega 2560, with 16 MHz main clock
// 
// Liabray:
//    At least the following libraries should be included before runing:
//      1. SparkFun_TB6612, included in FilMBot.ino (main)
//      2. PID, by Brett Beauregard, included in current loop.cpp
//
// Control Mode:
//         Use control_flag to set the control mode, either use the Key or the UART command to set it.
//             control_flag == 0, means using UART_PWM_control_mode, UART giving PWM reference
//             control_flag == 1, means using UART_PID_control_mode, current loop is closed and UART giving current reference
//             control_flag == 2, means using Joystick_control_mode, Joystick to give PWM reference
//             control_flag == 3..., means specific funtion, you can define it as you want, e.g., here using control_flag == 3 to shutdown the PWM output as 0
// 
//         In UART_control_mode:
//           by sending "Acoil1_ref,coil2_ref,coil3_ref,coil4_ref,control_flag,B" and 'A' and 'B' are head and tail, coil#_ref is positive integer, as reference values for coil# (command must follow the format, No Line Ending)
//           1. In UART_PWM_control_mode, value (coil#_ref - 1299)        will be duty cycle and apply to coil#
//           2. In UART_PID_control_mode, value (coil#_ref - 1299)/1500   will be current reference and apply to coil#
// 
//         In Joystick_control_mode:
//             by using Key2 to switch between these two mode (in the case control_flag == 2)
//             1. Joystick_absolute_control_mode, use Joystick to give absolute value for the coil PWM, joystic_x and joystick_y is correspongding coil 1 and 3, by pressing the key intergrated in the joystick, switching to coil 2 and 4
//             2. Joystick_reletive_control_mode, use Joystick to give adding value to each axis, joystic_x giving increment to coil1, and joystick_y giving increment to coil2 and 3
// 
// The way to use the code:
//        1. by default, it is in Joystick_absolute_control_mode;
//        2. Press Key2 to go to  Joystick_reletive_control_mode, and press again to go back;
//        3. Press Key1 to go to  UART_control_mode, and the Joystick is not valid anymore.
//        4. No matter in which mode, UART command is always valid, as it always set the control_flag in each UART command. 
// 
// 
//////********************************************************//////


#include <SparkFun_TB6612.h>
#include "serial.h"
#include "pin_init.h"
#include "key_joy_read.h"
#include "lcd.h"
#include "sensor_read.h"
#include "current_loop.h"
#include "lcd.h"


////***************************************Motor driver SparkFun_TB6612 ************************************//
// these constants are used to allow you to make your motor configuration 
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;
const int offsetC = 1;
const int offsetD = 1;

// Initializing motors.   change the orders to fit with the coils position
Motor motor3 = Motor(AIN1, AIN2, PWMA, offsetA, STBY1);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY1);
Motor motor4 = Motor(AIN3, AIN4, PWMC, offsetC, STBY2);
Motor motor1 = Motor(BIN3, BIN4, PWMD, offsetD, STBY2);
////***************************************Motor driver SparkFun_TB6612 ************************************//

////***************************************Varible Initialization************************************//
int A_fre = 100;  //frequency of the main loop, in Hz
int control_flag = 2;           // Not here, by dafult, control_flag is 2, means using Joystick to give PWM reference  
bool ser_flag = false;          // reset as false at begaining, but will preset as 1 when serial external event happend
bool timer_flag = false;        // reset as false at begaining, but will preset as 1 every timer interrupt
bool serial_print_flag= false;  // reset as false to deactive the UART data print, which uses a lot of time to print
bool lcd_flag = true;           // reset as false to deactive the LCD, which uses a lot of time to dispaly

int PWM_Coil[4] = {0};
int PID_PWM_out[4] = {0};
int coil=0, mode=1; 
int ErrorFlag = 0, FunFlag1 = 0, FunFlag2 = 0, FunFlag3 = 0, FunFlag4 = 0;
int PIDFlag = 0, PID_start_Flag = 0; 
double Ref_list[6] = {0, -0.1, 0.2, -0.4, 0.25, -0.45};
double Ref_I_old[4] = {0};

int count_i = 0;
int uart_i = 0;

int PWM_tran_flag = 0;
int PID_slow_i_flag[4] = {1,1,1,1};
int PID_slow_i[4] = {0,0,0,0};
int PID_ref_i[4] = {0,0,0,0};
////***************************************Varible Initialization************************************//


void setup() {
  //Set timer 1 and 3, fast PWM mode, ICRn as TOP count, the max duty cycle value for PWM is 1599
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
  ICR1 = 1600;
  
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  TCCR3A = _BV(COM3A1) | _BV(COM3B1) | _BV(WGM31);
  TCCR3B = _BV(WGM33) | _BV(WGM32) | _BV(CS30);
  ICR3 = 1600;                  // the max duty cycle value for PWM is 1599 

  //Initialize JoyStick and Key
  joy_key_init();

  //Initialize PID
  current_loop_init();

  //Initialize LCD
  if (lcd_flag){ lcd_initial(); }

  //Initialize Uart
  Serial.begin(500000);     //Set Uart serial port, baudrate 500000

  //Initialize current and temperature sensing, mainly for getting initial shift
  sensor_init();
  delay(500);

  //Set Interrupt 4
  noInterrupts();           // disable all interrupts
  TCCR4A = 0;
  TCCR4B = 0;
  TCNT4  = 0;
  OCR4A = 2000000 / A_fre;  // by seting A_fre t set the frequency of the Interrupt 4, which will work as the main function enabling interuput
  TCCR4B |= (1 << WGM42);   // CTC mode
  TCCR4B |= (1 << CS41);    // 8 prescaler 
  TIMSK4 |= (1 << OCIE4A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}


void loop() {
  // main code, running repeatedly accoring timer_flag preset by Interrupt 4:

  if(timer_flag)          // the main function will wait here unless the timer_flag got preset in the interupt function
  {
    count_i ++;
    if(count_i > 30000){ count_i = 300; }
    if(count_i == 200) { PID_start_Flag = 1; }

    // read the state of joystick and key
    joy_key_read();
  
    //use key of the joystick to selest which coil to contro in Joystick_absolute_control_mode
    if(JoyK_value == 0){
      while(JoyK_value == 0) JoyK_value = digitalRead(JoyK_Pin);  // "while" is used for remove the hand tremor when pressing the key
      coil = 1 - coil;  // when the flag "coil" is 0, coil 1 and coil 3 is controled in Joystick_absolute_control_mode, otherwise, coil 2 and 4 is controlled
    }
  
    // set PWM output value when mode is 0, Joystick_absolute_control_mode
    if( (FunFlag2 == 0)&& (PIDFlag == 0)){
      PWM_Coil[0] = JoyX_value * (1-coil);
      PWM_Coil[1] = JoyX_value * coil;
      PWM_Coil[2] = JoyY_value * (1-coil);
      PWM_Coil[3] = JoyY_value * coil;
    }
  
    //use Key1 to deactive or activate the current loop and Joystick control mode
    if(Key1_value == 0){
      while(Key1_value == 0) Key1_value = digitalRead(Key1_Pin);
      PIDFlag = 1 - PIDFlag;
      control_flag = 0;
    }

    //use Key2 to deactive or activate Joystick_reletive_control_mode
    if(Key2_value == 0){
      while(Key2_value == 0) Key2_value = digitalRead(Key2_Pin);
      FunFlag2 = 1 - FunFlag2;
      control_flag = 2;   // control_flag == 2 means using Joystick to give reference
    }
    
    if(Key3_value == 0){
      while(Key3_value == 0) Key3_value = digitalRead(Key3_Pin);
      FunFlag3 = 1 - FunFlag3;
    }
  
    if(Key4_value == 0){
      while(Key4_value == 0) Key4_value = digitalRead(Key4_Pin);
      FunFlag4 = 1 - FunFlag4;
    }
    
    // some functions
    if(FunFlag2 == 1){  // Joystick_reletive_control_mode
       PWM_Coil[0] = PWM_Coil[0] + JoyX_value*0.006;
       PWM_Coil[1] = PWM_Coil[1] + JoyY_value*0.003;
       PWM_Coil[2] = PWM_Coil[2] - JoyY_value*0.003;
       PWM_Coil[3] = 0;
    }
    if(FunFlag3 == 1){  // set all PWM duty cycle as 200, for testing
      PWM_Coil[0] = 200;
      PWM_Coil[1] = 200;
      PWM_Coil[2] = 200;
      PWM_Coil[3] = 200;
    }
    if(FunFlag4 == 1){  // reset as 0
      PWM_Coil[0] = 0;
      PWM_Coil[1] = 0;
      PWM_Coil[2] = 0;
      PWM_Coil[3] = 0;
    }
  
    //start pid loop for current control
    if (PID_start_Flag == 1){
      current_pid_setRef(0, 0, 0, 0);
      current_pid_start();
      PIDFlag = 1;
      PID_start_Flag = 0;
    }

    //Read commonds from external device via UART, in a formate "A,coil1_ref,coil2_ref,coil3_ref,coil4_ref,control_flag,B"
    //where 'A' and 'B' are head and tail for checking
    //coil#_ref are reference values for coil#, if in current loop actived mode, then use it as current loop reference;
    //if in PWM control mode, use it as PWM duty cycle value
    // Note the scale converting, detailed in serial.cpp
    if(ser_flag){
      for(int i = 0; i < 4; i++) { Ref_I_old[i] = Ref_I[i]; }
      uart_receive();
      for(int i = 0; i < 4; i++) { Ref_I[i] = Ref_Coil[i];  }
      if ( (abs(Ref_I_old[0] - Ref_I[0]) > 0.05)||(abs(Ref_I_old[1] - Ref_I[1]) > 0.05)||(abs(Ref_I_old[2] - Ref_I[2]) > 0.05)||(abs(Ref_I_old[3] - Ref_I[3]) > 0.05) ){
        PWM_tran_flag = 1;
      }
      
      for (int i = 0; i < 4; i++) { 
        if(Ref_I_old[i] != Ref_I[i]){
          PID_ref_i[i] = 400; // in this time, the fine adjustment for PID will not be used
        }
      }
      
      ser_flag = false;
    }
    if (control_flag == 1){ PIDFlag = 1; }  // control_flag == 1 means using PID control mode, uart giving current
    else {  PIDFlag = 0; }                  

    //read current value (as well as temperature) before the current loop
    sensor_read();

    // run pid computation
    if(PIDFlag==1){
      
      //if Ref_I changed suddenly over 0.05 A, run PWM-I translate function, to have less response time, coeefecients are from regression
      if(PWM_tran_flag != 0){
        if(PWM_tran_flag == 1){
          if ( abs(Ref_I_old[0] - Ref_I[0]) > 0.05 ){
            PID_PWM_out[0] = PID_PWM_out[0] + 1900 * (Ref_I[0] - Ref_I_old[0]) * 1;
          }
          if ( abs(Ref_I_old[1] - Ref_I[1]) > 0.05 ){
            PID_PWM_out[1] = PID_PWM_out[1] + 1900 * (Ref_I[1] - Ref_I_old[1]) * 1;
          }
          if ( abs(Ref_I_old[2] - Ref_I[2]) > 0.05 ){
            PID_PWM_out[2] = PID_PWM_out[2] + 2000 * (Ref_I[2] - Ref_I_old[2]) * 1;
          }
          if ( abs(Ref_I_old[3] - Ref_I[3]) > 0.05 ){
            PID_PWM_out[3] = PID_PWM_out[3] + 1900 * (Ref_I[3] - Ref_I_old[3]) * 1;
          }
        }
        PWM_tran_flag ++;
        if (PWM_tran_flag > 8){
          PWM_tran_flag = 0;
        }        
      } 
      else{
        current_pid_setRef(Ref_I[0], Ref_I[1], Ref_I[2], Ref_I[3]); 
        current_pid_compute();

        //PID fine adjustment
        for(int i = 0; i < 4; i++){
          PID_slow_i_flag[i] = 1;
          if(PID_ref_i[i] != 0){ PID_ref_i[i]--; }
          else{
            if(int(abs(PID_Iout[i])+0.9) == 1){     // means if a PID out value is 0.1, 
              PID_slow_i[i] ++;          
              if(PID_slow_i[i] > 59){PID_slow_i[i] = 0;}
              if(PID_slow_i[i] % 5 == 0){ PID_slow_i_flag[i] = 1; }
              else{ PID_slow_i_flag[i] = 0; }
            }
          }
        }
        
        PID_PWM_out[0] = PID_PWM_out[0] + round(PID_Iout[0])*PID_slow_i_flag[0];    // the symbol here determins the positive or negative feedback, in the case PWM and current from Uart has different symbol, inverse the symbol here
        PID_PWM_out[1] = PID_PWM_out[1] + round(PID_Iout[1])*PID_slow_i_flag[1];
        PID_PWM_out[2] = PID_PWM_out[2] + round(PID_Iout[2])*PID_slow_i_flag[2];
        PID_PWM_out[3] = PID_PWM_out[3] + round(PID_Iout[3])*PID_slow_i_flag[3];
        
      }

      for(int i = 0; i < 4; i++){
        if (Ref_I[i] == 0){ PID_PWM_out[i] = 0; }
        PWM_Coil[i] = PID_PWM_out[i];
      }
      
    }
    if ( (PIDFlag == 0)&&(control_flag == 0) ){     //control_flag == 0 means using PWM control mode, uart giving PWM
      for(int i = 0; i < 4; i++){
        PWM_Coil[i] = int(Ref_Coil[i]*1500); 
      }
    }
    

    if (control_flag == 3){   // control_flag == 3 means using specific control mode, uart giving orders, define the function here, e.g., here using control_flag == 3 to shutdown the PWM output as 0
      PWM_Coil[0] = 0;
      PWM_Coil[1] = 0;
      PWM_Coil[2] = 0;
      PWM_Coil[3] = 0;
    }
    


  //Make sure the output will not over the limit
    for(int i = 0; i < 4; i++){
      if(PWM_Coil[i] > 1599){
        PWM_Coil[i] = 1599;
      }
      if(PWM_Coil[i] < -1599){
        PWM_Coil[i] = -1599;
      }
      if(PWM_Coil[i] == 255){ // there is a bug that it will be full duty cycle when the value reach 255 (but theoretically 1599), may aslo caused by code or hareward problem
        PWM_Coil[i] = 256;
      }
      if(PWM_Coil[i] == -255){
        PWM_Coil[i] = -256;
      }
    }

    motor1.drive(PWM_Coil[0],0);
    motor2.drive(PWM_Coil[1],0);
    motor3.drive(PWM_Coil[2],0);
    motor4.drive(PWM_Coil[3],0);
  
    //********** comment the following for using less time **********//
    // Int format sending, using less time
    if (serial_print_flag){
      Serial.print(0x05);Serial.print(" ");
      Serial.print(int(I_value_1*20000)); Serial.print(" "); 
      Serial.print(int(I_value_2*20000));  Serial.print(" ");
      Serial.print(int(I_value_3*20000));  Serial.print(" ");
      Serial.print(int(I_value_4*20000));  Serial.print(" ");
      Serial.print(int(Tem_value_1*500));  Serial.print(" ");
      Serial.print(int(Tem_value_2*500));  Serial.print(" ");
      Serial.print(int(Tem_value_3*500));  Serial.print(" ");
      Serial.print(int(Tem_value_4*500));  Serial.print(" ");
      Serial.print(int(Ref_I[0]*1000));  Serial.print(" "); // this one could be a any value, for debuging
      Serial.print(0x0A);Serial.print(" ");
    }

    if (lcd_flag){ lcd_test2(); }
    //********** comment the above for using less time **********//
    
    timer_flag = false;
  }
}


void serialEvent() 
{
  ser_flag = true;
}


ISR(TIMER4_COMPA_vect)          // timer compare interrupt service routine
{
  timer_flag = true;
}
