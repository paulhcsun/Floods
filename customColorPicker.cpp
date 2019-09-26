#include "customColorPicker.h"
#include <math.h>
#include <stdlib.h>

#define PI 3.141592653589793

customColorPicker::customColorPicker(int counter, HSLAPixel center) {
    count = counter;
    ctr = center;
}

HSLAPixel customColorPicker::operator()(int x, int y) {

    HSLAPixel ret;
    int star = rand() % 80;

    double mult = (cos(count*PI/180) + 1.1)/2;
    count++;

    double lum = mult * 0.2 / 0.3;
    if (star == 1) {
        lum = 1.0;
    }
    ret.h = 248;
    ret.s = 1.0;
    ret.l = lum;
    return ret;
}