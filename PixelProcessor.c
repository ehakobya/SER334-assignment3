/**
* Implementation of PixelProcessor
*
* Completion time: 45 min
*
* @author Edgar Hakobyan
* @version 1.0
*/

//Include Files
#include "PixelProcessor.h"

extern struct Pixel** globalArr;

/**
 * Shift color of Pixel array. The dimension of the array is width * height. The shift value of RGB is
 * rShift, gShift，bShift. Useful for color shift.
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 * @param  rShift: the shift value of color r shift
 * @param  gShift: the shift value of color g shift
 * @param  bShift: the shift value of color b shift
 */
void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift){
    int i, j;

    int holder = 0;

    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            holder = pArr[i][j].blue + bShift;
            if (holder > 255) {
                globalArr[i][j].blue = 255;
            } else if (holder < 0) {
                globalArr[i][j].blue = 0;
            } else {
                globalArr[i][j].blue = pArr[i][j].blue + bShift;
            }
            holder = pArr[i][j].green + gShift;
            if (holder > 255) {
                globalArr[i][j].green = 255;
            } else if (holder < 0) {
                globalArr[i][j].green = 0;
            } else {
                globalArr[i][j].green = pArr[i][j].green + gShift;
            }
            holder = pArr[i][j].red + rShift;

            if (holder > 255) {
                globalArr[i][j].red = 255;
            } else if (holder < 0) {
                globalArr[i][j].red = 0;
            } else {
                globalArr[i][j].red = pArr[i][j].red + rShift;
            }
        }
    }
}
