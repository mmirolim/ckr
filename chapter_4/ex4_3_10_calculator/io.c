#include <ctype.h>
#include <stdio.h>
#define BUFSIZE 100
#define NUMBER 'n'

int getch(void);
void ungetch(char c);

int getop(char s[]) {
  int i, c;
  // skip spaces
  while ((s[0] = c = getch()) == ' ' || c == '\t')
    ;
  s[1] = '\0';

  if (!isdigit(c) && c != '-') {
    return c; // not a number
  } else if (c == '-') {
    // check if '-' is op or part of number
    c = getch();
    ungetch(c);
    if (!isdigit(c))
      return '-';
  }

  i = 0; // init i
  while ((s[++i]= c = getch()) == '.' || isdigit(c))
    ;
  
  s[i] = '\0';

  if (c != EOF)
    ungetch(c);

  return NUMBER;
}

char buf[BUFSIZE]; // buffer for getch/ungetch
int bufp = 0; // pointer to first element in buffer

int getch(){
  return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(char c) {
  if (bufp >= BUFSIZE)
    printf("ungetch: too many characters\n");
  else
    buf[bufp++] = c;
}
