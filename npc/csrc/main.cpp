#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Vtop.h"
#include "verilated.h"
#include "verilated_fst_c.h"
//#include <nvboard.h>

//static TOP_NAME dut;

//void nvboard_bind_all_pins(TOP_NAME* top);


int main(int argc, char** argv){
    // nvboard_bind_all_pins(&dut);
    // nvboard_init();
	int time = 10;
	VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop* top = new Vtop{contextp};
    VerilatedFstC *tfp= new VerilatedFstC;   //初始化VCD对象指针
    contextp->traceEverOn(true); //打开追踪
	top->trace(tfp,0);
    tfp->open("wave.fst");  //保存位置

while (time--){
  //  nvboard_update();
  int a = rand() & 1;
  int b = rand() & 1;
  top->a = a;
  top->b = b;
  top->eval();
  //   dut.eval();
  printf("a = %d, b = %d, f = %d\n", a, b, top->f);
  tfp->dump(contextp->time());  //dump wave
  contextp->timeInc(1);//仿真时间推进
  assert(top->f == (a ^ b));

}
delete top;
tfp->close();
delete contextp;
return 0;
}
