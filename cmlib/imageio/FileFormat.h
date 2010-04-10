#ifndef CMLIB_IMAGE_FILEFORMAT_H
#define CMLIB_IMAGE_FILEFORMAT_H

#include <string>
#include <cstdio>
#include <cctype>
#include <map>
#include <vector>
#include <algorithm>

#include "../image/TypeTrait.h" // to define DLLAPI on windows

namespace cmlib {
namespace image {

	enum FileFormat{
		FF_UNKNOWN, FF_PNG, FF_JPEG, FF_TIFF, FF_BMP, FF_TARGA, FF_PGM, FF_PPM,
		FF_EXR, FF_HDR, FF_PFM, FF_DPX, FF_UVE, FF_CRW, FF_FLT,
		FF_SIZE
	};

	typedef std::map<std::string,FileFormat> FormatTable;

	class Formater {
		public:
			// get file format by filename extension
			// this will be used in write()
			static FileFormat get_format(const std::string& filename)
			{
				using namespace std;

				string::size_type pos = filename.rfind('.');
				if (pos == string::npos)
					return FF_UNKNOWN;
				string ext = filename.substr(pos + 1);
				// convert to lower case
				transform(
					ext.begin(), 
					ext.end(), 
					ext.begin(), 
					ptr_fun<int,int>(tolower)
				);

				const FormatTable table = get_ext_table();
				FormatTable::const_iterator it = table.find(ext);
				if(it == table.end())
					return FF_UNKNOWN;
				else
					return it->second;
			}

			// get file format by its magic number
			// this will be used in read()
			static FileFormat get_format(FILE* f)
			{
				using namespace std;

				const int MAGIC_LENGTH = 12;

				char buf[MAGIC_LENGTH];
				int n_read = (int)fread(buf, 1, MAGIC_LENGTH, f);
				fseek(f, -n_read, SEEK_CUR);
				if(n_read < MAGIC_LENGTH){
					return FF_UNKNOWN;
				}

				string magic(buf, n_read);
				const FormatTable& table = get_magic_table();
				FormatTable::const_iterator it = table.upper_bound(magic);
				if(it == table.begin()) // smaller than the first key
					return FF_UNKNOWN;
				--it;
				if( magic.compare(0, it->first.length(), it->first) == 0)
					return it->second;

				return FF_UNKNOWN;
			}


			static const FormatTable& get_ext_table()
			{
				using namespace std;

				static FormatTable ext_table;   // table by extension
				static bool built = false;

				// this method is not thread safe.
				// however, it is acceptable because this doesn't make memory 
				// leak, but only rebuild the table.
				if(built == true)
					return ext_table;

				// not built yet
				built = true;
				ext_table["png"]  = FF_PNG;
				ext_table["jpg"]  = FF_JPEG;
				ext_table["jpeg"] = FF_JPEG;
				ext_table["tif"]  = FF_TIFF;
				ext_table["tiff"] = FF_TIFF;
				ext_table["bmp"] = FF_BMP;
				ext_table["tga"] = FF_TARGA;
				ext_table["ppm"] = FF_PPM;
				ext_table["pgm"] = FF_PGM;
				ext_table["exr"] = FF_EXR;
				ext_table["hdr"] = FF_HDR;
				ext_table["pfm"] = FF_PFM;
				ext_table["dpx"] = FF_DPX;
				ext_table["crw"] = FF_CRW;
				ext_table["flt"] = FF_FLT;

				return ext_table;
			}

			static const FormatTable& get_magic_table()
			{
				using namespace std;

				static FormatTable magic_table;
				static bool built = false;

				// not thread safe as above
				if(built == true)
					return magic_table;

				// not built yet
				built = true;

				magic_table["\x89PNG"]  = FF_PNG;

				magic_table["\xFF\xD8"]  = FF_JPEG;

				magic_table[string("MM\0\x2A",4)] = FF_TIFF;
				magic_table[string("II\x2A\0",4)] = FF_TIFF;

				magic_table["\x76\x2F\x31\x01"] = FF_EXR;

				magic_table["BM"] = FF_BMP;

				magic_table[string("\0\0\x02\0",4)] = FF_TARGA;
				magic_table[string("\0\0\x03\0",4)] = FF_TARGA;

				magic_table["#?RADIANCE"] = FF_HDR;

				magic_table["P5"] = FF_PGM;
				magic_table["P6"] = FF_PPM;

				magic_table["PF"] = FF_PFM;
				magic_table["Pf"] = FF_PFM;

				return magic_table;
			}
		private:
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_FILEFORMAT_H
