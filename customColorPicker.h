#ifndef CUSTOMCOLORPICKER_H
#define CUSTOMCOLORPICKER_H

#include "colorPicker.h"

class customColorPicker : public colorPicker
{
    public:
    
    customColorPicker(int counter, HSLAPixel center);

    virtual HSLAPixel operator()(int x, int y);

    private:
    int count;
    HSLAPixel ctr;

};

#endif