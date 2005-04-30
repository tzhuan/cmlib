
extern unsigned char* readPGM(const char* filename, int& width, int& height, int& depth);
extern bool writePGM(const char *filename, unsigned char* data, int width, int height, int depth);

extern unsigned char* readPPM(const char* filename, int& width, int& height, int& depth);
extern bool writePPM(const char *filename, unsigned char* data, int width, int height, int depth);
