/*
 *   Cineon and DPX image file format library routines.
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

#include "logImageLib.h"
#include "dpxlib.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>        /* strftime() */
#ifdef WIN32
#include <winsock.h>
#else
#include <netinet/in.h>  /* htonl() */
#endif
#include <string.h>      /* memset */

#define MIN_GAMMA 0.01
#define MAX_GAMMA 99.9
#define DEFAULT_GAMMA 0.435
#define DEFAULT_BLACK_POINT 95
#define DEFAULT_WHITE_POINT 685

int
logImageGetByteConversionDefaults(LogImageByteConversionParameters* params) 
{
	params->gamma = DEFAULT_GAMMA;
	params->blackPoint = DEFAULT_BLACK_POINT;
	params->whitePoint = DEFAULT_WHITE_POINT;
	return 0;
}

int
logImageSetByteConversion(DpxFile* logImage, const LogImageByteConversionParameters* params) 
{
	if ((params->gamma >= MIN_GAMMA) &&
			(params->gamma <= MAX_GAMMA) &&
			(params->blackPoint >= 0) &&
			(params->blackPoint < params->whitePoint) &&
			(params->whitePoint <= 1023)) {
		logImage->params.gamma = params->gamma;
		logImage->params.blackPoint = params->blackPoint;
		logImage->params.whitePoint = params->whitePoint;
		setupLut(logImage);
		return 0;
	}
	return 1;
}
