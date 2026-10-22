#include "helpers.h"


//x is block type, y is the row of the texture atlas you want to use, cellSizePixelsX and cellSizePixelsY is width and height respectively in pixels 
Rectangle getTextureAtlas(int x, int y, int cellSizePixelsX, int cellSizePixelsY) {

	return Rectangle{ (float)x * cellSizePixelsX, (float)y * cellSizePixelsY, (float)cellSizePixelsX, (float)cellSizePixelsY };
}