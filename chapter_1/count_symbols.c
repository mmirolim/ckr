#include <stdio.h>

int main()
{
  int c;
  long blanks, tabs, newlines;
  blanks = 0;
  tabs = 0;
  newlines = 0;

  while ((c = getchar()) != EOF) {
    if (c == ' ')
      ++blanks;
    if (c == '\n')
      ++newlines;
    if (c == '\t')
      ++tabs;
  }
  
  printf("blanks %ld, tabs %ld, newlines %ld\n", blanks, tabs, newlines);
}
