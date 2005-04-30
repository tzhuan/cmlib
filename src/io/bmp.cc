//
// bitmap.cpp
//
// handle MS bitmap I/O. For portability, we don't use the data structure defined in Windows.h
// However, there is some strange thing, the side of our structure is different from what it 
// should though we define it in the same way as MS did. So, there is a hack, we use the hardcoded
// constant, 14, instead of the sizeof to calculate the size of the structure.
// You are not supposed to worry about this part. However, I will appreciate if you find out the
// reason and let me know. Thanks.
//

#include "bmp.h"

unsigned char *readBMP(const char *fname, int &width, int &height, int &depth)
{ 
	FILE* file; 
	BMP_BITMAPFILEHEADER bmfh; 
	BMP_BITMAPINFOHEADER bmih; 
	BMP_DWORD pos; 
 
	if ( (file=fopen( fname, "rb" )) == NULL )  
		return NULL; 
	 
//	I am doing fread( &bmfh, sizeof(BMP_BITMAPFILEHEADER), 1, file ) in a safe way. :}
	fread( &(bmfh.bfType), 2, 1, file); 
	fread( &(bmfh.bfSize), 4, 1, file); 
	fread( &(bmfh.bfReserved1), 2, 1, file); 
	fread( &(bmfh.bfReserved2), 2, 1, file); 
	fread( &(bmfh.bfOffBits), 4, 1, file); 

	pos = bmfh.bfOffBits; 
 
	fread( &bmih, sizeof(BMP_BITMAPINFOHEADER), 1, file ); 
 
	// error checking
	if ( bmfh.bfType!= 0x4d42 ) {	// "BM" actually
		return NULL;
	}
	if ( bmih.biBitCount != 24 && bmih.biBitCount != 8 )  
		return NULL; 
/*
 	if ( bmih.biCompression != BMP_BI_RGB ) {
		return NULL;
	}
*/
	fseek( file, pos, SEEK_SET ); 
 
	width = bmih.biWidth; 
	height = bmih.biHeight; 
 
	int padWidth;
	if (bmih.biBitCount == 24) padWidth = width * 3; 
	else padWidth = width;

	int pad = 0; 
	if ( padWidth % 4 != 0 ) 
	{ 
		pad = 4 - (padWidth % 4); 
		padWidth += pad; 
	} 
 
	if (bmih.biBitCount == 24) 	depth = 3;
	else depth = 1;
		
	unsigned char *data = new unsigned char [width*height*depth];
	unsigned char *line = new unsigned char [padWidth];
//	int foo = fread( data, bytes, 1, file ); 
	
	// shuffle bitmap data such that it is (R,G,B) tuples in row-major order
	int i, j;
	j = 0;
	unsigned char temp;
	unsigned char* in;
	unsigned char* out;

	for ( j = 0; j < height; ++j )
	{ 
		if (!fread( line, padWidth, 1, file )) {
			delete [] data;
			delete [] line;
			return NULL;
		}

		in = line;
		out = data+(height-1-j)*width*depth;

		for ( i = 0; i < width; ++i )
		{
			if (depth==3) {
				out[1] = in[1];
				temp = in[2];
				out[2] = in[0];
				out[0] = temp;
			} else {
				out[0] = in[0];
			}

			in += depth;
			out += depth;
		}
		in += pad;
	}
			  
	delete [] line;

	fclose( file );

	return data; 
} 
 
bool writeBMP(const char *fname, unsigned char *data, int width, int height, int depth=3) 
{ 
	BMP_BITMAPFILEHEADER bmfh; 
	BMP_BITMAPINFOHEADER bmih; 
	if (depth!=3 && depth!=1)
		return false;

	int bytes, pad;
	bytes = width * depth;
	pad = (bytes%4) ? 4-(bytes%4) : 0;
	bytes += pad;
	bytes *= height;

	bmfh.bfType = 0x4d42;    // "BM"
	bmfh.bfSize = sizeof(BMP_BITMAPFILEHEADER) + sizeof(BMP_BITMAPINFOHEADER) + bytes;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = /*hack sizeof(BMP_BITMAPFILEHEADER)=14, sizeof doesn't work?*/ 
		14 + sizeof(BMP_BITMAPINFOHEADER) + ((depth==3) ? 0 : 1024);

	bmih.biSize = sizeof(BMP_BITMAPINFOHEADER);
	bmih.biWidth = width;
	bmih.biHeight = height;
	bmih.biPlanes = 1;
	bmih.biBitCount = (depth==3) ? 24 : 8;
	bmih.biCompression = BMP_BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = (int)(100 / 2.54 * 72);
	bmih.biYPelsPerMeter = (int)(100 / 2.54 * 72);
	if (depth==1) bmih.biClrUsed = 256;
	else bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	FILE *foo=fopen(fname, "wb"); 

	//	fwrite(&bmfh, sizeof(BMP_BITMAPFILEHEADER), 1, foo);
	fwrite( &(bmfh.bfType), 2, 1, foo); 
	fwrite( &(bmfh.bfSize), 4, 1, foo); 
	fwrite( &(bmfh.bfReserved1), 2, 1, foo); 
	fwrite( &(bmfh.bfReserved2), 2, 1, foo); 
	fwrite( &(bmfh.bfOffBits), 4, 1, foo); 

	fwrite(&bmih, sizeof(BMP_BITMAPINFOHEADER), 1, foo); 

	if (depth==1) {
		// write out color table
		unsigned char c[4];
		c[3]=0;
		for (int i=0; i<256; i++) {
			c[0]=c[1]=c[2]=i;
			fwrite(c, 4, 1, foo);
		}
	}

	bytes /= height;
	unsigned char* scanline = new unsigned char [bytes];
	for ( int j = 0; j < height; ++j )
	{
		memcpy( scanline, data + (height-1-j)*depth*width, bytes );
		if (depth==3) {
			for ( int i = 0; i < width; ++i )
			{
				unsigned char temp = scanline[i*3];
				scanline[i*3] = scanline[i*3+2];
				scanline[i*3+2] = temp;
			}
		}
		fwrite( scanline, bytes, 1, foo);
	}

	delete [] scanline;

	fclose(foo);

	return true;
}


