/*
 * kvimage.c : utility for koeva image processing
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

The primary goal of this feature is to get logical area in pixel of the green 
bean coffe (GBC) to be used as masking reference of the GBC object. The
extracted information would be an array of feature from the object.

*/

#include <stdlib.h>
#include <errno.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <koeva-util-debug.h>
#include <kvimage.h>

#define KOEVA_BLUR_KERNEL_SIZE 11
#define KOEVA_BLUR_SIGMA 3.75

#define KOEVA_CANNY_LOW_THRESHOLD 0
#define KOEVA_CANNY_HIGH_THRESHOLD 0

static CvPoint koeva_image_flood_fill_seed(IplImage *image)
{
	int x, y;

	x = 0, y = 0;

	return CvPoint(x, y);
}

/*
 * koeva_image_get_logical_image - produce object mask from GBC image input
 * @dst: logical image output
 * @src: image input
 *
 * Returns errors code
 */
static int koeva_image_get_logical_image(IplImage *dst, IplImage *src)
{
	ASSERT(dst->width == src->width, "Images width are not equal\n");
	ASSERT(dst->height == src->height, "Images height are not equal\n");

	cvCvtColor(src, dst, CV_RGB2GRAY);
	cvSmooth(dst, dst, CV_GAUSSIAN, KOEVA_BLUR_KERNEL_SIZE, 
			 KOEVA_BLUR_KERNEL_SIZE, KOEVA_BLUR_SIGMA, KOEVA_BLUR_SIGMA);
	cvCanny(dst, dst, KOEVA_CANNY_LOW_THRESHOLD, KOEVA_CANNY_HIGH_THRESHOLD,
			3, 0);
	cvDilate(dst, dst, NULL, 1);
	cvErode(dst, dst, NULL, 1);
	cvFloodFill(dst, koeva_image_flood_fill_seed(dst), cvScalarAll(255), 
				cvScalarAll(0), cvScalarAll(0), NULL, 4, NULL);

	return 0;
	
error:
	return -EPERM;
}

/*
 * koeva_image_color_average_red - calculate avg red color value of an GBC
 * @logical_image: logical image input
 * @src: image of an GBC
 *
 * Returns average value of red color or error code
 */
static int koeva_image_color_average_red(IplImage *logical_image, 
										 IplImage *src)
{

error:
	return -EPERM;
}

/*
 * koeva_image_color_average_green - calculate avg green color value of an GBC
 * @logical_image: logical image input
 * @src: image of an GBC
 *
 * Returns average value of green color or error code
 */
static int koeva_image_color_average_green(IplImage *logical_image, 
										   IplImage *src)
{

error:
	return -EPERM;
}

/*
 * koeva_image_color_average_blue - calculate avg blue color value of an GBC
 * @logical_image: logical image input
 * @src: image of an GBC
 *
 * Returns average value of blue color or error code
 */
static int koeva_image_color_average_blue(IplImage *logical_image, 
										  IplImage *src)
{

error:
	return -EPERM;
}

/*
 * koeva_image_area - counts area of logical image
 * @logical_image: logical image as input
 *
 * Returns pixel area of GBC
 */
static int koeva_image_area(IplImage *logical_image)
{

error:
	return -EPERM;
}

/*
 * koeva_image_edge - counts edge pixel of the GBC
 * @logical_image: logical image as input
 *
 * Returns edge counts of GBC
 */
static int koeva_image_edge(IplImage *logical_image)
{

error:
	return -EPERM;
}

/*
 * koeva_image_extract_feature - produce image_feature object
 * @src: one GBC in an image.
 *
 * Returns all an object struct of image_features.
 */
struct image_features * kv_image_extract_feature(IplImage *src)
{
	ASSERT(src, "%s : image source is NULL\n", __FUNCTION__);
	IplImage *logical_image = cvCreateImage(cvSize(src->width, src->height),
											src->depth, 1);
	struct image_feature *ret = malloc(sizeof(*ret));
	ASSERT_MEM(ret);

	koeva_image_get_logical_image(logical_image, src);
	ret->red = koeva_image_color_average_red(logical_image, src);
	ret->green = koeva_image_color_average_green(logical_image, src);
	ret->blue = koeva_image_color_average_green(logical_image, src);
	ret->area = koeva_image_area(logical_image);
	ret->edge = koeva_image_edge(logical_image);

error:
	return NULL;
}