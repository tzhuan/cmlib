float *readHDR(const char *hdrname, int &width, int &height, int& depth);
bool  writeHDR(const char *hdrname, float *radiance, int width, int height, int depth);

float *readFLT(const char *fltname, int &width, int &height, int &depth);
bool  writeFLT(const char *fltname, float *data, int width, int height, int depth);

//short int *readraw(char *rawname, int width, int height, int depth);


