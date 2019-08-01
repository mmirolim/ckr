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
    printf("pop: stack is empty\n");
    return 0;
  } 
  return stack[--sp];
}

double peek() {
  return stack[sp-1];
}

void clear() {
  sp = 0;
}

void swap() {
  double d;
  if (sp < 2) {
    printf("swap: error not enough elements to swap\n");
    return;
  }
  d = stack[sp-1];
  stack[sp -1] = stack[sp-2];
  stack[sp -2] = d;
}
