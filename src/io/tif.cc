#include "tif.h"

//#pragma comment (linker, "/defaultlib:libtiff.lib")


unsigned char *readTIF(const char *file_name, int &width, int &height, int &depth)
{
	TIFF *tif = TIFFOpen(file_name, "r");

	if (tif==NULL) {
		return (unsigned char*)NULL;
	}

	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
	TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &depth);
/*
	uint16 ori;
	TIFFGetField(tif, TIFFTAG_ORIENTATION, &ori);
	printf("orientation=%d\n", ori);
	
	uint16 photometric;	// should be PHOTOMETRIC_RGB (2) for Adobe Photoshop tif at least
	TIFFGetFieldDefaulted(tif, TIFFTAG_PHOTOMETRIC, &photometric);

	uint16 extra_samples, *sample_info;
	TIFFGetFieldDefaulted(tif, TIFFTAG_EXTRASAMPLES, &extra_samples, &sample_info);
	bool matte=((extra_samples == 1) && (*sample_info == EXTRASAMPLE_ASSOCALPHA));
*/
	int row_size=width*depth;
	unsigned char *pixels=new unsigned char [height*row_size];
	int scanline_size=TIFFScanlineSize(tif);
	unsigned char *scanline=new unsigned char [scanline_size];

	if (scanline == NULL) {
		TIFFClose(tif);
		return (unsigned char*)NULL;
	}

	unsigned char *p=pixels;
	for (int y=0; y < height; y++)
	{
		TIFFReadScanline(tif, (char *)p, (uint32)y, 0);
		//memcpy(p, scanline, scanline_size);
		p+=row_size;
	}
        
	delete [] scanline;

	return (unsigned char *)pixels;
}

// TODO: writing to a TIF file is not supported yet
bool writeTIF(const char *file_name, unsigned char *pixels, int width, int height, int depth, bool compress)
{
	if(depth != 3 && depth != 4)
	    return false;

	TIFF *out=TIFFOpen(file_name, "w");

	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, depth);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);

	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	if (compress) {
		// zlib, the same as PNG
		TIFFSetField(out, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
	} else {
		TIFFSetField(out, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	}

	if (TIFFWriteEncodedStrip(out, 0, pixels, width*height*depth) == 0){
		return false;
	}

	TIFFClose(out);

	// that's it 
	return true;
}

