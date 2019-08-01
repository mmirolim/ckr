#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXLEN 100
#include "io.c"
#include "stack.c"

void push(double d);
double pop();
double peek();
void clear();
void swap();
int getop(char s[]);

// reverse Polish calculator
int main()
{
  char s[MAXLEN];
  int type;
  double arg2;
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
      printf("%f\n" , pop());
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
      printf("error unsupported character >%s<" ,s);
      return 1;
    }
  }
}
