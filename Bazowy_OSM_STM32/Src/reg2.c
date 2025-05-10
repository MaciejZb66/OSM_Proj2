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
    //printf("(input: %f, expected: %f)",input, expected);
    float P = reg->Kp * (reg->expected - reg->input);
    float I = (reg->Kp /reg->Ti * reg->Ts * (reg->expected - reg->input)) + reg->last_i;        
    float D = reg->Kp * ((reg->expected - reg->input) - (reg->expected - reg->last_input))/reg->Ts * reg->Td;
    //printf("P: %.2f \t", P);
    reg->last_i = I;
    reg->last_input = reg->input;
    reg->output = P + I + D;
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
}