// Analysis of CONBIT diffusion.
// gcc -o diffusion{,.c} && ./diffusion | less
#include <stdio.h>

#define REGSIZE (8*sizeof(__uint64_t))
// 2 → 11, 3 → 111, 4 → 1111, …
#define MASK(size) (size == REGSIZE? ~0ull:((1ull << size) - 1))
// Rightward rotation.
#define ROR(a, shift, size) \
  (((a >> shift) | (a << (size - shift))) & MASK(size))

int diffusion(int, int);
int diffused(__uint64_t*, int);

int main() {
  for (int size = 2; size < REGSIZE + 1; size++) {
    for (int ngears = 1; ngears < size + 1; ngears++) {
      printf("size=%d\tngears=%d\tdiffusion=%d\n",
          size, ngears, diffusion(size, ngears));
    }
  }
}

int diffusion(int size, int ngears) {
  __uint64_t gears[ngears];
  __uint64_t gear = MASK(size);
  for (int i = 0; i < ngears; i++) { gears[i] = gear; }
  gears[ngears - 1] &= ~1;  // Set one bit whose diffusion we analyze.

  int round = 0;
  for (; !diffused(gears, ngears) && round < 4*size; round++) {
    // One COMBIT round, replacing addition with AND to detect diffusion.
    int rot = round % size;
    __uint64_t g0 = gears[0];
    for (int j = 0; j < ngears - 1; j++) {
      gears[j] = ROR(gears[j], rot, size) & gears[j+1];
    }
    gears[ngears-1] = ROR(gears[ngears-1], rot, size) & gears[0];
  }
  return round;
}

int diffused(__uint64_t gears[], int ngears) {
  for (int i = ngears - 1; i >= 0; i--) {
    if (gears[i] != 0) { return 0; }
  }
  return 1;
}
