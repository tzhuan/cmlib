#ifndef __TGA_H__
#define __TGA_H__

#define NOT_COLOR_MAPPED		0
#define	COLOR_MAPPED			1
#define TRUE_COLOR				2
#define BLACK_AND_WHITE			3
#define	RLE_COLOR_MAPPED		9
#define RLE_TRUE_COLOR			10
#define RLE_BLACK_AND_WHITE		11

typedef struct _TGAFileHeader
{
	unsigned char	id_length;
	unsigned char	colormap_type;
	unsigned char	image_type;

	unsigned short	first_entry_index;
	unsigned short	colormap_length;
	unsigned char	colormap_entry_size;

	unsigned short	x_origin;
	unsigned short	y_origin;
	unsigned short	width;
	unsigned short	height;
	unsigned char	pixel_depth;
	unsigned char	image_descriptor;

} TGAFileHeader;
																	// depth in file // read alpha or not
extern unsigned char *readTGA(const char *iname, int &width, int &height, int &depth, bool bAlpha=false);
extern bool writeTGA(const char *filename, unsigned char *data, int width, int height, int depth);

#endif


