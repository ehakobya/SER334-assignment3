/**
* Specification for an image ADT.
*
* Completion time: ?
*
* @author Edgar Hakobyan, Ruben Acuna
* @Date 03/20/2023
* @version 1.0
*/

// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../headers/Image.h"
#include "../headers/BMPHandler.h"

// FUNCTIONS
Image* image_create(struct Pixel** pArr, int width, int height) {
    Image * new_img = (Image*) malloc(sizeof (Image));
    new_img->width = width;
    new_img->height = height;
    new_img->pArr = pArr;
    return new_img;
}

void image_destroy(Image** img) {
    //TODO
}

struct Pixel** image_get_pixels(Image* img) {
    return img->pArr;
}

int image_get_width(Image* img) {
    return img->width;
}

int image_get_height(Image* img) {
    return img->height;
}

void image_apply_bw(Image* img) {
    for (int i = 0; i < img->width; i++) {
        for (int j = 0; j < img->height; j++) {
            unsigned char gray = img->pArr[i][j].red * 0.299 + img->pArr[i][j].green * 0.587 + img->pArr[i][j].blue * 0.114;
            img->pArr[i][j].red = gray;
            img->pArr[i][j].green = gray;
            img->pArr[i][j].blue = gray;
        }
    }
}

void image_apply_colorshift(Image* img, int rShift, int gShift, int bShift) {
    for (int i = 0; i < img->width; i++) {
        for (int j = 0; j < img->height; j++) {
            int newRed = img->pArr[i][j].red + rShift;
            int newGreen = img->pArr[i][j].green + gShift;
            int newBlue = img->pArr[i][j].blue + bShift;
            img->pArr[i][j].red = (unsigned char)((newRed > 255) ? 255 : (newRed < 0) ? 0 : newRed);
            img->pArr[i][j].green = (unsigned char)((newGreen > 255) ? 255 : (newGreen < 0) ? 0 : newGreen);
            img->pArr[i][j].blue = (unsigned char)((newBlue > 255) ? 255 : (newBlue < 0) ? 0 : newBlue);
        }
    }
}

void image_apply_resize(Image* img, float factor) {

    int newWidth = (int)(img->width * factor);
    int newHeight = (int)(img->height * factor);
    struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * newHeight);
    for (int i = 0; i < newHeight; i++) {
        pixels[i] = (struct Pixel*)malloc(sizeof(struct Pixel) * newWidth);
    }
    // Create new image
    Image* new_img = image_create(pixels, newWidth, newHeight);

    // Copy pixels and scale
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            int oldX = (int)(j / factor);
            int oldY = (int)(i / factor);
            new_img->pArr[i][j] = img->pArr[oldY][oldX];
        }
    }

    free(img->pArr);
    img->pArr = new_img->pArr;
    img->width = newWidth;
    img->height = newHeight;


//    int newHeight = image_get_height(img) * factor;
//    printf("%d\n", newHeight);
//    int newWidth = image_get_width(img) * factor;
//    printf("%d\n", newWidth);
//
//    struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * newHeight);
//    for (int p = 0; p < newHeight; p++) {
//        pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * newWidth);
//    }
//    printf("%d\n", sizeof(struct Pixel*));
//    printf("%d\n", sizeof(*pixels));
//
//    Image* img2 = image_create(pixels, newWidth, newHeight);
//    img2->pArr = pixels;

//    int i, j, oi, oj;
//    for (i = newHeight-1; i >= 0; i--) {
//        oi = i / factor;  // compute the original row index
//        for (j = newWidth-1; j >= 0; j--) {
//            oj = j / factor;  // compute the original column index
//            if (oi < 0 || oi >= img->height || oj < 0 || oj >= img->width) {
//                // handle out-of-bounds cases
//                img2->pArr[i][j] = (struct Pixel) {0, 0, 0};  // set to black
//            } else {
//                img2->pArr[i][j].red = img->pArr[oi][oj].red;
//                img2->pArr[i][j].green = img->pArr[oi][oj].green;
//                img2->pArr[i][j].blue = img->pArr[oi][oj].blue;
//                printf("pixels: R:%d G:%d B:%d\n", pixels[i][j].red,  pixels[i][j].green,  pixels[i][j].blue);
//                printf("pArr: R:%d G:%d B:%d\n", img->pArr[oi][oj].red,  img->pArr[oi][oj].green,  img->pArr[oi][oj].blue);

//                printf("i:%d j:%d oi:%d oj:%d\n", i,j,oi,oj);
//            }
//        }
//    }
//    printf("%llu\n", sizeof(*img));
//    for (int p = 0; p < img->height; p++) {
//        free(img->pArr[p]);
//    }
//
//
//    free(img->pArr);

//    img2->pArr = pixels;
//    img2->height = newHeight;
//    img2->width = newWidth;
//
//    img = img2;
//    printf("%llu\n", sizeof(*img));
//    printf("%llu\n", sizeof(*img2));
//    printf("%llu\n", sizeof(**pixels));
//    printf("%llu\n", sizeof(img->pArr));
//    printf("%llu\n", sizeof(img2->pArr));
}