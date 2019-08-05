#include <stdio.h>

int my_strncmp(char *s, char *t, int n);
int my_strncpy(char *s, char *t, int n);
void my_strncat(char *s, char *t, int n);

int main()
{
  char s[] = "hello";
  char t1[] = "helloo";
  printf("my_strncmp(s, t1, 5) = %d\n", my_strncmp(s, t1, 5));
  printf("my_strncmp(s, t1, 6) = %d\n", my_strncmp(s, t1, 2));
  char sc[20] = "Jurassic";
  char tc[] = "Parking";
  int n = my_strncpy(sc, tc, 4);
  printf("my_strncpy(s, t1, 4) > %d string >%s<\n", n, sc);

  char h[20] = "hello";
  char w[] = "infinity world";
  my_strncat(h, w, 8);
  printf("my_strncat(h, w, 8) = >%s<\n", h);
}

// s should hold +n chars
void my_strncat(char *s, char *t, int n) {
  while(*s)
    s++;
  int i = 0;
  for(; i < n && *t !='\0'; s++, t++, i++)
    *s = *t;
}

int my_strncpy(char *s, char *t, int n) {
  int i = 0;
  for (; i < n && *t !='\0'; s++, t++, i++)
    *s = *t;
  return i;
}

// n should be less or equal to t len
int my_strncmp(char *s, char *t, int n) {
  for (int i = 0; *s == *t && i < n; s++, t++, i++)
    if (*s == '\0')
      return 0;
  return *s - *t;
}
