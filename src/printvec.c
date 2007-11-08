/***************************************************************************
 *   Copyright (C) 2005 by Konstantinos Margaritis                         *
 *   markos@debian.gr                                                      *
 *                                                                         *
 *   This code is distributed under a BSD-type license                     *
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

void printbuf16(char *label, uint8_t *buf) {
    int i;
    printf("%s : ", label);
    for (i=0; i < 16; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}

void printbuf4(char *label, uint32_t *buf) {
    int i;
    printf("%s : ", label);
    for (i=0; i < 4; i++) {
        printf("%08x ", buf[i]);
    }
    printf("\n");
}

void printvec_text(char *label, vector uint8_t vc) {
    int i;
    union {
        vector uint8_t v; 
        uint8_t c[16]; 
    } vt_char;
    
    vt_char.v = vc;
    printf("%10s : ", label);
    for (i=0; i < 16; i++) {
        printf("%c ", vt_char.c[i]);
    }
    printf("\n");
}

void printvec_char(char *label, vector uint8_t vc) {
    int i;
    union {
        vector uint8_t v; 
        uint8_t c[16]; 
    } vt_char;
    
    vt_char.v = vc;
    printf("%10s : ", label);
    for (i=0; i < 16; i++) {
        printf("%02x ", vt_char.c[i]);
    }
    printf("\n");
}

void printvec_short(char *label, vector uint16_t vs) {
    int i;
    union {
        vector uint16_t v; 
        uint16_t s[8]; 
    } vt_short;
    
    vt_short.v = vs;
    printf("%10s : ", label);
    for (i=0; i < 8; i++) {
        printf("%04x ", vt_short.s[i]);
    }
    printf("\n");
}

void printvec_long(char *label, vector uint32_t vl) {
    int i;
    union {
        vector uint32_t v; 
        uint32_t l[4]; 
    } vt_int;
    
    vt_int.v = vl;
    printf("%10s : ", label);
    for (i=0; i < 4; i++) {
        printf("%08x ", vt_int.l[i]);
    }
    printf("\n");
}

#endif