#include "borderColorPicker.h"
#include <math.h>

borderColorPicker::borderColorPicker(HSLAPixel fillColor, PNG &img, double tolerance, HSLAPixel center)
{
    color = fillColor;
    im = img;
    tol = tolerance;
    ctr = center;
}

HSLAPixel borderColorPicker::operator()(int x, int y)
{
    for (int i = x - 3; i <= x + 3; i++)
    {
        for (int j = y - 3; j <= y + 3; j++)
        {
            if (i < (int)im.width() && j < (int)im.height() && i >= 0 && j >= 0)
            {

                HSLAPixel *pixel = im.getPixel(i, j);

                double colorDiff = pixel->dist(ctr);
                int xDist = (x - i);
                int yDist = (y - j);
                double distanceFrom = (pow(xDist, 2.0)) + (pow(yDist, 2.0));

                if (distanceFrom <= 9)
                {
                    if (colorDiff > tol)
                    {
                        return color;
                    }
                }
            }
            else
            {
                return color;
            }
        }
    }

    return *im.getPixel(x, y);
}