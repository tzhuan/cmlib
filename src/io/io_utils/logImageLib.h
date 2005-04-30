/*
 *   Common library definitions for Cineon and DPX image files.
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

#ifndef _LOG_IMAGE_LIB_H_
#define _LOG_IMAGE_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Magic numbers for normal (non byte-swapped) Cineon and Dpx files
 */

typedef struct _Log_Image_File_t_ DpxFile;


#define DPX_FILE_MAGIC 0x53445058

/*
 * Image 8 bit <-> 10 bit conversion parameters.
 */

typedef struct {
  float gamma;
  int blackPoint;
  int whitePoint;
} LogImageByteConversionParameters;

/* byte conversion routines for mapping logImage (usually) 10 bit values to 8 bit */
/* see Kodak docs for details... */

int logImageGetByteConversionDefaults(LogImageByteConversionParameters* params);
int logImageSetByteConversion(DpxFile* logImage, const LogImageByteConversionParameters* params);

#ifdef __cplusplus
}
#endif

#endif /* _LOG_IMAGE_LIB_H_ */
