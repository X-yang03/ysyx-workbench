/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
  printf("pc : %08x\n" , cpu.pc);
  printf("$0 : %08x\n" , cpu.dollar_0);	
	printf("ra : %08x\n" , cpu.ra);
	printf("sp : %08x\n" , cpu.sp);
	printf("gp : %08x\n" , cpu.gp);
	printf("tp : %08x\n" , cpu.tp); 

	printf("t0 : %08x\t" , cpu.t0);
  printf("t1 : %08x\t" , cpu.t1);
  printf("t2 : %08x\n" , cpu.t2);

  printf("s0 : %08x\t" , cpu.s0);
  printf("s1 : %08x\n" , cpu.s1);

  printf("a0 : %08x\t" , cpu.a0);
  printf("a1 : %08x\t" , cpu.a1);
  printf("a2 : %08x\t" , cpu.a2);
  printf("a3 : %08x\n" , cpu.a3);
  printf("a4 : %08x\t" , cpu.a4);
  printf("a5 : %08x\t" , cpu.a5);
  printf("a6 : %08x\t" , cpu.a6);
  printf("a7 : %08x\n" , cpu.a7);

  printf("s2 : %08x\t" , cpu.s2);
  printf("s3 : %08x\t" , cpu.s3);
  printf("s4 : %08x\t" , cpu.s4);
  printf("s5 : %08x\t" , cpu.s5);
  printf("s6 : %08x\n" , cpu.s6);
  printf("s7 : %08x\t" , cpu.s7);
  printf("s8 : %08x\t" , cpu.s8);
  printf("s9 : %08x\t" , cpu.s9);
  printf("s10 : %08x\t" , cpu.s10);
  printf("s11 : %08x\n" , cpu.s11);

  printf("t3 : %08x\t" , cpu.t3);
  printf("t4 : %08x\n" , cpu.t4);
  printf("t5 : %08x\t" , cpu.t5);
  printf("t6 : %08x\n" , cpu.t6);


}

word_t isa_reg_str2val(const char *s, bool *success) {
  return 0;
}
