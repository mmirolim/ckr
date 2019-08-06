#include <stdio.h>
#include <ctype.h>

#define MAXLEN 1000
#define MAXLINES 1000

int readline(char line[]);
int atoi(char s[]);
void mstrcpy(char d[], char s[]);

int main(int argc, char *argv[])
{
  // default tail length
  int n = 10;
  char lineptr[MAXLINES][MAXLEN];
  int p;
  char line[MAXLEN];

  while (--argc > 0 && (*++argv)[0] == '-') {
    switch ((*argv)[1]) {
    case 'n':
      // read number
      --argc;
      n = atoi(*++argv);
      break;
    default:
      printf("undefined arg >%c<\n", (*argv)[1]);
      return 1;
    }
  }
  
  if (n > MAXLINES) {
    printf("error: max lines %d exceeded\n", MAXLINES);
    return 1;
  }
  while (readline(line) != EOF) {
    mstrcpy(lineptr[p++%n], line);
  }
  p -= n;
  for (int i = 0; i < n; i++)
    printf("%s\n", lineptr[p++%n]);
}

int readline(char line[]) {
  char c;
  while ((c = getchar()) != '\n' && c != EOF) {
    *line++ = c;
  }
  *line = '\0';
  return c;
}

void mstrcpy(char d[], char s[]) {
  while (*s != '\0')
    *d++ = *s++;
  *d = '\0';
}

int atoi(char *s) {
  int n = 0;
  while (isdigit(*s)) {
    n = 10 * n + (*s - '0');
    s++;
  }

  return n;
}
