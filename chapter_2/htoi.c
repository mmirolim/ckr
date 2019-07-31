#include <stdio.h>

int htoi(char hex[]);

int main(){
  printf("%d == %d\n", 0xF, htoi("0xF"));
  printf("%d == %d\n", 0x7F, htoi("0x7F")); 
  printf("%d == %d\n", 0x63, htoi("0x63")); 
}

int htoi(char hex[]) {
  int i, d;
  int n = 0;
  // test for hex
  if (!(hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')))
      return n;
  
  for (i = 2; hex[i] != '\0'; ++i) {
    d = hex[i] - '0';
    if ( d > 9)
      d = hex[i] - 'A' + 10;
    n = n * 16 + d;
  }
  return n;
}
