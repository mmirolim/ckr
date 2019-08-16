/*
  Exercise 8-7. mal10c accepts a size request without checking its plausibility;
  free believes that the block it is asked to free contains a valid size field.
  Improve these routines so they take more pains with error checking

  Exercise 8-8 Write a routine bfree (p, n) that will free an arbitrary block p
  of n characters into the free list maintained by mal10c and free. By using
  bfree, a user can add a static or external array to the free list at any time
*/

#include <stdio.h>
#include <stdlib.h>

#define MiB 1073741824
void *xmalloc(unsigned size);
int xfree(void *);
int bfree(char *ar, unsigned n);

typedef long Align; /* for alignment to long boundary */

union header { /* block header */
  struct {
    union header *ptr; /* next block if on free list */
    unsigned size;     /* size of this block */
  } s;
  Align x;             /* force alignment of blocks */
};

typedef union header Header;

typedef struct obj {
  double p1, p2;
} Obj;

int main() {
  Obj *p;
  printf("sizeof Obj %lu\n", sizeof(Obj));
  int n = 5;
  p = (Obj *) xmalloc(n*sizeof(Obj));
  if (p == NULL)
    return 1;
  for (int i = 0; i < n; i++)
    p[i].p1 = 10.10, p[i].p2 = 20.20;

  printf("p[2] p.p1 %f, p.p2 %f\n", p[2].p1, p[2].p2);
  if (xfree((void *)p))
    printf("xfree error\n");
  
  printf("free *p == %p\n", p);
  p = (Obj *) xmalloc(n*sizeof(Obj));
  printf("p[2] p.p1 %f, p.p2 %f\n", p[2].p1, p[2].p2);

  // test error
  unsigned int correctsize;
  Header *bp = (Header *)p - 1;
  correctsize = bp->s.size;
  bp->s.size = 111;
  if (xfree((void *)p))
    printf("should return error\n");

  bp->s.size = correctsize;
  if (xfree((void *)bp))
    printf("unexpected error\n");
  else
    printf("OK\n");

  n = 30000;
  printf("Test bfree allocate static char *%d and malloc same space\n", n);
  static char arr[30000];
  if (bfree(arr, n)) {
    printf("bfree: error unexpected\n");
    return 1;
  }
  printf("malloc %d char arr\n", n-20);
  char *ap = (char *)xmalloc((n-20) * sizeof(char));
  if (ap == NULL)
    printf("unexpected error from xmalloc\n");
  printf("assing value and check in static arr\n");
  ap[100] = 'w';
  if (arr[100+sizeof(Header)] != 'w')
    printf("unexpected value in static array >%c<\n", arr[100+sizeof(Header)]);
  else
    printf("bfree check OK\n");
}

// malloc and free operates on list of with block of memory
// available memory is handled in free list
static Header base; /* empty list to get started */
static Header *freep = NULL; /* start of free list */
static Header *morecore(unsigned nunits);

/* xmalloc: general-purpose storage allocator inits to zero */
void *xmalloc(unsigned nbytes) {
  if (nbytes < 1 || nbytes > MiB) // checks nbytes range
     return NULL;
  Header *p, *prevp;
  unsigned nunits = (nbytes + sizeof(Header) -1) / sizeof(Header) +1;
  // check initial case
  if ((prevp = freep) == NULL) { // no free list yet
    base.s.ptr = freep = prevp = &base; // degenerate list
    base.s.size = 0;
  }
  // walk free list
  for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
    if (p->s.size >= nunits) { // big enough
      if (p->s.size == nunits) // exactly
	prevp->s.ptr = p->s.ptr; // remove from free list
      else { // allocate tail end
	p->s.size -= nunits; // resize block
	p += p->s.size; // move to block to return
	p->s.size = nunits; // set proper size for sliced block
      }
      freep = prevp; // set last free bloc
      // init to zero
      char *last = (char *)(p + p->s.size);
      for (char *s = (char *)(p+1); s < last; s++)
	*s = '\0';
      return (void *)(p+1);
    }
    if (p == freep) // wrapped around free list
      if ((p = morecore(nunits)) == NULL) // get more blocks
	return NULL; // none left
  }
}

#define NALLOC 1024 // mininum header sized units to request

/* morecore: ask system for more memory */
static Header *morecore(unsigned nunits) {
  char *cp, *sbrk(int);
  Header *up;

  if (nunits < NALLOC)
    nunits = NALLOC;

  cp = sbrk(nunits * sizeof(Header));
  if (cp == (char *) -1) // no space at all
    return NULL;
  
  up = (Header *) cp;
  up->s.size = nunits;
  xfree((void *)(up+1));
  return freep;
}

/* xfree: put block ap in free list */
int xfree(void *ap) {
  Header *bp, *p;
  bp = (Header *)ap - 1; // point to block header
  // check size is valid
  if (bp->s.ptr != NULL && (bp + bp->s.size) != bp->s.ptr) {
    return 1;
  }
  // walk over free list
  for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break; // freed block at start or end of arena

  if (bp + bp->s.size == p->s.ptr) { // if adjacent to next free block, join them
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr; // point to next free node
  if (p + p->s.size == bp) { // if adjacent to prev free block join them
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;

  freep = p;
  return 0;
}

/* bfree: put arbitrary block of n characters into the free list */
int bfree(char *ar, unsigned n) {
  Header *bp;
  unsigned size = n * sizeof(*ar) / sizeof(Header);
  if (size < 2) // too small
    return 1;

  // zero values
  for (char *s = ar; s < ar + n; s++)
    *s = '\0';
  bp = (Header *)ar;
  bp->s.size = size;
  bp->s.ptr = NULL;
  return xfree((void *)(bp + 1));
}
