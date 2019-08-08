#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100
#define QNUM 2

enum ttype {NAME, POINTER, PARENS, BRACKETS, TQUAL};
const char *type_qualifier[QNUM] = {"const", "static"};

void dcl(void);
void dirdcl(void);
int pointer(void);
void type_qualifier_list(char *);
void parameter_type_list(char *);
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
   dcl: optional pointer direct-del
   direct-dcl: name
	       (del)
	       direct-dcl (optional parameter-type-list)
	       direct-dcl (optional name-list)
               direct-del [optional size]
   pointer: * optional type-qualifier-list
            * optional type-qualifier pointer
	   
   type-qualifier-list: type-qualifier
                        type-qualifier-list type-qualifier
           
 */
int main()
{
  int c;
  error = 0;
  while (gettoken() != EOF) {
    type_qualifier_list(datatype);
    strcat(datatype, token); // first comes datatype
    out[0] = '\0';
    dcl(); // parse rest of line
    if ((tokentype != '\n' && tokentype != EOF) || error ) {
      printf("syntax error unexpected tokentype %d token %s\n", tokentype, token);
      name[0] = out[0] = '\0';
      error = 0;
      // consume line
      while((c = getch()) != '\n' && c != EOF)
	;
      continue;
    }

    printf("%s: %s %s\n", name, out, datatype);
    datatype[0] = '\0';
  }
  return 0;
}

int pointer() {
  int count = 0;
  if (tokentype != POINTER)
    return count;

  int c;
  count++;
  if (gettoken() == POINTER)
    count += pointer();
  else if (tokentype == NAME) {
    // check if type qualifier
    type_qualifier_list(datatype);
    count += pointer();
  }
  return count;
}

void type_qualifier_list(char *datatype) {
  for (int i = 0; i < QNUM; i++)
    if (strcmp(token, type_qualifier[i]) == 0) {
      strcat(datatype, token);
      strcat(datatype, " ");
      if (gettoken() == NAME)
	  type_qualifier_list(datatype);
    }
}

// parse a declarator
void dcl() {
  int np = 0;
  if (gettoken() == POINTER)
    np = pointer();
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
  
  if ((type = gettoken()) == '(') {
    strcat(out, " function with arguments ");
    char args [MAXTOKEN];
    parameter_type_list(args);
    if (tokentype == ')') {
      strcat(out, " returning");
      type = gettoken();
    } else {
      error = 1;
      printf("error: dirdcl missing closing PARENS ')' got %c\n", type);
      return;
    }
  }
  
  while (type == PARENS || type == BRACKETS) {
    if (type == PARENS) {
      strcat(out, " function returning");
    } else {
      strcat(out, " array");
      strcat(out, token);
      strcat(out, " of");
    }
    type = gettoken();
  }
}

void parameter_type_list(char *args) {
  int type;
    if (tokentype == ',') {
      strcat(out, " and ");
      gettoken();
      parameter_type_list(args);
      return;
    } else if (tokentype == ')') {
      return;
    } else if (tokentype == '(') {
      gettoken();
    }

    args[0] = '\0';    
    if (tokentype == NAME) {
      type_qualifier_list(args);
      if (args[0] == '\0') {  // not qualifier
	strcat(args, token);
      } else if (tokentype == NAME) {
	strcat(args, token);
      }
      gettoken();
      if (tokentype == NAME) {
	strcat(args, token);
	gettoken();
      }
      if (tokentype == POINTER) {
	int np = pointer();
	while (np -- >0)
	  strcat(out, "pointer to ");
      }
      strcat(out, args);
    } else {
      error = 1;
      printf("paramenter parse error unexpected type >%c< token >%s<\n", tokentype, token);
    }
    parameter_type_list(args);
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
  } else if (c == '*') {
    return tokentype = POINTER;
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
