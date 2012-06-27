Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)

zlib-1.2.4:
  just compile

jpeg-8a:
  patch with jpeg-8a.diff

libpng-1.5.2:
  cp config.h.in config.h
  cp scripts/pnglibconf.h.prebuilt pnglibconf.h

libtiff-3.9.2:
  cp libtiff/tif_config.vc.h libtiff/tif_config.h
  cp libtiff/tiffconf.vc.h libtiff/tiffconf.h

ilmbase-1.0.1:
  patch with ilmbase-1.0.1.diff

openexr-1.6.1:
  patch with openexr-1.6.1.diff
