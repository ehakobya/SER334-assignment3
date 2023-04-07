/**
* Implementation of BmpProcessor
*
* Completion time: 45 min
*
* @author Edgar Hakobyan
* @version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include "BmpProcessor.h"


void readBMPHeader(FILE* file, struct BMP_Header* header) {
    fread(&header->signature, sizeof(char)*2, 1, file); // read signature
    fread(&header->size, sizeof(int), 1, file); // read file size
    fread(&header->reserved1, sizeof(short), 1, file); // read reserved1
    fread(&header->reserved2, sizeof(short), 1, file); // read reserved2
    fread(&header->offset_pixel_array, sizeof(int), 1, file); // read offset
}

void writeBMPHeader(FILE* file, struct BMP_Header* header) {
    fwrite(&header->signature, sizeof(char)*2, 1, file); // write signature
    fwrite(&header->size, sizeof(int), 1, file); // write file size
    fwrite(&header->reserved1, sizeof(short), 1, file); // write reserved1
    fwrite(&header->reserved2, sizeof(short), 1, file); // write reserved2
    fwrite(&header->offset_pixel_array, sizeof(int), 1, file); // write offset
}

void readDIBHeader(FILE* file, struct DIB_Header* header) {
    fread(&header->size, sizeof(int), 1, file); // read header size
    fread(&header->width, sizeof(int), 1, file); // read image width
    fread(&header->height, sizeof(int), 1, file); // read image height
    fread(&header->planes, sizeof(short), 1, file); // read planes
    fread(&header->bitsPerPixel, sizeof(short), 1, file); // read bits per pixel
    fread(&header->compression, sizeof(int), 1, file); // read compression
    fread(&header->imageSize, sizeof(int), 1, file); // read image size
    fread(&header->horizRes, sizeof(int), 1, file); // read x pixel per meter
    fread(&header->vertRes, sizeof(int), 1, file); // read y pixel per meter
    fread(&header->colorNum, sizeof(int), 1, file); // read colors in color table
    fread(&header->importantColorNum, sizeof(int), 1, file); // read important color count
}

void writeDIBHeader(FILE* file, struct DIB_Header* header) {
    fwrite(&header->size, sizeof(int), 1, file); // write header size
    fwrite(&header->width, sizeof(int), 1, file); // write image width
    fwrite(&header->height, sizeof(int), 1, file); // write image height
    fwrite(&header->planes, sizeof(short), 1, file); // write planes
    fwrite(&header->bitsPerPixel, sizeof(short), 1, file); // write bits per pixel
    fwrite(&header->compression, sizeof(int), 1, file); // write compression
    fwrite(&header->imageSize, sizeof(int), 1, file); // write image size
    fwrite(&header->horizRes, sizeof(int), 1, file); // write x pixel per meter
    fwrite(&header->vertRes, sizeof(int), 1, file); // write y pixel per meter
    fwrite(&header->colorNum, sizeof(int), 1, file); // write colors in color table
    fwrite(&header->importantColorNum, sizeof(int), 1, file); // write important color count
}

void makeBMPHeader(struct BMP_Header* header, int width, int height) {
    header->signature[0] = 'B';
    header->signature[1] = 'M';
    int temp = width * height * 3;
//    header->file_size = 54 + ((4 - (temp % 4)) + (temp));
    header->size = 54 + width * height * 3;
    header->reserved1 = 0;
    header->reserved2 = 0;
}

void makeDIBHeader(struct DIB_Header* header, int width, int height) {
    header->size = 40;
    header->width = width;
    header->height = height;
    header->planes = 1;
    header->bitsPerPixel = 24; //3 bytes = 24 bits
    header->compression = 0;
    header->imageSize = width * height * 3; // TODO
    header->horizRes = 3780;
    header->vertRes = 3780;
    header->colorNum = 0;
    header->importantColorNum = 0;
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