#ifndef COMPRESSOR_GLOBAL_H
#define COMPRESSOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMPRESSOR_LIBRARY)
#  define COMPRESSORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define COMPRESSORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COMPRESSOR_GLOBAL_H