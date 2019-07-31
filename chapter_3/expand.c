#include <stdio.h>
#include "../lib/lib.h"
#define MAXLEN 1000

int expand_my_first_solution(char s[], char t[]);
void expand(char s[], char t[]);
int assert_str(char s1[], char s2[]);

int main()
{
  char t[MAXLEN];
  t[0] = '\0';

  // tests
  char patterns[][100] = {
      {"a-z"},
      {"f-a"}, {"a-b-c"},
      {"a-z0-9"},
      {"-d-o"},
      {"-b-d1-5-"},
      {"a-X"},
      {"-a-z 0-9 a-d-f -0-2 some text 1-1 WITH CAPITALS! 0-0 5-3 - 1-3-6-9"}};
  char expected_str[][100] = {
      {"abcdefghijklmnopqrstuvwxyz"},
      {"fedcba"}, {"abc"},
      {"abcdefghijklmnopqrstuvwxyz0123456789"},
      {"-defghijklmno"},
      {"-bcd12345-"},
      {"a-X"},
      {"-abcdefghijklmnopqrstuvwxyz 0123456789 abcdef -012 some text 1 WITH "
       "CAPITALS! 0 543 - 123456789"}};

  size_t test_cases = sizeof(patterns) / sizeof(patterns[0]);
  printf(">>>Test expand_my_first_solution:\n");
  for (int i = 0; i < test_cases; i++) {
    expand_my_first_solution(patterns[i], t);
    printf("pattern %s ", patterns[i]);
    assert_str(t, expected_str[i]);
  }
  printf("\n>>>Test expand:\n");  
  for (int i = 0; i < test_cases; i++) {
    expand(patterns[i], t);
    printf("pattern %s ", patterns[i]);
    assert_str(t, expected_str[i]);
  }
}

int expand_my_first_solution(char s[], char t[]) {
  int i = 0;
  int j = 0;
  if (s[i] == '-') {
    t[j] = '-';
    i++;
    j++;
  }
  int set_id;
  int last_set = -1;
  int from = 0;
  for (; s[i] != '\0'; i++) {
    if (s[i] == '-')
      continue;

    if (s[i] >= 'a' && s[i] <= 'z') {
      set_id = 'a';
    } else if (s[i] >= 'A' && s[i] <= 'Z') {
      set_id = 'A';
    } else if (s[i] >= '0' && s[i] <= '9') {
      set_id = '0';
    } else {
      // unsupported character
      printf("unsupported char %c\n", s[i]);
      return -1;
    }
    int id = s[i];
    if (last_set == set_id) {
      for (int i = from+1; i <= id; i++, j++) {
        t[j] = i;
      }
    } else {
      t[j++] = id;
    }
    last_set = set_id;    
    from = id;
  }
  if (s[--i] == '-')
    t[j++] = '-';

  t[j] = '\0';
  return 0;
}

int expandable(char c1, char c2) {
  if ((c1 >= 'a' && c1 <= 'z' && c2 >= 'a' && c2 <='z') ||
      (c1 >= 'A' && c1 <= 'Z' && c2 >= 'A' && c2 <= 'Z') ||
      (c1 >= '0' && c1 <= '9' && c2 >= '0' && c2 <= '9'))
    return 1;
  
  return 0;
}

// based on LingTalfi https://clc-wiki.net/wiki/K%26R2_solutions:Chapter_3:Exercise_3
void expand(char s1[], char s2[]) {
    char c, d, e;
    int i, j;
    i = j = 0;

    while ('\0' != (c = s1[i++])) {
        if (' ' != c && '-' == s1[i] && '\0' != s1[i + 1]) {
            i++;
	    // check that from-to is in same set
            if (!expandable(c, s1[i])) {
              s2[j++] = c;
	      s2[j++] = s1[i-1];
	      continue;
            }
	    
            d = s1[i];
	    while (c > d) {
		s2[j++] = c--;
	    }
	    while (c < d) {
		s2[j++] = c++;
	    }
        }
        else {
            s2[j++] = c;
        }
    }
    s2[j] = '\0';
}
