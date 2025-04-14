#include "serial.h"

int PWM_Rdata[6] = {0};
double Ref_Coil[4] = {0};
double Ref_Coil_old[4] = {0};
String Rdata = "";

//Serial UART receive data
void uart_receive(){
  int j = 0;
  while (Serial.available() > 0)  
    {
      Rdata += char(Serial.read());
      mode = 1;
      delayMicroseconds (1);
    }
    if (Rdata.length() > 0)
    {

        PWM_Rdata[0] = 0;  PWM_Rdata[1] = 0;  PWM_Rdata[2] = 0;  PWM_Rdata[3] = 0;  PWM_Rdata[4] = 0; PWM_Rdata[5] = 0;
        if( Rdata[0] == 'A' )
        {
          if( Rdata[Rdata.length()-1] == 'B')
          {
            ErrorFlag = 0;
            for(int i = 1; i < Rdata.length() ; i++)
            {
              if(Rdata[i] == ','){j++;}
              else if (Rdata[i] == 'B'){break;}
              else
              {
                PWM_Rdata[j] = PWM_Rdata[j] * 10 + (Rdata[i] - '0');
                if( abs(PWM_Rdata[j]-1299) > 1299 ){ ErrorFlag = j+1;}
              }
            }
            if(j != 5 ){ ErrorFlag = 9; } 

            Rdata = "";
          }
          else{ErrorFlag = 7;}
  
        }
        else
        {
          ErrorFlag = 8;
          Rdata = "";
        }

        if( ErrorFlag == 0 )
        {
          for(int i = 0; i < 4; i++){
            Ref_Coil_old[i] = Ref_Coil[i];
          }
          Ref_Coil[0] = (PWM_Rdata[0] - 1299) / 1500.00; // Scale switching, ensures UART transmission with positive integers, to use less time
          Ref_Coil[1] = (PWM_Rdata[1] - 1299) / 1500.00; 
          Ref_Coil[2] = (PWM_Rdata[2] - 1299) / 1500.00; 
          Ref_Coil[3] = (PWM_Rdata[3] - 1299) / 1500.00; 
          control_flag = PWM_Rdata[4]; 


        }
        else if(ErrorFlag != 0){
          Serial.print("Error: code ");
          Serial.println(ErrorFlag);
          ErrorFlag = 0;
        }
    }

}
