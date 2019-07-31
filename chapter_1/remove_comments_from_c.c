#include <stdio.h>
#define SLASH '/'
#define BACK_SLASH '\\'
#define SLCOMMENT '/' // single line comment symbol
#define MLCOMMENT '*'
#define IN_SLCOMMENT 2
#define IN_MLCOMMENT 1
#define OUT_COMMENT 0
#define QUOTE '"'
#define SQUOTE '\''
#define IN_STRING 1
#define OUT_STRING 0

// single line comment
int main()
{
  int nl, last_comment_state, comment_state, string_state, prev, cur;
  
  comment_state = OUT_COMMENT;
  string_state = OUT_STRING;
  nl = 0;
  // test slash as divide
  nl /= 10;
  // sl comment
  while ((cur = getchar()) != EOF /* inline comment */){
    if (cur == '\n')
      ++nl;
    // states
    last_comment_state = comment_state;
    if (string_state == OUT_STRING) {
      if  (cur == SLASH && last_comment_state == OUT_COMMENT) {
      	prev = cur;
	// look forward
      	cur = getchar();
      }
      if (prev == SLASH && cur == SLCOMMENT)
	comment_state = IN_SLCOMMENT;
      else if (prev == SLASH && cur == MLCOMMENT)
	comment_state = IN_MLCOMMENT;
      else if (comment_state == IN_SLCOMMENT && cur == '\n') {
	comment_state = OUT_COMMENT;
      }
      else if (comment_state == IN_MLCOMMENT && prev == MLCOMMENT && cur == SLASH) {
	comment_state = OUT_COMMENT;
	cur = getchar();
      }

    if (prev == SLASH && last_comment_state == OUT_COMMENT && comment_state == OUT_COMMENT)
      putchar(SLASH);
    }

    
    if (comment_state == OUT_COMMENT) {
      if (cur == QUOTE && string_state == OUT_STRING && prev != SQUOTE) {
	string_state = IN_STRING;
      } else if (prev != BACK_SLASH && cur == QUOTE && string_state == IN_STRING) {
	string_state = OUT_STRING;
      }
    }

    prev = cur;
    
    if (comment_state != OUT_COMMENT)
      continue;
    putchar(cur); // rightside comment
	
  }
  
  printf("quoted string \" //should stay\n");
  printf("/* it is not a comment */\n");
  /*
    MultiLine
    Comment
    MoreLine Comment
  */
}
