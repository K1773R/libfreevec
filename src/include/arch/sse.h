#define SIMD_PACKETSIZE     16
#ifdef LINUX64
#define WORDS_IN_PACKET     2
#else
#define WORDS_IN_PACKET     4
#endif

#define READ_PREFETCH_START1(addr)
#define READ_PREFETCH_START2(addr)
#define WRITE_PREFETCH_START1(addr)
#define WRITE_PREFETCH_START2(addr)
#define PREFETCH_STOP1
#define PREFETCH_STOP2
