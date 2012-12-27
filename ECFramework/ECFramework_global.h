#ifndef ECFRAMEWORK_GLOBAL_H
#define ECFRAMEWORK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ECFRAMEWORK_LIBRARY)
#  define ECFRAMEWORKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ECFRAMEWORKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ECFRAMEWORK_GLOBAL_H
