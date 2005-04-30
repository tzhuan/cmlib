/*
 *   DPX image file format library definitions.
 *
 *   Copyright 1999 - 2002 David Hodson <hodsond@acm.org>
 *
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *   for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#ifndef _DPX_LIB_H_
#define _DPX_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "logImageCore.h"

/* int functions return 0 for OK */

int dpxGetSize(const DpxFile* logImage, int* width, int* height, int* depth);
DpxFile* dpxOpen(const char* filename);
DpxFile* dpxCreate(const char* filename, int xsize, int ysize, int channels);

/* get/set scanline of converted bytes */
int dpxGetRowBytes(DpxFile* dpx, float* row, int y);
int dpxSetRowBytes(DpxFile* dpx, const float* row, int y);

/* closes file and deletes data */
void dpxClose(DpxFile* dpx);

#ifdef __cplusplus
}
#endif

#endif /* _DPX_LIB_H_ */
