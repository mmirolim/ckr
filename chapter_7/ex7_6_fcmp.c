/*
  Exercise 7-6. Write a program to compare two files, printing the first line
  where they differ.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1000

int main(int argc, char **argv)
{
  if (argc != 3) {
    printf("usage: f1name f2name");
    exit(1);
  }
  char *prog = argv[0];
  char *f1name, *f2name;
  FILE *f1, *f2;
  f1 = fopen((f1name = *++argv), "r");
  f2 = fopen((f2name = *++argv), "r");
  if (f1 == NULL || f2 == NULL) {
    fprintf(stderr, "%s error: can't open files %s %s\n", prog, f1name, f2name);
    exit(1);
  }
  int nl = 1;
  char f1line[MAXLEN], f2line[MAXLEN];
  for (fgets(f1line, MAXLEN, f1),fgets(f2line, MAXLEN, f2);
       strcmp(f1line, f2line) == 0;
       fgets(f1line, MAXLEN, f1), fgets(f2line, MAXLEN, f2), nl++)  
    if (feof(f1))
      return 0;

  printf("%d:%s%d:%s", nl, f1line, nl, f2line);
}
