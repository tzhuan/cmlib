extern unsigned char* readJPG(const char* filename, int& width, int& height, int& depth);
extern bool writeJPG(const char *filename, unsigned char* data, int width, int height, int depth, int quality=100);

