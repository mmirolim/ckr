/*
  Exercise 8-6. The standard library function calloc (n, size) returns a
  pointer to n objects of size size, with the storage initialized to zero. Write
  calloc, by calling mal10cor by modifying i
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *_calloc(int n, int size);

typedef  struct obj {
    int prop1;
    double prop2;
    char *prop3;
} Obj;


int main()
{
  Obj *arr;
  int n = 5;
  printf("malloc usage\n");
  for (int j = 0; j < 2; j++) {
    arr = malloc(n* sizeof(Obj));
    for (int i = 0; i < n; i++) {
      arr[i].prop1 = 1;
      arr[i].prop2 = 2.0;
      arr[i].prop3 = "string";
      printf("obj prop1 %d, prop2 %f, prop3 %s\n", arr[i].prop1, arr[i].prop2, arr[i].prop3);
    }
    printf("free allocated arr %p\n", arr);
    free(arr);
  }
  printf("test _calloc\n");
  arr = _calloc(n, sizeof(Obj));
  printf("_calloc new array %p\n", arr);
  for (int i = 0; i < n; i++) {
    printf("obj prop1 %d, prop2 %f, prop3 %s\n", arr[i].prop1, arr[i].prop2, arr[i].prop3);
  }
  free(arr);
}
  

void *_calloc(int n, int size) {
  void *p;
  if((p = malloc(n * size)) == NULL)
    return NULL;
  // init
  char *last = p + n*size;
  for (char *s = p; s < last; s++)
    *s = '\0';
  // or use memset(p. '\0', n*size);
  return p;
}
