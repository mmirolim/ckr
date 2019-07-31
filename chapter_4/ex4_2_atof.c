#include <stdio.h>
#include <limits.h>
#include "../lib/lib.h"

double atof(char s[]);

int main() {
  char test_inputs[][10] = {{"  -7.001"}, {" +123a"}, {"32.07"}, {"7e-2"}, {"10E3"}};
  double test_ouputs[] = {-7.001, 123.0, 32.07, 0.07, 10000};

  int test_failed = 0;
  for (int i = 0; i < 5; ++i) {
    double out = atof(test_inputs[i]);
    if (out != test_ouputs[i]) {
      test_failed = 1;
      printf("case %d failed conv %s %f != %f\n" , i, test_inputs[i], out, test_ouputs[i]);
    }
  }
  if (!test_failed) {
    printf("all tests passed OK\n");
  }
}

int valid_char(char c) {
  return c == '.' || c == 'e' || c == 'E' || (c >= '0' && c <= '9');
}

// TODO handle if number is too big
double atof(char s[]) {
  int sign;
  int i = 0;
  // skip spaces
  while (s[i] == ' ')
    i++;

  sign = s[i] == '-' ? -1: 1;
  if (s[i] == '-' || s[i] == '+')
    i++;
  
  double numerator = 0.0;
  double denominator = 0.0;

  for (;s[i] != '\0' && s[i] != 'e' && s[i] != 'E'; i++) {
    if (!valid_char(s[i])) {
      printf("invalid char %c\n" , s[i]);
      return 0.0;
    }
    if (s[i] == '.') {
      denominator = 1.0;
      continue;      
    }
    numerator = numerator * 10 + s[i] - '0';
    denominator *= 10;
  }
  if (denominator == 0) {
    denominator = 1.0;
  }
  double res = sign * numerator / denominator;
  
  if (s[i] == 'e' || s[i] == 'E') {
    sign = s[++i] == '-' ? -1: 1;
    if (s[i] == '-' || s[i] == '+')
      i++;
  }
  double power = 1.0;
  numerator = 0.0;
  while (s[i] != '\0') {
    numerator = numerator * 10 + s[i] - '0';
    i++;
  }

  for (int p = 0; p < numerator; p++)
    power *= 10;

  if (sign > 0)
    res *= power;
  else
    res /= power;

  return res;
}
