#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#endif


#include "io_utils/color.h"
#include "io_utils/resolu.h"

extern int checkheader(FILE *fin, char *fmt, FILE *fout);
extern void newheader(char *s, register FILE *fp);
extern void printargs(int ac, char **av, register FILE *fp);
extern void fputformat(char *s, FILE *fp);
extern int fwritecolrs(register COLR  *scanline, unsigned len, register FILE  *fp);

#ifdef _WINDOWS
// helper function
unsigned int filesize(const char *name)
{
	int fh;

	if ( (fh = open(name, O_RDONLY) ) != -1 ) {
		int size = filelength(fh);
		close(fh);
		return size;
	}
	else {
		return 0;
	}
}
#else
unsigned int filesize(const char *name)
{
	FILE *fd;

	if ( (fd = fopen(name, "r") ) != NULL ) {
		int size;
		fseek(fd, 0, SEEK_END);
		size=ftell(fd);
		fclose(fd);
		return size;
	}
	else {
		return 0;
	}
}
#endif

void realpixels_2_rgb(COLR *scanin, int xmax, float *realscan)
{
	int x;
	float v;

	for (x = 0; x < xmax; x++) {

		if ( scanin[x][EXP] == 0) {
			realscan[0] = 0;
			realscan[1] = 0;
			realscan[2] = 0;
		}
		else {
			v = ldexp(1.0/256, scanin[x][EXP]-128);
			realscan[0] = (scanin[x][RED] + 0.5)*v;
			realscan[1] = (scanin[x][GRN] + 0.5)*v;
			realscan[2] = (scanin[x][BLU] + 0.5)*v;
		}

		realscan+=3;
	}
}

void rgb_2_realpixels(float *realscan, int xmax, COLR *scanout)
{
	int x;
	float r, g, b, max;
	float factor;
	int exp;

	for (x = 0; x < xmax; x++) {
		r = realscan[x*3+RED];
		g = realscan[x*3+GRN];
		b = realscan[x*3+BLU];

		/* find max */
		max = r;
		if (max < g) max = g;
		if (max < b) max = b;

		/* find normalize factor */
		factor=1;
		exp=0;

		if (max>0) {
			while ((max > 1.0) || (max < 0.5)) {
				if (max>1.0) {
					max *= 0.5;	
					factor *= 0.5;
					exp++;
				} else {
					max *= 2;
					factor *= 2;
					exp--;
				}
			}

			r *= factor;
			g *= factor;
			b *= factor;

			scanout[x][RED] = (unsigned char)(r*255+0.5);
			scanout[x][GRN] = (unsigned char)(g*255+0.5);
			scanout[x][BLU] = (unsigned char)(b*255+0.5);
			scanout[x][EXP] = COLXS + exp;
		//} else if (max=0) {
		} else if (max==0) {
			scanout[x][RED] = 0;
			scanout[x][GRN] = 0;
			scanout[x][BLU] = 0;
			scanout[x][EXP] = 0;	
		} else {
			// it is an error
		}
	}
}

float *short2float(unsigned short *din, int width, int height, int depth)
{
	int x, y, k;
	float *p;
	unsigned short *q;

	float *dout=new float[width*height*depth];

	p=dout;
	q=din;
	for (y=0; y<height; y++) {
//		q=din+y*(width*depth);
		for (x=0; x<width; x++) {
			for (k=0; k<depth; k++) {
				*p = (float)(*q);
				p++;
				q++;
			}
		}
	}

	return dout;
}

unsigned short *float2short(float *din, int width, int height, int depth)
{
	int x, y, k;
	unsigned short *p;
	float *q;

//	unsigned short *dout=(unsigned short*)malloc(width*height*depth*sizeof(unsigned short));
	unsigned short *dout=new unsigned short [width*height*depth];

	p=dout;
	q=din;
	for (y=0; y<height; y++) {
		for (x=0; x<width; x++) {
			for (k=0; k<depth; k++) {
				*p = (unsigned short)(*q);
				p++;
				q++;
			}
		}
	}

	return dout;
}

float *short2float3(unsigned short *din, int width, int height, int depth)
{
	int x, y, k;
	float *p;
	unsigned short *q;

	if (depth==3) 
		return short2float(din, width, height, 3);
	else {
		float *dout=new float[width*height*3];

		p=dout;
		q=din;
		for (y=0; y<height; y++) {
			for (x=0; x<width; x++) {
				for (k=0; k<depth; k++) {
					p[0] = p[1] = p[2] = (float)(*q);
					p+=3;
					q++;
				}
			}
		}

		return dout;
	}
}

float *BayorGray2Color(float *din, int width, int height)
{
	int x, y;
	float *p;
	float *q;

//	float *dout=(float*)malloc(width*height*3*sizeof(float));
	float *dout=new float [width*height*3];

	float scale=1;//1.0/(1<<12);

	p=dout;
	q=din;
	for (y=0; y<height; y++) {
		for (x=0; x<width; x++) {
			if (y%2==0) {	// even y
				if (x%2==0) {	// even x => green
					p[0] = 0;
					p[1] = q[0]*scale;
					p[2] = 0;
				}
				else {	// odd x => red
					p[0] = q[0]*scale;
					p[1] = 0;
					p[2] = 0;
				}
			}
			else {	// odd y
				if (x%2==0) {	// even x => blue
					p[0] = 0;
					p[1] = 0;
					p[2] = q[0]*scale;
				}
				else {	// old x => green
					p[0] = 0;
					p[1] = q[0]*scale;
					p[2] = 0;
				}
			}

			p+=3;
			q++;
		}
	}

	return dout;
}

/*
unsigned short *readRAW(char *rawname, int &width, int &height, int &depth)
{
	bool bheader=false;

	if (width>0) {
		// verify user's input
		int size=filesize(rawname);
		if (size==0) {
			printf("%s doesn't exist\n", rawname);
			return NULL;
		}

		int d=width*height*depth*sizeof(unsigned short)-size;
		if (d<0) d=-d;

		if (d!=0 && d!=sizeof(int)*2) {
			printf("Warning bad dimension!\n");
			width=-1;
		}
		else {
			if (d==0)
				bheader=false;
			else
				bheader=true;
		}
	}

	if (width<=0) {
		// try to guess size
		unsigned int size=filesize(rawname);

		FILE *tfd=fopen(rawname, "r");
		fread(&width, sizeof(int), 1, tfd);
		fread(&height, sizeof(int), 1, tfd);

		if (size==(width*height*sizeof(unsigned short)+2*sizeof(int))) {
			depth=1;
			bheader=true;
		}
		else if (size==width*height*3*sizeof(unsigned short)+2*sizeof(int)) {
			depth=3;
			bheader=true;
		}
		else {
			// not stored width, height, try to guess
			if (size==1728*1152*sizeof(unsigned short)) {
				width  = 1728;
				height = 1152;
				depth  = 1;
				bheader=false;
			}
			else if (size==1728*1152*3*sizeof(unsigned short)) {
				width  = 1728;
				height = 1152;
				depth  = 3;
				bheader=false;
			}
			else {
				printf("Can't guess the dimension of the file!\n");
				return NULL;
			}
		}

		fclose(tfd);
	}

	FILE *fd=fopen(rawname, "r");
	unsigned short *rawdata;

//	rawdata = (unsigned short *)malloc(width*height*depth*sizeof(unsigned short));
	rawdata = new unsigned short [width*height*depth];

	if (rawdata==NULL) {
		return NULL;
	}

	fread(rawdata, sizeof(unsigned short), width*height*depth, fd);

	// mask out 4 bits
	unsigned short *p=rawdata;
	for (int y=0; y<height; y++) {
		for (int x=0; x<width; x++) {
			*p &= 0x0FFF;
			p++;
		}
	}

	fclose(fd);

	return rawdata;
}

bool writeRAW(char *rawname, int width, int height, int depth, unsigned short *data)
{
	FILE *fd=fopen(rawname, "wb");

	if (fd==NULL) return 0;

	fwrite(&width, sizeof(int), 1, fd);
	fwrite(&height, sizeof(int), 1, fd);

	fwrite(data, sizeof(unsigned short), width*height*depth, fd);

	fclose(fd);

	return 1;
}
*/

float *readHDR(const char *hdrname, int& width, int& height, int& depth)
{
	FILE *fd=fopen(hdrname, "rb");
	int ymax, xmax;
	COLR    *scanin;
// 	float 	*realscan;
//	register int    x;
	int	y;
	float	*radiance;

	if (fd==NULL) {
		return NULL;
	}

	/* get hdr header */
	if (checkheader(fd, COLRFMT, NULL) < 0 || fgetresolu(&xmax, &ymax, fd) < 0)
		return NULL;	// bad format

	width  = xmax;
	height = ymax;
	depth = 3;
	// allocate space for radiance
//	radiance = (float *)malloc(xmax*ymax*3*sizeof(float));
	radiance = new float [xmax*ymax*3];

	if (radiance==NULL) {
		return NULL;
	}

	/* allocate a scanline */
	scanin = new COLR[xmax];
	if (scanin == NULL) {
		delete [] radiance;
		return NULL;
	}

/*
	realscan = (float *)malloc(xmax*3*sizeof(float));
	if (realscan == NULL)
		quiterr("out of memory in skel2ra");
*/

	/* convert image */
//	for (y = ymax-1; y >= 0; y--) {
	for (y = 0; y <ymax; y++) {
		if (freadcolrs(scanin, xmax, fd) < 0) {
			// error reading Radiance picture
			delete [] radiance;
			delete [] scanin;
			return NULL;
		}

		int oy=y;
		realpixels_2_rgb(scanin, xmax, radiance+oy*3*xmax);
	}

	/* free scanline */
	delete [] scanin;

	fclose(fd);

	return radiance;
}

bool writeHDR(const char *hdrname, float *radiance, int width, int height, int depth)
{
	FILE *fd=fopen(hdrname, "wb");
	int ymax, xmax;
	COLR	*scanout;
// 	float 	*realscan;
//	register int	x;
	int	y;
	char    *argv[1];

	if(depth != 3)
	    return false;

	argv[0] = new char[128];
	strcpy(argv[0], "hdrio");

	xmax = width;
	ymax = height;

	/* put hdr header */
	newheader("RADIANCE", fd);
	printargs(1, argv, fd);
	fputformat(COLRFMT, fd);
//    putchar('\n');
	fprintf(fd, "\n");
	fprtresolu(xmax, ymax, fd);

	delete [] argv[0];

	/* allocate scanline */
	scanout = new COLR[xmax];
	if (scanout == NULL) {
		// out of memory in skel2ra
		return false;
	}

	/* convert image */
//	for (y = ymax-1; y >= 0; y--) {
	for (y = 0; y <ymax; y++) {
		/* read in one line of float pixels and convert into R,G,B,E format */
		/* See Graphics Gem II p.80 */

		int oy=y;
		rgb_2_realpixels(radiance+3*oy*xmax, xmax, scanout);
 
		if (fwritecolrs(scanout, xmax, fd) < 0) {
			delete [] scanout;
			return false;
		}
	}

	/* free scanline */
	delete [] scanout;

	fclose(fd);

	return true;
}

float *readFLT(const char *fltname, int &width, int &height, int &depth)
{
	FILE *fd=fopen(fltname, "rb");
	float *data;

	if (fd==NULL) return NULL;

	fread(&width,  sizeof(int), 1, fd);
	fread(&height, sizeof(int), 1, fd);

	// get file size
	unsigned int size = filesize(fltname) - 2*sizeof(int);
	depth = size / sizeof(float) / width / height;
	if(width*height*depth*sizeof(float) != size )
	    return NULL;

	data = new float [width*height*depth];

	if (data==NULL) {
		return NULL;
	}

	fread(data, sizeof(float)*width*depth, height, fd);

	fclose(fd);

	return data;
}

bool writeFLT(const char *fltname, float *data, int width, int height, int depth)
{
	FILE *fd=fopen(fltname, "wb");

	if (fd==NULL) return false;

	fwrite(&(width), sizeof(int), 1, fd);
	fwrite(&(height), sizeof(int), 1, fd);

	fwrite(data, sizeof(float)*width*depth, height, fd);

	fclose(fd);

	return true;
}


