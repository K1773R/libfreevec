/*********************************************************************************
 *   Copyright (C) 2008-2010 by Konstantinos Margaritis <markos@codex.gr>        *
 *   All rights reserved.                                                        *
 *                                                                               *
 * Redistribution and use in source and binary forms, with or without            *
 * modification, are permitted provided that the following conditions are met:   *
 *  1. Redistributions of source code must retain the above copyright            *
 *     notice, this list of conditions and the following disclaimer.             *
 *  2. Redistributions in binary form must reproduce the above copyright         *
 *     notice, this list of conditions and the following disclaimer in the       *
 *     documentation and/or other materials provided with the distribution.      *
 *  3. Neither the name of the Codex nor the                                     *
 *     names of its contributors may be used to endorse or promote products      *
 *     derived from this software without specific prior written permission.     *
 *                                                                               *
 * THIS SOFTWARE IS PROVIDED BY CODEX ''AS IS'' AND ANY                          *
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED     *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE        *
 * DISCLAIMED. IN NO EVENT SHALL CODEX BE LIABLE FOR ANY                         *
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;  *
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND   *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT    *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                  *
 *********************************************************************************/

/* $Id$ */

#include <stdint.h>
#include <stddef.h>
#include <altivec.h>

#ifdef LINUX64
#include "scalar64/memset.h"
#else
#include "scalar32/memset.h"
#endif

static inline __vector uint8_t simdpacket_set_from_byte(const uint8_t p) {
  __vector uint8_t v = vec_lde(0, &p);
  return vec_splat(v, 0);
}

#define MEMSET_ALTIVECWORD(ptr32, vc, len)  \
{                                           \
  vec_st(vc, 0, (uint8_t *) ptr32);         \
  ptr32 += 4; len -= ALTIVECWORD_SIZE;      \
}

#define MEMSET_LOOP_ALTIVECWORD(ptr32, vc, len)  \
{                                                \
  while (len >= ALTIVECWORD_SIZE) {              \
    MEMSET_ALTIVECWORD(ptr32, vc, len);          \
  }                                              \
}

#define MEMSET_LOOP_QUADWORD(ptr32, vc, len)  \
{                                             \
  WRITE_PREFETCH_START2(ptr32);               \
  uint32_t blocks = len >> LOG_ALTIVECQUAD;   \
  len -= blocks << LOG_ALTIVECQUAD;           \
  while (blocks--) {                          \
    vec_st(vc, 0, (uint8_t *)ptr32);          \
    vec_st(vc, 16, (uint8_t *)ptr32);         \
    vec_st(vc, 32, (uint8_t *)ptr32);         \
    vec_st(vc, 48, (uint8_t *)ptr32);         \
    ptr32 += 16;                              \
  }                                           \
}

