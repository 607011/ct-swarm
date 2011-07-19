// Copyright (c) 2005-2011 Oliver Lau <ola@ct.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef __DEFINES_H_
#define __DEFINES_H_

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#define SSE_ALIGNED __declspec(align(16))
#elif defined(__GNUC__)
#define SSE_ALIGNED __attribute__ ((aligned(16)))
#else
#define SSE_ALIGNED
#endif

#endif
