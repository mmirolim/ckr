#include <stdio.h>

void my_strcat(char *s, char *t);

int main()
{
  char s[20] = "hello";
  char t[] = "world";
  my_strcat(s, t);
  printf("strcat(s, t) = >%s<\n", s);
}
// s should have enough space to t
void my_strcat(char *s, char *t) {
  while(*s)
    s++;

  while ((*s++ = *t++))
    ;

}
