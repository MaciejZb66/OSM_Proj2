#include "reg2.h"

void PID_s_init(PID_s* reg, float kp, float ti, float td, float t){
    reg->Kp = kp;
    reg->Ti = ti;
    reg->Td = td;
    reg->Ts = t;
    reg->last_i = 0;
    reg->last_input = 0;
}

void Reg_s_step(PID_s* reg){
//    printf("(input: %2.4f, expected: %.2f)",reg->input, reg->expected);
    float P = reg->Kp*reg->Ts * (reg->expected - reg->input);
    float I = (reg->Kp /reg->Ti * reg->Ts * (reg->expected - reg->input)) + reg->last_i;        
//    float D = reg->Kp * ((reg->expected - reg->input) - (reg->expected - reg->last_input))/reg->Ts * reg->Td;
    if(I > 50) I = 50;
    if(I < -20) I = -20;
    reg->last_i = I;

//    if(D > 10) D = 10;
//    if(D < -10) D = -10;
//    printf("P: %.2f \t", P);
//    printf("I: %.2f \t", I);
//    printf("D: %.2f \t", D);

    reg->last_input = reg->input;

    if (reg->output > 70) reg->output = 70;
    if (reg->output < -70) reg->output = -70;
//    reg->output = P + I + D;
    reg->output = P + I;
}

void Inercja_s_init(Inercja_s* in,float  k, float t, float ti){
    in->last_input = 0;
    in->kp = k;
    in->ts = t;
    in->tin = ti;
}

void Inercja_s_step(Inercja_s* in){
    //local out =  (Tp / (5 + Tp)) * enter + (5 / (5 + Tp)) * last           -- 1/(5s+1)
    //local out = ((Tp / (3 + Tp)) * enter + (3 / (3 + Tp)) * last/2)*2     -- 2/(3s+1)
    in->output = ((in->ts / (in->tin + in->ts)) * in->input + (in->tin / (in->tin + in->ts)) * in->last_input/in->kp)*in->kp;
    in->last_input = in->input;
    in->output_i = (int)(10*in->output);
}
