#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLEN 1000
#define DEFAULTSLINES 10

int readline(char line[]);
int matoi(char s[]);
void mstrcpy(char *d, char s[]);

int main(int argc, char *argv[])
{
  // default tail length
  int num_lines = DEFAULTSLINES;
  char **ptrtolines;
  char line[MAXLEN];

  while (--argc > 0 && (*++argv)[0] == '-') {
    switch ((*argv)[1]) {
    case 'n':
      // read number
      --argc;
      num_lines = matoi(*++argv);
      break;
    default:
      printf("undefined arg >%c<\n", (*argv)[1]);
      return 1;
    }
  }

  int p = 0;
  ptrtolines = malloc(sizeof(*ptrtolines) * num_lines);
  if (!ptrtolines) {
    printf("out of memory for lines\n");
    return 1;
  }
  for (int i = 0; i < num_lines; i++) {
    ptrtolines[i] = malloc(sizeof(**ptrtolines) * MAXLEN);
    ptrtolines[i][0] = '\0';
  }

  while (readline(line) != EOF) {
    mstrcpy(ptrtolines[p++%num_lines], line);
  }

  if (p > num_lines)
    p -= num_lines; // TODO check when they are equal
  else
    p = 0;

  for (int i = 0; i < num_lines; i++) {
    if (ptrtolines[p%num_lines][0] != '\0')
      printf("%s\n", ptrtolines[p%num_lines]);
    p++;
  }
}

int readline(char line[]) {
  char c;
  int len = 0;
  while ((c = getchar()) != '\n' && c != EOF) {
    if (len > MAXLEN) {
      printf("line too long trimming");
      break;
    }
    *line++ = c;
    len++;
  }
  *line = '\0';
  return c;
}

void mstrcpy(char *d, char s[]) {
  while (*s != '\0') {
    *d++ = *s++;
  }
  *d = '\0';
}

int matoi(char *s) {
  int n = 0;
  while (isdigit(*s)) {
    n = 10 * n + (*s - '0');
    s++;
  }

  return n;
}
