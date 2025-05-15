#pragma once

typedef struct
{
    float Kp, Ti, Td, Ts, last_i, last_input;
    float input, expected, output;
}PID_s;

typedef struct{
    float last_input, kp, tin ,ts, output;
    float input;
}Inercja_s;

void PID_s_init(PID_s* reg, float kp, float ti, float td, float t);
void Reg_s_step(PID_s* reg);

void Inercja_s_init(Inercja_s* in,float  k, float t, float ti);
void Inercja_s_step(Inercja_s* in);
