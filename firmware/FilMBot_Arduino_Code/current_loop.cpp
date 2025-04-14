#include "current_loop.h"
#include "sensor_read.h"
#include <PID_v1.h>


double delat_t=0.002;
double error_I1=0.0;
double last_error_I1=0.0;
double sum_error_I1=0.0;
double rate_error_I1=0.0;


//Define Variables we'll be connecting to
// double Ref_I1=0, I1=0, PID_Iout1=0;
// double Ref_I2=0, I2=0, PID_Iout2=0;
// double Ref_I3=0, I3=0, PID_Iout3=0;
// double Ref_I4=0, I4=0, PID_Iout4=0;
double I1 = 0, I2 = 0, I3 = 0, I4 = 0;

double Ref_I[4] = {0}, PID_Iout[4] = {0};

//Specify the links and initial tuning parameters
double Kp_I1 = 150, Ki_I1 = 80, Kd_I1 = 0; //200,3,10
double Kp_I2 = 150, Ki_I2 = 80, Kd_I2 = 0;
double Kp_I3 = 150, Ki_I3 = 80, Kd_I3 = 0;
double Kp_I4 = 150, Ki_I4 = 80, Kd_I4 = 0;

//Specify the links and initial tuning parameters
PID PID1(&I1, &PID_Iout[0], &Ref_I[0],Kp_I1,Ki_I1,Kd_I1, DIRECT);
PID PID2(&I2, &PID_Iout[1], &Ref_I[1],Kp_I2,Ki_I2,Kd_I2, DIRECT);
PID PID3(&I3, &PID_Iout[2], &Ref_I[2],Kp_I3,Ki_I3,Kd_I3, DIRECT);
PID PID4(&I4, &PID_Iout[3], &Ref_I[3],Kp_I4,Ki_I4,Kd_I4, DIRECT);


double current_pid_setRef(double ref1, double ref2, double ref3, double ref4){
    Ref_I[0] = ref1;
    Ref_I[1] = ref2;
    Ref_I[2] = ref3;
    Ref_I[3] = ref4;
}

void current_loop_init(){
    PID1.SetOutputLimits(-1000,1000);
    PID2.SetOutputLimits(-1000,1000);
    PID3.SetOutputLimits(-1000,1000);
    PID4.SetOutputLimits(-1000,1000);
}

void current_pid_start(){
    PID1.SetMode(AUTOMATIC);
    PID2.SetMode(AUTOMATIC);
    PID3.SetMode(AUTOMATIC);
    PID4.SetMode(AUTOMATIC);
}

void current_pid_compute(){
  I1 = I_value_1;
  I2 = I_value_2;
  I3 = I_value_3;
  I4 = I_value_4;
  PID1.Compute();
  PID2.Compute();
  PID3.Compute();
  PID4.Compute();
}
