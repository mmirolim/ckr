#include <stdio.h>
#include <ctype.h>

int getfloat(double *f);
int getch(void);
void ungetch(char c);

int main()
{
  double f;
  for (int i = 0; i < 10 && getfloat(&f) != EOF; i++)
    printf("float %f\n", f);
}

int getfloat(double *pn) {
  int c, sign;
  while (isspace(c = getch()))
    ;
  if (!isdigit(c) && c != '+' && c != '-') {
    /* not a number */
    if (c != EOF) {
      ungetch(c);
      return 0;
    }
    return EOF; 
  }
  sign = (c == '-') ? -1 : 1;
  if (c != '-' && c != '+')
    ungetch(c);
      
  double numenator, denominator;
  numenator = denominator = 0.0;
  while ((c = getch()) != ' ' && c != '\n' && c != EOF) {
    if (c == '.') {
      denominator = 1.0;
      continue;
    }
    if (!isdigit(c))
      return 0; /* not a number */
    numenator = 10 * numenator + (c - '0');
    denominator *= 10;
  }
  if (c != EOF)
    ungetch(c);
  *pn = (denominator != 0) ? sign * numenator/denominator : sign * numenator;
  return 0;
}
#define BUFSIZE 100

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
