#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/lib.h"

void itoa(int n, char s[], int p);

int main()
{
  char s[10];
  int n = -192;
  itoa(n, s, 2);
  assert_str("-192", s);
  itoa(n, s, 5);
  assert_str(" -192", s);
  itoa(n, s, 6);
  assert_str("  -192", s);
}

void itoa(int n, char s[], int p) {
  int i, sign;
  unsigned n2;
  
  if ((sign = n) < 0)
    n2 = -n;
  else
    n2 = n;
  i = 0;
  do
    s[i++] = n2 % 10 + '0';
  while (n2 /= 10);

  if (sign < 0)
    s[i++] = '-';

  while( i < p)
    s[i++] = ' ';
  
  s[i] = '\0';
  reverse(s);
}
