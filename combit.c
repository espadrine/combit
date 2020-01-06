#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "./combit.h"
#define BUFSIZE 16384
typedef struct args { __int64_t bytes; __uint64_t *seed; int rval; } args_t;
args_t parseArgs(int argc, char **argv);

int main(int argc, char **argv) {
  args_t a = parseArgs(argc, argv);
  if (a.rval < 0) { return a.rval; }
  COMBIT_IT seed[4] = {0, 0, 0, 0};
  combit_state s = combit_init(seed);
  COMBIT_T buf[BUFSIZE];
  __int64_t cycles = 0, start;
  for (__int64_t bytes = a.bytes; bytes >= 0; bytes -= sizeof(buf)) {
    int wbytes = bytes < sizeof(buf)? bytes: sizeof(buf);
    for (int i = 0; i < wbytes / sizeof(COMBIT_T); i += COMBIT_NGEN) {
      start = _rdtsc();
      combit_gen(&s, &buf[i]);
      cycles += _rdtsc() - start;
    }
    ssize_t w = write(STDOUT_FILENO, buf, wbytes);
  }
  fprintf(stderr, "%f cpb\n", ((double)cycles) / a.bytes);
  return 0;
}

args_t parseArgs(int argc, char **argv) {
  args_t a;
  a.rval = 0;
  a.bytes = 0x7fffffffffffffff;
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      fprintf(stderr, "Usage: combit [args]\n");
      fprintf(stderr, "A PRNG.\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "  --bytes: as bytes.\n");
      fprintf(stderr, "  --seed: as hexadecimal. Not implemented.\n");
      a.rval = -1;
    } else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bytes") == 0) {
      a.bytes = atoll(argv[++i]);
    }
  }
  return a;
}
