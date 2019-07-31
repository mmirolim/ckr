#include <stdio.h>

void reverse(char[]);

int main()
{
  char s1[] = "hello world";
  printf("%s\n", s1);
  reverse(s1);
  printf("%s\n", s1);
}

void reverse(char s[])
{
  int i, len, temp;
  len = 0;
  while (s[len] != '\0')
    ++len;
  printf("len %d\n", len);
  for (i = 0; i < (len-1)/2; ++i) {
    temp = s[i];
    s[i] = s[len-1-i];
    s[len-1-i] = temp;
  }
}
