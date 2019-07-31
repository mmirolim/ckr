#include <stdio.h>
#define IN  1  /* inside a word */
#define OUT 0 /* outside a word */
#define MAXLEN 256 /* max word len */

int main()
{
  int len, longer_than_maxlen, i, c, state;
  
  int freq[MAXLEN];
  for (i = 0; i < MAXLEN; ++i)
    freq[i] = 0;
  
  state = OUT;
  len = 0;
  longer_than_maxlen = 0;
  while ((c = getchar()) != EOF) {
    if (c == ' ' || c == '\n' || c == '\t') {
      if (state == IN) {
	// new word
	if (len > MAXLEN) {
	  ++longer_than_maxlen;
	} else {
	  ++freq[len];
	}
	len = 0;
      }
      state = OUT;
      continue;
    } else if (state == OUT) {
      state = IN;
    }
    ++len;
  }
  int max;
  int j;  
  max = 0;
  for (i = 0; i < MAXLEN; ++i) {
    if (freq[i] > 0) {
      if (freq[i] > max)
	max = freq[i];
      printf("%d", i);
      for (j = 0; j < freq[i]; ++j)
	printf("#");
      printf("\n");
    }
  }
  printf("max %d\n", max);
  /* vertical histogram */
  for (i = max; i > 0; --i) {
    for (j = 0; j < MAXLEN; ++j) {
      if (freq[j] >= i)
	printf("  #");
      else if (freq[j] > 0)
	printf("   ");
    }
    putchar('\n');    
  }

  for (i = 0; i < MAXLEN; ++i) {
    if (freq[i]>0) {
      printf("%3d", i);
    }
  }
  printf("\nwords longer then %d, %d\n", MAXLEN, longer_than_maxlen);
}
