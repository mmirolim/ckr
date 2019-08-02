#include <stdio.h>

void itoa(int n, char s[]);
  
int main()
{
  char s[60];
  itoa(0, s);
  printf("itoa 0 = %s\n", s);
  
  itoa(10, s);
  printf("itoa 10 = %s\n", s);
  
  itoa(123, s);
  printf("itoa 123 = %s\n", s);

  itoa(-123, s);
  printf("itoa -123 = %s\n", s);
  int max_neg = -2147483648;
  itoa(max_neg, s);
  printf("itoa -2147483648 = %s\n", s);
}


int itoa_recur(unsigned n, int start, int id, char s[]) {
  int d = n%10;
  n = n/10;
  if (n == 0) {
    s[start] = d + '0';
    s[id + 1] = '\0';
    return id;
  }
  int len = itoa_recur(n, start, id + 1, s);
  s[len - id + start] = d + '0';
  return len;
}

void itoa(int n, char s[]) {
  unsigned n2;
  int i = 0;
  if (n < 0) {
    n2 = -n;
    s[i++] = '-';
  } else {
    n2 = n;
  }

  itoa_recur(n2, i, 0, s);
}
