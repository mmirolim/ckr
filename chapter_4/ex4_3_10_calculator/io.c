#include <ctype.h>
#include <stdio.h>
#define BUFSIZE 100
#define NUMBER 'n'
#define ADD '+'
#define SUB '-'
#define MUL '*'
#define DIV '/'
#define MOD '%'
#define TOP '\n'
#define PEEK 't'
#define CLEAR 'c'
#define SWAP 's'
#define SIN '1'
#define EXP '2'
#define POW '3'
#define VARASSIGN '$'

int getch(void);
void ungetch(char c);
int streq(char s1[], char s2[]);
void ungets(char s[]);

int getop(char s[]) {
  int i, c;
  // skip spaces
  while ((s[0] = c = getch()) == ' ' || c == '\t')
    ;

  s[1] = '\0';

  if (!isdigit(c) && c != '-') {
    // check for operators
    i = 0;
    if (c == TOP)
      return c; // do no need to read past newline

    // read word if not end of line
    while ((s[++i] = getch()) != ' ' && s[i] != '\n' && s[i] != EOF)
      ;
    ungetch(s[i]);
    s[i] = '\0';

    if (streq(s, "sin")) {
      c = SIN;
    } else if (streq(s, "pow")) {
      c = POW;
    } else if (streq(s, "exp")) {
      c = EXP;
    } else if (s[0] == VARASSIGN) {
      c = VARASSIGN;
      // rm varassing
      for (int j =1;s[j] != '\0'; j++)
	s[j-1] = s[j];
      s[i-1] = '\0';
    }
    return c;
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

void ungets(char s[]) {
  int i = 0;
  while (s[i++] != '\0')
    ;
  while (i > 0)
    ungetch(s[--i]);
}

int streq(char s1[], char s2[]) {
  int i, l1, l2;
  l1 = l2 = i = 0;
  while (s1[l1] != '\0')
    l1++;

  while (s2[l2] != '\0')
    l2++;

  if (l1 != l2)
    return 0;
  
  while (s1[i] == s2[i] && s1[i] != '\0')
    i++;
  if (i != l1)
    return 0;
  return 1;
}
