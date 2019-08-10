/*
  Exercise 6-6. Implement a simple version of the #define processor (i.e., no
  arguments) suitable for use with C programs, based on the routines of this sec-
  tion. You may also find getch and ungetch helpful.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEFMAXLEN 100
#define HASHSIZE 101
#define MAXLINELEN 1000

struct lnode {
  char *name, *defn;
  struct lnode *next;
};

struct lnode *set(char *, char *);
struct lnode *get(char *);
void del(char *);
void printtable();
int isdefine(char *word);
int isundef(char *word);
int getch(void);
void ungetch(int);
int skipnonewords(void);
void readword(char *);
void putword(char *);
unsigned hash(char *);
void rpword(char *); /* reads and puts word */
static struct lnode *hashtable[HASHSIZE]; /* pointer to table */

int main()
{
  int c;
  struct lnode *def;
  char line[MAXLINELEN];
  char word1[DEFMAXLEN];
  char word2[DEFMAXLEN];
  while ((c = skipnonewords()) != EOF) {
    if (c == '#') {
      putchar(c);
      rpword(word1);
      if (isdefine(word1)) {
	rpword(word1);
	rpword(word2);
        if (set(word1, word2) == NULL) {
	  printf("set failed\n");
	  return 1;
        }
      } else if (isundef(word1)) {
	rpword(word2);
	del(word2);
      }
    } else if (isalpha(c)) {
      ungetch(c);
      // possible defname
      readword(word1);
      if ((def = get(word1)) != NULL) {
	putword(def->defn);
      } else
	putword(word1);
    } else {
      putchar(c);
    }
  }
  
  //printtable();
}
void printtable() {
  struct lnode *np;
  for (int i = 0; i < HASHSIZE; i++)
    if ((np = hashtable[i]) != NULL)
      for(printf("\nid %d::", i); np != NULL; np = np->next)
	printf("(%s, %s)->", np->name, np->defn);
  putchar('\n');
}

int isdefine(char *word) {
  return strcmp("define", word) == 0;
}

int isundef(char *word) {
  return strcmp("undef", word) == 0;
}

/* hash: form hash value for string s */
unsigned hash(char *s) {
  unsigned hashval;
  for (hashval = 0; *s != '\0'; s++)
    hashval = *s + 31*hashval;
  return hashval%HASHSIZE;
}
/* get: gets pointer to definition from hashtable */
struct lnode *get(char *name) {
  struct lnode *np;
  for (np = hashtable[hash(name)]; np != NULL; np = np->next)
    if (strcmp(np->name, name) == 0)
      return np; // found
  return NULL; // no found
}

/* set: sets (name, defn) in hashtable, returns NULL on error */
struct lnode *set(char *name, char *defn) {
  struct lnode *np;
  unsigned hashval;
  if ((np = get(name)) == NULL) { /* not found */
    np = (struct lnode *) malloc(sizeof(*np));
    // check for errors
    if (np == NULL || (np->name = strdup(name)) == NULL)
      return NULL;
    hashval = hash(name);
    np->next = hashtable[hashval];
    hashtable[hashval] = np;
  } else {
    // already exists
    free((void *) np->defn); // clear prev def
  }
  // set/update defn value
  if ((np->defn = strdup(defn)) == NULL)
    return NULL;
  
  return np;
}
/* del: deletes def from hashtable */
void del(char *name) {
  unsigned hashval = hash(name);
  struct lnode *np, *prev;
  np = prev = NULL;
  for (np = hashtable[hashval]; np != NULL; np = np->next)
    if (strcmp(np->name, name) == 0) {
      // remove from list
      if (prev == NULL) // head
	hashtable[hashval] = np->next;
      else
	prev->next = np->next;
      // free memory
      free((void *) np->defn);
      free((void *) np->name);
      free(np);
    } else {
      prev = np;
    }
}
int skipnonewords() {
  int c, instring, incomment;
  instring = incomment = 0;
  int commentch, prev;
  // skip spaces, strings and comments
  for (c = getch(); (!isalnum(c) && c != '_' && c != '#' && c != EOF) || instring || incomment;
       c = getch()) {
    putchar(c); // put everything out
    if (c == '"' && !instring && !incomment)
      instring = 1;
    if (c == '"' && instring && prev != '\\')
      instring = 0;
    if (c == '/') {
      if ((c = getch()) == '/' && !instring && !incomment)
	incomment = 1, commentch = '/';
      else if (c == '*' && !instring && !incomment)
	incomment = 1, commentch = '*';
      else  { // handle divide '/
	ungetch(c);
	continue; 
      }
      putchar(c);
    }
    if (c == '\n' && incomment && commentch == '/')
      incomment = 0;
    if (c == '*' && incomment) {
      if ((c = getch()) == '/' && commentch == '*')
	incomment = 0;
      putchar(c);
    }
    prev = c;
  }

  return c;
}

void readword(char *word) {
  int c, wlen;
  word[0] = '\0';
  wlen = 0;
  c = skipnonewords();
  for (; isalnum(c) || c == '_'; c = getch()) {
    if (wlen == DEFMAXLEN-1) {
      printf("readword: error word to big\n");
      break;
    }
    word[wlen++] = c;
  }
  word[wlen] = '\0';
  ungetch(c);
}

void putword(char *word) {
  printf("%s", word);
}

void rpword(char *word) {
  readword(word);
  putword(word);
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


