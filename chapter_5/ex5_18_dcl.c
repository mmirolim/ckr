#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum {NAME, PARENS, BRACKETS};

void dcl(void);
void dirdcl(void);
int getch(void);
int gettoken(void);
int error;
int tokentype; /* last token type */
char token[MAXTOKEN]; /*last token */
char name[MAXTOKEN]; /* identifier name */
char datatype[MAXTOKEN]; /* data type */
char out[1000]; /* output buffer */

// convert C declaration to words
/* simplified grammar
   del: optional *'s direct-del
   direct-dcl: name
	       (del)
	       direct-dcl ( )
               direct-del [optional size]
 */
int main()
{
  int c;
  error = 0;
  while (gettoken() != EOF) {
    strcpy(datatype, token); // first comes datatype
    out[0] = '\0';
    dcl(); // parse rest of line
    if (tokentype != '\n' || error) {
      printf("syntax error\n");
      name[0] = out[0] = '\0';
      error = 0;
      // consume line
      while((c = getch()) != '\n' && c != EOF)
	;
      continue;
    }

    printf("%s: %s %s\n", name, out, datatype);
  }
  return 0;
}
// parse a declarator
void dcl() {
  int np;
  for (np = 0; gettoken() == '*';) // count '*'
    np++;
  dirdcl();
  while (np-- > 0)
    strcat(out, " pointer to");
}
// parse a direct declarator
void dirdcl() {
  int type;
  if (tokentype == '(') {
    dcl();
    if (tokentype != ')') {
      error = 1;
      printf("error: missing )\n");
      return;
    }
  } else if (tokentype == NAME) {
    strcpy(name, token);
  } else {
    error = 1;
    printf("error: expected name or (dcl)\n");
    return;
  }

  while ((type = gettoken()) == PARENS || type == BRACKETS)
    if (type == PARENS)
      strcat(out, " function returning");
    else {
      strcat(out, " array");
      strcat(out, token);
      strcat(out, " of");
    }
}

int gettoken() {

  int c;
  void ungetch(int);
  char *p = token;
  // skip blanks
  while ((c = getch()) == ' ' || c == '\t')
    ;

  if (c == '(') {
    if ((c = getch()) == ')') {
      strcpy(token, "()");
      return tokentype = PARENS;
    } else {
      ungetch(c);
      return tokentype = '(';
    }
  } else if (c == '[') {
    for (*p++ = c; (*p++ = getch()) != ']';) // consume size
      ;
    *p = '\0';
    return tokentype = BRACKETS;
  } else if (isalpha(c)) {
    for (*p++ = c; isalnum(c = getch());)
      *p++ = c;
    *p = '\0';
    ungetch(c);
    return tokentype = NAME;
  } else {
    return tokentype = c;
  }
}

#define STACKSIZE 100
int stack[STACKSIZE];
int sp = 0;

int getch() {
  if (sp > 0) {
    return stack[--sp];
  }
  return getchar();
}

void ungetch(int c) {
  if (sp > STACKSIZE) {
    printf("error: stack size limit\n");
    return;
  }
  stack[sp++] = c;
}
