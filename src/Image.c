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
#include "../headers/Image.h"
#include "../headers/BMPHandler.h"

// FORWARD DECLARATION
int clamp(int value);

// FUNCTIONS
Image* image_create(struct Pixel** pArr, int width, int height) {
    Image * new_img = (Image*) malloc(sizeof (pArr));
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
    //TODO
}

int clamp(int value) {
    if (value < 0) {
        return 0;
    } else if (value > 255) {
         return 255;
    } else {
        return value;
    }
}