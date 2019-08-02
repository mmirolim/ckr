#include <stdio.h>
#define swap(t, x, y)                                                          \
  {                                                                           \
    t temp = x;								\
    x = y;								\
    y = temp;								\
  }

int main()
{
  int x = 10;
  int y = 20;
  int temp = 100;
  printf("x %d, y %d\n", x, y);
  swap(int, x, y);
  printf("x %d, y %d\n", x, y);
  printf("temp %d\n", temp);
}
