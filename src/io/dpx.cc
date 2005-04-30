#include "io_utils/dpxlib.h"

float *readDPX(const char *filename, int &width, int &height, int &depth)
{
	DpxFile* dpxImage;
	if ((dpxImage = dpxOpen(filename)) == 0)
		return 0;

	LogImageByteConversionParameters conversion;
	logImageGetByteConversionDefaults(&conversion);
	logImageSetByteConversion(dpxImage, &conversion);
	dpxGetSize(dpxImage, &width, &height, &depth);
	
	int row_len = width * depth;
	float *buffer = new float[row_len * height];

	for (int y = 0; y < height; ++y)
		dpxGetRowBytes(dpxImage, buffer + y * row_len, y);

	dpxClose(dpxImage);

	return buffer;
}

bool writeDPX(const char *filename, float *buffer, int width, int height, int depth = 3) 
{
	DpxFile *dpxImage;
	if ((dpxImage = dpxCreate(filename, width, height, depth)) == 0) {
		return false;
	}

	LogImageByteConversionParameters conversion;
	logImageGetByteConversionDefaults(&conversion);
	logImageSetByteConversion(dpxImage, &conversion);
	
	int row_len = width * depth;
	for (int y = 0; y < height; ++y)
		dpxSetRowBytes(dpxImage, buffer + y * row_len, y);

	dpxClose(dpxImage);

	return true;
}

/*
int main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s in_dpx out_dpx\n", argv[0]);
		return 1;
	}
	int width, height, depth;
	float *buffer = readDPX(argv[1], width, height, depth);
	
	if (buffer == 0)
		return 1;
	
	for (int i = 0; i < width * height * depth; ++i)
		buffer[i] = 1.0 - buffer[i];
	
	writeDPX(argv[2], buffer, width, height, depth);
	
	return 0;
}
*/
