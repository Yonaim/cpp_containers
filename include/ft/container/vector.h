#ifndef FT_VECTOR_H
#define FT_VECTOR_H

#if defined(FT_VECTOR_IMPL_SIZED)
    #include "vector_sized.h"
#else
    #include "vector_3ptr.h" // default
#endif

#include "bvector.h"

#endif
