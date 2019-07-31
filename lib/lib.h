#include <stdio.h>
#include <stdlib.h>

int assert_str(char s1[], char s2[]) {
  int lens1, lens2;
  for (lens1 = 0; s1[lens1] != '\0'; lens1++)
    ;
  for (lens2 = 0; s2[lens2] != '\0'; lens2++)
    ;
  if (lens1 != lens2) {
    printf("\033[31massert_str failed\n s1 %s\n s2 %s\n len != s2 len\n\033[0m", s1, s2);
    return 0;
  }

  
  for (int i = 0; i < lens1; i++)
    if (s1[i] != s2[i]) {
      printf("\033[31massert_str failed\n s1 %s\n s2 %s\n s1[%d] '%c' != s2[%d] '%c'\n\033[0m", s1, s2, i, s1[i], i, s2[i]);
      return 0;      
    }


  printf("\x1B[32massert_str(%s, %s) ok\n\033[0m", s1, s2);
  return 1;
}

void reverse(char s[]) {
  int i, j, c;
  j = 0;
  while (s[j] != '\0')
    j++;
  for (i = 0, --j; i < j; i++, j--)
    c = s[i], s[i] = s[j], s[j] = c;
}


char *tobits_string(int x)
{
  char *s = malloc((sizeof(x)<<3)+1);
  int i, count;
  count = sizeof(x)<<3;
  for (i = 0; i < count; i++)
    s[i] = '0' | (((x>>(count -i-1)))&1);
  s[count] = '\0';
  return s;
}
