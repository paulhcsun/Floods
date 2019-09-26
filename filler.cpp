/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace. 
 *
 */
//#include "filler.h"

animation filler::fillStripeDFS(PNG &img, int x, int y, HSLAPixel fillColor,
                                int stripeSpacing, double tolerance, int frameFreq)
{
    stripeColorPicker a(fillColor, stripeSpacing);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

// animation filler::fillCustomBFS(PNG &img, int x, int y, int counter, double tolerance, int frameFreq)
// {
//     unsigned int middleX = (img.width()) / 2;
//     unsigned int middleY = (img.height()) / 2;
//     HSLAPixel *center = img.getPixel(middleX, middleY);

//     customColorPicker a(counter, *center);
//     return fill<Queue>(img, x, y, a, tolerance, frameFreq);
// }

// animation filler::fillCustomDFS(PNG &img, int x, int y, int counter, double tolerance, int frameFreq)
// {
//     unsigned int middleX = (img.width()) / 2;
//     unsigned int middleY = (img.height()) / 2;
//     HSLAPixel *center = img.getPixel(middleX, middleY);

//     customColorPicker a(counter, *center);
//     return fill<Stack>(img, x, y, a, tolerance, frameFreq);
// }

animation filler::fillBorderDFS(PNG &img, int x, int y,
                                HSLAPixel borderColor, double tolerance, int frameFreq)
{
    // unsigned int middleX = (img.width()) / 2;
    // unsigned int middleY = (img.height()) / 2;
    // HSLAPixel *center = img.getPixel(middleX, middleY);
    HSLAPixel *center = img.getPixel(x, y);

    borderColorPicker a(borderColor, img, tolerance, *center);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

/* Given implementation of a DFS rainbow fill. */
animation filler::fillRainDFS(PNG &img, int x, int y,
                              long double freq, double tolerance, int frameFreq)
{
    rainbowColorPicker a(freq);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillStripeBFS(PNG &img, int x, int y, HSLAPixel fillColor,
                                int stripeSpacing, double tolerance, int frameFreq)
{
    stripeColorPicker a(fillColor, stripeSpacing);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillBorderBFS(PNG &img, int x, int y,
                                HSLAPixel borderColor, double tolerance, int frameFreq)
{
    // unsigned int middleX = (img.width()) / 2;
    // unsigned int middleY = (img.height()) / 2;
    // HSLAPixel *center = img.getPixel(middleX, middleY);
    HSLAPixel *center = img.getPixel(x, y);

    borderColorPicker a(borderColor, img, tolerance, *center);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

/* Given implementation of a BFS rainbow fill. */
animation filler::fillRainBFS(PNG &img, int x, int y,
                              long double freq, double tolerance, int frameFreq)
{
    rainbowColorPicker a(freq);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

template <template <class T> class OrderingStructure>
animation filler::fill(PNG &img, int x, int y, colorPicker &fillColor,
                       double tolerance, int frameFreq)
{
    OrderingStructure<vector<int>> os;
    vector<vector<bool>> visited(img.width(), vector<bool>(img.height(), false));
    animation anim;
    int frame_counter = 0;

    vector<int> point;
    point.push_back(x);
    point.push_back(y);

    HSLAPixel *centerPixel = img.getPixel(x, y);
    HSLAPixel centerColor = *centerPixel;
    os.add(point);
    visited[x][y] = true;
    *centerPixel = fillColor(x, y);

    frame_counter++;
    if (frame_counter % frameFreq == 0)
    {
        anim.addFrame(img);
    }

    while (!os.isEmpty())
    {
        vector<int> temp = os.remove();
        int xCoord = temp[0];
        int yCoord = temp[1];

        // upRight
        fillSurroundingPixels(img, tolerance, xCoord + 1, yCoord - 1, fillColor,
                              frameFreq, anim, centerColor, visited, frame_counter, os);

        // up
        fillSurroundingPixels(img, tolerance, xCoord, yCoord - 1, fillColor,
                              frameFreq, anim, centerColor, visited, frame_counter, os);

        // upLeft
        fillSurroundingPixels(img, tolerance, xCoord - 1, yCoord - 1, fillColor,
                              frameFreq, anim, centerColor, visited, frame_counter, os);

        // left
        fillSurroundingPixels(img, tolerance, xCoord - 1, yCoord, fillColor,
                              frameFreq, anim, centerColor, visited, frame_counter, os);

        // downLeft
        fillSurroundingPixels(img, tolerance, xCoord - 1, yCoord + 1, fillColor,
                              frameFreq, anim, centerColor, visited, frame_counter, os);

        // down
        fillSurroundingPixels(img, tolerance, xCoord, yCoord + 1, fillColor,
                              frameFreq, anim, centerColor, visited, frame_counter, os);

        // downRight
        fillSurroundingPixels(img, tolerance, xCoord + 1, yCoord + 1, fillColor,
                              frameFreq, anim, centerColor, visited, frame_counter, os);

        // right
        fillSurroundingPixels(img, tolerance, xCoord + 1, yCoord, fillColor,
                              frameFreq, anim, centerColor, visited, frame_counter, os);
    }

    anim.addFrame(img);
    return anim;

    /**
     * @todo You need to implement this function!
     *
     * This is the basic description of a flood-fill algorithm: Every fill
     * algorithm requires an ordering structure, which is passed to this
     * function via its template parameter. For a breadth-first-search
     * fill, that structure is a Queue, for a depth-first-search, that
     * structure is a Stack. To begin the algorithm, you simply place the
     * given point in the ordering structure, marking it processed
     * (the way you mark it is a design decision you'll make yourself).
     * We have a choice to either change the color, if appropriate, when we
     * add the point to the OS, or when we take it off. In our test cases,
     * we have assumed that you will change the color when a point is added
     * to the structure. 
     *
     * Until the structure is empty, you do the following:

     * 1.Remove a point from the ordering structure, and then...:

            1.    add its unprocessed neighbors whose color values are within 
                  (or equal to) tolerance distance from the center, to the ordering structure.
            2.    use the colorPicker to set the new color of the neighbors
            3.    mark the neighbors as processed.
            4.    as you process a neighbor, if it is an appropriate frame, send the current 
                  PNG to the animation (as described below)."
     *
     *
     * 2.     When implementing your breadth-first-search and
     *        depth-first-search fills, you will need to explore neighboring
     *        pixels in some order.
     *
     *        For this assignment, each pixel p has 8 neighbors, consisting of 
     *        the 8 pixels who share an edge or corner with p. (We leave it to
     *        you to describe those 8 pixel locations, relative to the location
     *        of p.)
     *
     *        While the order in which you examine neighbors does not matter
     *        for a proper fill, you must use the same order as we do for
     *        your animations to come out like ours! 
     *
     *        The order you should put
     *        neighboring pixels **ONTO** the ordering structure (queue or stack) 
     *        is as follows: **UPRIGHT(+x,-y), UP(-y), UPLEFT(-x,-y), LEFT(-x), 
     *        DOWNLEFT(-x,+y), DOWN(+y), DOWNRIGHT(+x,+y), RIGHT(+x)**
     *
     *        If you do them in a different order, your fill may
     *        still work correctly, but your animations will be different
     *        from the grading scripts!
     *
     *        IMPORTANT NOTE: *UP*
     *        here means towards the top of the image, so since an image has
     *        smaller y coordinates at the top, this is in the *negative y*
     *        direction. Similarly, *DOWN* means in the *positive y*
     *        direction.  
     *
     * 3.     For every k pixels filled, **starting at the kth pixel**, you
     *        must add a frame to the animation, where k = frameFreq.
     *
     *        For example, if frameFreq is 4, then after the 4th pixel has
     *        been filled you should add a frame to the animation, then again
     *        after the 8th pixel, etc.  You must only add frames for the
     *        number of pixels that have been filled, not the number that
     *        have been checked. So if frameFreq is set to 1, a pixel should
     *        be filled every frame.
     * 4.     Finally, as you leave the function, send one last frame to the
     *        animation. This frame will be the final result of the fill, and 
     *        it will be the one we test against.
     */
}

void filler::fillSurroundingPixels(PNG &img, double tol, int xCoord, int yCoord,
                                   colorPicker &fillColor, int frameFreq, animation &anim,
                                   HSLAPixel centerColor, vector<vector<bool>> &visited,
                                   int &frame_counter,
                                   OrderingStructure<vector<int>> &os)
{
    vector<int> point;
    point.push_back(xCoord);
    point.push_back(yCoord);

    if (xCoord >= 0 && yCoord >= 0 && xCoord < (int)img.width() && yCoord < (int)img.height())
    {
        HSLAPixel *pixel = img.getPixel(xCoord, yCoord);
        double colorDiff = pixel->dist(centerColor);

        if (colorDiff <= tol)
        {
            if (!(visited[xCoord][yCoord]))
            {
                os.add(point);
                visited[xCoord][yCoord] = true;
                *pixel = fillColor(xCoord, yCoord);

                frame_counter++;
                if (frame_counter % frameFreq == 0)
                {
                    anim.addFrame(img);
                }
            }
        }
    }
}
