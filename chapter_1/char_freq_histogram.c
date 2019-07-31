#include <stdio.h>
#define MAXLEN 256

int main()
{
  long freq[MAXLEN];
  int c, i;
  long max;
  
  for (i = 0; i < MAXLEN; ++i)
    freq[i] = 0;
  
  max = 0;
  while ((c = getchar()) != EOF) {
    ++freq[c];
    if (max < freq[c])
      max = freq[c];
  }
  int j;
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
    if (freq[i] == 0)
      continue;
    
    if (i == '\n')
      printf(" \\n");
    else if (i == '\t')
      printf(" \\t");
    else if (i == ' ')
      printf(" \\s");
    else
      printf("  %c",i);
  }
  
  putchar('\n');
}
