#include <stdio.h>

int main()
{
  int c;
  int prev;
  prev = -1;
  while ((c = getchar()) != EOF) {
    if (c == ' ' && prev == ' ')
      continue;
    putchar(c);
    prev = c;
  }
}
