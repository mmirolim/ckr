#include <stdio.h>

void reverse(char s[]);

int main()
{
  char str[] = "abcd";
  reverse(str);
  printf("reverse abcd = %s\n", str);
  char str2[] = "<<<ABC>>>";
  reverse(str2);
  printf("reverse <<<ABC>>> = %s\n", str2);
}

int reverse_rec(int l, char s[]) {
  if (s[l] == '\0')
    return l-1;
  
  int len = reverse_rec(l + 1, s);
  if (l <= len/2)
    return len;
  
  int temp = s[len - l];
  s[len - l] = s[l];
  s[l] = temp;
  return len;
}

void reverse(char s[]) {
  reverse_rec(0, s);
}
