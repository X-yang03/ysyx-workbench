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
#include <cpu/cpu.h>
#include <memory/vaddr.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include <stdlib.h>

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();
bool new_wp();
bool free_wp(int NO);
void show_wp();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_x(char *args);
static int cmd_p(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Use si N to run N instructions", cmd_si},
  { "info", "info r to show the status of regfile; info w to show the status of watchpoints" , cmd_info},
  { "x" ,"Usage: x N EXPR to see the contents of RAM from EXPR" , cmd_x},
  {"p","Calculate the value of a expression",cmd_p},
  {"w","Usage: w expr -- set a watch point over a expression",cmd_w},
  {"d","Usage:d N -- delete watch point with NO N",cmd_d},

  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args){
	if(args == NULL){
	printf("Illegal number of parameters.\n");
	printf("Check help si to see the Usage.\n");
	return 1;
	}
	int num = atoi(args);
	cpu_exec(num);
	//printf("Successfully run %d instructions!\n",num);
	return 0;
}

static int cmd_info(char* args){
	if(args == NULL){
	printf("Illegal number of parameters.\n");
	printf("Check help info to see the Usage.\n");
	return 0;
	}
	if(strcmp(args,"r") == 0){
  isa_reg_display();
	printf("===================\n");

	}
	else if(strcmp(args,"w") == 0){
	//Todo: print watchpoint
    show_wp();
	}
	else{
	printf("Illegal parameters.\n");
	return 1;
	}
	return 0;
}

static int cmd_x(char *args){
  char *arg = strtok(NULL," ");
	if(args == NULL){
	printf("Illegal parameters.\n");
	return 0;
	}

	int N = atoi(args);  //string to int

	arg = strtok(NULL," ");
	if(arg == NULL){
    printf("Illegal Parameters.\n");
    return 0;
	}

  bool succ = true;
  vaddr_t addr = expr(arg,&succ);  //Todo: expr()
  if(!succ)
  {
    printf("Invalid Expression!\n");
    return 1;
  }

  printf("Bytes : \tLow ===> High\n");
  
	for (int i=0;i<N;i++){
		uint32_t data = vaddr_read(addr+4*i,4);
		printf("0x%08x :\t",addr+4*i);
		for(int j=0;j<4;j++){
			printf("%02x ",data&0xff);
			data = data >> 8 ;
		}
		printf("\n");
	
	}
	return 0;
}

static int cmd_p(char *args){
  if(args == NULL){
    printf("Empty Expression!\n");
    return 1;
  }
  init_regex();
  bool succ = true;
  int res = expr(args,&succ);
  if(succ){
    printf("Result: %d\n",res);
  }
  else
  {
    printf("Invalid Expression!\n");

  }
  
  return 0 ;
}
static int cmd_w(char *args){
  if(args == NULL){
    printf("Empty Expression!\n");
    return 1;
  }
  new_wp();
  return 0;
}
static int cmd_d(char *args){
  if(args == NULL){
    printf("N cannot be empty!\n");
    return 1;
  }
  int NO = atoi(args);
  bool succ = free_wp(NO);
  if(succ) printf("Successfully deleted watch point %d\n!",NO);
  else return 1;
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
