/**
* Image processing with threads
*
* Completion time: 15 Hours
*
* @author Edgar Hakobyan
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include "BmpProcessor.h"
#include "PixelProcessor.h"
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define THREAD_COUNT 4
#define BMP_HEADER_SIZE 14
#define BMP_DIB_HEADER_SIZE 40
#define MAXIMUM_IMAGE_SIZE 4096

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
struct threadData {
    int start;
    int end;
    int width;
    int height;
    struct Pixel **pArr;
};

struct Pixel **gloablArr;


/**
 * Method to apply yellow tint filter over an image
 * @param pixels pixel array
 * @param width image width
 * @param height image height
 * @param rShift shift red pixels by rShift
 * @param gShift shift green pixels by gShift
 * @param bShift shift blue pixels by bShift
 */
void applyYellowFilter(struct Pixel **pixels, int width, int height, int rShift, int gShift, int bShift) {
    // Reused from assignment 3
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int newRed = pixels[i][j].red + rShift;
            int newGreen = pixels[i][j].green + gShift;
            int newBlue = pixels[i][j].blue + bShift;
            pixels[i][j].red = (unsigned char) ((newRed > 255) ? 255 : (newRed < 0) ? 0 : newRed);
            pixels[i][j].green = (unsigned char) ((newGreen > 255) ? 255 : (newGreen < 0) ? 0 : newGreen);
            pixels[i][j].blue = (unsigned char) ((newBlue > 255) ? 255 : (newBlue < 0) ? 0 : newBlue);
        }
    }
}


/**
 * Method used by threads to blur an image.
 * @param param thread data
 * @return
 */
void *runnerBlur(void *param) {

    struct threadData *data = (struct threadData *) param;

    int red, green, blue, startCol, endCol, width, count = 0;
    startCol = data->start;
    endCol = data->end - 1;
    width = data->width;

    for (int i = 0; i < width; i++) {
        for (int j = startCol; j <= endCol; j++) {

            red = 0, green = 0, blue = 0, count = 0;

            if (i + 1 < width && j - 1 >= startCol) {
                blue += data->pArr[i + 1][j - 1].blue;
                green += data->pArr[i + 1][j - 1].green;
                red += data->pArr[i + 1][j - 1].red;
                count++;
            }

            if (i + 1 < width && j + 1 < endCol) {
                blue += data->pArr[i + 1][j + 1].blue;
                green += data->pArr[i + 1][j + 1].green;
                red += data->pArr[i + 1][j + 1].red;
                count++;
            }

            if (i + 1 < width) {
                blue += data->pArr[i + 1][j].blue;
                green += data->pArr[i + 1][j].green;
                red += data->pArr[i + 1][j].red;
                count++;
            }

            if (j + 1 < endCol) {
                blue += data->pArr[i][j + 1].blue;
                green += data->pArr[i][j + 1].green;
                red += data->pArr[i][j + 1].red;
                count++;
            }

            if (j - 1 >= startCol) {
                blue += data->pArr[i][j - 1].blue;
                green += data->pArr[i][j - 1].green;
                red += data->pArr[i][j - 1].red;
                count++;
            }

            blue += data->pArr[i][j].blue;
            green += data->pArr[i][j].green;
            red += data->pArr[i][j].red;
            count++;

            if (i - 1 >= 0 && j - 1 >= startCol) {
                blue += data->pArr[i - 1][j - 1].blue;
                green += data->pArr[i - 1][j - 1].green;
                red += data->pArr[i - 1][j - 1].red;
                count++;
            }
            if (i - 1 >= 0 && j + 1 < endCol) {
                blue += data->pArr[i - 1][j + 1].blue;
                green += data->pArr[i - 1][j + 1].green;
                red += data->pArr[i - 1][j + 1].red;
                count++;
            }
            if (i - 1 >= 0) {
                blue += data->pArr[i - 1][j].blue;
                green += data->pArr[i - 1][j].green;
                red += data->pArr[i - 1][j].red;
                count++;
            }

            gloablArr[i][j].blue = (int) round(blue / count);
            gloablArr[i][j].green = (int) round(green / count);
            gloablArr[i][j].red = (int) round(red / count);
        }
    }
    return 0;
}


/**
 * Method used by threads to make large sized holes in the image.
 * @param param thread data
 * @return
 */
void *runnerLargeHole(void *param) {

    int i, j, k;
    int width, height;
    int centerRow, centerCol;
    int numHoles, numLargeHoles, largeHoleRadius;

    struct threadData *data = (struct threadData *) param;

    width = data->width;
    height = data->height;

    // determines if picture is portrait or landscape
    if (width < height) {
        largeHoleRadius = (int) round(width * 0.12);
        numHoles = (int) round(width * 0.08);
    } else {
        largeHoleRadius = (int) round(height * 0.12);
        numHoles = (int) round(height * 0.08);
    }

    numLargeHoles = (int) round(numHoles * 0.25);

    for (i = 0; i < numLargeHoles; i++) {
        centerRow = rand() % width;
        centerCol = rand() % height;

        for (j = 0; j < width; j++) {
            for (k = 0; k < height; k++) {
                double r = sqrt(((j - centerRow) * (j - centerRow)) + ((k - centerCol) * (k - centerCol)));
                if (r < largeHoleRadius) {
                    gloablArr[j][k].blue = 0;
                    gloablArr[j][k].green = 0;
                    gloablArr[j][k].red = 0;
                }
            }
        }
    }
    return 0;
}


/**
 * Method used by threads to make medium sized holes in the image.
 * @param param thread data
 * @return
 */
void *runnerMediumHole(void *param) {

    int i, j, k;
    int width, height;
    int centerRow, centerCol;
    int numHoles, numLargeHoles, largeHoleRadius;

    struct threadData *data = (struct threadData *) param;

    width = data->width;
    height = data->height;

    // determines if picture is portrait or landscape
    if (width < height) {
        largeHoleRadius = (int) round(width * 0.08);
        numHoles = (int) round(width * 0.08);
    } else {
        largeHoleRadius = (int) round(height * 0.08);
        numHoles = (int) round(height * 0.08);
    }

    numLargeHoles = (int) round(numHoles * 0.5);

    for (i = 0; i < numLargeHoles; i++) {
        centerRow = rand() % width;
        centerCol = rand() % height;

        for (j = 0; j < width; j++) {
            for (k = 0; k < height; k++) {
                double r = sqrt(((j - centerRow) * (j - centerRow)) + ((k - centerCol) * (k - centerCol)));
                if (r < largeHoleRadius) {
                    gloablArr[j][k].blue = 0;
                    gloablArr[j][k].green = 0;
                    gloablArr[j][k].red = 0;
                }
            }
        }
    }
    return 0;
}


/**
 * Method used by threads to make small sized holes in the image.
 * @param param thread data
 * @return
 */
void *runnerSmallHole(void *param) {

    int i, j, k;
    int width, height;
    int centerRow, centerCol;
    int numHoles, numLargeHoles, largeHoleRadius;

    struct threadData *data = (struct threadData *) param;

    width = data->width;
    height = data->height;

    // determines if picture is portrait or landscape
    if (width < height) {
        largeHoleRadius = (int) round(width * 0.04);
        numHoles = (int) round(width * 0.08);
    } else {
        largeHoleRadius = (int) round(height * 0.04);
        numHoles = (int) round(height * 0.08);
    }

    numLargeHoles = (int) round(numHoles * 0.25);

    for (i = 0; i < numLargeHoles; i++) {
        centerRow = rand() % width;
        centerCol = rand() % height;

        for (j = 0; j < width; j++) {
            for (k = 0; k < height; k++) {
                double r = sqrt(((j - centerRow) * (j - centerRow)) + ((k - centerCol) * (k - centerCol)));
                if (r < largeHoleRadius) {
                    gloablArr[j][k].blue = 0;
                    gloablArr[j][k].green = 0;
                    gloablArr[j][k].red = 0;
                }
            }
        }
    }
    return 0;
}


/**
 * Main
 * @param argc number of arguments
 * @param argv argument array
 */
void main(int argc, char *argv[]) {

    char *inputFileName = NULL;
    char *outputFileName = NULL;
    char *filter = NULL;

    int width, height, colWidth;

    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-i") == 0)) {
            printf("-i\n");
            inputFileName = argv[i + 1];
        } else if ((strcmp(argv[i], "-o") == 0)) {
            printf("-o\n");
            outputFileName = argv[i + 1];
        } else if ((strcmp(argv[i], "-f") == 0)) {
            printf("-f\n");
            filter = argv[i + 1];
        }
    }

    FILE *file = fopen(inputFileName, "rb");
    if (file == NULL) {
        printf("---> Cannot open file: %s \n", inputFileName);
        exit(0);
    } else{
        printf("---> %s file opened\n", inputFileName);
    }


    // read BMP Header info
    struct BMP_Header *bmpHeader;
    bmpHeader = (struct BMP_Header *) malloc(sizeof(struct BMP_Header));
    readBMPHeader(file, bmpHeader);

    // read DIB Header info
    struct DIB_Header *dibHeader;
    dibHeader = (struct DIB_Header *) malloc(sizeof(struct DIB_Header));
    readDIBHeader(file, dibHeader);


    struct threadData data;
    width = dibHeader->width;
    height = dibHeader->height;
    data.width = width;
    data.height = height;
    data.pArr = (struct Pixel **) malloc(sizeof(struct Pixel *) * width);
    for (int i = 0; i < width; i++) {
        data.pArr[i] = malloc(sizeof(struct Pixel) * height);
    }

    gloablArr = (struct Pixel **) malloc(sizeof(struct Pixel *) * width);
    for (int i = 0; i < width; i++) {
        gloablArr[i] = malloc(sizeof(struct Pixel) * height);
    }

    readPixelsBMP(file, data.pArr, width, height);
    fclose(file);


    if (strcmp(filter, "b") == 0) {
        int rem = width % THREAD_COUNT;
        // determine column size based on given thread count
        if (rem != 0) {
            colWidth = (width - rem) / THREAD_COUNT;
        } else {
            colWidth = width / THREAD_COUNT;
        }

        pthread_t tids[THREAD_COUNT];
        pthread_attr_t attributes[THREAD_COUNT];

        for (int i = 0; i < THREAD_COUNT; i++) {
            data.start = i * colWidth;
            if (data.start > 0) {
                data.start -= 1;
            }
            data.end = ((i + 1) * colWidth);
            if (i == THREAD_COUNT - 1 && rem != 0) {
                data.end += rem;
            }

            pthread_attr_init(&attributes[i]);
            pthread_create(&tids[i], &attributes[i], runnerBlur, &data);
            pthread_join(tids[i], NULL);
        }

    } else if (strcmp(filter, "c") == 0) {

        applyYellowFilter(data.pArr, width, height, 60, 60, 0);

        pthread_t tid1, tid2, tid3;
        pthread_attr_t attr1, attr2, attr3;

        pthread_attr_init(&attr1);
        pthread_attr_init(&attr2);
        pthread_attr_init(&attr3);
        pthread_create(&tid1, &attr1, runnerMediumHole, &data);
        pthread_create(&tid2, &attr2, runnerSmallHole, &data);
        pthread_create(&tid3, &attr3, runnerLargeHole, &data);
        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);
        pthread_join(tid3, NULL);
    }

    FILE *outFile = fopen(outputFileName, "wb");
    if (outFile == NULL) {
        printf("---> Cannot open file: %s \n", outputFileName);
        exit(0);
    } else{
        printf("---> %s file opened\n", outputFileName);
    }

    writeBMPHeader(outFile, bmpHeader);
    writeDIBHeader(outFile, dibHeader);
    writePixelsBMP(outFile, gloablArr, dibHeader->width, dibHeader->height);

    fclose(outFile);
    printf("---> Finished writing to file\n");
    printf("---> File Closed\n");

    free(*data.pArr);
    free(bmpHeader);
    free(dibHeader);
    free(gloablArr);
    bmpHeader = NULL;
    dibHeader = NULL;
    *data.pArr = NULL;
    gloablArr = NULL;
}







