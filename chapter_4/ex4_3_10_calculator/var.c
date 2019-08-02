#include <stdio.h>
#define MAXVARS 26
#define LASTOUTVAR "Z"

int isvar_set[MAXVARS];
double var_store[MAXVARS];
int valid_var_name(char s[]);

void init_var_store() {
for (int i = 0; i < MAXVARS; i++)
  isvar_set[i] = 0;
}

int valid_var_name(char s[]) {
  return ( s[0] >= 'A' && s[0] <= 'Z');
}

int isvar(char s[]) {
  return valid_var_name(s) ? isvar_set[s[0]]: 0;
}

void putvar(char s[], double d) {
  var_store[s[0]] = d;
  isvar_set[s[0]] = 1;
}

double getvar(char s[]) {
  return var_store[s[0]];
}
