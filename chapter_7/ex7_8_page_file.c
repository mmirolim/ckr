/*
  Exercise 7-8. Write a program to print a set of files, starting each new one on a
  new page, with a title and a running page count for each file.
*/
#include <stdio.h>
#include <stdlib.h>

void pagefile(FILE *, char *, int);

int main(int argc, char **argv)
{
  FILE *fp;
  int lines_per_page = 10;
  while (--argc > 0)
    if ((fp = fopen(*++argv, "r")) == NULL) {
      fprintf(stderr, "error: can't open file %s\n", *argv);
      exit(1);
    } else
      pagefile(fp, *argv, 10);

  if (ferror(stdout)) {
    fprintf(stderr, "error: writing to stdout\n");
    exit(2);
  }
  return 0;
}

void pagefile(FILE *fp, char *fname, int lpp) {
  int pn = 1;
  fprintf(stdout, "\nfile - %s\n", fname);
  for (int c = getc(fp); c != EOF; c = getc(fp))
    if (c == '\n') {
      pn++;
      if (pn%lpp == 0)
	fprintf(stdout, "\n%s page %d\n", fname, pn/lpp);
      else
	putc(c, stdout);
    } else
      putc(c, stdout);
  
  if (pn%lpp != 0) // print for last page
    fprintf(stdout, "\n%s page %d\n", fname, pn/lpp + 1);
}
