#include <stdio.h>
#define STACKSIZE 100

double stack[STACKSIZE];
int sp = 0;

void push(double d) {
  if (sp > STACKSIZE)
    printf("push: stack is full\n");
  else
    stack[sp++] = d;
}

double pop() {
  if (sp == 0) {
    printf("pop: stack is empty");
    return 0;
  } 
  return stack[--sp];
}
