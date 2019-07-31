#include <stdio.h>
#include "../lib/lib.h"

void itoa1(int n, char s[]);
void itoa2(int n, char s[]);
void reverse(char s[]);
char *tobits_string(int x);

int main()
{
  int max = 2147483647;
  printf("max %d\n", max);
  int max_neg = -2147483648;
  printf("max neg %d\n", max_neg);
  char s[20];
  itoa1(max_neg, s);
  printf("itoa1(max_neg) = %s\n", s);
  itoa1(-0, s);
  printf("-0 %s\n", s);
  itoa2(max_neg, s);
  printf("itoa2(max_neg) = %s\n", s);
  printf("max_neg %d -max_neg %d\n" ,0, -0);
  printf("max     %s\nmax_neg %s\n", tobits_string(max), tobits_string(max_neg));
  printf("max+1     %d\nmax_neg+1 %d\n", max+1, max_neg+1);  
  printf("max+1     %s\nmax_neg+1 %s\n", tobits_string(max+1), tobits_string(max_neg+1));
  itoa1(-100, s);
  printf("itoa1(-100) = %s\n", s);
  itoa2(-100, s);
  printf("itoa2(-100) = %s\n", s);  
}
// can't handle the lasrgest negative number -(2^(wordsize-1))
void itoa1(int n, char s[]) {
  int i, sign;
  if ((sign = n) < 0)
    n = -n;
  i = 0;
  do
    s[i++] = n % 10 + '0';
  while ((n /= 10) > 0);

  if (sign < 0)
    s[i++] = '-';
  
  s[i] = '\0';
  reverse(s);
}
// another solution: unsigned can be used to store -n
void itoa2(int n, char s[]) {
  int i, sign;
  i = 0;
  if ((sign = n) < 0) {
    s[i++] = -(n % 10) + '0';    
    n /= 10;
    n = -n;
  }

  do
    s[i++] = n % 10 + '0';
  while ((n /= 10) > 0);

  if (sign < 0)
    s[i++] = '-';

  s[i] = '\0';
  reverse(s);
}
