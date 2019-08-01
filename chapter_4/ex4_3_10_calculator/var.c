#define MAXVARS 26
int isvar_set[MAXVARS];
double var_store[MAXVARS];

void init_var_store() {
for (int i = 0; i < MAXVARS; i++)
  isvar_set[i] = 0;
}

int isvar(char c) {
  return ( c >= 'A' && c <= 'Z') ? isvar_set[c - 'A']: 0;
}

int varp = 0;

int putvar(double d) {
  varp %= MAXVARS;
  int id = varp;
  var_store[id] = d;
  isvar_set[id] = 1;
  varp++;
  return id + 'A';
}

double getvar(char c) {
  return var_store[c - 'A'];
}
