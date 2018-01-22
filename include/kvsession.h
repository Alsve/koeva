/*
 * kvsession.h: utility for managing session state of application
 *
 * BSD 3-clause license
 * Author : Al Rayan, Bogor Agricultural University, 2018 
 *          <alrayan12t@apps.ipb.ac.id>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the BSD 3-Clause License as 
 * published by the BSD Organization; 3-Clause version of the
 * license.
 */

#ifndef KOEVA_SESSION__H
#define KOEVA_SESSION__H

typedef enum SNI_1_2907_2008_DEFECTS {
        D_HITAM,
        D_HITAM_SEBAGIAN,
        D_HITAM_PECAH,
        D_GELONDONG,
        D_COKLAT,
        D_KULIT_BESAR,
        D_KULIT_SEDANG,
        D_KULIT_KECIL,
        D_TANDUK,
        D_TANDUK_BESAR,
        D_TANDUK_SEDANG,
        D_TANDUK_KECIL,
        D_PECAH,
        D_MUDA,
        D_BERLUBANG_SATU,
        D_BERLUBANG_BANYAK,
        D_BERTUTUL,
        D_RANTING_TANAH_BATU_BESAR,
        D_RANTING_TANAH_BATU_SEDANG,
        D_RANTING_TANAH_BATU_KECIL,
        KOPI_BAGUS
} koeva_defect_list;

const double SNI_1_2907_2008_DEFECTS_VALUE[] = {
        1.00,
        0.50,
        0.50,
        1.00,
        0.25,
        1.00,
        0.50,
        0.20,
        0.50,
        0.50,
        0.20,
        0.10,
        0.20,
        0.20,
        0.10,
        0.20,
        0.10,
        5.00,
        2.00,
        1.00,
        0.00
};

struct koeva_session {
        unsigned int total_beans;
        unsigned int defects_array[KOPI_BAGUS];
        void (*add_defects)(struct koeva_session *this,
                            koeva_defect_list value);
        int (*what_grade)(struct koeva_session*);
};

extern struct koeva_session * kv_session_new();
extern void kv_session_destroy(struct koeva_session*);

#endif
