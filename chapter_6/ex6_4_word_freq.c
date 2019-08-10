/*
  Exercise 6-4. Write a program that prints the distinct words in its input sorted
into decreasing order of frequency of occurrence. Precede each word by its
count.
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXWORDLEN 100

struct wc {
  char *word;
  int count;
};

struct tnode {
  struct wc count;
  struct tnode *left;
  struct tnode *right;
};

int getword(char *, int lim);
void addtotree(struct tnode **, char *);
int countwords(struct tnode*);
int arrayfromtree(struct tnode *, struct wc **arr);
struct tnode *newtnode(char *);
void printtree(struct tnode *);
int cmpwc(const void *, const void *);

int main()
{
  char word[MAXWORDLEN];
  struct tnode *root;
  while (getword(word, MAXWORDLEN) != EOF)
    addtotree(&root, word);

  int num =  countwords(root);
  struct wc **arr = (struct wc **) malloc(sizeof(struct wc *) * num);
  arrayfromtree(root, arr);
  
  qsort(arr, num, sizeof(struct wc *), cmpwc);
  
  for (int i = 0; i < num; i++)
       if (arr[i] != NULL)
         printf("%10s :: %d\n", (*arr[i]).word, (*arr[i]).count);
}

int cmpwc(const void *w1, const void *w2) {
  int c1 =  (*((struct wc **)w1))->count;
  int c2 =  (*((struct wc **)w2))->count;
  return c2 - c1;
}

void printtree(struct tnode *root) {
  if (root != NULL) {
    printtree(root->left);
    printf("word: %s :: %d\n", root->count.word, root->count.count);
    printtree(root->right);
  }
}

int arrayfromtree(struct tnode *root, struct wc **arr) {
  int n = 0;
  if (root != NULL) {
    n += arrayfromtree(root->left, arr);
    *(arr+n) = &root->count;
    n++;    
    n += arrayfromtree(root->right, arr + n);
  }
  return n;
}

int countwords(struct tnode *root) {
  int n = 0;
  if (root != NULL) {
    n = countwords(root->left);
    n++;
    n += countwords(root->right);
  }
  return n;
}

void addtotree(struct tnode **b, char *word) {
  int cond;
  if (*b == NULL) { // new one
    *b = newtnode(word);
  } else if ((cond = strcmp((*b)->count.word, word)) == 0) {
    (*b)->count.count++;
  } else if (cond < 0) {
    addtotree(&(*b)->left, word);
  } else {
    addtotree(&(*b)->right, word);
  }
}

struct tnode *newtnode(char *word) {
  struct tnode *node = (struct tnode *) malloc(sizeof(struct tnode));
  node->count.count = 1;
  node->count.word = strdup(word);
  node->left = node->right = NULL;
  return node;
}

int getword(char *word, int lim) {
  int c;
  word[0] = '\0';
  int wlen = 0;
  int inword = 0;
  for (c = getchar(); c != EOF; c = getchar()) {
    if (isalpha(c) && !inword)
      inword = 1;

    if (!isalnum(c) && inword && c != '-' && c != '_') {       // found word
      inword = 0;
      word[wlen] = '\0';
      break;
    }

    if (inword)
      word[wlen++] = c;
  }

  return c;
}
