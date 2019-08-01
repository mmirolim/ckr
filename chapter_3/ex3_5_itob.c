#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/lib.h"

extern int assert_str(char s1[], char s2[]);
void itob(int n, char s[], int b);

int main()
{
  char s[20];
  int n = -192;
  itob(n, s, 2);
  assert_str("-11000000", s);
  itob(n, s, 3);
  assert_str("-21010", s);
  itob(n, s, 10);
  assert_str("-192", s);
  itob(n, s, 16);
  assert_str("-C0",s);
}

void itob(int n, char s[], int b) {
  if (b < 2 || b > 36) {
    fprintf(stderr, "does not support base %d\n", b);
    exit(EXIT_FAILURE);
  }
  
  int i, d, sign;
  unsigned n2;
  if ((sign = n) < 0)
    n2 = -n;
  else
    n2 = n;

  for (i = 0; n2 > 0; n2 /= b, i++) {
    d = n2 % b;
    s[i] = d > 10 ? d + 'A' - 10 : d + '0';
  }

  if (sign < 0)
    s[i++] = '-';
  
  s[i] = '\0';
  reverse(s);
}
