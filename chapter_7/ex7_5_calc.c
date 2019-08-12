/*
  Rewrite the postfix calculator of Chapter 4 to use scanf and/or sscanf to do the input and number conversion.
*/

#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAXLEN 100
#define STACKSIZE 100

void push(double);
double pop();

int main()
{
  char op[MAXLEN];
  char str[MAXLEN];
  char c = '\0';
  double d;
  while (scanf("%s%c", str, &c) == 2) { // read str and char after
    if (sscanf(str, "%lf", &d) == 1) // check conversion to number
      push(d);
    else if (sscanf(str, "%s", op)){
      if (strcmp(op, "+") == 0) {
	push(pop() + pop());
      } else if (strcmp(op, "-") == 0) {
	d = pop();
	push(pop() - d);
      } else if (strcmp(op, "*") == 0) {
	push(pop() * pop());
      } else if (strcmp(op, "/") == 0) {
	d = pop();
        if (d == 0) {
	  printf("division by zero\n");
	  break;
        }
	push(pop()/ d);
      } else if (strcmp(op, "sin") == 0) {
	push(sin(pop()));
      } 
      if ( c == '\n')
	printf("= %f\n", pop());
    }
  }

}

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
