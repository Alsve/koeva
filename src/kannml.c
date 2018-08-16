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

/*

Goals and Theory of Operation

The primary goal of this feature is to provide matlab-like set functions
that are used in neural-network operation in matlab. The function name
is made similar to matlab functions.

The matrix used in this feature is 2D matrix that being declared in the
header file (struct mat2D). There are three members of the structure:
m, n, arr. The member variable m and n is dimension of the matrix. On
the other hand arr is a pointer that is intended to be accessed to an
one dimensional array. Every function that return this struct should be
freed at the end of the function using mat2D_destroy(the_matrix_variable)
*/

#include <kannml.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

/*
 * repmat - Repeat copies of array
 * @_mat: input array or matrix
 * @_mt: repeat m times of dimension m
 * @_nt: repeat n times of dimension n
 *
 * Returns result matrix
 */
struct mat2D repmat(struct mat2D _mat, int _mt, int _nt)
{
        int i, j, x, y;
        int repedmat_pos;
        struct mat2D ret = mat2D_create(_mat.m * _mt, _mat.n * _nt);

        for (i = 0; i < _mt; i++)
            for (j = 0; j < _nt; j++)
                for(y = 0; y < _mat.m; y++)
                    for(x = 0; x < _mat.n; x++) {
                        repedmat_pos = i * _nt * _mat.m * _mat.n + j * _mat.n;
                        repedmat_pos += y * _mat.n * _nt + x;
                        ret.arr[repedmat_pos] = _mat.arr[y * _mat.n + x];
                    }

        return ret;
}

/*
 * bsxfun - element wise operation array
 * @_op: operation selector string
 * @_mat: input matrix that will be operated on
 * @_const: constant that alter the matrix in selected operator manner
 *
 */
void bsxfun(char * _op, struct mat2D _mat, double _const)
{
        int i, j;
        int pos;

        for (i = 0; i < _mat.m; i++)
                for (j = 0; j < _mat.n; j++) {
                        pos = i * _mat.n + j;
                        if (!strcmp(_op, "@plus"))
                                _mat.arr[pos] += _const;
                        else if (!strcmp(_op, "@minus"))
                                _mat.arr[pos] -= _const;
                        else if (!strcmp(_op, "@times"))
                                _mat.arr[pos] *= _const;
                        else if (!strcmp(_op, "@rdivide"))
                                _mat.arr[pos] /= _const;
                        else if (!strcmp(_op, "@ldivide"))
                                _mat.arr[pos] = _const / _mat.arr[pos];
                        else if (!strcmp(_op, "@exp"))
                                _mat.arr[pos] = exp(_mat.arr[pos]);
                }
}

/*
 * mat2D_transpose - transpose the matrix
 * @_mat: input matrix
 *
 * Returns transposed matrix
 */
struct mat2D mat2D_transpose(struct mat2D _mat)
{
        int i, j;
        struct mat2D ret = mat2D_create(_mat.n, _mat.m);

        for (i = 0; i < _mat.m; i++)
            for (j = 0; j < _mat.n; j++)
                ret.arr[j * ret.n + i] = _mat.arr[i * _mat.n + j];

        return ret;
}

static double __bsxfun_matrix_times_sum(struct mat2D _mat, struct mat2D _mat2,
                                        int _posx, int _posy) 
{
        int i;
        double val = 0;

        for (i = 0; i < _mat.n; i++)
                val += _mat.arr[_posy * _mat.n + i] * 
                    _mat2.arr[i * _mat2.n + _posx];

        return val;
}

/*
 * bsxfun_matrix - matrix-matrix operations
 * @_op: operation selector string
 * @_mat: first positioned input matrix
 * @_mat2: second positioned input matrix
 *
 * Returns result matrix
 */
struct mat2D bsxfun_matrix(char *_op, struct mat2D _mat, struct mat2D _mat2)
{
        int i, j;
        int pos;
        struct mat2D ret = mat2D_create(0, 0);

        if (!strcmp(_op, "@plus") || !strcmp(_op, "@minus")) {
                if (_mat.n != _mat2.n && _mat.m != _mat2.m) {
                        fprintf(stderr, "ERROR @%s: @plus OR @minus,"
                                        " matrix don't agree!\n", __FUNCTION__);
                        return ret;
                }

                ret.m = _mat.m;
                ret.n = _mat.n;
                ret.arr = malloc(sizeof(double) * ret.m * ret.n);

                for (i = 0; i < ret.m; i++)
                        for ( j = 0; j < ret.n; j++) {
                                pos = i * ret.n + j;
                                ret.arr[pos] = _mat.arr[pos];
                                ret.arr[pos] += !strcmp(_op, "@plus") ? 
                                               _mat2.arr[pos] : -_mat2.arr[pos];
                        }

        } else if (!strcmp(_op, "@times")) {
                if (_mat.n != _mat2.m) {
                        fprintf(stderr, "ERROR @%s: @times,"
                                        " matrix don't agree!\n", __FUNCTION__);
                        return ret;
                }

                ret.m = _mat.m;
                ret.n = _mat2.n;
                ret.arr = malloc(sizeof(double) * ret.m * ret.n);

                for (i = 0; i < ret.m; i++)
                        for (j = 0; j < ret.n; j++) {
                                pos = i * ret.n + j;
                                ret.arr[pos] = __bsxfun_matrix_times_sum(_mat, 
                                                                _mat2, j, i);
                        }

        } else if (!strcmp(_op, "@times_ewise")) {
        		if (_mat.m != _mat2.m && _mat.n != _mat2.n) {
                        fprintf(stderr, "ERROR @%s: @times_ewise,"
                                        " matrix don't agree!\n", __FUNCTION__);
                        return ret;
                }

                ret.m = _mat.m;
                ret.n = _mat.n;
                ret.arr = malloc(sizeof(double) * ret.m * ret.n);

                struct mat2D _mat2_reped = repmat(_mat2, _mat.m / _mat2.m, _mat.n / _mat2.n);

                for (i = 0; i < ret.m; i++)
                        for (j = 0; j < ret.n; j++) {
                                pos = i * ret.n + j;
                                ret.arr[pos] = _mat.arr[pos] * _mat2_reped.arr[pos];
                        }

                mat2D_destroy(_mat2_reped);
        }

        return ret;
}

/*
 * mapminmax_apply - normalization functions
 * @_mat: input matrix
 * @_step: normalizations parameter
 *
 * Returns normalized matrix
 */
struct mat2D mapminmax_apply(struct mat2D _mat, struct step1 _step)
{
        struct mat2D res = bsxfun_matrix("@minus", _mat, _step.xoffset);
        struct mat2D res2 = bsxfun_matrix("@times_ewise", res, _step.gain);
        bsxfun("@plus",  res2, _step.ymin);
        
        mat2D_destroy(res);

        return res2;
}

/*
 * mapminmax_reverse - revert normalization functions
 * @_mat: input matrix
 * @_step: reverting normalizations parameter
 *
 * Returns reverted matrix to produce output
 */
void mapminmax_reverse(struct mat2D _mat, struct step1 _step)
{
        bsxfun("@minus", _mat, _step.ymin);
        bsxfun("@rdivide", _mat, _step.gain.arr[0]);
        bsxfun("@plus", _mat, _step.xoffset.arr[0]);
}

/*
 * tansig_apply - tangent sigmoid function
 * @_mat: input matrix
 *
 * Returns result matrix
 */
struct mat2D tansig_apply(struct mat2D _mat)
{
        struct mat2D ret = mat2D_clone(_mat);

        bsxfun("@times", ret, -2);
        bsxfun("@exp", ret, 0);
        bsxfun("@plus", ret, 1);
        bsxfun("@ldivide", ret, 2);
        bsxfun("@minus", ret, 1);

        return ret;
}