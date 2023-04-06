/**
* Implementation of BMPHandler
*
* Completion time: 45 min
*
* @author Edgar Hakobyan
* @version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include "../headers/BMPHandler.h"


void readBMPHeader(FILE* file, struct BMP_Header* header) {
    fread(&header->signature, sizeof(char)*2, 1, file); // read signature
    fread(&header->file_size, sizeof(int), 1, file); // read file size
    fread(&header->reserved1, sizeof(short), 1, file); // read reserved1
    fread(&header->reserved2, sizeof(short), 1, file); // read reserved2
    fread(&header->offset, sizeof(int), 1, file); // read offset
}

void writeBMPHeader(FILE* file, struct BMP_Header* header) {
    fwrite(&header->signature, sizeof(char)*2, 1, file); // write signature
    fwrite(&header->file_size, sizeof(int), 1, file); // write file size
    fwrite(&header->reserved1, sizeof(short), 1, file); // write reserved1
    fwrite(&header->reserved2, sizeof(short), 1, file); // write reserved2
    fwrite(&header->offset, sizeof(int), 1, file); // write offset
}

void readDIBHeader(FILE* file, struct DIB_Header* header) {
    fread(&header->header_size, sizeof(int), 1, file); // read header size
    fread(&header->img_width, sizeof(int), 1, file); // read image width
    fread(&header->img_height, sizeof(int), 1, file); // read image height
    fread(&header->planes, sizeof(short), 1, file); // read planes
    fread(&header->bits_per_pixel, sizeof(short), 1, file); // read bits per pixel
    fread(&header->compression, sizeof(int), 1, file); // read compression
    fread(&header->img_size, sizeof(int), 1, file); // read image size
    fread(&header->x_pixel_per_m, sizeof(int), 1, file); // read x pixel per meter
    fread(&header->y_pixel_per_m, sizeof(int), 1, file); // read y pixel per meter
    fread(&header->num_color_table_colors, sizeof(int), 1, file); // read colors in color table
    fread(&header->num_important_colors, sizeof(int), 1, file); // read important color count
}

void writeDIBHeader(FILE* file, struct DIB_Header* header) {
    fwrite(&header->header_size, sizeof(int), 1, file); // write header size
    fwrite(&header->img_width, sizeof(int), 1, file); // write image width
    fwrite(&header->img_height, sizeof(int), 1, file); // write image height
    fwrite(&header->planes, sizeof(short), 1, file); // write planes
    fwrite(&header->bits_per_pixel, sizeof(short), 1, file); // write bits per pixel
    fwrite(&header->compression, sizeof(int), 1, file); // write compression
    fwrite(&header->img_size, sizeof(int), 1, file); // write image size
    fwrite(&header->x_pixel_per_m, sizeof(int), 1, file); // write x pixel per meter
    fwrite(&header->y_pixel_per_m, sizeof(int), 1, file); // write y pixel per meter
    fwrite(&header->num_color_table_colors, sizeof(int), 1, file); // write colors in color table
    fwrite(&header->num_important_colors, sizeof(int), 1, file); // write important color count
}

void makeBMPHeader(struct BMP_Header* header, int width, int height) {
    header->signature[0] = 'B';
    header->signature[1] = 'M';
    int temp = width * height * 3;
//    header->file_size = 54 + ((4 - (temp % 4)) + (temp));
    header->file_size = 54 + width * height * 3;
    header->reserved1 = 0;
    header->reserved2 = 0;
}

void makeDIBHeader(struct DIB_Header* header, int width, int height) {
    header->header_size = 40;
    header->img_width = width;
    header->img_height = height;
    header->planes = 1;
    header->bits_per_pixel = 24; //3 bytes = 24 bits
    header->compression = 0;
    header->img_size = width * height * 3; // TODO
    header->x_pixel_per_m = 3780;
    header->y_pixel_per_m = 3780;
    header->num_color_table_colors = 0;
    header->num_important_colors = 0;
}

void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {
    int padding;
    int rowSize;
    if ((width % 4) == 0) {
        padding = 0;
        rowSize = width;
    } else {
        rowSize = (4 - ((width * 3) % 4)) + (width * 3);
//    int rowSize = (width * 3 + 3) & ~3; // OTHER METHOD
        padding = rowSize - (width * 3);
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fread(&pArr[i][j], sizeof (struct Pixel), 1, file);
        }
        fseek(file, padding, SEEK_CUR); // skip reading file by places indicated in padding
    }
}

void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {
    int padding;
    int rowSize;
    if ((width % 4) == 0) {
        padding = 0;
        rowSize = width;
    } else {
        rowSize = (4 - ((width * 3) % 4)) + (width * 3);
//    int rowSize = (width * 3 + 3) & ~3; // OTHER METHOD
        padding = rowSize - (width * 3);
    }
    unsigned char zero[3] = {0, 0, 0};
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fwrite(&pArr[i][j], sizeof (struct Pixel), 1, file);
        }
        fwrite(zero, 1, padding, file); // add padding 0's
    }
}