#include <stdio.h>
#include <string.h>

#include "tga.h"

unsigned char *readTGA(const char *iname, int &width, int &height, int &depth)
{
    bool bAlpha = false;
	FILE *fd=fopen(iname, "rb");
	TGAFileHeader tga_header;

	if (fd==NULL) return NULL;

	fread(&tga_header.id_length, sizeof(tga_header.id_length), 1, fd);
	fread(&tga_header.colormap_type, sizeof(tga_header.colormap_type), 1, fd);
	fread(&tga_header.image_type, sizeof(tga_header.image_type), 1, fd);

	fread(&tga_header.first_entry_index, sizeof(tga_header.first_entry_index), 1, fd);
	fread(&tga_header.colormap_length, sizeof(tga_header.colormap_length), 1, fd);
	fread(&tga_header.colormap_entry_size, sizeof(tga_header.colormap_entry_size), 1, fd);

	fread(&tga_header.x_origin, sizeof(tga_header.x_origin), 1, fd);
	fread(&tga_header.y_origin, sizeof(tga_header.y_origin), 1, fd);
	fread(&tga_header.width, sizeof(tga_header.width), 1, fd);
	fread(&tga_header.height, sizeof(tga_header.height), 1, fd);
	fread(&tga_header.pixel_depth, sizeof(tga_header.pixel_depth), 1, fd);
	fread(&tga_header.image_descriptor, sizeof(tga_header.image_descriptor), 1, fd);

	// invalide image type
	if (tga_header.image_type==0 || tga_header.image_type>11) return NULL;

	width = tga_header.width;
	height = tga_header.height;
	depth = tga_header.pixel_depth;

	// read comment (image id)
	if (tga_header.id_length != 0) {
		char *comment=new char [tga_header.id_length+1];
		fread(comment, sizeof(char), tga_header.id_length, fd);
		delete [] comment;
	}

	unsigned char j, k;

	// color map data
	unsigned char *colormap=NULL;
	unsigned char *cur_color_entry;
	if (tga_header.colormap_type==COLOR_MAPPED) {
		colormap=new unsigned char [3*tga_header.colormap_length];
		// read colormap
		cur_color_entry=colormap;
		for (int i=0; i<tga_header.colormap_length; i++) {
			switch (tga_header.colormap_entry_size) {
				case 8:
				default:
					// gray image
					fread(cur_color_entry, sizeof(unsigned char), 1, fd);
					cur_color_entry[2]=cur_color_entry[1]=cur_color_entry[0];
					break;
				case 15:
				case 16:
					fread(&j, sizeof(unsigned char), 1, fd);					
					fread(&k, sizeof(unsigned char), 1, fd);	
					cur_color_entry[0]=(255*((int)(k&0x7c>>2)))/31;
					cur_color_entry[1]=(255*((int)( ((k&0x03)<<3)+((j&0xe0)>>5) )))/31;
					cur_color_entry[2]=(255*((int)(j&0x1f)))/31;
					break;
				case 24:
				case 32:
					// stored in BGR order in the file
					fread(cur_color_entry+2, sizeof(unsigned char), 1, fd);
					fread(cur_color_entry+1, sizeof(unsigned char), 1, fd);
					fread(cur_color_entry+0, sizeof(unsigned char), 1, fd);
					break;
			}
			cur_color_entry+=3;
		}
	}

	// image data, two type of packets, raw or run-length
	bool skip=false;
	int flag=0;
	unsigned char runlength=0;
	// allocate data
	int bytes;
	unsigned char *data;
	if (depth==32 && bAlpha) {
		bytes=4;
		data=new unsigned char [width*height*4];
	} else {
		bytes=3;
		data=new unsigned char [width*height*3];
	}

	unsigned char *q;
	unsigned char p[4];
	for (int y=height-1; y>=0; y--) {
		q=data+y*bytes*width;
		for (int x=0; x<width; x++) {
			// RLE format
			if (tga_header.image_type>=RLE_COLOR_MAPPED 
				&& tga_header.image_type<=RLE_BLACK_AND_WHITE) 
			{
				if (runlength!=0) {
					runlength--;
					skip = (flag!=0);
				} else {
					fread(&runlength, sizeof(unsigned char), 1, fd);
					flag = runlength & 0x80;
					if (flag!=0) {	// RLE packet
						runlength-=128;
					}
					skip=false;
				}
			}

			if (!skip) {
				switch (tga_header.pixel_depth) {
				case 8:
				default:
					fread(&j, sizeof(unsigned char), 1, fd);
					if (tga_header.colormap_type==COLOR_MAPPED) {
						p[0]=colormap[3*j+0];
						p[1]=colormap[3*j+1];
						p[2]=colormap[3*j+2];
					} else {
						p[0]=p[1]=p[2]=j;
					}
					break;
				case 15:
				case 16:
					fread(&j, sizeof(unsigned char), 1, fd);
					fread(&k, sizeof(unsigned char), 1, fd);
					p[0]=(255*((int)(k&0x7c>>2)))/31;
					p[1]=(255*((int)( ((k&0x03)<<3)+((j&0xe0)>>5) )))/31;
					p[2]=(255*((int)(j&0x1f)))/31;
					break;
				case 24:
				case 32:
					fread(p+2, sizeof(unsigned char), 1, fd);
					fread(p+1, sizeof(unsigned char), 1, fd);
					fread(p+0, sizeof(unsigned char), 1, fd);
					if (tga_header.pixel_depth==32)
						fread(p+3, sizeof(unsigned char), 1, fd);
					break;
				}
			}

			memcpy(q, p, bytes);
			q+=bytes;
		}
	}

	// free colormap if necessary
	if (colormap!=NULL) delete [] colormap;

	fclose(fd);

	depth = bytes;

	return data;
}

bool writeTGA(const char *filename, unsigned char *data, int width, int height, int depth)
{
	if(depth != 3 && depth != 4)
	    return false;

	FILE *fd=fopen(filename, "wb");

	if (fd==NULL) 
		return false;

	// write header
	TGAFileHeader tga_header;

	tga_header.id_length=0;		// no comment
	tga_header.colormap_type=NOT_COLOR_MAPPED;	// write out true color image (with/without alpha)
	tga_header.image_type=TRUE_COLOR;

	tga_header.first_entry_index=0;
	tga_header.colormap_length=0;
	tga_header.colormap_entry_size=0;

	tga_header.x_origin=0;
	tga_header.y_origin=0;
	tga_header.width=width;
	tga_header.height=height;
	tga_header.pixel_depth=depth*8;
	tga_header.image_descriptor=0;

	fwrite(&tga_header.id_length, sizeof(tga_header.id_length), 1, fd);
	fwrite(&tga_header.colormap_type, sizeof(tga_header.colormap_type), 1, fd);
	fwrite(&tga_header.image_type, sizeof(tga_header.image_type), 1, fd);

	fwrite(&tga_header.first_entry_index, sizeof(tga_header.first_entry_index), 1, fd);
	fwrite(&tga_header.colormap_length, sizeof(tga_header.colormap_length), 1, fd);
	fwrite(&tga_header.colormap_entry_size, sizeof(tga_header.colormap_entry_size), 1, fd);

	fwrite(&tga_header.x_origin, sizeof(tga_header.x_origin), 1, fd);
	fwrite(&tga_header.y_origin, sizeof(tga_header.y_origin), 1, fd);
	fwrite(&tga_header.width, sizeof(tga_header.width), 1, fd);
	fwrite(&tga_header.height, sizeof(tga_header.height), 1, fd);
	fwrite(&tga_header.pixel_depth, sizeof(tga_header.pixel_depth), 1, fd);
	fwrite(&tga_header.image_descriptor, sizeof(tga_header.image_descriptor), 1, fd);

	// no comments

	// no colormap

	// write image data
	unsigned char *p, *q;
	unsigned char *row=new unsigned char [width*depth];
	if (row==NULL) 
		return false;

	for (int y=height-1; y>=0; y--) {	// the input data is upside down
		p=data+y*width*depth;
		q=row;
		for (int x=0; x<width; x++) {
			q[2]=(*p++);
			q[1]=(*p++);
			q[0]=(*p++);
			if (depth==4) {
				q[3]=*p;
				p++;
			}
			q+=depth;
		}
		fwrite(row, width*depth, 1, fd);
	}

	delete [] row;

	fclose(fd);

	return true;
}

