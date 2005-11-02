#ifndef __GIL_FILE_IO_H__
#define __GIL_FILE_IO_H__

namespace Gil {

    // byte format
    unsigned char* readBMP(const char *filename, 
	    int& width, int& height, int &depth);
    bool writeBMP(const char *filename, unsigned char *data,
	    int width, int height, int depth); 

    unsigned char* readJPG(const char* filename,
	    int& width, int& height, int& depth);
    bool writeJPG(const char *filename, unsigned char* data,
	    int width, int height, int depth, int quality=100);

    unsigned char *readPNG(const char *file_name,
	    int &width, int &height, int &depth);
    bool writePNG(const char *file_name, unsigned char *pixels,
	    int width, int height, int depth);

    unsigned char* readPGM(const char* filename,
	    int& width, int& height, int& depth);
    bool writePGM(const char *filename, unsigned char* data,
	    int width, int height, int depth);

    unsigned char* readPPM(const char* filename,
	    int& width, int& height, int& depth);
    bool writePPM(const char *filename, unsigned char* data,
	    int width, int height, int depth);

    unsigned char *readTGA(const char *iname,
	    int &width, int &height, int &depth);
    bool writeTGA(const char *filename, unsigned char *data,
	    int width, int height, int depth);


    // float format
    float* readCRW(const char *filename, int &width, int &height, int &depth);

    unsigned char *readTIF(const char *file_name,
	    int &width, int &height, int &depth);
    bool writeTIF(const char *file_name, unsigned char *pixels,
	    int width, int height, int depth, bool compress=true);

    float* readDPX(const char *filename, int &width, int &height, int &depth);
    bool writeDPX(const char *filename, float *buffer,
	    int width, int height, int depth = 3);

    float *readHDR(const char *hdrname, int &width, int &height, int& depth);
    bool  writeHDR(const char *hdrname, float *radiance,
	    int width, int height, int depth);

    float *readFLT(const char *fltname, int &width, int &height, int &depth);
    bool  writeFLT(const char *fltname, float *data,
	    int width, int height, int depth);

    float* readPFM(const char *filename, int &width, int &height, int &depth);
    bool writePFM(const char *filename, float *data,
	    int width, int height, int depth);

    float* readUVE(const char *filename, int &width, int &height, int &depth);
    bool writeUVE(const char *filename, float *data,
	    int width, int height, int depth);

} // namespace Gil

#endif // __GIL_FILE_IO_H__
