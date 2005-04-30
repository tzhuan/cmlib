#ifndef _pngio_h
#define _pngio_h

#include "png.h"

unsigned char *readPNG(const char *file_name, int &width, int &height, int &depth);
bool writePNG(const char *file_name, unsigned char *pixels, int width, int height, int depth);

#endif
