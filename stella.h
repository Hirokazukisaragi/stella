#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR 1028
typedef enum{
  FALSE = 0,
  TRUE,
}BOOL;
BOOL bflag;
BOOL iflag;
BOOL stateflag;
typedef enum{
  OP_ADD=1,
  OP_INT,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_EQL,
  OP_ASSIGN,
  OP_NOTEQ,
  OP_GT,
  OP_LT,
  END_OF_OP,
  OP_BEGIN,
  OP_THEN,
  OP_TRUE,
  OP_ELSE,
  OP_PUSH,
  OP_POP,
  OP_PRINT,
  OP_RET,
  OP_IF,
  OP_FUNCD,
  OP_TURE,
  OP_FALSE,
  OP_END_OF_FILE,
  OP_NOTKW,
  OP_NOP,
  OP_EOP,
  OP_NIL,
}TokenKind;

typedef struct OPinfo{
  char *opname;
  TokenKind kind;
}OPinfo;
typedef struct KWinfo{
  char *tkname;
  TokenKind kind;
}KWinfo;
KWinfo g_keywordtbl[] = {
  {"if",OP_IF},
  {"else",OP_ELSE},
  {"then",OP_THEN},
  {"return",OP_RET},
  {"fn",OP_FUNCD},
  {"print",OP_PRINT},
  {"END_OF_KW",OP_NOP},
};
OPinfo g_optbl[] = {
  {"+",OP_ADD},
  {"-",OP_SUB},
  {"*",OP_MUL},
  {"/",OP_DIV},
  {"=",OP_ASSIGN},
  {"<",OP_LT},
  {">",OP_GT},
  {"==",OP_EQL},
  {"!",OP_NOTEQ},
  {"OP_END_OF_FILE",OP_END_OF_FILE},
  {"END_OF_OP",OP_EOP},
};
typedef struct{
  char name[255];
  TokenKind kind;
  union {
    int num;
    char str[MAX_STR];
  }u;
}Token;
typedef struct stack{
  Token token;
  //struct stack *next;
  //struct stack *prev;
  int current;
}stack;
void initial(FILE *fp);
Token *getNToken(void);
void parsing(void);
void exec_vm(void);
BOOL is_al(char ch);
char getCurrent(void);
char getNchar(void);
void st_push(Token *tkn);
Token *st_pop(void);
Token st_peek(void);
void skipWS(void);
int parsInt(void);
int parsTerm(void);
int parsExpr(void);
void exec_push(Token *tkn);
Token *exec_pop(void);
