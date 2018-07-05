/*
 * kvimage.h : utility for koeva image processing
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

#ifndef KOEVA_IMAGE__H
#define KOEVA_IMAGE__H

struct image_features {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char area;
    unsigned char edge;
};

struct image_features * kv_image_extract_feature(IplImage *src);

#endif
