#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum {NAME, PARENS, BRACKETS};

void undcl(void);
int getch(void);
void ungetch(int);
int gettoken(void);
int error;
int tokentype; /* last token type */
char token[MAXTOKEN]; /*last token */
char name[MAXTOKEN]; /* identifier name */
char datatype[MAXTOKEN]; /* data type */
char out[1000]; /* output buffer */

// undcl converting x () * [] * () char =>char (*(*x())[])()
int main()
{
  int type, c;
  char temp[MAXTOKEN];
  int prev_type;
  error = 0;
  while (gettoken() != EOF) {
    strcpy(out, token);
    while ((type = gettoken()) != '\n' && type != EOF) {
      if (type == PARENS || type == BRACKETS) {
	strcat(out, token);
      } else if (type == '*') {
	// TODO add check for pointer
        sprintf(temp, "(*%s)", out);
	strcpy(out, temp);
      } else if (type == NAME) {
        if (prev_type == '*') {
	  // strip redundant () from out
	  int i;
	  for (i = 1; out[i] != '\0'; i++)
	    out[i-1] = out[i];
	  out[i-2] = '\0';
        }
        sprintf(temp, "%s %s", token, out);
	strcpy(out, temp);
      } else {
	printf("error: invalid input at %s\n", token);
	error = 1;
	break;
      }
      prev_type = type;
    }
    if (!error)
      printf("%s\n", out);
    else {
      while ((c = getch()) != '\n' && c != EOF) // consume invalid line
	;
      error = 0;
    }
  }
  return 0;
}
int gettoken() {

  int c;
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

