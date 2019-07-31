#include <stdio.h>
#include <stdlib.h>

int setbits(int x, int p, int n, int y);
int invert(int x, int p, int n);
int rightrot(int x, int n);
char *tobits_string(int x);
int bitcount(unsigned x);

int main()
{
  int x, p, n, y;
  x = 0b0110101;
  y = 0b0101111;
  p = 2;
  n = 3;
  printf("%s = setbits %s at %d for %d from %s\n", tobits_string(setbits(x, p, n, y)), tobits_string(x), p, n, tobits_string(y));
  printf("%s = invert %s at %d for %d\n", tobits_string(invert(x,p,n)), tobits_string(x), p, n);
  printf("%s = rightrot %s at %d to %d\n", tobits_string(rightrot(x,n)), tobits_string(x), p, n);
  printf("two's complement number systemx %s, x&(x-1) %s\n", tobits_string(x), tobits_string(x&(x-1)));
  printf(" x %s\n-x %s\n", tobits_string(x), tobits_string(-x));
  printf("x %s bitcount %d\n", tobits_string(x), bitcount(x));
  printf("y %s bitcount %d\n", tobits_string(y), bitcount(y));
}

int bitcount(unsigned x) {
  int b = 0;
  while (x != 0) {
    x&=(x-1);
    ++b;
  }
  return b;
}

int rightrot(int x, int n) {
  return (x>>n) | (x <<((sizeof(x)<<3)-n));
}

int invert(int x, int p, int n) {
  return x ^ (~(~0 << n) << (p+1-n));
}

int setbits(int x, int p, int n, int y) {
  return (x & ~(~(~0 <<n)<<(p+1-n))) | (y & ~(~0 << n)) << (p+1-n);
}

int setbits_with_loop(int x, int p, int n, int y) {
  int res = x;
  for (int i = 0; i < n; ++i)
    res = res | ((y >>i)&1)<<(p+1-(n-i));
  return res;
}

char *tobits_string(int x)
{
  char *s = malloc((sizeof(x)<<3)+1);
  int i, count;
  count = sizeof(x)<<3;
  for (i = 0; i < count; i++)
    s[i] = '0' | (((x>>(count -i-1)))&1);
  s[count] = '\0';
  return s;
}
