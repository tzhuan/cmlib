#include <algorithm>
#include <cctype>
#include "FileFormat.h"

namespace Gil {
    const FileFormat get_format(const string& filename)
    {
	string::size_type pos = filename.rfind('.');
	if (pos == string::npos)
	    return FF_UNKNOWN;
	string ext = filename.substr(pos + 1);
	tranform(ext.begin(), ext.end(), ext.begin(), std::tolower);
	const char*(strs[]) = {
	    "pgm", "ppm", "bmp", "tga", "png", "jpg", "jpeg", "tif", 
	    "tiff", "hdr", "flt", "pfm", "crw", "uve", "dpx"};
	FileFormat fmts[] = {
	    FF_PGM, FF_PPM, FF_BMP, FF_TGA, FF_PNG, FF_JPG, FF_JPG, FF_TIF,
	    FF_TIF, FF_HDR, FF_FLT, FF_PFM, FF_CRW, FF_UVE, FF_DPX};
	const int size = sizeof(strs);
	for (int i = 0; i < size; ++i)
	    if (ext == strs[i])
		return fmts[i];
	return FF_UNKNOWN;
    }

    const FileType get_type(const FileFormat& format)
    {
	FileFormat fmts[] = {
	    FF_UNKNOWN, 
	    FF_PGM, FF_PPM, FF_BMP, FF_TGA, FF_PNG, FF_JPG, FF_TIF, 
	    FF_HDR, FF_FLT, FF_PFM, FF_CRW, FF_UVE, FF_DPX
	};
	FileType tps[] = {
	    FT_UNKNOWN,
	    FT_BYTE, FT_BYTE, FT_BYTE, FT_BYTE, FT_BYTE, FT_BYTE, 
	    FT_FLOAT, FT_FLOAT, FT_FLOAT, FT_FLOAT, FT_FLOAT, FT_FLOAT
	};
	const int size = sizeof(fmts);
	for (int i = 0; i < size; ++i)
	    if (format == fmts[i])
		return tps[i];
	return FT_UNKNOWN;
    }
}
