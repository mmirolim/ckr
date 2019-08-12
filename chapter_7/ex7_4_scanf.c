#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#define MAXARGSIZE 100

int myscanf(char *fmt, ...);
int main()
{
  double f;
  int d, matches;
  char word[MAXARGSIZE];
  while ((matches = myscanf("d %f %d-%s\n", &f, &d, word)) == 3)
    printf("double %f, int %d, string %s\n", f, d, word);
  printf("matches %d\n", matches);
  return 0;
}
void skipblanks(void);
void readstring(char *);
void readnumber(char *);
int getch(void);
void ungetch(int);

int myscanf(char *fmt, ...) {
  va_list ap;
  char *p, *sval;
  double *dval;
  int *ival, c, matches;
  matches = 0;
  char str[MAXARGSIZE];
  va_start(ap, fmt); /* make ap point to 1st unnamed arg */
  for (c = getch(), p = fmt; *p && c != EOF; p++, c = getch()) {
    while (isspace(*p) && *p != '\n') // TODO handle in format
      p++;
    if (isspace(c))
      skipblanks();
    if (*p != '%') {
      if (*p != c) {
       printf("error wrong format expecting >%c<, got >%c<\n", *p, c);
       return matches;
      }
      continue;
    }
    // %
    if (!isspace(c))
      ungetch(c);
    switch (*++p) {
    case 'f':
      readnumber(str);
      dval = va_arg(ap, double *);
      *dval = atof(str);
      matches++;
      break;
    case 'd':
      readnumber(str);
      ival = va_arg(ap, int *);
      *ival = atoi(str);
      matches++;
      break;
    case 's':
      sval = va_arg(ap, char *);
      readstring(sval);
      matches++;
      break;
    default:
      printf("myscanf: error unhandle fmt char %c\n", *p);
      va_end(ap);
      return c;
    }
  }
  ungetch(c);
  va_end(ap);
  return matches;
}

void skipblanks() {
  int c;
  while(isspace(c = getch()) && c != '\n')
	;
  ungetch(c);
}

void readstring(char *word) {
  int c;
  while (!isspace(c = getch()))
    *word++ = c;
  *word = '\0';
  ungetch(c);
}

void readnumber(char *word) {
  int c;
  if ((c = getch())  == '-') {
    *word++ = c;
    c = getch();
  }

  while (isdigit(c) || c == '.') {
    *word++ = c;
    c = getch();
  }
  *word = '\0';
  ungetch(c);
}

#define BUFSIZE 100
int stack[BUFSIZE];
int sp = 0; // stack pointer

int getch() {
  if (sp > 0)
    return stack[--sp];
  return getchar();
}

void ungetch(int c) {
  if (sp >= BUFSIZE) {
    printf("ungetch error: buffer full");
    return;
  }
  stack[sp++] = c;
}
