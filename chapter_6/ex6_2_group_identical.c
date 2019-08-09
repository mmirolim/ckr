/*
   Exercise 6-2 K&R 
   Write a program that reads a C program and prints in alphabetical order each group
   of variable names that are identical in the first 6 characters but different somewhere
   thereafter. Don't count words within strings and comments. Make 6 a parameter that 
   can be set from the command line.
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXWORDLIM 100

struct lnode {
  char *word;
  struct lnode *next;
};

struct tnode {
  char *cmp_word;
  struct tnode *left;
  struct tnode *right;
  struct lnode *words;
};

int getword(char *word, int lim);
void addtowords(struct lnode **, char *);
void addtotree(struct tnode **, char *);
void printtree(struct tnode *);
struct lnode *newlnode(char *);
struct tnode *newtnode(char *);

static int max_len_cmp = 3;

int main(int argc, char **argv)
{
  char word[MAXWORDLIM];
  int c;
  struct lnode *list, *node;
  struct tnode *root;

  // parse command -n
  while (--argc > 0 && (*++argv)[0] == '-')
    while ((c = *++argv[0]))
      switch (c) {
      case 'n':
	--argc;
	max_len_cmp = atoi(*++argv);
	break;
      default:
	printf("error: unsupported args %s\n", *argv);
	return 1;
      }
  while (getword(word, MAXWORDLIM) != EOF)
      addtotree(&root, word);

  printtree(root);
}

char *mystrdup(char *s, int size);
struct lnode *lalloc(void);
struct tnode *talloc(void);
int mystrcmp(char *s1, char *s2);

void addtotree(struct tnode **p, char *word) {
  int cond;
  if (*p == NULL) {
    // new word
    *p = newtnode(word);
  } else if ((cond = mystrcmp(word, (*p)->cmp_word)) == 0) {
    addtowords(&(*p)->words, word);
  } else if (cond < 0) {
    addtotree(&(*p)->left, word);
  } else {
    addtotree(&(*p)->right, word);
  }
}

void printlist(struct lnode *node) {
  for (; node != NULL; node = node->next)
    printf("%s->", node->word);
}

void printtree(struct tnode *node) {
  if (node != NULL) {
    printtree(node->left);
    printf("cmp_word: %6s::", node->cmp_word);
    printlist(node->words);
    printf("\n");
    printtree(node->right);
  }
}

// add unique words to list
void addtowords(struct lnode **words, char *word) {
  struct lnode *node = newlnode(word);
  if (words == NULL) {
    *words = node;
    return;
  }
  struct lnode *p, *last;
  p = *words;
  for (; p != NULL; p = p->next)
    if (strcmp(p->word, word) == 0)
      return;
    else
      last = p;
  
  if (p == NULL)
    last->next = node; // unique
}

struct lnode *lalloc() {
  return (struct lnode *) malloc(sizeof(struct lnode));
}

struct tnode *talloc() {
  return (struct tnode *) malloc(sizeof(struct tnode));
}
/* 
   getword: get next word or character from input
   handles underscore, string constants, comments
 */
int getword(char *word, int lim) {
  int c, getch();
  void ungetch(int);
  char *w = word;
  c = getch();
  while (!isalpha(c) && c != EOF) {
    // skip blanks
    while (isspace(c))
      c = getch();
    // skip strings
    if (c == '"')
      while ((c = getch()) != '"')
	;

    if (c == '/') {
      // skip comments
      if ((c = getch()) == '/') {
	while ((c = getch()) != '\n')
	  ;
      } else if  ( c == '*') {
	int prev = 0;
	for (; c != EOF;) {
          if ((c = getch()) == '/' && prev == '*') {
	    break;
          }
	  prev = c;
	}
      }
    }
    // exit or continue
    if (isalpha(c))
      break;
    else
      c = getch();
  }
  
  if (c != EOF)
    *w++ = c;
  
  if (!isalpha(c)) {
    *w = '\0';
    return c;
  }
  
  for (; --lim > 0; w++)
    if (!isalnum(*w = getch()) && *w != '_') {
      ungetch(*w);
      break;
    }

  *w = '\0';
  return word[0];
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

char *mystrdup(char *s, int size) {
  char *p;
  p = (char *) malloc(sizeof(size + 1));
  if (p != NULL)
    strncpy(p, s, size);
  return p;
}

struct lnode *newlnode(char *word) {
  struct lnode *node = lalloc();
  node->word = mystrdup(word, strlen(word));
  return node;
}

struct tnode *newtnode(char *word) {
  struct tnode *node = talloc();
  node->cmp_word = mystrdup(word, max_len_cmp);
  node->words = newlnode(word);
  node->left = node->right = NULL;
  return node;
}

int mystrcmp(char *s1, char *s2) {
  return strncmp(s1, s2, max_len_cmp);
}
