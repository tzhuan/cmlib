struct UVEHeader
{
	int width;
	int height;
	int channels;
	float umax;
	float umin;
	float vmax;
	float vmin;
	float emax;
	float emin;
	int   compress_length;
	char  reserved[14];
};

extern float* readUVE(const char *filename, int &width, int &height, int &depth);
extern bool writeUVE(const char *filename, float *data, int width, int height, int depth);

