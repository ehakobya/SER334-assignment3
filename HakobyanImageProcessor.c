#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BMPHandler.h"
#include "Image.h"

void displayBMP(struct BMP_Header BMP);

void displayDIB(struct DIB_Header DIB);

int main(int argc, char *argv[]) {

    char *inputFileName = argv[1];

    char *w = NULL;
    char *outputFileName;
    float s = 0;
    int r = 0, g = 0, b = 0;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-w") == 0) {
            w = argv[i];
            continue;
        } else if (strcmp(argv[i], "-r") == 0) {
            r = atoi(argv[i + 1]);
            continue;
        } else if (strcmp(argv[i], "-g") == 0) {
            g = atoi(argv[i + 1]);
            continue;
        } else if (strcmp(argv[i], "-b") == 0) {
            b = atoi(argv[i + 1]);
            continue;
        } else if (strcmp(argv[i], "-s") == 0) {
            s = atof(argv[i + 1]);
            continue;
        } else if (strcmp(argv[i], "-o") == 0) {
            outputFileName = argv[i + 1];
//            continue;
        }
    }

    //START - HEADER FUNCTIONALITY MINIMAL EXAMPLE (DO NOT SUBMIT)
    struct BMP_Header BMP;
    struct DIB_Header DIB;

    FILE *file_input = fopen(inputFileName, "rb");
    if (file_input != NULL) printf("--> File Opened\n");

    readBMPHeader(file_input, &BMP);
    readDIBHeader(file_input, &DIB);

//    printf("------------\n");
//    displayBMP(BMP);
//    printf("------------\n");
//    displayDIB(DIB);
//    printf("------------\n");


    struct Pixel **pixels = (struct Pixel **) malloc(sizeof(struct Pixel *) * DIB.img_height);
    for (int p = 0; p < DIB.img_height; p++) {
        pixels[p] = (struct Pixel *) malloc(sizeof(struct Pixel) * DIB.img_width);
    }

    readPixelsBMP(file_input, pixels, DIB.img_width, DIB.img_height);
    fclose(file_input);

    Image *img = image_create(pixels, DIB.img_width, DIB.img_height);

    if (strcmp(w, "-w") == 0) {
        image_apply_bw(img);
    }
    if ( r != 0 || b != 0 || g != 0) {
        image_apply_colorshift(img, r,g,b);
    }
    if (s != 0) {
        image_apply_resize(img, s);
    }

    makeBMPHeader(&BMP, img->width, img->height);
    makeDIBHeader(&DIB, img->width, img->height);

    if (outputFileName != NULL) {
        FILE *file_output = fopen(outputFileName, "wb");
        writeBMPHeader(file_output, &BMP);
        writeDIBHeader(file_output, &DIB);
        writePixelsBMP(file_output, image_get_pixels(img), image_get_width(img), image_get_height(img));
        fclose(file_output);
        printf("\nFinished Writing to File");
    }
    return 0;
}

// for debugging purposes
void displayBMP(struct BMP_Header BMP) {
    printf("Signature:\t%c %c\n", BMP.signature[0], BMP.signature[1]);
    printf("File Size:\t%d\n", BMP.file_size);
    printf("Reserved1:\t%d\n", BMP.reserved1);
    printf("Reserved2:\t%d\n", BMP.reserved2);
    printf("Offset:\t\t%d\n", BMP.offset);
}

// for debugging purposes
void displayDIB(struct DIB_Header DIB) {
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