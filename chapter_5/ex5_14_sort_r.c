#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINES 5000
#define MAXLEN 1000

int readlines(char *lines[], int max_lines);
void writelines(char *lines[], int nlines);
void my_qsort(void *lines[], int left, int right,
	      int (*comp)(void *, void *), int (*reverse)(int));
int numcmp(const char *s1, const char *s2);
int reverse(int);
int noop(int);

int main(int argc, char *argv[])
{
  
  int sort_reverse = 0;
  int numeric = 0;
  char c;
  while(--argc > 0 && (*++argv)[0] == '-')
    while ((c = *++argv[0])) {
      switch (c) {
      case 'r':
	sort_reverse = 1;
	break;
      case 'n':
	numeric = 1;
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

  my_qsort((void **) lines, 0, read_lines-1,
	   (int (*)(void *, void *)) (numeric ? numcmp : strcmp),
	   (sort_reverse ? reverse : noop));
  writelines(lines, read_lines);
}

int readlines(char *lines[], int max_lines) {
  char *p;
  char line[MAXLEN];
  int read_lines = 0;
  int i = 0;
  char c;
  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      line[i] = '\0';
      if (read_lines +1 > max_lines) {
	return read_lines; // limit reached
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
void my_qsort(void *v[], int left, int right, int (*comp)(void *, void *), int (*mod)(int)) {
  int i, last;
  void swap(void *v[], int, int);
  if (left >= right)
    return;
  swap(v, left, (left + right)/2);
  last = left;
  for (i = left + 1; i <= right; i++)
    if ((*mod)((*comp)(v[i], v[left])) < 0)
      swap(v, ++last, i);

  swap(v, left, last);
  my_qsort(v, left, last-1, comp, mod);
  my_qsort(v, last+1, right, comp, mod);
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

int noop(int res) {
  return res;
}

int reverse(int res) {
  if (res == 0)
    return res;
  else
    return -1*res;
}
