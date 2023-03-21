/**
* Specification for an image ADT.
*
* Completion time: ?
*
* @author Edgar Hakobyan, Ruben Acuna
* @Date 03/20/2023
* @version 1.0
*/
#include <stdio.h>
#include <stdlib.h>
#include "../headers/Image.h"
#include "../headers/BMPHandler.h"

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
    //TODO
}

void image_apply_resize(Image* img, float factor) {
    //TODO
}