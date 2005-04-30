#include <stdio.h>

#define REVERSEBYTES(source, dest)		\
{										\
	char *j = (char *) source;			\
	char *dj = (char *) dest;			\
	dj[0] = j[3];						\
	dj[1] = j[2];						\
	dj[2] = j[1];						\
	dj[3] = j[0];						\
}

float* readPFM(const char* filename, int &width, int &height, int &depth)
{
	FILE *infile = fopen(filename, "rb");

	if (infile==NULL) {
		return NULL;
	}

	int a = fgetc(infile);
	int b = fgetc(infile);
	char junk = fgetc(infile);

	if ((a != 'P') || ((b != 'F') && (b != 'f')))
	{
		fclose(infile);
		return NULL;
	}

	b = (b == 'F');		// 'F' = RGB,  'f' = monochrome
	depth = (b ? 3 : 1);

	fscanf(infile, "%d %d%c", &width, &height, &junk);
	if ((width <= 0) || (height <= 0))
	{
		fclose(infile);
		return NULL;
	}

	float scalefactor;
	fscanf(infile, "%f%c", &scalefactor, &junk);

	float *radiance = new float [width*height*depth];

	if (!radiance)
	{
		fclose(infile);
		return NULL;
	}

	a = width * depth;
	float *fbuf = new float[a];
	for (int y = 0; y < height; y++)
	{
		if (fread(fbuf, sizeof(float), a, infile) != (size_t) a)
		{
			fclose(infile);
			delete fbuf;
			delete radiance;
			return NULL;
		}

		float *cur = radiance+(height-1-y)*a;
		float *temp = fbuf;
		for (int x = 0; x < width; x++)
		{
			if (b)			// color
			{
				if (scalefactor > 0.0)		// MSB
				{
					REVERSEBYTES(temp++, cur+0);
					REVERSEBYTES(temp++, cur+1);
					REVERSEBYTES(temp++, cur+2);
				}
				else						// LSB
				{
					cur[0] = *temp++;
					cur[1] = *temp++;
					cur[2] = *temp++;
				}
				cur+=3;
			}
			else			// black and white
			{
				float c;
				if (scalefactor > 0.0)		// MSB
				{
					REVERSEBYTES(temp++, &c);
				}
				else						// LSB
					c = *temp++;
				*cur = c;
				cur++;
			}
		}
	}

	delete fbuf;

	fclose(infile);

	return radiance;
}

bool writePFM(const char *filename, float *data, int width, int height, int depth)
{
	if(depth != 3 && depth != 1)
	    return false;
	
	FILE *outfile = fopen(filename, "wb");

	if (outfile==NULL) 
		return false;

	fputc('P', outfile);
	if(depth == 3)
	    fputc('F', outfile);
	else
	    fputc('f', outfile);
	fputc(0x0a, outfile);

	fprintf(outfile, "%d %d", width, height);
	fputc(0x0a, outfile);

	fprintf(outfile, "%f", -1.0f);
	fputc(0x0a, outfile);

	int rowWidth=depth*width;
	for (int y = height - 1; y >= 0; y--)
	{
		fwrite(data+y*rowWidth, sizeof(float)*rowWidth, 1, outfile);
	}

	fclose(outfile);

	return true;
}
