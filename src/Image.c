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

    /*
    int i, j;
    Image * tmp = (Image*) malloc(sizeof(Image));
    int newH = img->height * factor;
    int newW = img->width * factor;

    tmp->pArr = (struct Pixel**)malloc(sizeof(struct Pixel*) * tmp->height);
    for (i = 0; i < tmp->height; i++) {
        tmp->pArr[i] = (struct Pixel*)malloc(sizeof(struct Pixel) * tmp->width);
        for (j = 0; j < tmp->width; j++) {
            tmp->pArr[i][j] = img->pArr[i/factor][j/factor];
        }
    }
    for (i = 0; i < img->height; i++)
        free(img->pArr[i]);
    free(img->pArr);


     6 == 6.0

    printf("%d\n", newWidth);
    printf("%d\n", newHeight);

    for (int i = 0 ; i < image_get_height(img) ; i++) {
        for (int j = 0 ; j < image_get_height(img) ; j++) {
            printf("| R:%d G:%d B:%d | ", img->pArr[i][j].red,img->pArr[i][j].green,img->pArr[i][j].blue);
        }
        printf("\n");
    }
    printf("\n");

 2D pixel array
    struct Pixel** newArr = (struct Pixel**)malloc((sizeof(struct Pixel*)* newHeight));
    printf("%d\n", sizeof(newArr));
    printf("%d\n", sizeof(struct Pixel*));
    printf("%d\n", newHeight);
    for (int p = 0; p < newHeight; p++) {
        newArr[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * newWidth);
    }

    struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * 6);
    for (int p = 0; p < 6; p++) {
        pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * 6);
    }

    int newH = img->height * factor;
    int newW = img->width * factor;
    struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * newH);
    for (int p = 0; p < newH; p++) {
        pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * newW);
        for (int q = 0; q < newW; q++) {
            pixels[p][q].red = 0;
            pixels[p][q].green = 0;
            pixels[p][q].blue = 0;
        }
    }
    for (int i = 0; i < newH; i++) {
        for (int j = 0; j < newW; j++) {
            int original_i = round(i / factor);
            int original_j = round(j / factor);
            pixels[i][j] = img->pArr[original_i][original_j];
        }
    }

    for (int i = 0; i < image_get_height(img); i++) {
        free(img->pArr[i]);
    }
    free(img->pArr);

    img->pArr = pixels;
    img->height = newH;
    img->width = newW;

    printf("-------%d\n", newHeight);

    struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * newHeight);
    for (int p = 0; p < newHeight; p++) {
        pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * newWidth);
    }
    printf("%llu\n", sizeof(newPix));
    printf("%llu\n", sizeof(struct Pixel*));
    printf("%d\n", newHeight);


    for (int i = 0 ; i < newHeight ; i++) {
        for (int j = 0 ; j < newWidth ; j++) {
            printf("| R:%d G:%d B:%d | ", newArr[i][j].red,newArr[i][j].green,newArr[i][j].blue);
        }
        printf("\n");
    }

    int i;
    int j;
    for (i = 0 ; i < newHeight ; i++) {
        for (j = 0 ; j < newWidth ; j++) {
            int original_i = floor(i / 2);
            printf("Orig I: %d\n", original_i);
            int original_j = floor(j / 2);
            printf("Orig J: %d\n", original_j);
            printf("I: %d\n", i);
            printf("J: %d\n", j);
//            int original_index = original_i* image_get_width(img) + original_j;
//            int new_index = i + newWidth + j;
            pixels[i][j] = img->pArr[original_i][original_j];
            printf("%d %d %d\n", img->pArr[original_i][original_j].red, img->pArr[original_i][original_j].green, img->pArr[original_i][original_j].blue);
            printf("----\n");
        }
    }
    img->pArr = pixels;
    img->height = newHeight;
    img->width = newWidth;

    int newHeight = round(image_get_height(img) * factor);
    int newWidth = round(image_get_width(img) * factor);

    if (newHeight <= 0 || newWidth <= 0) {
        printf("Error: Invalid dimensions after resizing\n");
        return;
    }

    struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * newHeight);
    if (pixels == NULL) {
        printf("Error: Failed to allocate memory for pixel array\n");
        return;
    }
    for (int p = 0; p < newHeight; p++) {
        pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * newWidth);
        if (pixels[p] == NULL) {
            printf("Error: Failed to allocate memory for pixel row %d\n", p);
            for (int q = 0; q < p; q++) {
                free(pixels[q]);
            }
            free(pixels);
            return;
        }
    }

    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            int original_i = round(i / factor);
            int original_j = round(j / factor);
            if (original_i < 0 || original_i >= image_get_height(img) || original_j < 0 || original_j >= image_get_width(img)) {
                printf("Warning: Invalid original pixel coordinates (%d, %d) for new pixel (%d, %d)\n", original_i, original_j, i, j);
                pixels[i][j].red = 0;
                pixels[i][j].green = 0;
                pixels[i][j].blue = 0;
            } else {
                pixels[i][j] = img->pArr[original_i][original_j];
            }
        }
    }

    for (int i = 0; i < image_get_height(img); i++) {
        free(img->pArr[i]);
    }
    free(img->pArr);

    img->pArr = pixels;
    img->height = newHeight;
    img->width = newWidth;
    */

    uint32_t i = 0, j = 0;
    Image *tmp = (Image*)malloc(sizeof(Image));
    tmp->height = img->height * factor;
    tmp->width = img->width * factor;

    tmp->pArr = (struct Pixel**)malloc(sizeof(struct Pixel*) * tmp->height);
    for (i = 0; i < tmp->height; i++) {
        tmp->pArr[i] = (struct Pixel*)malloc(sizeof(struct Pixel) * tmp->width);
        for (j = 0; j < tmp->width; j++) {
            tmp->pArr[i][j] = img->pArr[(int)floorf(i/factor)][(int)floorf(j/factor)];
        }
    }

    for (i = 0; i < img->height; i++)
        free(img->pArr[i]);
    free(img->pArr);

    img->width = tmp->width;
    img->height = tmp->height;
    img->pArr = tmp->pArr;
    free(tmp);

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