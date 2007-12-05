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
#include <stdlib.h>

#ifdef HAVE_ALTIVEC_H
#include <altivec.h>

#include "libfreevec.h"
#include "macros/memcmp.h"
#include "macros/common.h"

#ifdef VEC_GLIBC
int memcmp(void *src1pp, const void *src2pp, size_t len) {
#else
int vec_memcmp(void *src1pp, const void *src2pp, size_t len) {
#endif

  const uint8_t *src2 = (uint8_t *) src2pp;
  const uint8_t *src1 = (uint8_t *) src1pp;

  if (len >= sizeof(uint32_t)) {
    // Prefetch some stuff
    READ_PREFETCH_START1(src1);
    READ_PREFETCH_START2(src2);

    uint32_t src1al = (uint32_t)src1 % sizeof(uint32_t);
    if (src1al)
      MEMCMP_UNTIL_SRC1_WORD_ALIGNED(src1, src2, len, src1al);

    // Take the word-aligned long pointers of src2 and src1.
    uint8_t src2al = (uint32_t)(src2) % sizeof(uint32_t);

    uint32_t *src1l = (uint32_t *)(src1);
    const uint32_t *src2l = (uint32_t *)(src2 -src2al);

    // Now src1 is word aligned. If possible (ie if there are enough bytes left)
    // we want to align it to 16-byte boundaries as well.
    // For this we have to know the word-alignment of src2 also.
    src1al = (uint32_t)src1 % ALTIVECWORD_SIZE;
    if (src1al)
      MEMCMP_UNTIL_SRC1_IS_ALTIVEC_ALIGNED(src1, src1l, src2, src2l, len, src1al, src2al);

    if (len >= ALTIVECWORD_SIZE) {
      // Check for the alignment of src2
      if (((uint32_t)(src2) % ALTIVECWORD_SIZE) == 0) {
        // Now, both buffers are 16-byte aligned, compare using altivec with no alignment tricks
        MEMCMP_LOOP_SINGLE_ALTIVEC_WORD_ALIGNED(src1, src1l, src2, src2l);
      } else {
        // src2 is not 16-byte aligned so we have to a little trick with Altivec.
        MEMCMP_LOOP_SINGLE_ALTIVEC_WORD_UNALIGNED(src1, src1l, src2, src2l, src2al);
      }
      PREFETCH_STOP1;
      PREFETCH_STOP2;
    }

    MEMCMP_REST_WORDS(src1, src1l, src2, src2l, len, src2al);
    src1 = (uint8_t *) src1l;
    src2 = (uint8_t *) src2l +src2al;

    MEMCMP_NIBBLE(src1, src2, len);
    return 0;
  } else {
    MEMCMP_NIBBLE(src1, src2, len);
    return 0;
  }
}
#endif
