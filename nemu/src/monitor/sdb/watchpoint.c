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

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char expr[32];
  uint32_t val;

  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

bool new_wp(char *args){
    Assert(free_ != NULL, "Watch points exceeded Maximum!\n");
    bool succ = true;
    int res = expr(args,&succ);
    if(!succ)
    {
      printf("Invalid Expression!\n");
      return false;
    }

    WP* wp = free_;
    free_ = free_->next;
    wp->next = head;
    head = wp;

    strcpy(wp->expr,args);
    wp->val = res;
    printf("Successfully inserted a watch point %d!\n",wp->NO);
    return true;
}


bool free_wp(int N){
  Assert(head != NULL, "Empty watch points!\n");
  WP* wp = head;
  WP* prec = NULL;  // once freed wp, need to connect prec and wp->next
  while(head->NO != N && wp!=NULL){
    prec = wp;
    wp = wp->next;
  }
  if(wp == NULL){
    printf("Invalid NO!\n");
    return false;
  }
  if(prec != NULL)  prec->next = wp->next;  // reconnect the link list
  else head = wp->next;
  wp->next = free_;
  free_ = wp;
  memset(wp->expr,0,sizeof(char)*32);
  wp->val = 0;
  return true;
  
}

void show_wp(){
  if(head == NULL){
    printf("Empty watch points!\n");
    return;
  }
  printf("NO\t\tExpr\t\tVal\n");
  WP* wp = head;
  while(wp != NULL){
    printf("%2d\t\t%s\t\t0x%x(%u)\n",wp->NO,wp->expr,wp->val,wp->val);
    wp = wp->next;
  }
}

bool wp_changed(){
  WP* wp = head;
  bool flag = false;
  while(wp != NULL){
    bool succ = true;
    uint32_t curr_val = expr(wp->expr,&succ);
    if(curr_val != wp->val){
      if(!flag){
        printf("Reached watch points :\n");
      }
      printf("%d : %u --> %u\n", wp->NO,wp->val,curr_val);
      wp->val = curr_val;
      flag = true;
    }
    wp = wp->next;
  }
  return flag;
}
