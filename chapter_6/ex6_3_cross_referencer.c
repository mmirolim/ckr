/*
  Exercise 6-3. Write a cross-referencer that prints a list of all words in a docu-
  ment, and, for each word, a list of the line numbers on which it occurs. Remove
  noise words like "the," "and," and so on. 0
*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORDLEN 100
#define EXCLEN (sizeof exclude_words / sizeof(char *))

char *exclude_words[] = {"a", "an", "of", "on", "so" ,
			 "in", "for", "the", "and"};

struct lnode {
  unsigned num;
  struct lnode *next;
};

struct wnode {
  char *word;
  struct wnode *next;
};

struct tnode {
  char *word;
  struct lnode *refs;
  struct tnode *left;
  struct tnode *right;
};

int getwords_perline(struct wnode **,char *, char *[]);
void addtotree(struct tnode **, char *, int);
void printtree(struct tnode *);
void printwordlist(struct wnode *);
int istoexclude(char *, char *[]);

int main()
{
  int ln = 1;
  char word[MAXWORDLEN];
  struct wnode *words;
  struct tnode *root;
  while (getwords_perline(&words, word, exclude_words) != EOF) {
    for (; words != NULL; words = words->next) {
      addtotree(&root, words->word, ln);
    }
    ln++;
    // free word list, before reuse
    struct wnode *temp;
    for (struct wnode *node = words; node != NULL;) {
      temp = node;
      node = node->next;
      free(temp);
      free(temp->word);
    }

    if (ln == 10)
      break;
  }
  
  printtree(root);
  return 0;
}

int istoexclude(char *word, char *words[]) {
  for (int i = 0; i < EXCLEN; i++)
    if (strcmp(word, words[i]) == 0)
      return 1;
  return 0;
}

void printwordlist(struct wnode *list) {
  for (; list != NULL; list = list->next)
    printf("%s->", list->word);

  printf("\n");
}

void printnumlist(struct lnode *list) {
  for (; list != NULL; list = list->next)
    printf("%d->", list->num);

  printf("\n");
}

void printtree(struct tnode *tree) {
  if (tree != NULL) {
    printtree(tree->left);
    printf("%10s :: ", tree->word);
    printnumlist(tree->refs);
    printf("\n");
    printtree(tree->right);
  }
}

void addtowordlist(struct wnode **, char *);

int getwords_perline(struct wnode **list, char *word, char *exclude_words[]) {
  int inword, c, getch();
  inword = 0;
  word[0] = '\0';
  int wlen = 0;
  for (c = getch(); c != '\n' && c != EOF; c = getch()) {
    if (isalpha(c) && !inword)
      inword = 1;

    if ((isspace(c) || c == '.' ) && inword) {       // found word
      inword = 0;
      word[wlen] = '\0';
      if (wlen > 1 && !istoexclude(word, exclude_words)) {
	addtowordlist(list, word);
      }
      word[0] = '\0';
      wlen = 0;
    }

    if (inword && (isalnum(c) || c == '-' || c == '_')) {
      word[wlen++] = c;
    }
  }
  // consume unfinished words
  if (wlen > 1) {
    if (word[wlen - 1] == '-') {
      wlen--;
      inword = 0;
      for (c = getch(); isalnum(c) || isspace(c); c = getch()) {
	if (isspace(c) && inword)
	  break;
	if (isalnum(c) && !inword)
	  inword = 1;
	if (inword)
	  word[wlen++] = c;
      }
    }
    word[wlen] = '\0';
    if (!istoexclude(word, exclude_words))
      addtowordlist(list, word);
  }
  word[0] = '\0';
  return c;
}

struct wnode *newwnode(char *word) {
  struct wnode *node = (struct wnode *) malloc(sizeof(struct wnode));
  node->word = strdup(word);
  node->next = NULL;
  return node;
}

// prepends word to list
void addtowordlist(struct wnode **list, char *word) {
  struct wnode *node = newwnode(word);
  if (*list == NULL) {
    *list = node;
    return;
  }
  struct wnode *temp = *list;
  node->next = temp;
  *list  = node;
}

struct lnode *newlnode(int n) {
  struct lnode *node = (struct lnode *) malloc(sizeof(struct lnode));
  node->num = n;
  node->next = NULL;
  return node;
}

// TODO lines should be unique
void addtonumlist(struct lnode **list, int n) {
  struct lnode *node = newlnode(n);
  if (*list == NULL) {
    *list = node;
    return;
  }
  struct lnode *temp = *list;
  node->next = temp;
  *list  = node;
}

struct tnode *newtnode(char *word, int n) {
  struct tnode *node = (struct tnode *) malloc(sizeof(struct tnode));
  node->word = strdup(word);
  addtonumlist(&node->refs, n);
  node->left = node->right = NULL;
  return node;
}

void addtotree(struct tnode **tree, char *word, int n) {
  int cond;
  if (*tree == NULL) {
    *tree = newtnode(word, n);
  } else if ((cond = strcmp((*tree)->word, word)) == 0) {
    addtonumlist(&(*tree)->refs, n);
  } else if (cond < 0) {
    addtotree(&(*tree)->left, word, n);
  } else {
    addtotree(&(*tree)->right, word, n);
  }
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
