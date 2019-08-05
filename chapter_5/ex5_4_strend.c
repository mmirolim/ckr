#include <stdio.h>

int strend(char *s, char *t);

int main()
{
  char s[] ="longwordwithsuffix";
  char t1[] ="suffix";
  char t2[] ="notsuffix";
  printf("strend(s, t1) = %d\n", strend(s, t1));
  printf("strend(s, t1) = %d\n", strend(s, t2));
}

int strend(char *s, char *t) {
  while(*s)
    s++;
  int tlen = 0;
  while(*t)
    tlen++, t++;
  
  for (; *s == *t; s--, t--, tlen--)
    if (tlen == 0)
      return 1;

  return 0;
}
