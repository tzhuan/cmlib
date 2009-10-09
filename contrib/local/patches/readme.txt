zlib-1.2.3:
  just compile

jpeg-7:
  cp jconfig.vc jconfig.h
  patch jmorecfg.h

libpng-1.2.40:
  cp config.h.in config.h

libtiff-3.9.1:
  cp tif_config.vc.h tif_config.h

ilmbase:
  patch ilmbase-1.0.1.diff

openexr:
  patch openexr-1.6.1.diff
