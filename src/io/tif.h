#ifndef _tif_h
#define _tif_h

#include "tiffio.h"

unsigned char *readTIF(const char *file_name, int &width, int &height, int &depth);
bool writeTIF(const char *file_name, unsigned char *pixels, int width, int height, int depth, bool compress=true);

#endif

