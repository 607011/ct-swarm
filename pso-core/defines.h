#ifndef _DEFINES_H_
#define _DEFINES_H_

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#define SSE_ALIGNED __declspec(align(16))
#elif defined(__GNUC__)
#define SSE_ALIGNED __attribute__ ((aligned(16)))
#else
#define SSE_ALIGNED
#endif

#endif
