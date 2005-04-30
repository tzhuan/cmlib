/*
 *   Dpx image file format library routines.
 *
 *   Copyright 1999 - 2003 David Hodson <hodsond@acm.org>
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

#include "dpxfile.h"
#include "dpxlib.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>        /* strftime() */
#ifdef WIN32
#include <winsock.h>
#define rint(x) ((int)(x+0.5))
#else
#include <netinet/in.h>  /* htonl() */
#endif
#include <string.h>      /* memset */

int
dpxGetSize(const DpxFile* logImage, int* width, int* height, int* depth) 
{
	*width = logImage->width;
	*height = logImage->height;
	*depth = logImage->depth;
	return 0;
}

static void
fillDpxChannelInfo(DpxFile* dpx, DpxChannelInformation* chan, int des) {

  chan->signage = 0;
  chan->ref_low_data = htonl(0);
  chan->ref_low_quantity = htonf(0.0);
  chan->ref_high_data = htonl(1023);
  chan->ref_high_quantity = htonf(2.046);
  chan->designator1 = des;
  chan->transfer_characteristics = 0;
  chan->colourimetry = 0;
  chan->bits_per_pixel = 10;
  chan->packing = htons(1);
  chan->encoding = 0;
  chan->data_offset = 0;
  chan->line_padding = htonl(0);
  chan->channel_padding = htonl(0);
  chan->description[0] = 0;
}


static void
fillDpxFileInfo(
  DpxFile* dpx, DpxFileInformation* fileInfo, const char* filename) {

  time_t fileClock;
  struct tm* fileTime;

  /* Note: always write files in network order */
  /* By the spec, it shouldn't matter, but ... */

  fileInfo->magic_num = htonl(DPX_FILE_MAGIC);
  fileInfo->offset = htonl(dpx->imageOffset);
  strcpy(fileInfo->vers, "v1.0");
  fileInfo->file_size = htonl(dpx->imageOffset +
    pixelsToLongs(dpx->height * dpx->width * dpx->depth) * 4);
  fileInfo->ditto_key = 0;
  fileInfo->gen_hdr_size = htonl(
    sizeof(DpxFileInformation) +
    sizeof(DpxImageInformation) +
    sizeof(DpxOriginationInformation));
  fileInfo->ind_hdr_size = htonl(sizeof(DpxMPIInformation));
  fileInfo->user_data_size = 0;
  strncpy(fileInfo->file_name, filename, 99);
  fileInfo->file_name[99] = 0;

  fileClock = time(0);
  fileTime = localtime(&fileClock);
  strftime(fileInfo->create_date, 24, "%Y:%m:%d:%H:%M:%S%Z", fileTime);
  /* Question: is %Z in strftime guaranteed to return 3 chars? */
  fileInfo->create_date[23] = 0;

  fileInfo->creator[0] = 0;
  fileInfo->project[0] = 0;
  fileInfo->copyright[0] = 0;
  fileInfo->key = 0xFFFFFFFF; /* same in any byte order */
}

static void
fillDpxImageInfo(
  DpxFile* dpx, DpxImageInformation* imageInfo) {
  imageInfo->orientation = 0;
  imageInfo->channels_per_image = htons(1);
  imageInfo->pixels_per_line = htonl(dpx->width);
  imageInfo->lines_per_image = htonl(dpx->height);

  if (dpx->depth == 1) {
    fillDpxChannelInfo(dpx, &imageInfo->channel[0], 0);

  } else if (dpx->depth == 3) {
    fillDpxChannelInfo(dpx, &imageInfo->channel[0], 50);
  }
}

static void
fillDpxOriginationInfo(
  DpxFile* dpx, DpxOriginationInformation* originInfo, DpxFileInformation* fileInfo) {
}

static void
initDpxMainHeader(DpxFile* dpx, DpxMainHeader* header, const char* shortFilename) 
{
  memset(header, 0, sizeof(DpxMainHeader));
  fillDpxFileInfo(dpx, &header->fileInfo, shortFilename);
  fillDpxImageInfo(dpx, &header->imageInfo);
  fillDpxOriginationInfo(dpx, &header->originInfo, &header->fileInfo);
}

int
dpxGetRowBytes(DpxFile* dpx, float* row, int y) {

  /* Note: this code is bizarre because DPX files can wrap */
  /* packed longwords across line boundaries!!!! */

  int readLongs;
  int longIndex;
  int numPixels = dpx->width * dpx->depth;
  int pixelIndex;

  /* only seek if not reading consecutive lines */
  /* this is not quite right yet, need to account for leftovers */
  if (y != dpx->fileYPos) {
    int lineOffset = pixelsToLongs(y * dpx->width * dpx->depth) * 4;
    if (fseek(dpx->file, dpx->imageOffset + lineOffset, SEEK_SET) != 0) {
      return 1;
    }
    dpx->fileYPos = y;
  }

  /* read enough longwords */
  readLongs = pixelsToLongs(numPixels - dpx->pixelBufferUsed);
  if (fread(dpx->lineBuffer, 4, readLongs, dpx->file) != (size_t)readLongs) {
    fprintf(stderr, "Couldn't read line %d length %d\n", y, readLongs * 4);
    return 1;
  }
  ++dpx->fileYPos;

  /* convert longwords to pixels */
  pixelIndex = dpx->pixelBufferUsed;

  /* this is just strange */
  if (dpx->depth == 1) {
    for (longIndex = 0; longIndex < readLongs; ++longIndex) {
      unsigned long t = ntohl(dpx->lineBuffer[longIndex]);
      dpx->pixelBuffer[pixelIndex] = t & 0x3ff;
      t = t >> 10;
      dpx->pixelBuffer[pixelIndex+1] = t & 0x3ff;
      t = t >> 10;
      dpx->pixelBuffer[pixelIndex+2] = t & 0x3ff;
      pixelIndex += 3;
    }
  } else /* if (dpx->depth == 3) */ {
    for (longIndex = 0; longIndex < readLongs; ++longIndex) {
      unsigned long t = ntohl(dpx->lineBuffer[longIndex]);
      t = t >> 2;
      dpx->pixelBuffer[pixelIndex+2] = t & 0x3ff;
      t = t >> 10;
      dpx->pixelBuffer[pixelIndex+1] = t & 0x3ff;
      t = t >> 10;
      dpx->pixelBuffer[pixelIndex] = t & 0x3ff;
      pixelIndex += 3;
    }
  }
  dpx->pixelBufferUsed = pixelIndex;

  /* extract required pixels */
  for (pixelIndex = 0; pixelIndex < numPixels; ++pixelIndex) {
    row[pixelIndex] = dpx->lut10[dpx->pixelBuffer[pixelIndex]];
  }

  /* save remaining pixels */
  while (pixelIndex < dpx->pixelBufferUsed) {
    dpx->pixelBuffer[pixelIndex - numPixels] = dpx->pixelBuffer[pixelIndex];
    ++pixelIndex;
  }
  dpx->pixelBufferUsed -= numPixels;

  /* done! */
  return 0;
}

int
dpxSetRowBytes(DpxFile* dpx, const float* row, int y) {

  /* Note: this code is bizarre because DPX files can wrap */
  /* packed longwords across line boundaries!!!! */

  int writeLongs;
  int longIndex;
  int numPixels = dpx->width * dpx->depth;
  int pixelIndex;
  int pixelIndex2;
  double base, f_i;

  /* only seek if not reading consecutive lines */
  /* this is not quite right yet */
  if (y != dpx->fileYPos) {
    int lineOffset = pixelsToLongs(y * dpx->width * dpx->depth) * 4;
    if (fseek(dpx->file, dpx->imageOffset + lineOffset, SEEK_SET) != 0) {
      return 1;
    }
    dpx->fileYPos = y;
  }

  /* put new pixels into pixelBuffer */
  
  base = dpx->black_base;
  for (pixelIndex = 0; pixelIndex < numPixels; ++pixelIndex) {
	f_i = base + (1.0 - base) * row[pixelIndex];
    dpx->pixelBuffer[dpx->pixelBufferUsed + pixelIndex] = 
		(int)rint(convertFrom(f_i, dpx->params.whitePoint, dpx->params.gamma));
  }
  dpx->pixelBufferUsed += numPixels;

  /* pack into longwords */
  writeLongs = dpx->pixelBufferUsed / 3;
  /* process whole line at image end */
  if (dpx->fileYPos == (dpx->height - 1)) {
    writeLongs = pixelsToLongs(dpx->pixelBufferUsed);
  }
  pixelIndex = 0;
  if (dpx->depth == 1) {
    for (longIndex = 0; longIndex < writeLongs; ++longIndex) {
      unsigned long t = dpx->pixelBuffer[pixelIndex] |
          (dpx->pixelBuffer[pixelIndex+1] << 10) |
          (dpx->pixelBuffer[pixelIndex+2] << 20);
      dpx->lineBuffer[longIndex] = htonl(t);
      pixelIndex += 3;
    }
  } else {
    for (longIndex = 0; longIndex < writeLongs; ++longIndex) {
      unsigned long t = dpx->pixelBuffer[pixelIndex+2] << 2 |
          (dpx->pixelBuffer[pixelIndex+1] << 12) |
          (dpx->pixelBuffer[pixelIndex] << 22);
      dpx->lineBuffer[longIndex] = htonl(t);
      pixelIndex += 3;
    }
  }

  /* write them */
  if (fwrite(dpx->lineBuffer, 4, writeLongs, dpx->file) != (size_t)writeLongs) {
    fprintf(stderr, "Couldn't write line %d length %d\n", y, writeLongs * 4);
    return 1;
  }
  ++dpx->fileYPos;

  /* save remaining pixels */
  pixelIndex2 = 0;
  while (pixelIndex < dpx->pixelBufferUsed) {
    dpx->pixelBuffer[pixelIndex2] = dpx->pixelBuffer[pixelIndex];
    ++pixelIndex;
    ++pixelIndex2;
  }
  dpx->pixelBufferUsed = pixelIndex2;

  return 0;
}


DpxFile* 
dpxOpen(const char* filename) {

  DpxMainHeader header;

  DpxFile* dpx = (DpxFile*)malloc(sizeof(DpxFile));
  if (dpx == 0) {
    return 0;
  }

  /* for close routine */
  dpx->file = 0;
  dpx->lineBuffer = 0;
  dpx->pixelBuffer = 0;

  dpx->file = fopen(filename, "rb");
  if (dpx->file == 0) {
    dpxClose(dpx);
    return 0;
  }
  dpx->reading = 1;

  if (fread(&header, sizeof(header), 1, dpx->file) == 0) {
    fprintf(stderr, "Not enough data for header in \"%s\".\n", filename);
    dpxClose(dpx);
    return 0;
  }

  /* let's assume dpx files are always network order */
  if (header.fileInfo.magic_num != ntohl(DPX_FILE_MAGIC)) {
    fprintf(stderr, "Bad magic number %8.8X in \"%s\".\n",
      ntohl(header.fileInfo.magic_num), filename);
    dpxClose(dpx);
    return 0;
  }

  if (ntohs(header.imageInfo.channel[0].packing) != 1) {
    fprintf(stderr, "Unknown packing %d\n", header.imageInfo.channel[0].packing);
    dpxClose(dpx);
    return 0;
  }


  dpx->width = ntohl(header.imageInfo.pixels_per_line);
  dpx->height = ntohl(header.imageInfo.lines_per_image);
  dpx->depth = ntohs(header.imageInfo.channels_per_image);
  /* Another DPX vs Cineon wierdness */
  if (dpx->depth == 1) {
    switch (header.imageInfo.channel[0].designator1) {
    case 50: dpx->depth = 3; break;
    case 51: dpx->depth = 4; break;
    case 52: dpx->depth = 4; break;
    default: break;
    }
  }
  dpx->bitsPerPixel = 10;
  /* dpx->bitsPerPixel = header.imageInfo.channel[0].bits_per_pixel; */
  dpx->imageOffset = ntohl(header.fileInfo.offset);

  dpx->lineBufferLength = pixelsToLongs(dpx->width * dpx->depth);
  dpx->lineBuffer = (unsigned long *)malloc(dpx->lineBufferLength * 4);
  if (dpx->lineBuffer == 0) {
    fprintf(stderr, "Couldn't malloc line buffer of size %d\n", dpx->lineBufferLength * 4);
    dpxClose(dpx);
    return 0;
  }

  /* could have 2 pixels left over */
  dpx->pixelBuffer = (unsigned short *)malloc((dpx->lineBufferLength * 3 + 2) * sizeof(unsigned short));
  if (dpx->pixelBuffer == 0) {
    fprintf(stderr, "Couldn't malloc pixel buffer of size %d\n",
        (dpx->width * dpx->depth + 2 + 2) * sizeof(unsigned short));
    dpxClose(dpx);
    return 0;
  }
  dpx->pixelBufferUsed = 0;

  if (fseek(dpx->file, dpx->imageOffset, SEEK_SET) != 0) {
    fprintf(stderr, "Couldn't seek to image data start at %d\n", dpx->imageOffset);
    dpxClose(dpx);
    return 0;
  }
  dpx->fileYPos = 0;

  logImageGetByteConversionDefaults(&dpx->params);
  setupLut(dpx);

  /*
  dpx->getRowBytes = &dpxGetRowBytes;
  dpx->setRowBytes = 0;
  dpx->getRow = 0;
  dpx->setRow = 0;
  dpx->close = &dpxClose;
  */

  return dpx;
}

DpxFile*
dpxCreate(const char* filename, int width, int height, int depth) {

  /* Note: always write files in network order */
  /* By the spec, it shouldn't matter, but ... */

  DpxMainHeader header;
  const char* shortFilename = 0;

  DpxFile* dpx = (DpxFile*)malloc(sizeof(DpxFile));
  if (dpx == 0) {
    fprintf(stderr, "Failed to malloc dpx file structure.\n");
    return 0;
  }

  memset(&header, 0, sizeof(header));

  /* for close routine */
  dpx->file = 0;
  dpx->lineBuffer = 0;
  dpx->pixelBuffer = 0;

  if ((dpx->file = fopen(filename, "wb")) == 0) {
    dpxClose(dpx);
    return 0;
  }
  dpx->reading = 0;

  dpx->width = width;
  dpx->height = height;
  dpx->depth = depth;
  dpx->bitsPerPixel = 10;
  dpx->imageOffset = sizeof(DpxMainHeader);

  dpx->lineBufferLength = pixelsToLongs(dpx->width * dpx->depth);
  dpx->lineBuffer = (unsigned long *)malloc(dpx->lineBufferLength * 4);
  if (dpx->lineBuffer == 0) {
    fprintf(stderr, "Couldn't malloc line buffer of size %d\n", dpx->lineBufferLength * 4);
    dpxClose(dpx);
    return 0;
  }

  dpx->pixelBuffer = (unsigned short *)malloc((dpx->lineBufferLength * 3 + 2) * sizeof(unsigned short));
  if (dpx->pixelBuffer == 0) {
    fprintf(stderr, "Couldn't malloc pixel buffer of size %d\n",
        (dpx->width * dpx->depth + 2 + 2) * sizeof(unsigned short));
    dpxClose(dpx);
    return 0;
  }
  dpx->pixelBufferUsed = 0;

  /* find trailing part of filename */
  shortFilename = strrchr(filename, '/');
  if (shortFilename == 0) {
    shortFilename = filename;
  } else {
    ++shortFilename;
  }
  initDpxMainHeader(dpx, &header, shortFilename);

  if (fwrite(&header, sizeof(header), 1, dpx->file) == 0) {
    fprintf(stderr, "Couldn't write image header\n");
    dpxClose(dpx);
    return 0;
  }
  dpx->fileYPos = 0;

  logImageGetByteConversionDefaults(&dpx->params);
  setupLut(dpx);

  /*
  dpx->getRowBytes = 0;
  dpx->setRowBytes = &dpxSetRowBytes;
  dpx->getRow = 0;
  dpx->setRow = 0;
  dpx->close = &dpxClose;
  */

  return dpx;
}

void
dpxClose(DpxFile* dpx) {

  if (dpx == 0) {
    return;
  }

  if (dpx->file) {
    fclose(dpx->file);
    dpx->file = 0;
  }

  if (dpx->lineBuffer) {
    free(dpx->lineBuffer);
    dpx->lineBuffer = 0;
  }

  if (dpx->pixelBuffer) {
    free(dpx->pixelBuffer);
    dpx->pixelBuffer = 0;
  }

  free(dpx);
}

