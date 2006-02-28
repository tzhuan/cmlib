#ifndef GIL_FILE_FORMAT_H
#define GIL_FILE_FORMAT_H

#include <string>
#include <cstdio>

#include "Color.h" // to define DLLAPI on windows

namespace gil {

    enum FileFormat{
	FF_UNKNOWN, FF_PNG, FF_JPEG, FF_TIFF, FF_BMP, FF_TARGA, FF_PGM, FF_PPM,
	FF_EXR, FF_HDR, FF_PFM, FF_DPX, FF_UVE, FF_CRW, FF_FLT,
	FF_SIZE
    };

    // get file format by filename extension
    // this will be used in write()
    FileFormat DLLAPI get_format(const std::string& name);

    // get file format by its magic number
    // this will be used in read()
    FileFormat DLLAPI get_format(FILE* f);

} // namespace gil

#endif // GIL_FILE_FORMAT_H
