#pragma once

#if (defined _WIN32 || defined __CYGWIN__) && defined(__GNUC__)
#	define PVK_IMPORT_API __declspec(dllimport)
#	define PVK_EXPORT_API __declspec(dllexport)
#else
#	define PVK_IMPORT_API __attribute__((visibility("default")))
#	define PVK_EXPORT_API __attribute__((visibility("default")))
#endif

#ifdef PVK_BUILD_STATIC_LIBRARY
#	define PVK_API
#elif defined(PVK_BUILD_DYNAMIC_LIBRARY)
#	define PVK_API PVK_EXPORT_API
#elif defined(PVK_USE_DYNAMIC_LIBRARY)
#	define PVK_API PVK_IMPORT_API
#elif defined(PVK_USE_STATIC_LIBRARY)
#	define PVK_API
#else
#	define PVK_API
#endif
