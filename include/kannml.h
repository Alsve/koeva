/*
 * kannml.h : matlab like functions for artificial neural network
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

#ifndef KOEVA_ANN_ML__H
#define KOEVA_ANN_ML__H

struct mat2D {
        int m;
        int n;
        double *arr;
};

struct step1 {
        double ymin;
        struct mat2D gain;
        struct mat2D xoffset;
};

#define mat2D_create(m, n) \
        {(m), (n), m * n  != 0 ? malloc(sizeof(double) * (m) * (n)) : NULL}

#define mat2D_destroy(mat) \
        do{free(mat.arr);} while(0)

#define mat2D_clone(mat)                                             \
        ({                                                           \
          struct mat2D ____r = mat2D_create(mat.m, mat.n);           \
          memcpy(____r.arr, mat.arr, sizeof(double) * mat.m * mat.n);\
          ____r;                                                     \
        })

#define mat2D_size(mat, dim) \
        ((dim) == 1 ? (mat).m : (dim) == 2 ? (mat).n : 0)

struct mat2D repmat(struct mat2D _mat, int _mt, int _nt);
void bsxfun(char * _op, struct mat2D _mat, double _const);
struct mat2D mat2D_transpose(struct mat2D _mat);
struct mat2D bsxfun_matrix(char *_op, struct mat2D _mat, struct mat2D _mat2);
struct mat2D mapminmax_apply(struct mat2D _mat, struct step1 _step);
void mapminmax_reverse(struct mat2D _mat, struct step1 _step);
struct mat2D tansig_apply(struct mat2D _mat);

#endif 