#ifndef __GIL_FILE_FORMAT_H__
#define __GIL_FILE_FORMAT_H__

#include <string>

namespace Gil {
    enum FileType {
	FT_UNKNOWN, FT_BYTE, FT_FLOAT, FT_SIZE
    };

    enum FileFormat{
	FF_UNKNOWN, FF_PGM, FF_PPM, FF_BMP, FF_TGA, FF_PNG, FF_JPG, FF_TIF,
	FF_HDR, FF_FLT, FF_PFM, FF_CRW, FF_UVE, FF_DPX,
	FF_SIZE
    };

    const FileFormat get_format(const std::string&);
    const FileType get_type(const FileFormat&);

} // namespace Gil

#endif // __GIL_FILE_FORMAT_H__
