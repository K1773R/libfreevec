/***************************************************************************
 *   Copyright (C) 2005-2007 by CODEX                                      *
 *   Konstantinos Margaritis <markos@codex.gr>                             *
 *                                                                         *
 *   This code is distributed under the LGPL license                       *
 *   See http://www.gnu.org/copyleft/lesser.html                           *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <limits.h>

#ifdef HAVE_ALTIVEC_H
#include <altivec.h>

#include "libfreevec.h"
#include "macros/memcpy.h"

#ifdef VEC_GLIBC
void *memcpy(void *dstpp, const void *srcpp, size_t len) {
#else
void *vec_memcpy(void *dstpp, const void *srcpp, size_t len) {
#endif

  const uint8_t *src = srcpp;
  uint8_t *dst = dstpp;

  if (len >= sizeof(uint32_t)) {
    // Prefetch some stuff
    READ_PREFETCH_START1(src);
    WRITE_PREFETCH_START2(dst);

    // Copy until dst is word aligned
    MEMCPY_FWD_UNTIL_DEST_IS_WORD_ALIGNED(dst, src, len);

    // Now dst is word aligned. We'll continue by word copying, but
    // for this we have to know the word-alignment of src also.
    uint8_t srcoffset4 = ((uint32_t)(src) % sizeof(uint32_t)), sh_l, sh_r;
    sh_l = srcoffset4 * CHAR_BIT; sh_r = CHAR_BIT*sizeof(uint32_t) - sh_l;

    // Take the word-aligned long pointers of src and dest.
    uint32_t *dstl = (uint32_t *)(dst);
    const uint32_t *srcl = (uint32_t *)(src -srcoffset4);

    // While we're not 16-byte aligned, move in 4-byte long steps.
    MEMCPY_FWD_UNTIL_DEST_IS_ALTIVEC_ALIGNED(dstl, srcl, len, srcoffset4, sh_l, sh_r);
    src = (uint8_t *) srcl +srcoffset4;

    // Now, dst is 16byte aligned. We can use Altivec if len >= 32
    if (((uint32_t)(src) % ALTIVECWORD_SIZE) == 0) {
      if (len >= QUAD_ALTIVECWORD) {
        MEMCPY_FWD_LOOP_QUADWORD_ALTIVEC_ALIGNED(dstl, src, len);
      }
      while (len >= ALTIVECWORD_SIZE) {
        MEMCPY_SINGLEQUADWORD_ALTIVEC_ALIGNED(dstl, src, 0);
        dstl += 4; src += ALTIVECWORD_SIZE; len -= ALTIVECWORD_SIZE;
      }
    } else {
      if (len >= QUAD_ALTIVECWORD) {
        MEMCPY_FWD_LOOP_QUADWORD_ALTIVEC_UNALIGNED(dstl, src, len);
      }
      while (len >= ALTIVECWORD_SIZE) {
        MEMCPY_SINGLEQUADWORD_ALTIVEC_UNALIGNED(dstl, src, 0);
        dstl += 4; src += ALTIVECWORD_SIZE; len -= ALTIVECWORD_SIZE;
      }
    }
    srcl = (uint32_t *)(src -srcoffset4);

    // Stop the prefetching
    PREFETCH_STOP1;
    PREFETCH_STOP2;

    // Copy the remaining bytes using word-copying
    // Handle alignment as appropriate
    if (srcoffset4 == 0) {
      MEMCPY_FWD_REST_WORDS_ALIGNED(dstl, srcl, len);
      src = (uint8_t *) srcl;
    } else {
      MEMCPY_FWD_REST_WORDS_UNALIGNED(dstl, srcl, len, sh_l, sh_r);
      src = (uint8_t *) srcl +srcoffset4;
    }
    // For the end copy we have to use char * pointers.
    dst = (uint8_t *) dstl;
  }
  // Copy the remaining bytes
  MEMCPY_FWD_NIBBLE(dst, src, len);
  return dstpp;
}

#endif