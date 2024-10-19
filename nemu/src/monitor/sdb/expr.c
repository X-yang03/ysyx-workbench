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

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <memory/vaddr.h>

enum {
  TK_NOTYPE = 256, TK_DEC, TK_HEX, TK_REG, TK_MINUS, TK_PTR, TK_NOT, TK_DIV, 
  TK_MUL, TK_ADD, TK_SUB, TK_EQ, TK_NEQ, TK_AND, TK_OR

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", TK_ADD},         // plus
  {"==", TK_EQ},         // equal
  {"!=", TK_NEQ},        // not equal
  {"0[xX][0-9a-fA-F]+", TK_HEX}, // hexadecimal
  {"[1-9][0-9]*|0", TK_DEC}, // decimal
  {"\\-", TK_SUB},       // subtraction
  {"\\*", TK_MUL},       // multiplication or pointer
  {"\\/", TK_DIV},       // division
  {"\\(", '('},          // left parenthesis
  {"\\)", ')'},          // right parenthesis
  {"\\&\\&", TK_AND},    // logical AND
  {"\\|\\|", TK_OR},     // logical OR
  {"\\!", TK_NOT},       // logical NOT
  {"\\pc", TK_REG},     // register pc
  {"\\$0", TK_REG},      // register $0
  {"ra", TK_REG},        // register ra
  {"[stg]p", TK_REG},    // registers sp, tp, gp
  {"t[0-6]", TK_REG},    // registers t0-t6
  {"s[0-9]|1[0-1]", TK_REG}, // registers s0-s11
  {"a[0-7]", TK_REG}     // registers a0-a7

};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  int priority; // used to find dominant op
  char str[32]; //every token should be no more than 32 char
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        if(rules[i].token_type == TK_NOTYPE) break;
        Assert(nr_token<32,"Too many tokens!");
        //classify the token
        tokens[nr_token].type = rules[i].token_type;  
        
        // recognize TK_PTR and TK_MINUS
        if(tokens[nr_token].type == TK_MUL && ( nr_token == 0 ||(tokens[nr_token-1].type != TK_DEC && 
            tokens[nr_token].type!= TK_HEX && tokens[nr_token-1].type != TK_REG && tokens[nr_token-1].type != ')')))
            tokens[nr_token].type = TK_PTR;

        if(tokens[nr_token].type == TK_SUB && ( nr_token == 0 ||(tokens[nr_token-1].type != TK_DEC && 
            tokens[nr_token].type!= TK_HEX && tokens[nr_token-1].type != TK_REG && tokens[nr_token-1].type != ')')))
            tokens[nr_token].type = TK_MINUS;

        tokens[nr_token].priority = tokens[nr_token].type; 
        //type is ordered by priority!

        switch (rules[i].token_type) {

          case TK_DEC: case TK_HEX: case TK_REG:
            Assert(substr_len<32,"Length of numbers should be no more than 31!\n");
            //KISS protocol
            strncpy(tokens[nr_token].str,substr_start,substr_len);  //copy the string
            tokens[nr_token].str[substr_len] = '\0';
            tokens[nr_token].priority = -1; // priority for dec hex and reg
            break;

          default: break;
        }

        nr_token++;

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_parentheses(int p,int q){
  int par = 0;
  if(tokens[p].type == '(' && tokens[q].type == ')'){
      for(int i = p + 1;i < q;i++){
        if(tokens[i].type == '(') par++;
        else if(tokens[i].type == ')') par--;
    }
    if(par == 0) return true;
  }
  return false;
}

int dominant_op(int p , int q){ // find dominant operator between p and q
  int in_parentheses = 0;
  int min_priority = 0;
  int _dominant = 0;
  for(int i = p; i <= q; i++){
    if(tokens[i].type == '(') in_parentheses++;
    else if (tokens[i].type == ')') in_parentheses--;

    if(in_parentheses == 0 && tokens[i].priority != -1){ // with no parentheses
        if(tokens[i].priority > min_priority){
          min_priority = tokens[i].priority;
          _dominant = i;
        }
        else if(tokens[i].priority == min_priority && tokens[i].priority > TK_NOT){ // for - * and ! , dominant operator is the most left one
          _dominant = i;
        }

    }
  }
  //printf("%d, %d\n",_dominant,tokens[_dominant].type);
  return _dominant;

}

uint32_t eval(int p , int q) // tokens[p] and tokens[q] NOT CHARACTER[p] AND CHARACTER[q]!
{
  Assert(p<=q,"Bad Expression!");
  if(p == q){ // a single token
    int val = 0;
    switch (tokens[p].type)
    {
    case TK_DEC:
      return atoi(tokens[p].str);
      break;
    
    case TK_HEX:
      
      sscanf(tokens[p].str,"%x",&val);
      return val;

    case TK_REG:
      if (strcmp(tokens[p].str , "pc") == 0) return cpu.pc;
      else if (strcmp(tokens[p].str , "$0") == 0) return cpu.dollar_0;
      else if (strcmp(tokens[p].str , "ra") == 0) return cpu.ra;
      else if (strcmp(tokens[p].str , "sp") == 0) return cpu.sp;
      else if (strcmp(tokens[p].str , "tp") == 0) return cpu.tp;
      else if (strcmp(tokens[p].str , "gp") == 0) return cpu.gp;
      else if (tokens[p].str[0] == 't')
      {
        if (tokens[p].str[1] < '3') return cpu.gpr[tokens[p].str[1] - '0' + 5]; //t0 - t2
        else return cpu.gpr[tokens[p].str[1] - '3' + 28]; //t3 - t6
      }
      else if (tokens[p].str[0] == 's')
      {
        if(tokens[p].str[1] < '2') return cpu.gpr[tokens[p].str[1] - '0' + 8]; //s0 - s1
        else if(tokens[p].str[1] == 'p') return cpu.gpr[2]; //sp
        else return cpu.gpr[tokens[p].str[1] - '2' + 18]; //s2 - s11
      }
      
      else if (tokens[p].str[0] == 'a') return cpu.gpr[tokens[p].str[1] - '0' + 10]; //a0 - a7
    default:
      break;
    }

  }
  else if(check_parentheses(p,q)){
    return eval(p+1,q-1);
  }
  else{
    int op = dominant_op(p,q);
    uint32_t val1 = 0 ,val2 = 0;
    if (tokens[op].type > TK_NOT )
      val1 = eval(p,op-1); // if op is - * !, val1 is the operator
    val2 = eval(op+1,q);
    switch (tokens[op].type)
    {
    case TK_ADD:
      return val1 + val2;
    case TK_SUB:
      return val1 - val2;
    case TK_MUL:
      return val1 * val2;
    case TK_DIV:
      return val1 / val2;
    case TK_AND:
      return val1 && val2;
    case TK_OR:
      return val1 || val2;

    case TK_MINUS: //val1 is '-'
      return -val2;
    case TK_PTR: // val1 is '*' , val2 is address
      return vaddr_read(val2,4);
    case TK_NOT:
      return !val2;
    case TK_EQ:
      return val1 == val2;
    case TK_NEQ:
      return val1 != val2;

    default:
      Assert(0,"Bad operation!\n");
      break;
    }
  }

  return 0;
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  int par = 0;
  for(int i =0; i < nr_token; i++){
    if(tokens[i].type == '(') par++;
    else if(tokens[i].type == ')') par--;
    if(par < 0 ) break; // e.g. (1+2)))((+4
  }
  if(par != 0 ){
    *success = false;
    printf("Unmatched Parentheses!\n");
    return 0;
  }

  return eval(0,nr_token-1);
}
