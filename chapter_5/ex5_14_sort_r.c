#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXLINES 5000
#define MAXLEN 1000

int readlines(char *lines[], int max_lines);
void writelines(char *lines[], int nlines);
void my_qsort(void *lines[], int left, int right,
	      int (*comp)(void *, void *), int order);
int numcmp(const char *s1, const char *s2);
int insenstrcmp(const char *s1, const char *s2);
int dircmp(const char *s1, const char *s2);
int dirfcmp(const char *s1, const char *s2);

int main(int argc, char *argv[])
{
  
  int sort_order = 1;
  int case_insensitive = 0;
  int numeric = 0;
  int dir_sort = 0;
  char c;
  while(--argc > 0 && (*++argv)[0] == '-')
    while ((c = *++argv[0])) {
      switch (c) {
      case 'r':
	sort_order = -1;
	break;
      case 'n':
	numeric = 1;
	break;
      case 'f':
	case_insensitive = 1;
	break;
      case 'd':
	dir_sort = 1;
	break;
      default:
	printf("error: undefined arg %c\n", c);
	return 1;
      }
    }  
  char *lines[MAXLINES];
  // readlines
  int read_lines = readlines(lines, MAXLINES);
  if (read_lines > MAXLINES) {
    printf("input too big to sort\n");
    return 1;
  }
  void *cmp = numeric ? numcmp : (case_insensitive) ?
    (dir_sort) ? dirfcmp :insenstrcmp : (dir_sort) ? dircmp : strcmp;
  my_qsort((void **) lines, 0, read_lines-1,
	   (int (*)(void *, void *)) (cmp),
	   sort_order);
  writelines(lines, read_lines);
}

int readlines(char *lines[], int max_lines) {
  char *p;
  char line[MAXLEN];
  int read_lines = 0;
  int i = 0;
  char c;
  while ((c = getchar())) {
    if ( i == 0 && c == EOF)
      break;

    if (c == '\n' || c == EOF) {
      line[i] = '\0';
      if (read_lines +1 > max_lines) {
	break; // limit reached
      }
      lines[read_lines] = malloc(sizeof(*lines)* i);
      strcpy(lines[read_lines], line);
      read_lines++;
      i = 0;
      continue;
    }
    line[i++] = c;
  }
  return read_lines;
}

void writelines(char *lines[], int nlines) {
  while (nlines--)
    printf("%s\n", *lines++);
}

// qsort in order defined by comp function
void my_qsort(void *v[], int left, int right, int (*comp)(void *, void *), int order) {
  int i, last;
  void swap(void *v[], int, int);
  if (left >= right)
    return;
  swap(v, left, (left + right)/2);
  last = left;
  for (i = left + 1; i <= right; i++)
    if (order * ((*comp)(v[i], v[left])) < 0)
      swap(v, ++last, i);

  swap(v, left, last);
  my_qsort(v, left, last-1, comp, order);
  my_qsort(v, last+1, right, comp, order);
}

void swap(void *v[], int i, int j) {
  void *temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

int numcmp(const char *s1, const char *s2) {
  double d1 = atof(s1);
  double d2 = atof(s2);
  if (d1 < d2)
    return -1;
  else if (d1 > d2)
    return 1;
  else
    return 0;
}

int insenstrcmp(const char *s1, const char *s2) {
  for(; tolower(*s1) == tolower(*s2); s1++, s2++)
    if (*s1 == '\0')
      return 0;
  return tolower(*s1) - tolower(*s2);
}

char nextvalid(const char *s) {
  while (*s != '\0') {
    if (isblank(*s) || isalnum(*s))
      break;
    s++;
  }
  return *s;
}

int dircmp(const char *s1, const char *s2) {
  while (nextvalid(s1) == nextvalid(s2)) {
    if (*s1 == '\0')
      return 0;
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

int dirfcmp(const char *s1, const char *s2) {
  while (tolower(nextvalid(s1)) == tolower(nextvalid(s2))) {
    if (*s1 == '\0')
      return 0;
    s1++;
    s2++;
  }
  return tolower(*s1) - tolower(*s2);
}
