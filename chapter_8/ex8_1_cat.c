/*
   Exercise 8-1. Rewrite the program cat from Chapter 7 using read, write,
   open and close instead of their standard library equivalents. Perform experi-
   ments to determine the relative speeds of the two versions.
*/
#include <stdio.h>
#include <stdarg.h>
#include <syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void error(char *, ...);

int main(int argc, char *argv[])
{
  int f, n;
  char buf [BUFSIZ];
  if (argc == 1)
    error("usage: file file ...");
  while (--argc > 0) {
    if ((f = open(*++argv, O_RDONLY)) == -1)
      error("cat: can't open %s", *argv);
    else
      while ((n = read(f, buf, BUFSIZ)) > 0)
	if (write(1, buf, n) != n)
	  error("cat: write to stdout");
    close(f);
  }
  return 0;
}

void error(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "error: ");
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");
  va_end(args);
  exit(1);
}
