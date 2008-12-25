#ifndef CMLIB_IMAGE_EXCEPTION_H
#define CMLIB_IMAGE_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace cmlib {
namespace image {

	/**
	 * this exception will be thrown when the image file is corrupt
	 */
	class InvalidFormat : public std::runtime_error {
		public:
			InvalidFormat(const std::string& msg)
				: std::runtime_error(msg)	
			{
				// empty
			}
	};

	// throw this if you meet an unexpected end of file
	class EndOfFile : public std::runtime_error {
		public:
			EndOfFile(const std::string& msg)
				: std::runtime_error(msg)
			{
				// empty
			}
	};

	// throw this if you meed an unknow IO error
	class IOError : public std::runtime_error {
		public:
			IOError(const std::string& msg)
				: std::runtime_error(msg)
			{
				// empty
			}
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_EXCEPTION_H
