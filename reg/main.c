#include "main.h"

PID_s pid;
Inercja_s in3;
Inercja_s in4;
float tp = 0.1;

void Draw_info(int real_temp, int exp_temp, bool window){
    printf("real: %d, exp: %d, wind: %d\n", real_temp, exp_temp, window);
}

int main(void){
    // PID_s_init(&pid, 10, 3, 0.9, tp);
    PID_s_init(&pid, 7.0, 5.6, 1.0, tp);
    Inercja_s_init(&in3, 1, tp, 5);
    Inercja_s_init(&in4, 2, tp, 3);
    pid.expected = 18;
    in3.last_input = 17.5;
    in4.last_input = 17.5;
    in4.output = 17.5;
  /* Infinite loop */
  for(int i = 0;i < 200; i++)
  {
        printf("%d.\t", i);
	    pid.input = in4.output;
	    Reg_s_step(&pid);
	    //output = pid.output;
	    in3.input = pid.output;
	    Inercja_s_step(&in3);
	    in4.input = in3.output;
	    Inercja_s_step(&in4);
        if(i > 80){
            pid.expected = 45.0;
        }
	    Draw_info((int)(pid.input * 10), (int)(pid.expected * 10), false);
	    // wykres_draw(&entry);
//	    Draw_info(104, 152, false);
  }
    return 0;
}