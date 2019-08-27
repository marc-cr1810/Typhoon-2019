#ifndef Ty_PORTS_H
#define Ty_PORTS_H

#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>
#include <regex>

#include "Config.h"

#ifdef MS_WINDOWS

typedef int8_t    Ty_int8_t;
typedef int16_t   Ty_int16_t;
typedef int32_t   Ty_int32_t;
typedef int64_t   Ty_int64_t;

typedef uint8_t   Ty_uint8_t;
typedef uint16_t  Ty_uint16_t;
typedef uint32_t  Ty_uint32_t;
typedef uint64_t  Ty_uint64_t;

#else

typedef __int8_t    Ty_int8_t;
typedef __int16_t   Ty_int16_t;
typedef __int32_t   Ty_int32_t;
typedef __int64_t   Ty_int64_t;

typedef __uint8_t   Ty_uint8_t;
typedef __uint16_t  Ty_uint16_t;
typedef __uint32_t  Ty_uint32_t;
typedef __uint64_t  Ty_uint64_t;

#endif

typedef float       Ty_float_t;
typedef double      Ty_double_t;

typedef std::string Ty_string_t;

#endif