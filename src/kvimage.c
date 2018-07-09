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

#define KV_FOR_HEIGHT_WIDTH(iplimage) \
        for (i = 0; i < (iplimage)->height; i++) \
        for (j = 0; j < (iplimage)->width; j++)

inline double rgbtodo(unsigned char RGB)
{
    return RGB * 2 / 255 - 1;
}

static CvPoint koeva_image_flood_fill_seed(IplImage *image)
{
        int i, j, pix;
        int x, y;
        IplImage *clone = cvCloneImage(image);

        x = 0, y = 0;

        cvFloodFill(clone, cvPoint(0, 0), cvScalarAll(255), cvScalarAll(0),
                    cvScalarAll(0), NULL, 4, NULL);

        KV_FOR_HEIGHT_WIDTH(clone) {
            if (!clone->imageData[i * clone->widthStep + j]) {
                x = j;
                y = i;
                goto image_flood_fill_seed_out_nested_for;
            }
        }

image_flood_fill_seed_out_nested_for:
        cvReleaseImage(&image);

        return cvPoint(x, y);
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
 * Returns average value of RGB color or error code
 */
static struct RGBColors koeva_image_color_average(IplImage *logical_image, 
                                                  IplImage *src)
{
        int i, j, pix;
        size_t red_sum = 0;
        size_t gre_sum = 0;
        size_t blu_sum = 0;
        size_t counts = 0;

        ASSERT(logical_image, "Logical image is missing");
        ASSERT(src, "Source image is missing");
        ASSERT(logical_image->width == src->width, 
                "Images width are not equal\n");
        ASSERT(logical_image->height == src->height, 
                "Images height are not equal\n");

        KV_FOR_HEIGHT_WIDTH(logical_image) {
                pix = i * logical_image->widthStep +
                      j * logical_image->nChannels;
                if ((unsigned char)logical_image->imageData[pix])) {
                        red_sum += (unsigned char) 
                                   src->imageData[pix + 2];
                        gre_sum += (unsigned char) 
                                   src->imageData[pix + 1];
                        blu_sum += (unsigned char) 
                                   src->imageData[pix];
                        counts++;
                }
        }
                        
        return (struct RGBColors) {rgbtodo(red_sum / counts),
                                   rgbtodo(gre_sum / counts),
                                   rgbtodo(blu_sum / counts)};
error:
        return -EPERM;
}

/*
 * koeva_image_area - counts area of logical image
 * @logical_image: logical image as input
 *
 * Returns pixel area of GBC
 */
static size_t koeva_image_area(IplImage *logical_image)
{
        int i, j, pix;
        size_t area = 0;

        KV_FOR_HEIGHT_WIDTH(logical_image) {
                pix = i * logical_image->widthStep + j;

                if ((unsigned char) logical_image->imageData[pix])
                      area++;
        }

        return area;
error:
        return -EPERM;
}

/*
 * koeva_image_edge - counts edge pixel of the GBC
 * @logical_image: logical image as input
 *
 * Returns edge counts of GBC
 */
static size_t koeva_image_edge(IplImage *logical_image)
{
        int i, j, pix;
        size_t edge = 0;
        IplImage *clone = cvCloneImage(logical_image);

        cvCanny(clone, clone, KOEVA_CANNY_LOW_THRESHOLD, 
                KOEVA_CANNY_HIGH_THRESHOLD, 3, 0);

        KV_FOR_HEIGHT_WIDTH(clone) {
                pix = i * clone->widthStep + j;

                if ((unsigned char) clone->imageData[pix])
                      edge++;
        }

        cvReleaseImage(&image);

        return edge;
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
        ret->color = koeva_image_color_average(logical_image, src);
        ret->area = koeva_image_area(logical_image);
        ret->edge = koeva_image_edge(logical_image);

        return ret;
error:
        return NULL;
}