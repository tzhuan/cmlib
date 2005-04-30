#ifndef __DPX_H__
#define __DPX_H__

extern float *readDPX(const char *filename, int &width, int &height, int &depth);
extern bool writeDPX(const char *filename, float *buffer, int width, int height, int depth = 3);

#endif
