#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "zlib.h"

#include "uve.h"

//#pragma comment (linker, "/defaultlib:zlib.lib")

#define CHECK_ERR(err, msg, r) { \
    if (err != Z_OK) { \
        fprintf(stderr, "%s error: %d\n", msg, err); \
        return r; \
    } \
}

// use ? bits for u and v each, 8 bits for error
/*
#define		UV_BITS		12
#define		U_MASK		0xFFF00000
#define		V_MASK		0x000FFF00
*/
/*
#define		UV_BITS		10
#define		U_MASK		0x0FFC0000
#define		V_MASK		0x0003FF00
*/
#define		UV_BITS		8
#define		U_MASK		0x00FF0000
#define		V_MASK		0x0000FF00

#define		E_MASK		0x000000FF
#define		UV_DIVS		(1<<UV_BITS)
#define		E_BITS		8
#define		E_DIVS		(1<<E_BITS)

bool CheckZlibVersion()
{
	// check zlib version
    static const char* myVersion = ZLIB_VERSION;

    if (zlibVersion()[0] != myVersion[0]) {
        fprintf(stderr, "incompatible zlib version\n");
        return false;

    } else if (strcmp(zlibVersion(), ZLIB_VERSION) != 0) {
        fprintf(stderr, "warning: different zlib version\n");
    }

	return true;
}

void adjust(float &max, float &min, int divs)
{
	int idx=(int)((-min/(max-min))*(divs-1));
	float new_uinterval = (max-min)*((float)divs/(divs-1));	// enalrge a little 
	float d=new_uinterval/(divs-1);
	min = -idx*d;
	max = min+new_uinterval;
}

void FindMaxMin(float *data, int width, int height, 
				float &umax, float &umin, 
				float &vmax, float &vmin,
				float &emax, float &emin)
{
	umax = vmax = emax = -1.0e10;
	umin = vmin = emin =  1.0e10;

	float *p=data;
	for (int y=0; y<height; y++) {
		for (int x=0; x<width; x++) {
			if (p[0]>umax) umax=p[0];
			if (p[0]<umin) umin=p[0];

			if (p[1]>vmax) vmax=p[1];
			if (p[1]<vmin) vmin=p[1];

			if (p[2]>emax) emax=p[2];
			if (p[2]<emin) emin=p[2];

			p+=3;
		}
	}

	// TODO: adjust max and min so that zero can be represented?
	adjust(umax, umin, UV_DIVS);
	adjust(vmax, vmin, UV_DIVS);
	adjust(emax, emin, E_DIVS);
}

float *DecodeFloatBuffer(unsigned int *buf, 
						 int width, int height,
						 float umax, float umin, float vmax, float vmin, 
						 float emax, float emin)
{
	float *data = new float [width*height*3];

	// find zero index
	float idu=(UV_DIVS-1)/(umax-umin);
	float idv=(UV_DIVS-1)/(vmax-vmin);
	float ide=(E_DIVS-1)/(emax-emin);
	int zero_uidx=(int)((0-umin)*idu+0.5);
	int zero_vidx=(int)((0-vmin)*idv+0.5);
	int zero_eidx=(int)((0-emin)*ide+0.5);

	float du=(umax-umin)/(UV_DIVS-1);
	float dv=(vmax-vmin)/(UV_DIVS-1);
	float de=(emax-emin)/(E_DIVS-1);

	unsigned int *p=buf;
	float *q=data;
	for (int y=0; y<height; y++) {
		for (int x=0; x<width; x++) {
			int uidx = (p[0] & U_MASK) >> (UV_BITS+E_BITS);
			int vidx = (p[0] & V_MASK) >> (E_BITS);
			int eidx = (p[0] & E_MASK);

			if (uidx==zero_uidx) q[0]=0;
			else q[0] = uidx*du + umin;

			if (vidx==zero_vidx) q[1]=0;
			else q[1] = vidx*dv + vmin;

			if (eidx==zero_eidx) q[2]=0;
			else q[2] = eidx*de + emin;

			q+=3;
			p++;
		}
	}

	return data;
}

unsigned int *
EncodeFloatBuffer(float *data, int width, int height,
				  float umax, float umin, float vmax, float vmin, float emax, float emin)
{
	unsigned int *buf=new unsigned int [width*height];

	float du=(UV_DIVS-1)/(umax-umin);
	float dv=(UV_DIVS-1)/(vmax-vmin);
	float de=(E_DIVS-1)/(emax-emin);

	float *p=data;
	unsigned int *q=buf;
	for (int y=0; y<height; y++) {
		for (int x=0; x<width; x++) {
			int uidx=(int)((p[0]-umin)*du+0.5);
			int vidx=(int)((p[1]-vmin)*dv+0.5);
			int eidx=(int)((p[2]-emin)*de+0.5); 

			*q= (uidx<<(UV_BITS+E_BITS)) | (vidx<<(E_BITS)) | eidx ;

			q++;
			p+=3;
		}
	}

	return buf;
}

float* readUVE(const char *filename, int &width, int &height, int &depth)
{
	if (!CheckZlibVersion()) 
		return NULL;

	FILE *infile = fopen(filename, "rb");

	if (infile==NULL) {
		return NULL;
	}

	UVEHeader header;

	// read header
	fread(&header, sizeof(UVEHeader), 1, infile);
	width  = header.width;
	height = header.height;
	depth = header.channels;

	// read compressed data
	uLong compress_len=header.compress_length;
	unsigned char *compress_buf=new unsigned char [compress_len];
	fread(compress_buf, 1, compress_len, infile);

	// uncompressed data to buffer
	uLong uncompress_len=width*height*4;
	unsigned char *uncompress_buf=new unsigned char [uncompress_len];
    int err = uncompress(uncompress_buf, &uncompress_len, compress_buf, compress_len);
    CHECK_ERR(err, "uncompress", NULL);

	// convert buffer to float buffer
	float *data = DecodeFloatBuffer((unsigned int*)uncompress_buf, width, height,
									header.umax, header.umin, header.vmax, header.vmin, header.emax, header.emin);

	fclose(infile);

	delete [] compress_buf;
	delete [] uncompress_buf;

	return data;
}

bool writeUVE(const char *filename, float *data, int width, int height, int channels)
{
	if(channels != 3)
	    return false;

	if (!CheckZlibVersion()) 
		return false;

	FILE *outfile = fopen(filename, "wb");

	if (outfile==NULL) 
		return false;

	// convert float buffer to buffer
	float umax, umin, vmax, vmin, emax, emin;
	FindMaxMin(data, width, height, umax, umin, vmax, vmin, emax, emin);

	// check if dynamic range is too big
	if ( (umax-umin) > (1<<(UV_BITS-3))) {
		printf("Dynamic range (%f,%f) is too big\n", umin, umax);
		return false;
	}
	if ( (vmax-vmin) > (1<<(UV_BITS-3))) {
		printf("Dynamic range (%f,%f) is too big\n", vmin, vmax);
		return false;
	}
	unsigned int *uncompress_buf = EncodeFloatBuffer(data, width, height, umax, umin, vmax, vmin, emax, emin);
	uLong uncompress_len=width*height*4;

	// compressed buffer
	uLong compress_len=uncompress_len+64;
	unsigned char *compress_buf=new unsigned char [compress_len];
    int err = compress(compress_buf, &compress_len, (unsigned char *)uncompress_buf, uncompress_len);
    CHECK_ERR(err, "compress", false);

	// prepare header
	UVEHeader header;
	header.width    = width;
	header.height   = height;
	header.channels = 3;
	header.umax     = umax;
	header.umin     = umin;
	header.vmax     = vmax;
	header.vmin     = vmin;
	header.emax     = emax;
	header.emin     = emin;
	header.compress_length = compress_len;

	// write header
	fwrite(&header, sizeof(UVEHeader), 1, outfile);

	// write compressed data
	fwrite(compress_buf, 1, compress_len, outfile);

	fclose(outfile);

	delete [] compress_buf;
	delete [] uncompress_buf;

	return true;
}
/*
// only valid if use 8 bits for flows
void readUVE(char *filename, int &width, int &height,
			   unsigned char* &uve, 
			   float &umax, float &umin, 
			   float &vmax, float &vmin,
			   float &emax, float &emin)
{
	if (!CheckZlibVersion()) 
		return NULL;

	FILE *infile = fopen(filename, "rb");

	if (infile==NULL) {
		return NULL;
	}

	UVEHeader header;

	// read header
	fread(&header, sizeof(UVEHeader), 1, infile);
	width  = header.width;
	height = header.height;

	// read compressed data
	uLong compress_len=header.compress_length;
	unsigned char *compress_buf=new unsigned char [compress_len];
	fread(compress_buf, 1, compress_len, infile);

	// uncompressed data to buffer
	uLong uve=width*height*4;
	unsigned char *uncompress_buf=new unsigned char [uncompress_len];
    int err = uncompress(uncompress_buf, &uncompress_len, compress_buf, compress_len);
    CHECK_ERR(err, "uncompress", NULL);

	// convert buffer to float buffer
	float *data = DecodeFloatBuffer((unsigned int*)uncompress_buf, width, height,
									header.umax, header.umin, header.vmax, header.vmin, header.emax, header.emin);

	fclose(infile);

	delete [] compress_buf;
	delete [] uncompress_buf;

	return data;
}
*/

