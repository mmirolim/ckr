#include <stdio.h>
#include <stdlib.h>
#define MAXLEN 100
#include "io.c"
#include "stack.c"

void push(double d);
double pop();
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
    case '+':
      push(pop()+pop());
      break;
    case '-':
      arg2 = pop();
      push(pop() - arg2);
      break;
    case '*':
      push(pop()*pop());
      break;
    case '/':
      arg2 = pop();
      if (arg2 == 0.0) {
	printf("error division by zero\n");
	return 1;
      }
      push(pop()/ arg2);
      break;
    case '%':
      arg2 = pop();
      if (arg2 == 0.0) {
	printf("error modulus is zero\n");
	return 1;
      }
      push((int)pop()%(int)arg2);
      break;
    case '\n':
      printf("%f\n" , pop());
      break;
    default:
      printf("error unsupported character >%s<" ,s);
      return 1;
    }
  }
}
