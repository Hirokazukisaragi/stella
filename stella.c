/******************************************************************************
Copyright 2018 HIROKAZU SENO

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stella.h"

#define MAX_STR_LEN 765554
#define STACK_MAX 2048
char st_srcode[MAX_STR_LEN];
stack *g_stack;
Token *tstream;
stack *exec_stack;
int g_pc = 0;
int g_ip = 0;
int g_sp = 0;
int e_sp = 0;
int r_sp = 0;
int main(int argc,char *argv[]){
  FILE *fp;
  fp = fopen(argv[1],"r");
  initial(fp);
  parsing();
  //exec_vm();
//  printf("%s\n",st_srcode);
  fclose(fp);

  return 0;
}
void initial(FILE *fp){
  fread(st_srcode,sizeof(char),MAX_STR_LEN,fp);
  g_stack = malloc(sizeof(stack) * STACK_MAX);
  exec_stack = malloc(sizeof(stack) * STACK_MAX);
  tstream = malloc(sizeof(Token) * STACK_MAX);
}
void parsing(void){
  Token *tkn;
  Token *cur;
  int i = 0;
  int k = 0;
  int top = 0;
  int lval,rval;
  while(1){
    tkn = getNToken();
    switch (tkn->kind) {
      case OP_INT:
      //tstream[top].u.num = tkn->u.num;
      exec_push(tkn);
      //exec_stack[++g_sp] = tkn->u.num;
      break;
      case OP_ADD:
      cur = exec_pop();
      rval = cur->u.num;
      cur = exec_pop();
      lval = cur->u.num;
      cur->u.num = lval + rval;
      exec_push(cur);
      break;
      case OP_SUB:
      cur = exec_pop();
      rval = cur->u.num;
      cur = exec_pop();
      lval = cur->u.num;
      cur->u.num = lval - rval;
      exec_push(cur);
      break;
      case OP_MUL:
      cur = exec_pop();
      rval = cur->u.num;
      cur = exec_pop();
      lval = cur->u.num;
      cur->u.num = lval * rval;
      exec_push(cur);
      break;
      case OP_DIV:
      cur = exec_pop();
      rval = cur->u.num;
      cur = exec_pop();
      lval = cur->u.num;
      cur->u.num = lval / rval;
      exec_push(cur);
      break;
      case OP_PRINT:
      cur = exec_pop();
      printf("printer:%d\n",cur->u.num);
    }
      st_push(tkn);
      if(tkn->kind == OP_END_OF_FILE){
      printf("Program is correct end.\n");
      break;
    }
  }
}
void exec_vm(void){
}
BOOL is_al(char ch){
  if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')){
    return TRUE;
  }else{
    return FALSE;
  }
}
BOOL is_num(void){
  if(st_srcode[g_pc] >= '0' && st_srcode[g_pc] <= '9'){
    return TRUE;
  }else{
    return FALSE;
  }
}
Token *getNToken(){
  Token *ret;
  ret = malloc(sizeof(Token));

  ret->kind = OP_NIL;
  char tname[255];
  char la;
  BOOL iskw = FALSE;
  int i = 0;
  int j = 0;
  int k = 0;
  int rnum = 0;
  BOOL flag = FALSE;

  skipWS();
  if(st_srcode[g_pc] == '\0'){
    ret->kind = OP_END_OF_FILE;

    return ret;
  }
  for(;;){
    la = getNchar();
    tname[i] = st_srcode[g_pc];

    if(!is_al(la)){
      break;
    }
    i++;
    g_pc++;

  }
  i++;
  tname[i] = '\0';
  skipWS();
  while(g_keywordtbl[j].kind != OP_NOP){
    if(!strcmp(tname,g_keywordtbl[j].tkname)){
      ret->kind = g_keywordtbl[j].kind;
      strcpy(ret->name,g_keywordtbl[j].tkname);
      iskw = TRUE;
      //break;
    }
    j++;
  }
  while(g_optbl[k].kind != OP_EOP){
    if(!strcmp(tname,g_optbl[k].opname)){
      ret->kind = g_optbl[k].kind;
      strcpy(ret->name,g_optbl[k].opname);
      iskw = TRUE;
      break;
    }
    k++;
  }
  if(is_num()){
    ret->kind = OP_INT;
    rnum = st_srcode[g_pc] - '0';
    g_pc++;
  }
  while(is_num()){
    if(is_num()){
      rnum *= 10;
      rnum += st_srcode[g_pc] - '0';
      g_pc++;
    }else{
      break;
    }
  }
  if(ret->kind == OP_INT){
    ret->u.num = rnum;
    return ret;
  }
  //g_pc++;

  skipWS();
  la = getCurrent();
  switch (la) {
    case '+':
    ret->kind = OP_ADD;
    iskw = TRUE;
    g_pc++;
    return ret;
    break;
    case '-':
    ret->kind = OP_SUB;
    iskw = TRUE;
    g_pc++;
    return ret;
    break;
    case '*':
    ret->kind = OP_MUL;
    iskw = TRUE;
    g_pc++;
    return ret;
    break;
    case '/':
    ret->kind = OP_DIV;
    iskw = TRUE;
    g_pc++;
    return ret;
    break;
    case '=':
    la = getNchar();
    if(la == '='){
      ret->kind = OP_EQL;
      iskw = TRUE;
      g_pc++;
      break;
    }else{
      ret->kind = OP_ASSIGN;
      //g_pc--;
      iskw = TRUE;
      break;
    }
    case '!':
    ret->kind = OP_NOTEQ;
    iskw = TRUE;
    break;
    case '<':
    ret->kind = OP_LT;
    iskw = TRUE;
    break;
    case '>':
    ret->kind = OP_GT;
    iskw = TRUE;
    break;
    case '\0':
    ret->kind = OP_END_OF_FILE;
    iskw = TRUE;
    break;
  }
  if(iskw){
    g_pc++;
    return ret;
  }else{
    //ret->kind = OP_NOTKW;
    if(ret->kind == OP_INT){
      ret->u.num = rnum;
    }else{
      strcpy(ret->u.str,tname);
    }
    strcpy(ret->name,tname);
    return ret;
  }
  return ret;
}
char getCurrent(void){
  char ch;
  ch = st_srcode[g_pc];
  return ch;
}
char getNchar(void){
  char ch;
  if(st_srcode[g_pc] == '\0'){
    return '\0';
  }
  g_pc++;
  ch = st_srcode[g_pc];
  g_pc--;
  return ch;
}
void st_push(Token *tkn){
  g_sp++;
  g_stack[g_sp].token = *tkn;
}
Token *st_pop(void){
  Token *ret;
  ret = malloc(sizeof(Token));
  g_sp--;
  //stack *stk;
  //printf("p:%d\n", g_sp);
  if(g_sp <= 0){
    printf("STACK is EMPTY!\n");
    exit(1);
  }
  if(g_stack[g_sp].token.kind == OP_INT){
    ret->u.num = g_stack[g_sp].token.u.num;
  }
  else
  {
    //strcpy(ret->name, g_stack[g_sp].token.name);
  }
  ret->kind = g_stack[g_sp].token.kind;
  //g_sp--;
  return ret;
}
Token st_peek(void){
  //return g_stack->token;
  return g_stack[g_sp].token;
}
void skipWS(void){
  while(st_srcode[g_pc] == ' ' || st_srcode[g_pc] == '\t' || st_srcode[g_pc] == '\n'){
    g_pc++;
  }
}
int parsInt(void){
  int acc;
  acc = g_stack[g_sp].token.u.num;
  //st_pop();
  return acc;
}
int parsTerm(void){
  int acc;
  acc = parsInt();
  return acc;
}
int parsExpr(void){
  int acc;
  acc = parsTerm();
  return acc;
}
void exec_push(Token *tkn){
  g_sp++;
  g_stack[g_sp].token = *tkn;
}
Token *exec_pop(void){
  Token *ret;
  ret = malloc(sizeof(Token));
  g_sp--;
  //stack *stk;
  //printf("p:%d\n", g_sp);
  if(g_sp <= 0){
    printf("STACK is EMPTY!\n");
    exit(1);
  }
  ret = &g_stack[g_sp].token;
  return ret;
}
