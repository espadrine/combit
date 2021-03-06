#ifndef COMBIT_H
#define COMBIT_H
#include <immintrin.h>
#include <x86intrin.h>
#ifndef COMBIT_DEBUG
# define COMBIT_DEBUG 0
#endif
#ifndef COMBIT_REGSIZE
# define COMBIT_REGSIZE 64
#endif
#define COMBIT_REGADDR ((COMBIT_REGSIZE)-1)
#if COMBIT_REGSIZE == 8
# define COMBIT_IT __uint8_t
#elif COMBIT_REGSIZE == 16
# define COMBIT_IT __uint16_t
#elif COMBIT_REGSIZE == 32
# define COMBIT_IT __uint32_t
#elif COMBIT_REGSIZE == 64
# define COMBIT_IT __uint64_t
#endif
#define COMBIT_T COMBIT_IT
#define COMBIT_BUFSIZE 16
#define COMBIT_STATE_SIZE 128
typedef struct combit_state {
  COMBIT_IT gears[COMBIT_STATE_SIZE];
  __uint64_t counter;
  __uint128_t s;
  __m256i simdcounter;
  __m256i simdgears[COMBIT_BUFSIZE];
} combit_state;

void combit_gen(combit_state *s, COMBIT_T buf[COMBIT_BUFSIZE]);
void print_bits(COMBIT_T v);
void print_m256i(__m256i avx);
void print_m128i(__m128i avx);
void print_state(combit_state *s);
void combit_detect_cycle(combit_state *s);

#define COMBIT_ROT(a,n) (((a) >> (n)) | ((a) << (COMBIT_REGSIZE - (n))))

// Nothing up my sleeve: those are the hex digits of Φ,
// the least approximable irrational number.
// $ echo 'scale=310;obase=16;(sqrt(5)-1)/2' | bc
COMBIT_IT combit_pi[COMBIT_STATE_SIZE] = {
#if COMBIT_REGSIZE == 8
  0x9E, 0x37, 0x79, 0xB9, 0x7F, 0x4A, 0x7C, 0x15,
  0xF3, 0x9C, 0xC0, 0x60, 0x5C, 0xED, 0xC8, 0x34,
  0x10, 0x82, 0x27, 0x6B, 0xF3, 0xA2, 0x72, 0x51,
  0xF8, 0x6C, 0x6A, 0x11, 0xD0, 0xC1, 0x8E, 0x95,
  0x27, 0x67, 0xF0, 0xB1, 0x53, 0xD2, 0x7B, 0x7F,
  0x03, 0x47, 0x04, 0x5B, 0x5B, 0xF1, 0x82, 0x7F,
  0x01, 0x88, 0x6F, 0x09, 0x28, 0x40, 0x30, 0x02,
  0xC1, 0xD6, 0x4B, 0xA4, 0x0F, 0x33, 0x5E, 0x36,
  0xF0, 0x6A, 0xD7, 0xAE, 0x97, 0x17, 0x87, 0x7E,
  0x85, 0x83, 0x9D, 0x6E, 0xFF, 0xBD, 0x7D, 0xC6,
  0x64, 0xD3, 0x25, 0xD1, 0xC5, 0x37, 0x16, 0x82,
  0xCA, 0xDD, 0x0C, 0xCC, 0xFD, 0xFF, 0xBB, 0xE1,
  0x62, 0x6E, 0x33, 0xB8, 0xD0, 0x4B, 0x43, 0x31,
  0xBB, 0xF7, 0x3C, 0x79, 0x0D, 0x94, 0xF7, 0x9D,
  0x47, 0x1C, 0x4A, 0xB3, 0xED, 0x3D, 0x82, 0xA5,
  0xFE, 0xC5, 0x07, 0x70, 0x5E, 0x4A, 0xE6, 0xE5,
#elif COMBIT_REGSIZE == 16
  0x9E37, 0x79B9, 0x7F4A, 0x7C15, 0xF39C, 0xC060, 0x5CED, 0xC834,
#elif COMBIT_REGSIZE == 32
  0x9E3779B9, 0x7F4A7C15, 0xF39CC060, 0x5CEDC834, 0x1082276B, 0xF3A27251, 0xF86C6A11, 0,
  0xD0C18E95, 0x2767F0B1, 0x53D27B7F, 0x0347045B, 0x5BF1827F, 0x01886F09, 0x28403002, 0,
  0xC1D64BA4, 0x0F335E36, 0xF06AD7AE, 0x9717877E, 0x85839D6E, 0xFFBD7DC6, 0x64D325D1, 0,
  0xC5371682, 0xCADD0CCC, 0xFDFFBBE1, 0x626E33B8, 0xD04B4331, 0xBBF73C79, 0x0D94F79D, 0,
  0x471C4AB3, 0xED3D82A5, 0xFEC50770, 0x5E4AE6E5, 0xE73A9B91, 0xF3AA4DB2, 0x87AE44F3, 0,
  0x32E923A7, 0x3CB91648, 0xE428E975, 0xA3781EB0, 0x1B49D867, 0x4FA15084, 0x19E0EAA4, 0,
  0x038B352D, 0x9BAD30F4, 0x485B71A8, 0xEF64452A, 0x0DD40DC8, 0xCB8F9A2D, 0x4C514F1B, 0,
  0x229DCAA2, 0x22AC268E, 0x9666E4A8, 0x66769145, 0xF5F5880A, 0x9D0ACD3B, 0x9E8C682F, 0,
  0x4F810320, 0xABEB9403, 0x4E70F216, 0x08C061AB, 0x1C1CAEF1, 0xEBDCEFBC, 0x72134ECF, 0,
  0x06ED82BF, 0xB7D8EB1A, 0x41901D65, 0xF5C8CAB2, 0xACCBC32E, 0xAB1FBE82, 0x84F2B44B, 0,
  0xA2E834C5, 0x893A39EA, 0x7865443F, 0x489C37F8, 0x742ACD89, 0x5AFD87B4, 0x67D22A40, 0,
  0xD098F30D, 0xD2CAFDEB, 0x3ABB3A13, 0x507B46B3, 0xD757FC04, 0x001906E1, 0x767D40C3, 0,
  0xA3792A26, 0xEEEF2AB5, 0xBD6685B9, 0x15B56294, 0x00FAA684, 0xECBA752D, 0xDDCB5D18, 0,
  0x576D77B6, 0x52AC0D99, 0x99736866, 0x04128F0C, 0xD4274359, 0x6DEB2D42, 0xC789D64B, 0,
  0x92658610, 0xB5B95C71, 0x9ADEB8CE, 0x28732634, 0x4E31D59A, 0x59963220, 0xB1A4C427, 0,
  0x71D454EC, 0x78F12393, 0xBFB4AC54, 0x188E5911, 0x3D7C3544, 0x1F15AA34, 0xA1140703, 0,
#elif COMBIT_REGSIZE == 64
  0x9E3779B97F4A7C15, 0xF39CC0605CEDC834, 0x1082276BF3A27251, 0xF86C6A11D0C18E95,
  0x2767F0B153D27B7F, 0x0347045B5BF1827F, 0x01886F0928403002, 0xC1D64BA40F335E36,
  0xF06AD7AE9717877E, 0x85839D6EFFBD7DC6, 0x64D325D1C5371682, 0xCADD0CCCFDFFBBE1,
  0x626E33B8D04B4331, 0xBBF73C790D94F79D, 0x471C4AB3ED3D82A5, 0xFEC507705E4AE6E5,

  0xE73A9B91F3AA4DB2, 0x87AE44F332E923A7, 0x3CB91648E428E975, 0xA3781EB01B49D867,
  0x4FA1508419E0EAA4, 0x038B352D9BAD30F4, 0x485B71A8EF64452A, 0x0DD40DC8CB8F9A2D,
  0x4C514F1B229DCAA2, 0x22AC268E9666E4A8, 0x66769145F5F5880A, 0x9D0ACD3B9E8C682F,
  0x4F810320ABEB9403, 0x4E70F21608C061AB, 0x1C1CAEF1EBDCEFBC, 0x72134ECF06ED82BF,

  0xB7D8EB1A41901D65, 0xF5C8CAB2ACCBC32E, 0xAB1FBE8284F2B44B, 0xA2E834C5893A39EA,
  0x7865443F489C37F8, 0x742ACD895AFD87B4, 0x67D22A40D098F30D, 0xD2CAFDEB3ABB3A13,
  0x507B46B3D757FC04, 0x001906E1767D40C3, 0xA3792A26EEEF2AB5, 0xBD6685B915B56294,
  0x00FAA684ECBA752D, 0xDDCB5D18576D77B6, 0x52AC0D9999736866, 0x04128F0CD4274359,

  0x6DEB2D42C789D64B, 0x92658610B5B95C71, 0x9ADEB8CE28732634, 0x4E31D59A59963220,
  0xB1A4C42771D454EC, 0x78F12393BFB4AC54, 0x188E59113D7C3544, 0x1F15AA34A1140703,
  0x5F006FC3AD70FC0D, 0x6331C6D479F484BF, 0x39CBFBD95E664E39, 0xBEBDC5B09D9D5B8F,
  0x8905D9805C819945, 0x7D444A909316CC58, 0xD38B3A25C714BE8D, 0x422964E9724C033B,

  0x6748ACBC2E05CAAF, 0x737C95622E6483FA, 0x0707999AFC482995, 0x1701C6ED5F27CE23,
  0x1770965541F5F287, 0x97F2FECBD984D343, 0x5CE547C88C38466E, 0x2865235683D4447E,
  0x4B32757903058CA1, 0x1A903F3BAA386493, 0x2B80A7D02D61B50C, 0x1FF281D5DD2947DA,
  0x4624BBC7C7B94962, 0x717AED284395C42F, 0x253C17F492F401BB, 0x6C17A95C3296B424,
#endif
};

combit_state combit_init(COMBIT_IT seed[4]) {
  combit_state s = { {0}, 0, 1, _mm256_set_epi64x(0, 0, 0, 0) };
  for (int i = 0; i < COMBIT_STATE_SIZE; i += 2) {
    for (int j = 0; j < 4; j++) {
      s.gears[i + 0] = combit_pi[i + 0];
      s.gears[i + 1] = combit_pi[i + 1] ^ seed[j];
    }
  }
  for (int i = 0; i < COMBIT_BUFSIZE; i++) {
    s.simdgears[i] = _mm256_load_si256((__m256i*)&s.gears[(256/COMBIT_REGADDR)*i]);
  }
  COMBIT_T buf[COMBIT_BUFSIZE];
  // Given incremental rotations, every bit of the initial state will assume
  // every bit position in an A-bit number within A rounds. If B ≤ A,
  // each bit of state therefore affects all bits of state at least once.
  // The proof is fascinating, but see diffusion.c for evidence.
  // Here, we use a liberal 20 rounds that is good for A ≤ 64 and B ≤ 14.
  for (char i = 20; i >= 0; i--) { combit_gen(&s, buf); }
  return s;
}

// Initial idea:
// We wish to strech an initial random state for all its randomness’ worth.
// For instance, A = a1 a2 a3… a32.
// Since bits are independent, a1⊕a2 is also random.
// We can XOR A with a rotated A and get a random 32-bit number.
// But we must never get a bit that we got before: if we output a1⊕a2⊕a3,
// we must never use a2⊕a3⊕a1 in any future output.
// To exhaust all allowed possibilities,
// we must rotate through all possible combinations of the bits.
//
// We walk through possibilities by generating a mask over all rotations:
// 1000…0101 is a XOR of rotations 0, 2 and 31.
// Rotation 31 is always included to prevent masks that are rotationally
// equivalent: for instance, a 2-bit number (a1 a2) will give
// (a1 a2) for mask 01 and (a2 a1) for mask 10, which reuses a combination from
// the rotationally equivalent mask 01.
// As a result, the mask is 31-bit long.
//
// To limit the cost of each update, we go through masks using a Gray code
// instead of increment. We go through the binary-reflected Gray code
// by flipping the mask bit whose index is that of the least significant 1 in an
// incrementing counter (0, 1, 0, 2, 0, 1, 0, 3, …).
// We XOR the rotation corresponding to that index.
//
// A 32-bit input produces 2^31 32-bit outputs, which is reached
// after 2^31 × 32 bits / 1 GiB/s = 8 s.
// Combining two gives ~50 years; four gives ~10× the age of the universe.
// The state for four is 256 bits (4 32-bit outputs plus the 4 32-bit counters).
//
// Issue in the base idea: take a 4-bit state, eg. 0101.
// Going through the process manually makes it clear
// that obtaining a 0011 state is impossible.

inline void combit_gen(combit_state *s, COMBIT_T buf[COMBIT_BUFSIZE]) {
  //for (char i = 0; i < COMBIT_BUFSIZE; i++) {
  //  char rot = (++s->counter) & COMBIT_REGADDR;
  //  COMBIT_IT s0 = s->gears[0];
#if COMBIT_DEBUG >= 2
    print_state(s);
    __int64_t start = _rdtsc();
#endif
  // Notation: each family has a name COMBIT_NNNAxB,
  // where NNN describes its variant, A is the gear size in bits,
  // and B is the number of gears used.

  // COMBIT_C: Barrel roll fed by a counter.
  // Reversible (⇒ bijection).
  // State size: S = A×(B+1)
  // Cycle between 2^(A-3) and 2^S÷8 = 2^(A×(B+1)-3) bytes.
  // An interesting aspect is that we still get quite far with a zero state.
  // (eg. 8×4: 2GiB of PractRand.)
  // Benchmarks: please keep in mind the theoretical max cycle, as we get close.
  // 64×3: 0.3 cpb; RNG_test stdin64:      0.008 GiB (mod3n)
  // 64×4: 0.4 cpb; RNG_test stdin64:      0.512 GiB (Low4 FPF)
  // 64×5: 0.5 cpb; RNG_test stdin64:    128 GiB (midly suspicious mod3n at 64GiB)
  //  8×3: 2.5 cpb; RNG_test stdin8:       0.256 GiB (very suspicious FPF at 64MB) (max 512MB)
  //  8×4: 3.4 cpb; RNG_test stdin8:      32 GiB (unusual Gap-16 at 8GiB) (max 128GiB)
  //  8×5: 4.2 cpb; RNG_test stdin8:    4096 GiB (mildly suspicious DC6 at 1TB) (max 32TiB)
  //  8×6: 5.0 cpb; RNG_test stdin8:  >16384 GiB (est. 256TiB) (max 8PiB)
  //  8×3: TestU01: 2/35/86  (Collision Birthday…)
  //  8×4: TestU01: 1/7/5  (Birthday Collision Serial)
  //  8×5: TestU01: 0/1/1  (Birthday)
    //s->gears[0] = COMBIT_ROT(s->gears[0], rot) + s->gears[1];
    //s->gears[1] = COMBIT_ROT(s->gears[1], rot) + s->gears[2];
    //s->gears[2] = COMBIT_ROT(s->gears[2], rot) + s->gears[3];
    //s->gears[3] = COMBIT_ROT(s->gears[3], rot) + s->gears[4];
    //s->gears[4] = COMBIT_ROT(s->gears[4], rot) + s->gears[5];
    //s->gears[5] = COMBIT_ROT(s->gears[5], rot) + s->gears[6];
    //s->gears[6] = COMBIT_ROT(s->gears[6], rot) + s->gears[7];
    //s->gears[3] = COMBIT_ROT(s->gears[3], rot) + s->counter;

    //COMBIT_IT s1 = s->gears[1], s2 = s->gears[2], s3 = s->gears[3], s4 = s->gears[4], s5 = s->gears[5], s6 = s->gears[6], s7 = s->gears[7];
    //s->gears[0] = (s0 >> 0) + s1;
    //s->gears[1] = (s2 << 3) + s3;
    //s->gears[2] = (s1 >> 1) + s0;
    //s->gears[3] = (s3 << 0) + s2 + s->counter;

  // COMBIT_F: Barrel roll with feedback from the first gear. Reversible.
  // State size: S = A×B+log2(A); max cycle = 2^S÷8 bytes.
  // Same first B-1 steps as COMBIT_C, only the last step changes.
  // Benchmarks: (computed with rot = ctz(counter))
  // 64×4: 0.4 cpb; RNG_test stdin64:      8 GiB
  // 64×5: 0.5 cpb; RNG_test stdin64:     64 GiB
  // 64×6: 0.6 cpb; RNG_test stdin64: >32768 GiB
  // 64×7: 0.8 cpb; RNG_test stdin64:        GiB
  // 64×8: 0.9 cpb; RNG_test stdin64:        GiB
  // 32×4: 0.9 cpb; RNG_test stdin32:    128 GiB
  // 32×5: 1.1 cpb; RNG_test stdin32:     32 GiB
  // 32×6: 1.3 cpb; RNG_test stdin32: >32768 GiB
  // 16×4: 1.7 cpb; RNG_test stdin16:      2 GiB
  // 16×5: 2.2 cpb; RNG_test stdin16:     16 GiB
  // 16×6: 2.6 cpb; RNG_test stdin16: >32768 GiB
  //  8×4: 3.4 cpb; RNG_test stdin8:       2 GiB (max 4GiB)
  //  8×5: 4.2 cpb; RNG_test stdin8:       8 GiB (rot=cnt: 256GiB, unusual FPF at 64GiB) (max 1TB)
  //  8×6: 5.2 cpb; RNG_test stdin8:  >32768 GiB (unusual DC6 at 8TB) (max 256TB)
    //s->gears[7] = COMBIT_ROT(s->gears[7], rot) + s->gears[0];

  // COMBIT_SL: Barrel roll with no intermediate layers.
  // Same first B-1 steps as COMBIT_C, only the last step changes.
  // If we use an even number of gears in a round, the round is irreversible:
  // any given state has 2^A possible previous states.
  // Indeed, one equation is linearly dependent on all the others.
  // eg. s0' = s0+s1, s1' = s1+s0 (with 2-gear round and rot = 0).
  // This could be useful as forward secrecy.
  //
  // Benchmarks:
  // (note: rot = ctz(counter) is barely stronger on even, much weaker on odd.)
  // 64×3:     cpb; RNG_test stdin64:      0.016 GiB (unusual mod3n at 4MB)
  // 64×4:     cpb; RNG_test stdin64:      0.064 GiB (unusual mod3n at 16MB)
  // 64×5:     cpb; RNG_test stdin64:  >8192 GiB
  // 64×6:     cpb; RNG_test stdin64:    128 GiB (unusual mod3n at 64GiB)
  // 32×3: 0.7 cpb; RNG_test stdin32:      0.004 GiB (unusual mod3n at 1MB)
  // 32×4: 0.9 cpb; RNG_test stdin32:      0.016 GiB (unusual mod3n at 4MB)
  // 32×5: 1.1 cpb; RNG_test stdin32: >32768 GiB (ctz: Low8 FPF at 4TB)
  // 32×6: 1.3 cpb; RNG_test stdin32:     64 GiB (unusual mod3n at 16GiB) (ctz: mod3n at 128GiB)
  // 32×7: 1.5 cpb; RNG_test stdin32:  >2048 GiB (est. >300EB)
  // 32×8: 1.8 cpb; RNG_test stdin32: >16384 GiB (est. 256TB from even extrapolation)
  // 16×3:     cpb; RNG_test stdin16:      0.002 GiB (unusual mod3n at 512KB) (max 32TiB)
  // 16×4:     cpb; RNG_test stdin16:      0.000256 GiB (sudden Gap-16)
  // 16×5:     cpb; RNG_test stdin16:   4096 GiB (sudden Gap-16, DC6, FPF and BCFN) (max 128ZiB)
  //  8×8:     cpb; RNG_test stdin8:       0.256 GiB (sudden Gap-16)
    //s->gears[4] = COMBIT_ROT(s->gears[4], rot) + s0;

  // COMBIT_SL, AVX2 version.
  // 32×8: 0.8 cpb; RNG_test stdin32:  >4096 GiB (AVX2)
  // 32×8: 0.5 cpb; RNG_test stdin32:    128 GiB (AVX2; output 0||4) suspicious mod3n at 32GiB
    //__m256i shifted = _mm256_set_epi32(0, 7, 6, 5, 4, 3, 2, 1);
    //__m256i g1 = _mm256_load_si256((__m256i*)s->gears);
    //__m256i gs1 = _mm256_permutevar8x32_epi32(g1, shifted);
    //__m256i hr1 = _mm256_or_si256(_mm256_srli_epi32(g1,                  rot),
    //                              _mm256_slli_epi32(g1, COMBIT_REGSIZE - rot));
    //_mm256_store_si256((__m256i*)s->gears, _mm256_add_epi32(gs1, hr1));
    //__m256i g2 = _mm256_load_si256((__m256i*)&s->gears[8]);
    //__m256i gs2 = _mm256_permutevar8x32_epi32(g2, shifted);
    //__m256i hr2 = _mm256_or_si256(_mm256_srli_epi32(g2,                  rot),
    //                              _mm256_slli_epi32(g2, COMBIT_REGSIZE - rot));
    //_mm256_store_si256((__m256i*)&s->gears[8], _mm256_add_epi32(gs2, hr2));

  // COMBIT_SLC: Combined SL (single-layer) and C (counter feeding).
  // 64×3: 0.4 cpb; RNG_test stdin64:      4 GiB (VERY SUSPICIOUS mod3n at 2GiB)
  // 64×4: 0.4 cpb; RNG_test stdin64:    128 GiB (very suspicious mod3n at 64GiB)
  // 64×5: 0.5 cpb; RNG_test stdin64: >16384 GiB
  // 64×6: 0.7 cpb; RNG_test stdin64:    512 GiB (very suspicious mod3n at 256GiB)
  //  8×3:     cpb; RNG_test stdin64:      0.000064 GiB (sudden Gap-16)
  //  8×4:     cpb; RNG_test stdin64:      0.000016 GiB (sudden Gap-16)
  //  8×5:     cpb; RNG_test stdin64:      0.016 GiB (sudden Gap-16)
  //  8×6:     cpb; RNG_test stdin64:      0.064 GiB (sudden Gap-16)
  //  8×7:     cpb; RNG_test stdin64:      4 GiB (sudden Low1 Gap-16)
  //  8×8:     cpb; RNG_test stdin64:      0.512 GiB (sudden Gap-16)
    //s->gears[4] = COMBIT_ROT(s->gears[4], rot) + s0 + s->counter;

  // COMBIT_SLI: single-layer; parallel & intertwined rounds in 8-int state.
  // 64×8: 0.5 cpb; RNG_test stdin64: >32768 GiB (AVX2)
    //__m256i g1 = _mm256_load_si256((__m256i*)s->gears); // g = gears
    //__m256i g2 = _mm256_load_si256((__m256i*)&s->gears[4]);
    //__m256i s1 = _mm256_permute4x64_epi64(g1, 0x39); // s = shifted
    //__m256i s2 = _mm256_permute4x64_epi64(g2, 0x39); // 00111001: 0, 3, 2, 1.
    //__m256i r1 = _mm256_or_si256(_mm256_srli_epi64(g1,                  rot),
    //                             _mm256_slli_epi64(g1, COMBIT_REGSIZE - rot));
    //__m256i n1 = _mm256_add_epi64(s1, r1); // n = new
    //__m256i n2 = _mm256_add_epi64(s2, r1);
    //_mm256_store_si256((__m256i*)&s->gears[4], n2);
    //_mm256_store_si256((__m256i*)s->gears, _mm256_xor_si256(n1, n2));

  // COMBIT_SLT: 2 rounds in 4-int state with increased mixing.
  // 64×4: 0.4 cpb; RNG_test stdin64:     16 GiB (sudden Low4 FPF failure)
    //__m256i g = _mm256_load_si256((__m256i*)s->gears); // g = gears
    //__m256i br = _mm256_permute4x64_epi64(g, 0x39); // br = barrel roll
    //__m256i br2 = _mm256_permute4x64_epi64(g, 0x4e); // 01001110: 1, 0, 3, 2.
    //__m256i rg = _mm256_or_si256(_mm256_srli_epi64(g,                  rot),
    //                             _mm256_slli_epi64(g, COMBIT_REGSIZE - rot));
    //__m256i n = _mm256_add_epi64(br, rg); // n = new; rg = rotate gears
    //_mm256_store_si256((__m256i*)s->gears, _mm256_xor_si256(n, br2));

  // Best mixing with 4-int round within 7-int state:
  //   a b c d e f g
  //   a b c (de ef fg gd)
  //   a b (cde def efg fgc) gd
  //   a (bcde cdef defg efgb) fgc gd
  //   (abcde bcdef cdefg defga) efgb fgc gd
  //   abcde bcdef cdefg (defgab efgbc fgcd gdefa)
  //   abcde bcdef (cdefgab defgabc efgbcd fgcde) gdefa
  //   abcde (bcdefga cdefgab defgabc efgbcd) fgcde gdefa
  //   (abcdefg bcdefga cdefgab defgabc) efgbcd fgcde gdefa
  // Mixing of each state int: 7 7 7 7 6 5 5
  // Mixing within first int of each round: 2 3 4 5 6 7 7 7
  // Idea abandoned because of poor results (1GiB fail) and segfaults with AVX.

  // Weaker 32-bit version, good for 64GiB in raw JS.
    //COMBIT_IT s1 = s->gears[1];
    //s1 = ((s1        + (s1 <<  8)) & 0xff000000)
    //  | (((s1 << 11) + (s1 << 16)) & 0x00ff0000)
    //  | (((s1 >>  9) + (s1 >> 16)) & 0x0000ff00)
    //  |  ((s1        + (s1 >>  8)) & 0x000000ff);
    //s->gears[1] = s0;
    //s->gears[0] = s1 + s->counter;
    //buf[i] = s1 ^ s0;

  // LEHMER
    //buf[i] = (s->s *= 0xda942042e4dd58b5) >> 64;  // 0.2 cpb

//    buf[i] = s->gears[0]; // | ((COMBIT_T)(s->gears[4]) << COMBIT_REGSIZE);
//#if COMBIT_DEBUG >= 2
//    __int64_t end = _rdtsc();
//    printf("Cycles: %ld\n", end - start);
//    //combit_detect_cycle(s);
//#endif
//#if COMBIT_DEBUG >= 1
//    print_bits(buf[i]); printf("\n");
//#endif
//  }

  // COMBIT_SLCP: P parallel COMBIT_SLC initialized randomly.
  // 64×3×4:      cpb; RNG_test stdin64: >16384 GiB
  // 64×5×16: 0.3 cpb; RNG_test stdin64: >32768 GiB
  s->simdcounter = _mm256_add_epi64(s->simdcounter, _mm256_set_epi64x(1, 1, 1, 1));
  __m128i simdrot = _mm_and_si128(_mm256_castsi256_si128(s->simdcounter), _mm_set_epi64x(COMBIT_REGADDR, COMBIT_REGADDR));
  __m128i simdrevrot = _mm_sub_epi64(_mm_set_epi64x(COMBIT_REGSIZE, COMBIT_REGSIZE), simdrot);
#define COMBIT_SLCP_STEP(START, STEP) \
  __m256i g##STEP##_##START = _mm256_load_si256((__m256i*)&s->gears[(START) + STEP*4*4]); \
  __m256i r##STEP##_##START = _mm256_or_si256(_mm256_srl_epi64(g##STEP##_##START, simdrot), _mm256_sll_epi64(g##STEP##_##START, simdrevrot));
#define COMBIT_SLCP_END(START, STEP, NEXTSTEP) \
  __m256i n##STEP##_##START = _mm256_add_epi64(r##STEP##_##START, g##NEXTSTEP##_##START); \
  _mm256_store_si256((__m256i*)&s->gears[(START) + STEP*4*4], n##STEP##_##START);
#define COMBIT_SLCP_PRNG(START) \
  COMBIT_SLCP_STEP(START, 0) \
  COMBIT_SLCP_STEP(START, 1) \
  COMBIT_SLCP_STEP(START, 2) \
  COMBIT_SLCP_STEP(START, 3) \
  COMBIT_SLCP_STEP(START, 4) \
  COMBIT_SLCP_END(START, 0, 1) \
  COMBIT_SLCP_END(START, 1, 2) \
  COMBIT_SLCP_END(START, 2, 3) \
  COMBIT_SLCP_END(START, 3, 4) \
  __m256i n4_##START = _mm256_add_epi64(r4_##START, _mm256_add_epi64(g0_##START, s->simdcounter)); \
  _mm256_store_si256((__m256i*)&s->gears[(START) + 4*4*4], n4_##START);
  COMBIT_SLCP_PRNG(0);
  COMBIT_SLCP_PRNG(4);
  COMBIT_SLCP_PRNG(8);
  COMBIT_SLCP_PRNG(12);
  for (char i = 0; i < COMBIT_BUFSIZE; i++) { buf[i] = s->gears[5*i]; }

  // COMBIT_SLCPX: 16 parallel COMBIT_SLC with a 64-bit counter spanning gears.
  // 32×7×16: 0.7 cpb
  //  8×8×16:     cpb (should be much faster because we do 8 PRNGs in parallel, but AVX-512 only)
//  s->simdcounter = _mm256_add_epi64(s->simdcounter, _mm256_set_epi64x(1, 1, 1, 1));
//  __m256i shifted = _mm256_set_epi32(7, 0, 6, 5, 4, 3, 2, 1);
//  __m128i simdrot = _mm_and_si128(_mm256_castsi256_si128(s->simdcounter), _mm_set_epi64x(COMBIT_REGADDR, COMBIT_REGADDR));
//  __m128i simdrevrot = _mm_sub_epi64(_mm_set_epi64x(COMBIT_REGSIZE, COMBIT_REGSIZE), simdrot);
//#define COMBIT_SLCPX_PRNG(START) \
//  __m256i g##START = s->simdgears[START]; \
//  __m256i p##START = _mm256_permutevar8x32_epi32(g##START, shifted); \
//  __m256i r##START = _mm256_or_si256(_mm256_srl_epi32(g##START, simdrot), _mm256_sll_epi32(g##START, simdrevrot)); \
//  s->simdgears[START] = _mm256_add_epi32(p##START, r##START);
//  COMBIT_SLCPX_PRNG( 0); COMBIT_SLCPX_PRNG( 1); COMBIT_SLCPX_PRNG( 2); COMBIT_SLCPX_PRNG( 3);
//  COMBIT_SLCPX_PRNG( 4); COMBIT_SLCPX_PRNG( 5); COMBIT_SLCPX_PRNG( 6); COMBIT_SLCPX_PRNG( 7);
//  COMBIT_SLCPX_PRNG( 8); COMBIT_SLCPX_PRNG( 9); COMBIT_SLCPX_PRNG(10); COMBIT_SLCPX_PRNG(11);
//  COMBIT_SLCPX_PRNG(12); COMBIT_SLCPX_PRNG(13); COMBIT_SLCPX_PRNG(14); COMBIT_SLCPX_PRNG(15);
//  for (char i = 0; i < COMBIT_BUFSIZE; i++) { buf[i] = _mm256_extract_epi32(s->simdgears[i], 0); }

#if COMBIT_DEBUG >= 1
  for (char i = 0; i < COMBIT_BUFSIZE; i++) { print_bits(buf[i]); printf("\n"); }
#endif
}

#if COMBIT_DEBUG >= 1
void print_bits(COMBIT_T v) {
  for (int i = 0; i < COMBIT_REGSIZE; i++) {
    printf("%d", (char)(v >> (COMBIT_REGSIZE - 1)));
    v <<= 1;
  }
}

void print_m256i(__m256i avx) {
  COMBIT_IT s[256 / COMBIT_REGSIZE];
  _mm256_store_si256((__m256i*)s, avx);
  for (int i = 0; i < 256 / COMBIT_REGSIZE; i++) {
    print_bits(s[i]);
  }
}

void print_m128i(__m128i avx) {
  COMBIT_IT s[128 / COMBIT_REGSIZE];
  _mm_store_si128((__m128i*)s, avx);
  for (int i = 0; i < 128 / COMBIT_REGSIZE; i++) {
    print_bits(s[i]);
  }
}

void print_state(combit_state *s) {
  printf(" c: "); print_bits(s->counter); printf("\n");
  for (int i = 0; i < 8; i++) {
    printf(" %d: ", i);
    print_bits(s->gears[i]);
    printf("\n");
  }
}

COMBIT_IT cycle0 = 0, cycle1 = 0, cycle2 = 0, cycle3 = 0;
__uint64_t cycle_counter = 0, cycle_to_set = 0x3;
void combit_detect_cycle(combit_state *s) {
  if (cycle_counter++ == cycle_to_set) {
    cycle0 = s->gears[0]; cycle1 = s->gears[1];
    cycle2 = s->gears[2]; cycle3 = s->gears[3];
    return;
  }
  if (s->gears[0] == cycle0
   && s->gears[1] == cycle1
   && s->gears[2] == cycle2
   && s->gears[3] == cycle3
   //&& s->counter == 1
      ) {
    fprintf(stderr, "FULL CYCLE REACHED after %ld; %ld\n", cycle_counter - cycle_to_set, s->counter);
  }
}
#endif
#endif
