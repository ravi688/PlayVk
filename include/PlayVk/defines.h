
#pragma once

#include <common/defines.h>
#include <PlayVk/api_defines.h>

#if !defined(PVK_RELEASE) && !defined(PVK_DEBUG)
#	warning "None of PVK_RELEASE && PVK_DEBUG is defined; using PVK_DEBUG"
#	define PVK_DEBUG
#endif
