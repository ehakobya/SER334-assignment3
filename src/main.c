#include <stdio.h>
#include <stdlib.h>
#include "../headers/BMPHandler.h"
#include "../headers/Image.h"

void printBMP(struct BMP_Header BMP);
void printDIB(struct DIB_Header DIB);

int main() {

//    int rowSize = (10 * 3 + 3) & ~3;
//    int temp = (4 - (30 % 4)) + (30);
//    printf("%d %d %d", ~3, temp, rowSize);

    //START - HEADER FUNCTIONALITY MINIMAL EXAMPLE (DO NOT SUBMIT)
    struct BMP_Header BMP;
    struct DIB_Header DIB;

    FILE* file_input = fopen("../assets/wb.bmp", "rb");
    if (file_input != NULL) printf("--> File Opened\n");

    readBMPHeader(file_input, &BMP);
    readDIBHeader(file_input, &DIB);

    printf("------------\n");
    printBMP(BMP);
    printf("------------\n");
    printDIB(DIB);
    printf("------------\n");


    struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * DIB.img_height);
    for (int p = 0; p < DIB.img_height; p++) {
        pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * DIB.img_width);
    }
//    printf("%llu\n", sizeof(pixels));
//    printf("%llu\n", sizeof(struct Pixel*));
//    printf("%d\n", DIB.img_height);

    readPixelsBMP(file_input, pixels, DIB.img_width, DIB.img_height);
    fclose(file_input);

    Image* img = image_create(pixels, DIB.img_width, DIB.img_height);


    printf("R:%d G:%d B:%d  |  ", img->pArr[0][0].red, img->pArr[0][0].green, img->pArr[0][0].blue); // blue
    printf("R:%d G:%d B:%d\n", img->pArr[1][1].red, img->pArr[1][1].green, img->pArr[1][1].blue); // green
    printf("R:%d G:%d B:%d  |  ", img->pArr[1][0].red, img->pArr[1][0].green, img->pArr[1][0].blue); // red
    printf("R:%d G:%d B:%d\n", img->pArr[0][1].red, img->pArr[0][1].green, img->pArr[0][1].blue); // white
    printf("----------------------------------\n");

//    image_apply_bw(img);
    image_apply_colorshift(img, 0, 0, 0);

    printf("R:%d G:%d B:%d  |  ", img->pArr[0][0].red, img->pArr[0][0].green, img->pArr[0][0].blue); // blue
    printf("R:%d G:%d B:%d\n", img->pArr[1][1].red, img->pArr[1][1].green, img->pArr[1][1].blue); // green
    printf("R:%d G:%d B:%d  |  ", img->pArr[1][0].red, img->pArr[1][0].green, img->pArr[1][0].blue); // red
    printf("R:%d G:%d B:%d\n", img->pArr[0][1].red, img->pArr[0][1].green, img->pArr[0][1].blue); // white

    // print original image
//    for (int i = 0; i < image_get_height(img); i++) {
//        for (int j = 0; j < image_get_width(img); j++) {
//            printf("%d ", img->pArr[i][j]);
//        }
//        printf("\n");
//    }

//    image_apply_resize(img, 1);
//    image_apply_colorshift(img, 255, 0, 0);

    /*
    // print new image
    for (int i = 0; i < image_get_height(img); i++) {
        for (int j = 0; j < image_get_width(img); j++) {
            printf("%d ", img->pArr[i][j]);
        }
        printf("\n");
    }
     */

//    makeBMPHeader(&BMP, img->width, img->height);
    FILE* file_output = fopen("outputXXXX.bmp", "wb");
    writeBMPHeader(file_output, &BMP);
    writeDIBHeader(file_output, &DIB);
    writePixelsBMP(file_output, image_get_pixels(img), image_get_width(img), image_get_height(img));
    fclose(file_output);

    /*
    FILE* file_output = fopen("ttt_usagesample.bmp", "wb");
    writeBMPHeader(file_output, &BMP);
    writeDIBHeader(file_output, &DIB);
    writePixelsBMP(file_output, image_get_pixels(img), image_get_width(img), image_get_width(img));
    image_destroy(img);
    fclose(file_output);
    //END - HEADER FUNCTIONALITY MINIMAL EXAMPLE (DO NOT SUBMIT)
*/
    return 0;
}

void printBMP(struct BMP_Header BMP) {
    printf("Signature:\t%c %c\n", BMP.signature[0],BMP.signature[1]);
    printf("File Size:\t%d\n", BMP.file_size);
    printf("Reserved1:\t%d\n", BMP.reserved1);
    printf("Reserved2:\t%d\n", BMP.reserved2);
    printf("Offset:\t\t%d\n", BMP.offset);
}

void printDIB(struct DIB_Header DIB) {
    printf("Header Size:\t%d\n", DIB.header_size);
    printf("Image Width:\t%d\n", DIB.img_width);
    printf("Image Height:\t%d\n", DIB.img_height);
    printf("Planes:\t%d\n", DIB.planes);
    printf("Bits Per P:\t%d\n", DIB.bits_per_pixel);
    printf("Compression:\t%d\n", DIB.compression);
    printf("Image Size:\t%d\n", DIB.img_size);
    printf("X Pixel P/M:\t%d\n", DIB.x_pixel_per_m);
    printf("Y Pixel P/M:\t%d\n", DIB.y_pixel_per_m);
    printf("Color Table:\t%d\n", DIB.num_color_table_colors);
    printf("Sig Colors:\t%d\n", DIB.num_important_colors);
}