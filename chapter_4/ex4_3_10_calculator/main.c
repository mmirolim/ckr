#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "io.c"
#include "stack.c"
#include "var.c"
#define MAXLEN 100

void push(double d);
double pop();
double peek();
void clear();
void swap();
int getop(char s[]);
void init_var_store();
int isvar(char s[]);
double getvar(char s[]);
void putvar(char s[], double d);
void ungets(char s[]);

// reverse Polish calculator
int main()
{
  char s[MAXLEN];
  int type;
  double out, arg2;
  init_var_store();
  int last_op;
  while ((type = getop(s)) != EOF) {
    switch (type) {
    case NUMBER:
      push(atof(s));
      break;
    case ADD:
      push(pop()+pop());
      break;
    case SUB:
      arg2 = pop();
      push(pop() - arg2);
      break;
    case MUL:
      push(pop()*pop());
      break;
    case DIV:
      arg2 = pop();
      if (arg2 == 0.0) {
	printf("error division by zero\n");
	return 1;
      }
      push(pop()/ arg2);
      break;
    case MOD:
      arg2 = pop();
      if (arg2 == 0.0) {
	printf("error modulus is zero\n");
	return 1;
      }
      push(fmod(pop(),arg2));
      break;
    case TOP:
      if (last_op == VARASSIGN) 
	break; // do not store result form var assign
      
      out = pop();
      putvar(LASTOUTVAR, out);
      printf("%s=%f\n" , LASTOUTVAR , out);
      break;
    case PEEK:
      printf("%f\n", peek());
      break;
    case CLEAR:
      clear();
      break;
    case SWAP:
      swap();
      break;
    case SIN:
      push(sin(pop()));
      break;
    case EXP:
      push(exp(pop()));
      break;
    case POW:
      arg2 = pop();
      push(pow(pop(), arg2));
      break;
    case VARASSIGN:
      arg2 = pop();
      if (valid_var_name(s)) {
	putvar(s, arg2);
	printf("%c=%f\n" , s[0] , arg2);	
      }
      else
	printf("error unsupported character in variable assign >%s<" ,s);
      break;
    default:
      if (isvar(s)) {
	push(getvar(s));
      } else {
	printf("error unsupported character >%s<" ,s);
	return 1;
      }
    }
    last_op = type;
  }
}
