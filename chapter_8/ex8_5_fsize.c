/*
  Exercise 8-5. Modify the fsize program to print the other information con-
  tained in the inode entry.
*/
#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#define NAME_MAX 255

typedef struct { /* portable directory entry */
  long ino; /* inode number */
  char name[NAME_MAX+1]; /* name + '\0' terminator */
} Dirent;

typedef struct { /* minimal DIR: no buffering, etc */
  int fd; // file descriptor
  Dirent d; // the directory entry
} mDIR;

void fsize(char *);
mDIR *open_dir(char *dirname);
Dirent *read_dir(mDIR *dir);
void close_dir(mDIR *dir);

/* print file sizes recursively */
int main(int argc, char *argv[])
{
  if (argc == 1) /* default: current directory */
    fsize("."); 
  else
    while (--argc >0)
      fsize(*++argv);

  return 0;
}

void dirwalk(char *name, void (*fn)(char *));

/* fsize: print size of file "name" in directory recursively */
void fsize(char *name) {
  struct stat stbuf;
  if (stat(name, &stbuf) == -1) {
    fprintf(stderr, "fsize: can't access %s\n", name);
    return;
  }
  if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
    dirwalk(name, fsize);
  printf("%8ld %s\n", stbuf.st_size, name);
}

#define MAX_PATH 1024

/* dirwalk: apply fn func to all files in dir */
void dirwalk(char *dirname, void (*fn)(char *)) {
  char name[MAX_PATH];
  Dirent *dp;
  mDIR *dirp;

  if ((dirp = open_dir(dirname)) == NULL) {
    fprintf(stderr, "dirwalk: can't open %s\n", dirname);
    return;
  }
  while ((dp = read_dir(dirp)) != NULL) {
    if (strcmp(dp->name, ".") == 0 | strcmp(dp->name, "..") == 0) // skip self and parent
      continue;
    if (strlen(dirname) + strlen(dp->name) + 2 > sizeof(name))
      fprintf(stderr, "dirwalk: name too long %s/%s\n", dirname, dp->name);
    else {
      sprintf(name, "%s/%s", dirname, dp->name);
      // call fn
      (*fn)(name);
    }
  }
  close_dir(dirp);
}

/* opendir: open a directory for readdir calls */
mDIR *open_dir(char *dname) {
  int fd;
  struct stat stbuf;
  mDIR *dp;
  if ((fd = open(dname, O_RDONLY, 0)) == -1
      || fstat(fd, &stbuf) == -1
      || (stbuf.st_mode & S_IFMT) != S_IFDIR
      || (dp = (mDIR *) malloc(sizeof(mDIR))) == NULL)
    return NULL;
  dp->fd = fd;
  return dp;
}

/* closedir: close directory opened by opendir */
void close_dir(mDIR *dir) {
  if (dir) {
    close(dir->fd);
    free(dir);
  }
}

#include <sys/dir.h> // local directory structure
#include <errno.h>
#define BUF_SIZE 1024

struct mydirect {
 long           d_ino;
 off_t          d_off;
 unsigned short d_reclen;
 unsigned char  d_type;
 char           d_name[];
};        // local directory structure

char buf[BUF_SIZE];

/* readdir: read directory entries in sequence */
Dirent *read_dir(mDIR *dir) {
  struct mydirect *dirbuf;
  static Dirent d; // store data for readdir returned
  static int bpos, nread;

  if (nread == 0 || bpos >= nread) {
    bpos = 0;
    // system specific call
    if ((nread = syscall(SYS_getdents64, dir->fd, buf, BUF_SIZE)) == -1 || nread == 0) {
      return NULL;
    }
  }
  
  for (;;) {
    dirbuf = (struct mydirect *) (buf + bpos);
    if (dirbuf->d_ino == 0) // slot is not in use, maybe removed
      continue;
    d.ino = dirbuf->d_ino;
    strncpy(d.name, dirbuf->d_name, NAME_MAX);
    d.name[NAME_MAX] = '\0';
    bpos += dirbuf->d_reclen; // move position to next entry
    return &d;      
  }

  return NULL;
}
