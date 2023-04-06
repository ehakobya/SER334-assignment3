/**
* Image processing with threadss
*
* Completion time: (estimation of hours spent on this program)
*
* @author Edgar Hakobyan
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

//TODO: finish me

//UNCOMMENT BELOW LINE IF USING SER334 LIBRARY/OBJECT FOR BMP SUPPORT
#include "../headers/BmpProcessor.h"


////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define BMP_HEADER_SIZE 14
#define BMP_DIB_HEADER_SIZE 40
#define MAXIMUM_IMAGE_SIZE 4096
#define THREAD_COUNT 0

/////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATION
typedef struct Image Image;

void readBMPHeader(FILE *file, struct BMP_Header *header);

void writeBMPHeader(FILE *file, struct BMP_Header *header);

void readDIBHeader(FILE *file, struct DIB_Header *header);

void writeDIBHeader(FILE *file, struct DIB_Header *header);

void makeBMPHeader(struct BMP_Header *header, int width, int height);

void makeDIBHeader(struct DIB_Header *header, int width, int height);

void readPixelsBMP(FILE *file, struct Pixel **pArr, int width, int height);

void writePixelsBMP(FILE *file, struct Pixel **pArr, int width, int height);

struct Image *image_create(struct Pixel **pArr, int width, int height);

struct Pixel **image_get_pixels(Image *img);

int image_get_width(Image *img);

int image_get_height(Image *img);

void applyBoxBlur(Image *img, int blurRadius);


////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
struct Image {
    struct Pixel **pArr;
    int width;
    int height;
};


////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

//TODO: finish me


//TODO: change back to void
int main(int argc, char *argv[]) {

//    char *inputFileName = argv[1];
//
//    char *w = NULL;
//    char *outputFileName;
//    float s = 0;
//    int r = 0;
//    int g = 0;
//    int b = 0;
//
//    for (int i = 0; i < argc; i++) {
//        if (strcmp(argv[i], "-w") == 0) {
//            w = argv[i];
//            continue;
//        } else if (strcmp(argv[i], "-r") == 0) {
//            r = atoi(argv[i + 1]);
//            continue;
//        } else if (strcmp(argv[i], "-g") == 0) {
//            g = atoi(argv[i + 1]);
//            continue;
//        } else if (strcmp(argv[i], "-b") == 0) {
//            b = atoi(argv[i + 1]);
//            continue;
//        } else if (strcmp(argv[i], "-s") == 0) {
//            s = atof(argv[i + 1]);
//            continue;
//        } else if (strcmp(argv[i], "-o") == 0) {
//            outputFileName = argv[i + 1];
//        }
//    }

    char inputFileName[] = "../assets/test3.bmp";
    char outputFileName[] = "../output/output.bmp";

    struct BMP_Header BMP;
    struct DIB_Header DIB;

    FILE *file_input = fopen(inputFileName, "rb");
    if (file_input != NULL) printf("--> File opened\n");

    readBMPHeader(file_input, &BMP);
    readDIBHeader(file_input, &DIB);

    struct Pixel **pixels = (struct Pixel **) malloc(sizeof(struct Pixel *) * DIB.width);
    for (int p = 0; p < DIB.height; p++) {
        pixels[p] = (struct Pixel *) malloc(sizeof(struct Pixel) * DIB.width);
    }
    readPixelsBMP(file_input, pixels, DIB.width, DIB.height);
    fclose(file_input);

    Image *img = image_create(pixels, DIB.width, DIB.height);


    applyBoxBlur(img, 10);
    printf("--> Box blur applied\n");


    makeBMPHeader(&BMP, img->width, img->height);
    makeDIBHeader(&DIB, img->width, img->height);

    if (outputFileName != NULL) {
        FILE *file_output = fopen(outputFileName, "wb");
        writeBMPHeader(file_output, &BMP);
        writeDIBHeader(file_output, &DIB);
        writePixelsBMP(file_output, image_get_pixels(img), image_get_width(img), image_get_height(img));
        printf("--> Finished writing to file\n");
        fclose(file_output);
        printf("--> File closed\n");
    }


    //TODO: Delete afterwards
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//FILTER FUNCTIONS
/**
 * Blur box algorithm takes an input image and blur radius/strength.
 * For each pixel in the array, it applies blur by calculation the average
 * rgb values of the neighbors specified by the radius and including rgb of the current pixel.
 * @param img: Pointer to the input image for modification
 * @param width: Width of the image
 * @param height: Height of the image
 * @param blurRadius: Blur radius/strength
 */
void applyBoxBlur(Image *img, int blurRadius) {

    int width = image_get_width(img);
    int height = image_get_height(img);

    struct Pixel **temp = (struct Pixel **) malloc(sizeof(struct Pixel *) * width);
    for (int i = 0; i < height; i++) {
        temp[i] = (struct Pixel *) malloc(sizeof(struct Pixel) * width);
    }
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            int redSum = 0, greenSum = 0, blueSum = 0;
            int count = 0;

            /*
                 0   1   2   3   4   5
               +---+---+---+---+---+---+
            1  |   |   |   |   |   |   |
               +---+---+---+---+---+---+
            2  |   | O | O | O |   |   |
               +---+---+---+---+---+---+
            3  |   | O | X | O |   |   |
               +---+---+---+---+---+---+
            4  |   | O | O | O |   |   |
               +---+---+---+---+---+---+
            5  |   |   |   |   |   |   |
               +---+---+---+---+---+---+

               - For example, current pixel to be transformed is 'X'
               - We need to go to each 'O' get their value and add it to sum
               to go to position [2][1] we need to subtract half of the radius
               from current position for both x and y.
               - That way we loop over in a circle (clockwise) around the current target
               and collect it's neighbors rgb values and add it to the sum.
            */
            for (int ky = -blurRadius / 2; ky <= blurRadius / 2; ky++) {
                for (int kx = -blurRadius / 2; kx <= blurRadius / 2; kx++) {

                    int px = x + kx; // store the iteration index (denoted by O)
                    int py = y + ky; // store the iteration index (denoted by O)

                    // conditional makes sure that there is no access of uninitialized memory.
                    // prevents errors when target index (denoted by X) is on the edges or corners
                    /*
                         0   1   2   3   4   5
                       +---+---+---+---+---+---+
                    1  | X | O |   |   |   |   |
                       +---+---+---+---+---+---+
                    2  | O | O |   |   |   |   |
                       +---+---+---+---+---+---+
                    3  |   |   |   |   |   |   |
                       +---+---+---+---+---+---+
                    4  |   | O | O | O |   |   |
                       +---+---+---+---+---+---+
                    5  |   | O | X | O |   |   |
                       +---+---+---+---+---+---+
                     */
                    if (px >= 0 && py >= 0 && px < width && py < height) {
                        redSum += img->pArr[py][px].red;
                        greenSum += img->pArr[py][px].green;
                        blueSum += img->pArr[py][px].blue;
                        count++;
                    }
                }
            }
            // assign each pixel to the averaged value divided by the number
            // of pixels (denoted by O) that were iterated (including self)
            temp[y][x].red = redSum / count;
            temp[y][x].green = greenSum / count;
            temp[y][x].blue = blueSum / count;
        }
    }

    // Copy the contents of temp back to the original image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            img->pArr[y][x] = temp[y][x];
        }
    }

    // Free the memory allocated for temp
    for (int i = 0; i < height; i++) {
        free(temp[i]);
    }
    free(temp);
}


////////////////////////////////////////////////////////////////////////////////
//HELPER FUNCTIONS
/**
* Read BMP header of a BMP file.
* @param  file: A pointer to the file being read
* @param  header: Pointer to the destination BMP header
*/
void readBMPHeader(FILE *file, struct BMP_Header *header) {
    fread(&header->signature, sizeof(char) * 2, 1, file); // read signature
    fread(&header->size, sizeof(int), 1, file); // read file size
    fread(&header->reserved1, sizeof(short), 1, file); // read reserved1
    fread(&header->reserved2, sizeof(short), 1, file); // read reserved2
    fread(&header->offset_pixel_array, sizeof(int), 1, file); // read offset
}

/**
 * Write BMP header of a file. Useful for creating a BMP file.
 * @param  file: A pointer to the file being written
 * @param  header: The header to write to the file
 */
void writeBMPHeader(FILE *file, struct BMP_Header *header) {
    fwrite(&header->signature, sizeof(char) * 2, 1, file); // write signature
    fwrite(&header->size, sizeof(int), 1, file); // write file size
    fwrite(&header->reserved1, sizeof(short), 1, file); // write reserved1
    fwrite(&header->reserved2, sizeof(short), 1, file); // write reserved2
    fwrite(&header->offset_pixel_array, sizeof(int), 1, file); // write offset
}

/**
 * Read DIB header from a BMP file.
 * @param  file: A pointer to the file being read
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE *file, struct DIB_Header *header) {
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

/**
 * Write DIB header of a file. Useful for creating a BMP file.
 * @param  file: A pointer to the file being written
 * @param  header: The header to write to the file
 */
void writeDIBHeader(FILE *file, struct DIB_Header *header) {
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

/**
 * Make BMP header based on width and height. Useful for creating a BMP file.
 * @param  header: Pointer to the destination BMP header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makeBMPHeader(struct BMP_Header *header, int width, int height) {
    header->signature[0] = 'B';
    header->signature[1] = 'M';
    int temp = width * height * 3;
    header->size = 54 + width * height * 3;
    header->reserved1 = 0;
    header->reserved2 = 0;
}

/**
* Make new DIB header based on width and height.Useful for creating a BMP file.
* @param  header: Pointer to the destination DIB header
* @param  width: Width of the image that this header is for
* @param  height: Height of the image that this header is for
*/
void makeDIBHeader(struct DIB_Header *header, int width, int height) {
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

/**
 * Read Pixels from BMP file based on width and height.
 * @param  file: A pointer to the file being read
 * @param  pArr: Pixel array to store the pixels being read
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void readPixelsBMP(FILE *file, struct Pixel **pArr, int width, int height) {
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
            fread(&pArr[i][j], sizeof(struct Pixel), 1, file);
        }
        fseek(file, padding, SEEK_CUR); // skip reading file by places indicated in padding
    }
}

/**
 * Write Pixels from BMP file based on width and height.
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image to write to the file
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void writePixelsBMP(FILE *file, struct Pixel **pArr, int width, int height) {
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
            fwrite(&pArr[i][j], sizeof(struct Pixel), 1, file);
        }
        fwrite(zero, 1, padding, file); // add padding 0's
    }
}

/** Creates a new image and returns it.
 * @param  pArr: Pixel array of this image.
 * @param  width: Width of this image.
 * @param  height: Height of this image.
 * @return A pointer to a new image.
*/
struct Image *image_create(struct Pixel **pArr, int width, int height) {
    struct Image *new_img = (Image *) malloc(sizeof(Image));
    new_img->width = width;
    new_img->height = height;
    new_img->pArr = pArr;
    return new_img;
}

/** Returns a double pointer to the pixel array.
 * @param  img: the image.
*/
struct Pixel **image_get_pixels(Image *img) {
    return img->pArr;
}

/** Returns the width of the image.
 * @param  img: the image.
*/
int image_get_width(Image *img) {
    return img->width;
}

/** Returns the height of the image.
 * @param  img: the image.
*/
int image_get_height(Image *img) {
    return img->height;
}