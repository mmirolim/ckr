#include <stdio.h>

void squeeze(char s[], char charset[]);

int main(){
  char s1[]  = "aabcaabc";
  char charset[] = "b";
  char expect [] = "bb";
  printf("%s", s1);
  squeeze(s1, charset);
  printf(" squeeze %s expected to get %s, got %s\n", charset, expect, s1);
}

void squeeze(char s[], char charset[]) {
  int i, j, k, lenset;
  i = j = k = lenset = 0;
  while (charset[lenset] != '\0')
    lenset++;
  while (s[i] != '\0') {
    k = 0;
    while ( k < lenset && charset[k] != s[i])
      k++;
    if (k == lenset)
      s[j++] = s[i];
    i++;
  }
  s[j] = '\0';
}
