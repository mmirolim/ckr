/*
  Exercise 8-3. Design and write _flushbuf, fflush, and fclose.
 */
#include <syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 10
#define OPENMAX 10
#define EOF (-1)
#define ERR (-2)
#define PERMS 0666 /* RW for owner, group, others */
#define FROMSTART 0
#define FROMCUR 1
#define FROMEND 2

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
int flushbuf(char *c, FILE *);
int fflush(FILE *);
int fclose(FILE *);
int fseek(FILE *fp, long offset, int at);

#define getc(p) (--(p)->cnt >= 0 ? (unsigned char) *(p)->ptr++ : fillbuf(p))
#define putc(c, p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (c) : flushbuf((char *)(&c), p))

int main(int argc, char *argv[]) {
  init_iob();
  char buf[BUFSIZE];
  if (argc != 3)
    return 1;
  FILE *f, *fw;
  if ((f = fopen(*++argv, "r")) == NULL)
    return 1;
  if ((fw = fopen(*++argv, "w")) == NULL)
    return 2;
  int c;
  while ((c = getc(f)) != EOF)
    putc(c, fw);

  if (fclose(f))
    return 1;

  // test fseek
  if (fseek(fw, -3, FROMCUR))
    return 1;
  else {
    c = 'w';
    putc(c, fw);
  }

  if (fclose(fw))
    return 1;
  
  return 0;
}

/* fflush: flush buf to file */
int fflush(FILE *fp) {
  return flushbuf(NULL, fp);
}

/* fclose: flush and close file */
int fclose(FILE *fp) {
  if (fflush(fp))
    return 1;
  if (close(fp->fd) < 0)
    return 1;
  free(fp->base);
  fp->base = fp->ptr = NULL;
  fp->cnt = 0;
  fp->fd = -1;
  fp->flag.eof = fp->flag.err = fp->flag.read = fp->flag.unbuf = fp->flag.write = 0;
  fp->flag.unused = 1;
  return 0;
}
/* flushbuf: allocate and write buf to file */
int flushbuf(char *c, FILE *fp) {
  if (fp->flag.err)
    return 1;
  if (!fp->flag.write) {
    fp->cnt = 0;
    fp->ptr = fp->base;
    return 0;
  }
  if (fp->flag.unbuf) {
    if (c != NULL && write(fp->fd, c, 1) != 1) {
      fp->flag.err = 1;
      return 1;
    } else
      return 0;
  }

  int wn = (fp->cnt < 0) ? BUFSIZE : BUFSIZE - fp->cnt;
  
  if (fp->base == NULL) {
    if ((fp->base = (char *)malloc(BUFSIZE)) == NULL) {
      fp->flag.err = 1;
      return 1;
    }
  } else if (write(fp->fd, fp->base, wn) != wn) {
    fp->flag.err = 1;
    return 1;
  }

  fp->cnt = BUFSIZE;
  fp->ptr = fp->base;
  if (c != NULL) {
    fp->cnt--;
    *fp->ptr++ = *c;
  }
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

/* fseek: position cursor in a file */
int fseek(FILE *fp, long offset, int whence) {
  if (fp->flag.err)
    return 1;
  if (!fp->flag.unbuf) {
    if (fp->flag.write) {
      if (flushbuf(NULL, fp))
	return 1;
    } else if (fp->flag.read){
      if (whence == FROMCUR) {
	  offset -= fp->cnt;
      }
      // trash read buf
      fp->cnt = 0;
      fp->ptr = fp->base;
    }
  }
  return (lseek(fp->fd, offset, whence) < 0);
}
