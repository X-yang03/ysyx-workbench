#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Vtop.h"
#include "verilated.h"
#include "verilated_fst_c.h"
#include <nvboard.h>

static TOP_NAME dut;

void nvboard_bind_all_pins(TOP_NAME* top);

void init_board(){
     nvboard_bind_all_pins(&dut);
     nvboard_init();
}

int main(int argc, char** argv){
	int time = 10;
	VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop* top = new Vtop{contextp};
    VerilatedFstC *tfp= new VerilatedFstC;   //初始化VCD对象指针
    contextp->traceEverOn(true); //打开追踪
	top->trace(tfp,0);
    tfp->open("wave.fst");  //保存位置
    init_board();
    tfp->close();

   while(true){
       dut.eval();
       nvboard_update();
   }
    delete top;
    delete contextp;
    return 0;
}
