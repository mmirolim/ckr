#include <stdio.h>
#include <ctype.h>
#define UNDEFOP 0
#define MATHOP 1
#define NUMBER 2

void push(double d);
double pop();
int getop(char arg[]);
double atof(char arg[]);

int main(int argc, char *argv[])
{
  if (argc == 1) {
    printf("Usage: expr 4 3 4 + -");
    return 1;
  }

  double d;
  char arg[10];
  while (--argc)
    switch (getop(*++argv)) {
    case MATHOP:
      switch (*argv[0]) {
      case '+':
	push(pop() + pop());
	break;
      case '-':
	d = pop();
	push(pop() - d);
	break;
      default:
	printf("undefined math operation %s\n", argv[0]);
	return 1;
      }
      break;
    case NUMBER:
      d = atof(*argv);
      push(d);
      break;
    default:
      printf("undefined operation %s\n", *argv);
      return 1;
    }

  printf("= %g\n", pop());
}

double atof(char arg[]) {
  double numerator, denominator;
  int sign;
  if (*arg == '-') {
    sign = -1;
    arg++;
  } else {
    sign = 1;
  }

  numerator = denominator = 0;
  while (isdigit(*arg) || *arg == '.') {
    if (*arg == '.') {
      denominator = 1;
      arg++;
      continue;
    }
    numerator = 10 * numerator + *arg - '0';
    denominator *= 10;
    arg++;
  }
  if (!denominator)
    denominator = 1.0;

  return sign * numerator/ denominator;
}

int getop(char arg[]) {
  if (isdigit(arg[0]))
    return NUMBER;

  if (*arg == '-' && isdigit(*++arg))
      return NUMBER;

  return MATHOP;
}

double stack[10];
double *sp = &stack[0];

void push(double d) {
  *sp++ = d;
}

double pop() {
  return *--sp;
}
