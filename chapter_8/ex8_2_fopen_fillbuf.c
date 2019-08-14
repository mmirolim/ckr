/*
  Rewrite fopen and _fillbuf with fields instead of explicit bit
  operations. Compare code size and execution speed
 */
#include <syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 1024
#define OPENMAX 10
#define EOF (-1)
#define ERR (-2)
#define PERMS 0666 /* RW for owner, group, others */

typedef struct flag {
  int unused, read, write, unbuf, eof, err;
} FFLAG;

typedef struct iobuf {
  int cnt;
  char *ptr;
  char *base;
  FFLAG flag;
  int fd;
} FILE;

FILE iob[OPENMAX];
void init_iob() {
  FILE *f;
  for (f = iob; f < iob + OPENMAX; f++) {
    f->base = f->ptr = NULL;
    f->cnt = 0;
    f->fd = -1;
    f->flag.eof = f->flag.err = f->flag.read = f->flag.unbuf = f->flag.write = 0;
    f->flag.unused = 1;
  }
}

FILE *fopen(char *name, char *mode);
int fillbuf(FILE *);

#define getc(p) (--(p)->cnt >= 0 ? (unsigned char) *(p)->ptr++ : fillbuf(p))
#define putc(p) write(1, (char *)(p), 1)

int main(int argc, char *argv[]) {
  init_iob();
  char buf[BUFSIZE];
  if (argc != 2)
    return 1;
  FILE *f;
  if ((f = fopen(*++argv, "r")) == NULL)
    return 1;

  int c, n;
  while ((c = getc(f)) != EOF)
    putc(&c);

  return 0;
}

/* fopen: open file, return file ptr */
FILE *fopen(char *name, char *mode) {
  int fd;
  FILE *fp;
  
  for (fp = iob; fp < iob + OPENMAX; fp++)
    if (fp->flag.unused) /* found avail slot */
      break;

  if (fp >= iob + OPENMAX)
    return NULL;

  switch (*mode) {
  case 'r':
    fd = open(name, O_RDONLY, 0);
    fp->flag.read = 1;
    break;
  case 'w':
    fd = creat(name, PERMS);
    fp->flag.write = 1;
    break;
  case 'a':
    if ((fd = open(name, O_WRONLY, 0)) == -1)
      fd = creat(name, PERMS);
    lseek(fd, 0L, 2);
    fp->flag.write = 1;
    break;
  default:
    return NULL;
  }
  if (fd == -1)
    return NULL;

  fp->fd = fd;
  fp->cnt = 0;
  fp->base = NULL;
  fp->flag.unused = 0;
  return fp;
}

/* fillbuf: allocate and fill input buffer */
int fillbuf(FILE *fp) {
  int bufsize = (fp->flag.unbuf) ? 1 : BUFSIZE;
  if (!fp->flag.read || fp->flag.eof || fp->flag.err)
    return EOF;
  if (fp->base == NULL)
    if ((fp->base = (char *) malloc(bufsize)) == NULL)
      return EOF;
  fp->ptr = fp->base;
  fp->cnt = read(fp->fd, fp->ptr, bufsize);
  if (--fp->cnt < 0) {
    if (fp->cnt == -1) {
      fp->flag.eof = EOF;
    } else {
      fp->flag.err = ERR;
    }
    fp->cnt = 0;
    return EOF;
  }

  return (unsigned char) *fp->ptr++;
}
