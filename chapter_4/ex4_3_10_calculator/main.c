#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXLEN 100
#include "io.c"
#include "stack.c"
#include "var.c"

void push(double d);
double pop();
double peek();
void clear();
void swap();
int getop(char s[]);
void init_var_store();
int isvar(char c);
double getvar(char c);
int putvar(double d);

// reverse Polish calculator
int main()
{
  char s[MAXLEN];
  int type;
  double out, arg2;
  init_var_store();
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
      push((int)pop()%(int)arg2);
      break;
    case TOP:
      out = pop();
      printf("%c=%f\n" , putvar(out), out);
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
    default:
      if (isvar(s[0])) {
	push(getvar(s[0]));
      } else {
	printf("error unsupported character >%s<" ,s);
	return 1;
      }
    }
  }
}
