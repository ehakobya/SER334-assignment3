/**
* Implementation of BMPHandler
*
* Completion time: XXXXXXX
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
    //TODO
}

void makeDIBHeader(struct DIB_Header* header, int width, int height) {
    //TODO
}

void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {
    //TODO
}

void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {
    //TODO
}