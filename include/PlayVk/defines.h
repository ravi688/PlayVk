
#pragma once

#include <common/defines.h>

#ifdef PLAYVK_STATIC_LIBRARY
#	define PLAYVK_API
#elif PLAYVK_DYNAMIC_LIBRARY
#	define PLAYVK_API __declspec(dllimport)
#elif BUILD_DYNAMIC_LIBRARY
#	define PLAYVK_API __declspec(dllexport)
#else
#	define PLAYVK_API
#endif