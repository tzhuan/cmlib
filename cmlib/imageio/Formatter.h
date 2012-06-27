/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef CMLIB_IMAGE_FORMATTER_H
#define CMLIB_IMAGE_FORMATTER_H

#include <cstdio>
#include "../image/Color.h"
#include "FileFormat.h"

#ifdef CMLIB_IMAGE_IO_PNG
	#include "io/png.h"
#endif 

#ifdef CMLIB_IMAGE_IO_EXR
	#include "io/exr.h"
#endif 

#ifdef CMLIB_IMAGE_IO_JPEG
	#include "io/jpeg.h"
#endif 

#ifdef CMLIB_IMAGE_IO_TIFF
	#include "io/tiff.h"
#endif 

#ifdef CMLIB_IMAGE_IO_HDR
	#include "io/hdr.h"
#endif 

#ifdef CMLIB_IMAGE_IO_PPM
	#include "io/ppm.h"
#endif 

#ifdef CMLIB_IMAGE_IO_PFM
	#include "io/pfm.h"
#endif 

#ifdef CMLIB_IMAGE_IO_BMP
	#include "io/bmp.h"
#endif 

#ifdef CMLIB_IMAGE_IO_FLT
	#include "io/flt.h"
#endif 

#endif // CMLIB_IMAGE_FORMATTER_H
