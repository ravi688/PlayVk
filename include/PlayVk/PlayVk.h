
#pragma once

#include <PlayVk/defines.h>
#include <PlayVk/assert.h>
#include <PlayVk/debug.h>

/* <begin> Configuration Switches */
#define PVK_STATIC static
#define PVK_LINKAGE extern
#define PVK_INLINE inline
#define PVK_DEBUG
// #define PVK_IMPLEMENTATION
// #define PVK_USE_GLFW
/* <end> Configuration Switches */

#ifdef PVK_USE_WIN32_SURFACE
#	define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan/vulkan.h> 	// for vulkan

#include <stdio.h> 			// printf, puts, fopen, fclose
#include <stdlib.h> 		// malloc
#include <string.h> 		// memset
#include <math.h> 			// sin, cos

#if defined(__cplusplus) && (__cplusplus >= 201103L)
#	define PVK_CONSTEXPR constexpr
#else
#	define PVK_CONSTEXPR const
#endif /* C++11 */

/* Logging functions */
#define PVK_FETAL_ERROR(...) debug_log_fetal_error(__VA_ARGS__)
#define PVK_WARNING(...) debug_log_warning( __VA_ARGS__)
#define PVK_ERROR(...) debug_log_error(__VA_ARGS__)
#define PVK_INFO(...) debug_log_info(__VA_ARGS__)
#define PVK_LOG(description, ...) debug_log(description, __LINE__, __FUNCTION__, __FILE__, __VA_ARGS__)
#define PVK_ASSERT(condition) _assert(condition)

/* Memory functions */
#define PVK_MALLOC(size) malloc(size)
#define PVK_FREE(ptr) free(ptr)
#define PVK_MEMSET(ptr, u8Value, count) memset(ptr, u8Value, count)

#ifdef __cplusplus
extern "C" {
#endif

#define PVK_NEW(type) (type*)__PVK_NEW(sizeof(type))
#define PVK_NEWV(type, count) (type*)__PVK_NEW(sizeof(type) * count)
PVK_LINKAGE void* __PVK_NEW(size_t size);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void* __PVK_NEW(size_t size)
{ 
	void* block = PVK_MALLOC(size); 
	PVK_MEMSET(block, 0, size); 
	return block; 
}
#endif
#define PVK_DELETE(ptr) __PVK_DELETE((char**)&(ptr))
PVK_LINKAGE void __PVK_DELETE(char** ptr);
#ifdef PVK_IMPLEMENTATION 
PVK_LINKAGE void __PVK_DELETE(char** ptr) 
{ 
	if(*ptr == NULL) {  PVK_WARNING("You are trying to free an invalid memory block, ptr = NULL"); return;  }
	PVK_FREE((void*)(*ptr)); 
	*ptr = NULL; 
}
#endif

/* Mathematics */

PVK_STATIC PVK_CONSTEXPR double PVK_PI = 3.1415926;
PVK_STATIC PVK_CONSTEXPR double PVK_INVERSE_PI = 0.3183098;
PVK_STATIC PVK_CONSTEXPR double PVK_INVERSE_180 = 0.0055555;
PVK_STATIC PVK_CONSTEXPR double PVK_RAD2DEG = PVK_INVERSE_PI * 180.0;
PVK_STATIC PVK_CONSTEXPR double PVK_DEG2RAD = PVK_INVERSE_180 * PVK_PI;
#define PVK_DEG * PVK_DEG2RAD
#define PVK_RAD

PVK_STATIC PVK_INLINE PVK_CONSTEXPR void __pvkScaleFloats(uint32_t count, float* const values, const float scalar)
{
	for(int i = 0; i < count; i++)
		values[i] *= scalar;
}

PVK_STATIC PVK_INLINE PVK_CONSTEXPR float __pvkMat3Det(float m[3][3])
{
	return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) 
			- m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) 
			+ m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

typedef union PvkVec2
{
	struct 
	{
		float x, y;
	};
	struct 
	{
		float s, t;
	};
	float v[2];
} PvkVec2;

typedef union PvkVec3
{
	struct
	{
		float x, y, z;
	};
	struct
	{
		float r, g, b;
	};
	float v[3];
	PvkVec2 xy;
} PvkVec3;

PVK_STATIC PVK_INLINE PVK_CONSTEXPR float pvkVec3Magnitude(PvkVec3 v) { return sqrt(v.x*v.x + v.y*v.y + v.z*v.z); }
PVK_STATIC PVK_INLINE PVK_CONSTEXPR PvkVec3 pvkVec3Normalize(PvkVec3 v) { float m = 1 / pvkVec3Magnitude(v); return (PvkVec3){ v.x*m, v.y*m, v.z*m }; }

typedef union PvkVec4
{
	struct
	{
		float x, y, z, w;
	};
	struct
	{
		float r, g, b, a;
	};
	float v[4];
	struct
	{
		PvkVec2 xy;
		PvkVec2 zw;
	};
	PvkVec3 xyz;
} PvkVec4;

PVK_STATIC PVK_INLINE PVK_CONSTEXPR PvkVec4 pvkVec4Zero() { return (PvkVec4) { 0, 0, 0, 0 }; }

typedef union PvkMat4
{
	struct 
	{
		float v[4][4];
	};
	struct
	{
		PvkVec4 r0;
		PvkVec4 r1;
		PvkVec4 r2;
		PvkVec4 r3;
	};
} PvkMat4;

PVK_STATIC PVK_INLINE PVK_CONSTEXPR PvkMat4 pvkMat4Identity()
{
	return (PvkMat4)
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

PVK_STATIC PVK_INLINE PVK_CONSTEXPR PvkMat4 pvkMat4Zero() { return (PvkMat4) { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; }

PVK_LINKAGE PvkMat4 pvkMat4Mul(PvkMat4 m1, PvkMat4 m2);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkMat4 pvkMat4Mul(PvkMat4 m1, PvkMat4 m2)
{
	PvkMat4 m = pvkMat4Zero();
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			for(int k = 0; k < 4; k++)
				m.v[i][j] += m1.v[i][k] * m2.v[k][j];
	return m;
}
#endif

PVK_LINKAGE PvkMat4 pvkMat4Transpose(PvkMat4 m);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkMat4 pvkMat4Transpose(PvkMat4 m)
{
	for(int i = 0; i < 4; i++)
		for(int j = i + 1; j < 4; j++)
		{
			float t = m.v[i][j];
			m.v[i][j] = m.v[j][i];
			m.v[j][i] = t;
		}
	return m;
}
#endif

PVK_LINKAGE PvkMat4 pvkCofactorMatrix(PvkMat4 m);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkMat4 pvkCofactorMatrix(PvkMat4 m)
{
	PvkMat4 result;
	float minor[3][3];
	float sign = 1;
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			for(int p = 0, h = 0; p < 4; p++)
			{
				if(p == i) 
					continue;
				for(int q = 0, k = 0; q < 4; q++)
					if(q == j)
						continue;
					else
						minor[h][k++] = m.v[p][q];
				h++;
			}
			result.v[i][j] = sign * m.v[i][j] * __pvkMat3Det(minor);
			sign *= -1.0f;
		}
	return result;
}
#endif

/* NOTE: This is not working as expected */
PVK_LINKAGE PvkMat4 pvkMat4Adjoint(PvkMat4 m);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkMat4 pvkMat4Adjoint(PvkMat4 m)
{
	return pvkMat4Transpose(pvkCofactorMatrix(m));
}
#endif

PVK_LINKAGE float pvkMat4Determinant(PvkMat4 m);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE float pvkMat4Determinant(PvkMat4 m)
{
	float minor[3][3];
	float det = 0;
	float sign = 1;
	for(int i = 0; i < 4; i++)
	{
		for(int p = 1, h = 0; p < 4; p++, h++)
		{
			for(int q = 0, k = 0; q < 4; q++)
				if(q == i)
					continue;
				else
					minor[h][k++] = m.v[p][q];
		}
		det += sign * m.v[0][i] * __pvkMat3Det(minor);
		sign *= -1;
	}
	return det;
}
#endif

// static PvkMat4 pvkMat4Inverse(PvkMat4 m)
// {
// 	PvkMat4 result = pvkMat4Adjoint(m);
// 	float det = pvkMat4Determinant(m);
// 	__pvkScaleFloats(16, (float*)&result, det);
// 	return result;
// }

PVK_LINKAGE PvkMat4 pvkMat4Inverse(PvkMat4 m);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkMat4 pvkMat4Inverse(PvkMat4 m)
{
	float inverse_det = 1 / pvkMat4Determinant(m);
	PvkMat4 _m;
	_m.v[0][0] = m.v[1][1] * (m.v[2][2] * m.v[3][3] - m.v[2][3] * m.v[3][2]) - m.v[1][2] * (m.v[2][1] * m.v[3][3] - m.v[2][3] * m.v[3][1]) + m.v[1][3] * (m.v[2][1] * m.v[3][2] - m.v[2][2] * m.v[3][1]);
	_m.v[1][0] = m.v[1][0] * (m.v[2][2] * m.v[3][3] - m.v[2][3] * m.v[3][2]) - m.v[1][2] * (m.v[2][0] * m.v[3][3] - m.v[2][3] * m.v[3][0]) + m.v[1][3] * (m.v[2][0] * m.v[3][2] - m.v[2][2] * m.v[3][0]);
	_m.v[2][0] = m.v[1][0] * (m.v[2][1] * m.v[3][3] - m.v[2][3] * m.v[3][1]) - m.v[1][1] * (m.v[2][0] * m.v[3][3] - m.v[2][3] * m.v[3][0]) + m.v[1][3] * (m.v[2][0] * m.v[3][1] - m.v[2][1] * m.v[3][0]);
	_m.v[3][0] = m.v[1][0] * (m.v[2][1] * m.v[3][2] - m.v[2][2] * m.v[3][1]) - m.v[1][1] * (m.v[2][0] * m.v[3][2] - m.v[2][2] * m.v[3][0]) + m.v[1][2] * (m.v[2][0] * m.v[3][1] - m.v[2][1] * m.v[3][0]);
 	_m.v[0][1] = m.v[0][1] * (m.v[2][2] * m.v[3][3] - m.v[2][3] * m.v[3][2]) - m.v[0][2] * (m.v[2][1] * m.v[3][3] - m.v[2][3] * m.v[3][1]) + m.v[0][3] * (m.v[2][1] * m.v[3][2] - m.v[2][2] * m.v[3][1]);
 	_m.v[1][1] = m.v[0][0] * (m.v[2][2] * m.v[3][3] - m.v[2][3] * m.v[3][2]) - m.v[0][2] * (m.v[2][0] * m.v[3][3] - m.v[2][3] * m.v[3][0]) + m.v[0][3] * (m.v[2][0] * m.v[3][2] - m.v[2][2] * m.v[3][0]);
 	_m.v[2][1] = m.v[0][0] * (m.v[2][1] * m.v[3][3] - m.v[2][3] * m.v[3][1]) - m.v[0][1] * (m.v[2][0] * m.v[3][3] - m.v[2][3] * m.v[3][0]) + m.v[0][3] * (m.v[2][0] * m.v[3][1] - m.v[2][1] * m.v[3][0]);
 	_m.v[3][1] = m.v[0][0] * (m.v[2][1] * m.v[3][2] - m.v[2][2] * m.v[3][1]) - m.v[0][1] * (m.v[2][0] * m.v[3][2] - m.v[2][2] * m.v[3][0]) + m.v[0][2] * (m.v[2][0] * m.v[3][1] - m.v[2][1] * m.v[3][0]);
	_m.v[0][2] = m.v[0][1] * (m.v[1][2] * m.v[3][3] - m.v[1][3] * m.v[3][2]) - m.v[0][2] * (m.v[1][1] * m.v[3][3] - m.v[1][3] * m.v[3][1]) + m.v[0][3] * (m.v[1][1] * m.v[3][2] - m.v[1][2] * m.v[3][1]);
	_m.v[1][2] = m.v[0][0] * (m.v[1][2] * m.v[3][3] - m.v[1][3] * m.v[3][2]) - m.v[0][2] * (m.v[1][0] * m.v[3][3] - m.v[1][3] * m.v[3][0]) + m.v[0][3] * (m.v[1][0] * m.v[3][2] - m.v[1][2] * m.v[3][0]);
	_m.v[2][2] = m.v[0][0] * (m.v[1][1] * m.v[3][3] - m.v[1][3] * m.v[3][1]) - m.v[0][1] * (m.v[1][0] * m.v[3][3] - m.v[1][3] * m.v[3][0]) + m.v[0][3] * (m.v[1][0] * m.v[3][1] - m.v[1][1] * m.v[3][0]);
	_m.v[3][2] = m.v[0][0] * (m.v[1][1] * m.v[3][2] - m.v[1][2] * m.v[3][1]) - m.v[0][1] * (m.v[1][0] * m.v[3][2] - m.v[1][2] * m.v[3][0]) + m.v[0][2] * (m.v[1][0] * m.v[3][1] - m.v[1][1] * m.v[3][0]);
	_m.v[0][3] = m.v[0][1] * (m.v[1][2] * m.v[2][3] - m.v[1][3] * m.v[2][2]) - m.v[0][2] * (m.v[1][1] * m.v[2][3] - m.v[1][3] * m.v[2][1]) + m.v[0][3] * (m.v[1][1] * m.v[2][2] - m.v[1][2] * m.v[2][1]);
	_m.v[1][3] = m.v[0][0] * (m.v[1][2] * m.v[2][3] - m.v[1][3] * m.v[2][2]) - m.v[0][2] * (m.v[1][0] * m.v[2][3] - m.v[1][3] * m.v[2][0]) + m.v[0][3] * (m.v[1][0] * m.v[2][2] - m.v[1][2] * m.v[2][0]);
	_m.v[2][3] = m.v[0][0] * (m.v[1][1] * m.v[2][3] - m.v[1][3] * m.v[2][1]) - m.v[0][1] * (m.v[1][0] * m.v[2][3] - m.v[1][3] * m.v[2][0]) + m.v[0][3] * (m.v[1][0] * m.v[2][1] - m.v[1][1] * m.v[2][0]);
	_m.v[3][3] = m.v[0][0] * (m.v[1][1] * m.v[2][2] - m.v[1][2] * m.v[2][1]) - m.v[0][1] * (m.v[1][0] * m.v[2][2] - m.v[1][2] * m.v[2][0]) + m.v[0][2] * (m.v[1][0] * m.v[2][1] - m.v[1][1] * m.v[2][0]);
	_m.v[0][0] *= inverse_det;
	_m.v[0][1] *= -inverse_det;
	_m.v[0][2] *= inverse_det;
	_m.v[0][3] *= -inverse_det;
	_m.v[1][0] *= -inverse_det;
	_m.v[1][1] *= inverse_det;
	_m.v[1][2] *= -inverse_det;
	_m.v[1][3] *= inverse_det;
	_m.v[2][0] *= inverse_det;
	_m.v[2][1] *= -inverse_det;
	_m.v[2][2] *= inverse_det;
	_m.v[2][3] *= -inverse_det;
	_m.v[3][0] *= -inverse_det;
	_m.v[3][1] *= inverse_det;
	_m.v[3][2] *= -inverse_det;
	_m.v[3][3] *= inverse_det;
	return _m;
}
#endif

/* Affine transformation */

PVK_LINKAGE PvkVec4 pvkMat4MulVec4(PvkMat4 m, PvkVec4 v);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkVec4 pvkMat4MulVec4(PvkMat4 m, PvkVec4 v)
{
	PvkVec4 fv = pvkVec4Zero();
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			fv.v[i] += m.v[i][j] * v.v[j];
	return fv;
}
#endif

PVK_STATIC PVK_INLINE PVK_CONSTEXPR PvkMat4 pvkMat4Translate(PvkVec3 displacement)
{
	return (PvkMat4)
	{
		1, 0, 0, displacement.x,
		0, 1, 0, displacement.y,
		0, 0, 1, displacement.z,
		0, 0, 0, 			  1,
	};
}

PVK_LINKAGE PvkMat4 pvkMat4Rotate(PvkVec3 v);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkMat4 pvkMat4Rotate(PvkVec3 v)
{
	float xc0 = cos(v.x), xs0 = sin(v.x);
	float yc0 = cos(v.y), ys0 = sin(v.y);
	float zc0 = cos(v.z), zs0 = sin(v.z);
	return pvkMat4Mul((PvkMat4) 
	{
		zc0, -zs0, 0, 0, 			// rotation around z axis
		zs0,  zc0, 0, 0,
		0,      0, 1, 0,
		0,      0, 0, 1
	}, pvkMat4Mul((PvkMat4) 
	{
		yc0,  0, ys0, 0, 			// rotation around y axis
		0,    1,   0, 0,
		-ys0, 0, yc0, 0,
		0,    0,   0, 1
	}, (PvkMat4)
	{
		1,   0,    0, 0,			// rotation around x axis
		0, xc0, -xs0, 0,
		0, xs0,  xc0, 0,
		0,   0,    0, 1
	}));
}
#endif

PVK_STATIC PVK_INLINE PVK_CONSTEXPR PvkMat4 pvkMat4Scale(PvkVec3 v)
{
	return (PvkMat4)
	{
		v.x, 0, 0, 0,
		0, v.y, 0, 0,
		0, 0, v.z, 0,
		0, 0,   0, 1
	};
}

PVK_LINKAGE PvkMat4 pvkMat4Transform(PvkVec3 position, PvkVec3 rotation);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkMat4 pvkMat4Transform(PvkVec3 position, PvkVec3 rotation)
{
	return pvkMat4Mul(pvkMat4Translate(position), pvkMat4Rotate(rotation));
}
#endif

/* Resulting checking */
#define PVK_CHECK(result) pvkCheckResult(result, __LINE__, __FUNCTION__, __FILE__)

PVK_LINKAGE void pvkCheckResult(VkResult result, uint32_t line_no, const char* function_name, const char* source_name);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkCheckResult(VkResult result, uint32_t line_no, const char* function_name, const char* source_name)
{
	if(result != VK_SUCCESS)
		PVK_FETAL_ERROR("Result != VK_SUCCESS, Result = %lld", result);
}
#endif

#ifdef PVK_USE_GLFW
/* Windowing system */
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct PvkWindow
{
	void* handle;
	uint32_t width;
	uint32_t height;
} PvkWindow;


#ifdef PVK_DEBUG
PVK_LINKAGE void glfwErrorCallback(int code, const char* description);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void glfwErrorCallback(int code, const char* description)
{
	PVK_ERROR("GLFW: %d, %s", code, description);
}
#endif /* PVK_IMPLEMENTATION */
#endif /* PVK_DEBUG */

PVK_LINKAGE void windowResizeCallbackHandler(GLFWwindow* window, int width, int height);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void windowResizeCallbackHandler(GLFWwindow* window, int width, int height)
{
	PvkWindow* pvkWindow = (PvkWindow*)glfwGetWindowUserPointer(window);
	pvkWindow->width = width;
	pvkWindow->height = height;
}
#endif

PVK_LINKAGE PvkWindow* pvkWindowCreate(uint32_t width, uint32_t height, const char* title, bool full_screen, bool resizable);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkWindow* pvkWindowCreate(uint32_t width, uint32_t height, const char* title, bool full_screen, bool resizable)
{
	PvkWindow* window = PVK_NEW(PvkWindow);
	glfwInit();
#ifdef PVK_DEBUG
	glfwSetErrorCallback(glfwErrorCallback);
#endif
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
	window->handle = glfwCreateWindow(width, height, title, full_screen ? glfwGetPrimaryMonitor() : NULL, NULL);
	window->width = width;
	window->height = height;
	glfwSetWindowUserPointer((GLFWwindow*)(window->handle), (void*)window);
	glfwSetFramebufferSizeCallback((GLFWwindow*)(window->handle), windowResizeCallbackHandler);
	return window;
}
#endif

PVK_STATIC PVK_INLINE bool pvkWindowShouldClose(PvkWindow* window)
{
	return glfwWindowShouldClose((GLFWwindow*)window->handle);
}

PVK_STATIC PVK_INLINE void pvkWindowPollEvents(PvkWindow* window)
{
	glfwPollEvents();
}

PVK_LINKAGE void pvkWindowDestroy(PvkWindow* window);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkWindowDestroy(PvkWindow* window)
{
	glfwDestroyWindow((GLFWwindow*)window->handle);
	glfwTerminate();
	PVK_DELETE(window);
}
#endif

PVK_STATIC PVK_INLINE void pvkWindowGetFramebufferExtent(PvkWindow* window, int* out_width, int* out_height)
{
	glfwGetFramebufferSize((GLFWwindow*)window->handle, out_width, out_height);
}

PVK_LINKAGE VkSurfaceKHR pvkWindowCreateVulkanSurface(PvkWindow* window, VkInstance instance);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkSurfaceKHR pvkWindowCreateVulkanSurface(PvkWindow* window, VkInstance instance)
{
	VkSurfaceKHR surface;
	PVK_CHECK(glfwCreateWindowSurface(instance, (GLFWwindow*)window->handle, NULL, &surface));
	return surface;
}
#endif
#endif  /* ifdef PVK_USE_GLFW */

/* Vulkan */

typedef struct PvkEnabledLayerAndExtensionInfo
{
	uint32_t layerCount;
	const char* const* layerNames;
	uint32_t extensionCount;
	const char* const* extensionNames;
} PvkEnabledLayerAndExtensionInfo;

PVK_LINKAGE void __pvkCheckForInstanceExtensionSupport(uint32_t count, const char* const* extensions);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void __pvkCheckForInstanceExtensionSupport(uint32_t count, const char* const* extensions)
{
	uint32_t supportedCount;
	PVK_CHECK(vkEnumerateInstanceExtensionProperties(NULL, &supportedCount, NULL));
	VkExtensionProperties* supportedExtensions = PVK_NEWV(VkExtensionProperties, supportedCount);
	PVK_CHECK(vkEnumerateInstanceExtensionProperties(NULL, &supportedCount, supportedExtensions));

	for(uint32_t i = 0; i < count; i++)
	{
		bool found = false;
		for(uint32_t j = 0; j < supportedCount; j++)
		{
			if(strcmp(extensions[i], supportedExtensions[j].extensionName) == 0)
			{
				found = true;
				break;
			}
		}
		if(!found)
			PVK_FETAL_ERROR("Extension \"%s\" isn't supported by the vulkan implementation", extensions[i]);
	}
	PVK_DELETE(supportedExtensions);
}
#endif

PVK_LINKAGE VkInstance __pvkCreateVulkanInstance(const char* appName, uint32_t appVersion, uint32_t apiVersion, PvkEnabledLayerAndExtensionInfo* enabledInfo);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkInstance __pvkCreateVulkanInstance(const char* appName, uint32_t appVersion, uint32_t apiVersion, PvkEnabledLayerAndExtensionInfo* enabledInfo)
{
	VkInstance instance;
	VkApplicationInfo appInfo = { };
	{
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = appName;
		appInfo.applicationVersion = appVersion;
		appInfo.apiVersion = appVersion;
	};

	if(enabledInfo->extensionCount != 0)
		__pvkCheckForInstanceExtensionSupport(enabledInfo->extensionCount, enabledInfo->extensionNames);
	
	VkInstanceCreateInfo icInfo = { };
	{
		icInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		icInfo.pApplicationInfo = &appInfo;
		icInfo.enabledLayerCount = enabledInfo->layerCount;
		icInfo.ppEnabledLayerNames = enabledInfo->layerNames;
		icInfo.enabledExtensionCount = enabledInfo->extensionCount;
		icInfo.ppEnabledExtensionNames = enabledInfo->extensionNames;
	};
	PVK_CHECK(vkCreateInstance(&icInfo, NULL, &instance));
	return instance;
}
#endif

PVK_LINKAGE VkInstance pvkCreateVulkanInstanceWithExtensions(uint32_t count, ...);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkInstance pvkCreateVulkanInstanceWithExtensions(uint32_t count, ...)
{
	const char* extensions[count];
	uint32_t extensionCount = count;

	va_list args;
	va_start(args, count);

	while(count > 0)
	{
		--count;
		extensions[count] = (char*)va_arg(args, const char*);
	}

	va_end(args);

	PvkEnabledLayerAndExtensionInfo enabledInfo = { };
	{
		enabledInfo.extensionCount = extensionCount;
		enabledInfo.extensionNames = extensions;
	};
	return __pvkCreateVulkanInstance("Default Vulkan App", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0, &enabledInfo);
}
#endif

typedef enum PvkShaderType
{
	PVK_SHADER_TYPE_VERTEX = 1UL << 0,
	PVK_SHADER_TYPE_TESSELLATION = 1UL << 1,
	PVK_SHADER_TYPE_GEOMETRY = 1UL << 2,
	PVK_SHADER_TYPE_FRAGMENT = 1UL << 3
} PvkShaderType;

typedef PvkShaderType PvkShaderFlags;

typedef struct PvkPhysicalDeviceRequirements
{
	VkPhysicalDeviceType type;		// device type
	VkFormat format; 				// surface format
	VkColorSpaceKHR colorSpace; 	// color space
	VkPresentModeKHR presentMode; 	// present mode
	PvkShaderFlags shaders; 		// required shaders
	uint32_t imageCount; 			// image count in the swapchain
	uint32_t imageWidth;			// image width in the swapchain
	uint32_t imageHeight;			// image height in the swapchain
	bool samplerYcbcrConversion; 	// samplerYcbcrConversion feature enable
} PvkPhysicalDeviceRequirements;


PVK_LINKAGE bool __pvkIsPresentModeSupported(VkPhysicalDevice device, VkSurfaceKHR surface, VkPresentModeKHR mode);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE bool __pvkIsPresentModeSupported(VkPhysicalDevice device, VkSurfaceKHR surface, VkPresentModeKHR mode)
{
	uint32_t presentModeCount;
	PVK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, NULL));
	VkPresentModeKHR* presentModes = PVK_NEWV(VkPresentModeKHR, presentModeCount);
	PVK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, presentModes));

	bool isPresentModeSupported = false;
	for(int i = 0; i < presentModeCount; i++)
		if(presentModes[i] == mode)
		{
			isPresentModeSupported = true;
			break;
		}
	PVK_DELETE(presentModes);
	if(!isPresentModeSupported)
		PVK_WARNING("Requested present mode is not supported");
	return isPresentModeSupported;
}
#endif

PVK_LINKAGE bool __pvkIsSurfaceFormatSupported(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceFormatKHR surfaceFormat);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE bool __pvkIsSurfaceFormatSupported(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceFormatKHR surfaceFormat)
{
	uint32_t formatCount;
	PVK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, NULL));
	VkSurfaceFormatKHR* formats = PVK_NEWV(VkSurfaceFormatKHR, formatCount);
	PVK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats));

	bool isFormatSupported = false;
	for(int i = 0; i < formatCount; i++)
		if(memcmp(&formats[i], &surfaceFormat, sizeof(VkSurfaceFormatKHR)) == 0)
		{
			isFormatSupported = true;
			break;
		}
	PVK_DELETE(formats);
	if(!isFormatSupported)
		PVK_WARNING("Requested surface format is not supported");
	return isFormatSupported;
}
#endif

PVK_LINKAGE bool __pvkIsDeviceTypeSupported(VkPhysicalDevice device, VkPhysicalDeviceType type);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE bool __pvkIsDeviceTypeSupported(VkPhysicalDevice device, VkPhysicalDeviceType type)
{
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(device, &properties);
	PVK_INFO("Found Device: %s", properties.deviceName);
	return properties.deviceType == type;
}
#endif

PVK_LINKAGE bool __pvkIsDeviceFeaturesSupported(VkPhysicalDevice device, VkPhysicalDeviceFeatures2* requiredFeatures2);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE bool __pvkIsDeviceFeaturesSupported(VkPhysicalDevice device, VkPhysicalDeviceFeatures2* requiredFeatures2)
{
	VkPhysicalDeviceSamplerYcbcrConversionFeatures conversionFeatures = { };
	conversionFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES;
	VkPhysicalDeviceFeatures2 features2 = { };
	features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	if(requiredFeatures2->pNext != NULL)
		features2.pNext = &conversionFeatures;
	vkGetPhysicalDeviceFeatures2(device, &features2);
	VkPhysicalDeviceFeatures* requiredFeatures = &requiredFeatures2->features;
	VkPhysicalDeviceFeatures* features = &features2.features;

	bool isSupported = true;
	isSupported &= requiredFeatures->geometryShader? (features->geometryShader && requiredFeatures->geometryShader) : true;
	isSupported &= requiredFeatures->tessellationShader? (features->tessellationShader && requiredFeatures->tessellationShader) : true;
	isSupported &= requiredFeatures->samplerAnisotropy? (features->samplerAnisotropy == requiredFeatures->samplerAnisotropy) : true;
	isSupported &= requiredFeatures->textureCompressionETC2? (features->textureCompressionETC2 == requiredFeatures->textureCompressionETC2) : true;
	isSupported	&= (requiredFeatures2->pNext != NULL) ? (reinterpret_cast<VkPhysicalDeviceSamplerYcbcrConversionFeatures*>(requiredFeatures2->pNext)->samplerYcbcrConversion == conversionFeatures.samplerYcbcrConversion) : true;
	if(!isSupported)
		PVK_WARNING("Requested GPU features are not supported");
	return isSupported;
}
#endif

PVK_LINKAGE bool __pvkIsPhysicalDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, PvkPhysicalDeviceRequirements* requirements);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE bool __pvkIsPhysicalDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, PvkPhysicalDeviceRequirements* requirements)
{

	VkSurfaceCapabilitiesKHR capabilities;
	PVK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities));

	bool isImageSupported = true;
	isImageSupported &= (requirements->imageCount >= capabilities.minImageCount) && (requirements->imageCount <= capabilities.maxImageCount);
	// isImageSupported &= (requirements->imageWidth >= capabilities.minImageExtent.width) && (requirements->imageWidth <= capabilities.maxImageExtent.width);
	// isImageSupported &= (requirements->imageHeight >= capabilities.minImageExtent.height) && (requirements->imageHeight <= capabilities.maxImageExtent.height);

	VkPhysicalDeviceSamplerYcbcrConversionFeatures conversionFeatures = { };
	conversionFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES;
	conversionFeatures.samplerYcbcrConversion = requirements->samplerYcbcrConversion ? VK_TRUE : VK_FALSE;
	VkPhysicalDeviceFeatures2 requiredFeatures2 = { };
	requiredFeatures2.pNext = requirements->samplerYcbcrConversion ? &conversionFeatures : NULL;
	requiredFeatures2.features.geometryShader = (requirements->shaders & PVK_SHADER_TYPE_GEOMETRY) ? VK_TRUE : VK_FALSE;
	requiredFeatures2.features.tessellationShader = (requirements->shaders & PVK_SHADER_TYPE_TESSELLATION) ? VK_TRUE : VK_FALSE;


	return isImageSupported 
			&& __pvkIsDeviceTypeSupported(device, requirements->type)
			&& __pvkIsPresentModeSupported(device, surface, requirements->presentMode)
			&& __pvkIsSurfaceFormatSupported(device, surface, (VkSurfaceFormatKHR) { requirements->format, requirements->colorSpace })
			&& __pvkIsDeviceFeaturesSupported(device, &requiredFeatures2);
}
#endif

PVK_LINKAGE VkPhysicalDevice pvkGetPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, 
											VkPhysicalDeviceType gpuType, 
											VkFormat format, 
											VkColorSpaceKHR colorSpace, 
											VkPresentModeKHR presentMode,
											uint32_t imageCount,
											bool samplerYcbcrConversion);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkPhysicalDevice pvkGetPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, 
											VkPhysicalDeviceType gpuType, 
											VkFormat format, 
											VkColorSpaceKHR colorSpace, 
											VkPresentModeKHR presentMode,
											uint32_t imageCount,
											bool samplerYcbcrConversion)
{
	uint32_t deviceCount = 0;
	VkResult result = vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
	if(result == VK_INCOMPLETE)
		PVK_FETAL_ERROR("No Vulkan Compatible Device is found. Exiting...");
	else PVK_CHECK(result);
	VkPhysicalDevice* devices = PVK_NEWV(VkPhysicalDevice, deviceCount);
	PVK_CHECK(vkEnumeratePhysicalDevices(instance, &deviceCount, devices));

	VkPhysicalDevice device = VK_NULL_HANDLE, fallbackDevice = VK_NULL_HANDLE;

	char selectedGPUName[VK_MAX_PHYSICAL_DEVICE_NAME_SIZE];
	for(int i = 0; i < deviceCount; i++)
	{
		PvkPhysicalDeviceRequirements requirements = { };
		{
			requirements.type = gpuType;
			requirements.format = format;
			requirements.colorSpace = colorSpace;
			requirements.presentMode = presentMode;
			requirements.shaders = (PvkShaderFlags)(PVK_SHADER_TYPE_GEOMETRY | PVK_SHADER_TYPE_TESSELLATION);
			requirements.imageCount = imageCount;
			requirements.samplerYcbcrConversion = samplerYcbcrConversion;
			// .imageWidth = 800,
			// .imageHeight = 800
		};
		if(__pvkIsPhysicalDeviceSuitable(devices[i], surface, &requirements))
		{
			device = devices[i];
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(devices[i], &properties);
			strcpy(selectedGPUName, properties.deviceName);
			break;
		}
		else
			fallbackDevice = devices[i];
	}

	if(device == VK_NULL_HANDLE)
	{
		PVK_WARNING("Unable to find the requested physical device");
		device = fallbackDevice;
	}
	
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(device, &properties);
	strcpy(selectedGPUName, properties.deviceName);
	PVK_INFO("Selected physical device: %s", selectedGPUName);

	PVK_DELETE(devices);
	return device;
}
#endif

PVK_LINKAGE uint32_t pvkFindQueueFamilyIndex(VkPhysicalDevice device, VkQueueFlags queueFlags);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE uint32_t pvkFindQueueFamilyIndex(VkPhysicalDevice device, VkQueueFlags queueFlags)
{
	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, NULL);
	VkQueueFamilyProperties* properties = PVK_NEWV(VkQueueFamilyProperties, count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, properties);

	uint32_t index = UINT32_MAX;
	for(int i = 0; i < count; i++)
		if((properties[i].queueFlags & queueFlags) == queueFlags)
		{
			index = i;
			break;
		}
	PVK_DELETE(properties);

	if(index == UINT32_MAX)
		PVK_WARNING("Unable to find Queue Family with the requested QueueFlags");
	return index;
}
#endif

PVK_LINKAGE uint32_t pvkFindQueueFamilyIndexWithPresentSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE uint32_t pvkFindQueueFamilyIndexWithPresentSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, NULL);

	for(int i = 0; i < count; i++)
	{
		VkBool32 supported;
		PVK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supported));
		if(supported)
			return i;
	}
	return UINT32_MAX;
}
#endif

PVK_LINKAGE void __pvkUnionUInt32(uint32_t count, const uint32_t* values, uint32_t* out_count, uint32_t* out_values);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void __pvkUnionUInt32(uint32_t count, const uint32_t* values, uint32_t* out_count, uint32_t* out_values)
{
	uint32_t uniqueCount = 0;

	// calculate the max value
	uint32_t maxValue = values[0];
	for(int i = 1; i < count; i++)
		if(values[i] > maxValue)
			maxValue = values[i];

	// look up table
	PVK_ASSERT(sizeof(bool) == 1);
	bool exists[maxValue + 1]; 
	PVK_MEMSET(exists, false, maxValue + 1);

	for(int i = 0; i < count; i++)
	{
		if(!exists[values[i]])
		{
			out_values[uniqueCount] = values[i];
			uniqueCount++;
			exists[values[i]] = true;
		}
	}
	*out_count = uniqueCount;
}
#endif

PVK_LINKAGE VkDevice pvkCreateLogicalDeviceWithExtensions(VkInstance instance, VkPhysicalDevice physicalDevice, 
													uint32_t queueFamilyCount, uint32_t* queueFamilyIndices, bool samplerYcbcrConversion,
													uint32_t extensionCount, ...);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkDevice pvkCreateLogicalDeviceWithExtensions(VkInstance instance, VkPhysicalDevice physicalDevice, 
													uint32_t queueFamilyCount, uint32_t* queueFamilyIndices, bool samplerYcbcrConversion,
													uint32_t extensionCount, ...)
{
	// union operation
	uint32_t uniqueQueueFamilyCount;
	uint32_t uniqueQueueFamilyIndices[queueFamilyCount];
	__pvkUnionUInt32(queueFamilyCount, queueFamilyIndices, &uniqueQueueFamilyCount, uniqueQueueFamilyIndices);

	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo* queueCreateInfos = PVK_NEWV(VkDeviceQueueCreateInfo, uniqueQueueFamilyCount);
	
	for(int i = 0; i < uniqueQueueFamilyCount; i++)
	{
		queueCreateInfos[i] = (VkDeviceQueueCreateInfo)
		{
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = uniqueQueueFamilyIndices[i],
			.queueCount = 1,
			.pQueuePriorities = &queuePriority
		};
	}

	// create extensions array from the variable arguments list
	const char* extensions[extensionCount];
	va_list args;
	va_start(args, extensionCount);
	for(int i = 0; i < extensionCount; i++)
		extensions[i] = va_arg(args, const char*);
	va_end(args);

	// check for device extension support
	uint32_t supportedExtensionCount;
	PVK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &supportedExtensionCount, NULL));
	VkExtensionProperties* supportedExtensions = PVK_NEWV(VkExtensionProperties, supportedExtensionCount);
	PVK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &supportedExtensionCount, supportedExtensions));

	for(uint32_t i = 0; i < extensionCount; i++)
	{
		bool found = false;
		for(uint32_t j = 0; j < supportedExtensionCount; j++)
		{
			if(strcmp(extensions[i], supportedExtensions[j].extensionName) == 0)
			{
				found = true;
				break;
			}
		}
		if(!found)
			PVK_FETAL_ERROR("Extension \"%s\" isn't supported by the vulkan physical device", extensions[i]);
	}
	PVK_DELETE(supportedExtensions);

	// TODO: make features configurable
	VkPhysicalDeviceFeatures features = { };
	VkPhysicalDeviceSamplerYcbcrConversionFeatures samplerYcbcrConversionFeatures = { };
	samplerYcbcrConversionFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES;
	samplerYcbcrConversionFeatures.samplerYcbcrConversion = VK_TRUE;

	VkDeviceCreateInfo dcInfo = { };
	{
		dcInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		dcInfo.pNext = &samplerYcbcrConversionFeatures;
		dcInfo.queueCreateInfoCount = uniqueQueueFamilyCount;
		dcInfo.pQueueCreateInfos = queueCreateInfos;
		dcInfo.enabledExtensionCount = extensionCount;
		dcInfo.ppEnabledExtensionNames = extensions;
		dcInfo.pEnabledFeatures = &features;
	};
	VkDevice device;
	PVK_CHECK(vkCreateDevice(physicalDevice, &dcInfo, NULL, &device));
	PVK_DELETE(queueCreateInfos);
	return device;
}
#endif

#ifdef PVK_USE_WIN32_SURFACE
PVK_LINKAGE VkSurfaceKHR pvkCreateSurface(VkInstance vkInstance, HINSTANCE instance, HWND handle);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkSurfaceKHR pvkCreateSurface(VkInstance vkInstance, HINSTANCE instance, HWND handle)
{
	VkWin32SurfaceCreateInfoKHR createInfo = { };
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	createInfo.hinstance = instance;
	createInfo.hwnd = handle;
	VkSurfaceKHR surface;
    PVK_CHECK(vkCreateWin32SurfaceKHR(vkInstance, &createInfo, NULL, &surface));
    return surface;
}
#endif
#endif /* if PVK_USE_WIN32_SURFACE */

PVK_LINKAGE VkSwapchainKHR pvkCreateSwapchain(VkDevice device, VkSurfaceKHR surface, uint32_t minImageCount,
											uint32_t width, uint32_t height, 
											VkFormat format, VkColorSpaceKHR colorSpace, VkPresentModeKHR presentMode,
											uint32_t queueFamilyCount, uint32_t* queueFamilyIndices, VkSwapchainKHR oldSwapchain);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkSwapchainKHR pvkCreateSwapchain(VkDevice device, VkSurfaceKHR surface, uint32_t minImageCount,
											uint32_t width, uint32_t height, 
											VkFormat format, VkColorSpaceKHR colorSpace, VkPresentModeKHR presentMode,
											uint32_t queueFamilyCount, uint32_t* queueFamilyIndices, VkSwapchainKHR oldSwapchain)
{
	// union operation
	uint32_t uniqueQueueFamilyCount;
	uint32_t uniqueQueueFamilyIndices[queueFamilyCount];
	__pvkUnionUInt32(queueFamilyCount, queueFamilyIndices, &uniqueQueueFamilyCount, uniqueQueueFamilyIndices);

	VkSwapchainCreateInfoKHR scInfo = { };
	{
		scInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		scInfo.surface = surface;
		scInfo.minImageCount = minImageCount;
		scInfo.imageFormat = format;
		scInfo.imageColorSpace = colorSpace;
		scInfo.imageExtent = (VkExtent2D) { width, height };
		scInfo.imageArrayLayers = 1;
		scInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		scInfo.imageSharingMode = (uniqueQueueFamilyCount > 1) ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;
		scInfo.queueFamilyIndexCount = uniqueQueueFamilyCount;
		scInfo.pQueueFamilyIndices = uniqueQueueFamilyIndices;
		scInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		scInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		scInfo.presentMode = presentMode;
		scInfo.clipped = VK_TRUE;
		scInfo.oldSwapchain = oldSwapchain;
	};

	VkSwapchainKHR swapchain;
	PVK_CHECK(vkCreateSwapchainKHR(device, &scInfo, NULL, &swapchain));
	return swapchain;
}
#endif

PVK_LINKAGE VkCommandPool pvkCreateCommandPool(VkDevice device, VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkCommandPool pvkCreateCommandPool(VkDevice device, VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex)
{
	VkCommandPoolCreateInfo cInfo = { };
	{
		cInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cInfo.flags = flags;
		cInfo.queueFamilyIndex = queueFamilyIndex;
	};
	VkCommandPool commandPool;
	PVK_CHECK(vkCreateCommandPool(device, &cInfo, NULL, &commandPool));
	return commandPool;
}
#endif

PVK_LINKAGE VkCommandBuffer* __pvkAllocateCommandBuffers(VkDevice device, VkCommandPool pool, VkCommandBufferLevel level, uint32_t count);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkCommandBuffer* __pvkAllocateCommandBuffers(VkDevice device, VkCommandPool pool, VkCommandBufferLevel level, uint32_t count)
{
	VkCommandBufferAllocateInfo info = { };
	{
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = pool;
		info.level = level;
		info.commandBufferCount = count;
	};

	VkCommandBuffer* commandBuffers = PVK_NEWV(VkCommandBuffer, count);
	PVK_CHECK(vkAllocateCommandBuffers(device, &info, commandBuffers));
	return commandBuffers;
}
#endif

PVK_STATIC PVK_INLINE VkCommandBuffer* pvkAllocateCommandBuffers(VkDevice device, VkCommandPool pool, VkCommandBufferLevel level)
{
	return __pvkAllocateCommandBuffers(device, pool, level, 3);
}

PVK_LINKAGE VkSemaphore pvkCreateSemaphore(VkDevice device);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkSemaphore pvkCreateSemaphore(VkDevice device)
{
	VkSemaphoreCreateInfo cInfo = { };
	{ cInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO; };
	VkSemaphore semaphore;
	PVK_CHECK(vkCreateSemaphore(device, &cInfo, NULL, &semaphore));
	return semaphore;
}
#endif

PVK_LINKAGE VkFence pvkCreateFence(VkDevice device, VkFenceCreateFlags flags);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkFence pvkCreateFence(VkDevice device, VkFenceCreateFlags flags)
{
	VkFenceCreateInfo cInfo = { };
	{ cInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO; cInfo.flags = flags; };
	VkFence fence;
	PVK_CHECK(vkCreateFence(device, &cInfo, NULL, &fence));
	return fence;
}
#endif

typedef struct PvkSemaphoreCircularPool
{
	VkSemaphore* semaphores;
	uint32_t reserveCount;
	uint32_t acquiredIndex;
} PvkSemaphoreCircularPool;

PVK_LINKAGE PvkSemaphoreCircularPool* pvkCreateSemaphoreCircularPool(VkDevice device, uint32_t reserveCount);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkSemaphoreCircularPool* pvkCreateSemaphoreCircularPool(VkDevice device, uint32_t reserveCount)
{
	PvkSemaphoreCircularPool* pool = (PvkSemaphoreCircularPool*)PVK_MALLOC(sizeof(PvkSemaphoreCircularPool));
	pool->semaphores = (VkSemaphore*)PVK_MALLOC(sizeof(VkSemaphore) * reserveCount);
	pool->reserveCount = reserveCount;
	pool->acquiredIndex = 0;
	for(uint32_t i = 0; i < reserveCount; i++)
		pool->semaphores[i] = pvkCreateSemaphore(device);
	return pool;
}
#endif

PVK_LINKAGE void pvkDestroySemaphoreCircularPool(VkDevice device, PvkSemaphoreCircularPool* pool);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkDestroySemaphoreCircularPool(VkDevice device, PvkSemaphoreCircularPool* pool)
{
	for(uint32_t i = 0; i < pool->reserveCount; i++)
		vkDestroySemaphore(device, pool->semaphores[i], NULL);
	PVK_MEMSET((void*)pool, 0, sizeof(pool));
	PVK_FREE(pool);
}
#endif

PVK_LINKAGE VkSemaphore pvkSemaphoreCircularPoolAcquire(PvkSemaphoreCircularPool* pool, uint32_t* outIndex);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkSemaphore pvkSemaphoreCircularPoolAcquire(PvkSemaphoreCircularPool* pool, uint32_t* outIndex)
{
	VkSemaphore semaphore = pool->semaphores[pool->acquiredIndex];
	if(outIndex != NULL)
		*outIndex = pool->acquiredIndex;
	pool->acquiredIndex = (pool->acquiredIndex + 1) % pool->reserveCount;
	return semaphore;
}
#endif

PVK_LINKAGE VkSemaphore pvkSemaphoreCircularPoolRecreate(VkDevice device, PvkSemaphoreCircularPool* pool, uint32_t index);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkSemaphore pvkSemaphoreCircularPoolRecreate(VkDevice device, PvkSemaphoreCircularPool* pool, uint32_t index)
{
	vkDestroySemaphore(device, pool->semaphores[index], NULL);
	pool->semaphores[index] = pvkCreateSemaphore(device);
	return pool->semaphores[index];
}
#endif

PVK_LINKAGE void pvkResetFences(VkDevice device, uint32_t fenceCount, VkFence* fences);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkResetFences(VkDevice device, uint32_t fenceCount, VkFence* fences)
{
	VkResult result = vkWaitForFences(device, fenceCount, fences, VK_TRUE, 0);
	if(result != VK_TIMEOUT)
		PVK_CHECK(result);
	PVK_CHECK(vkResetFences(device, fenceCount, fences));
}
#endif

typedef struct PvkFencePool
{
	VkFence* fences;
	uint32_t reserveCount;
} PvkFencePool;

PVK_LINKAGE PvkFencePool* pvkCreateFencePool(VkDevice device, uint32_t reserveCount);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkFencePool* pvkCreateFencePool(VkDevice device, uint32_t reserveCount)
{
	PvkFencePool* pool = (PvkFencePool*)PVK_MALLOC(sizeof(PvkFencePool));
	pool->fences = (VkFence*)PVK_MALLOC(sizeof(VkFence) * reserveCount);
	pool->reserveCount = reserveCount;
	for(uint32_t i = 0; i < reserveCount; i++)
		pool->fences[i] = pvkCreateFence(device, VK_FENCE_CREATE_SIGNALED_BIT);
	return pool;
}
#endif

PVK_LINKAGE void pvkDestroyFencePool(VkDevice device, PvkFencePool* pool);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkDestroyFencePool(VkDevice device, PvkFencePool* pool)
{
	PVK_CHECK(vkWaitForFences(device, pool->reserveCount, pool->fences, VK_TRUE, 0));
	for(uint32_t i = 0; i < pool->reserveCount; i++)
		vkDestroyFence(device, pool->fences[i], NULL);
	PVK_MEMSET((void*)pool, 0, sizeof(pool));
	PVK_FREE(pool);
}
#endif

PVK_LINKAGE bool pvkFencePoolAcquire(VkDevice device, PvkFencePool* pool, VkFence* outFence);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE bool pvkFencePoolAcquire(VkDevice device, PvkFencePool* pool, VkFence* outFence)
{
	VkFence* fences = pool->fences;
	uint32_t fenceCount = pool->reserveCount;
	for(uint32_t i = 0; i < fenceCount; i++)
	{
		VkResult result = vkWaitForFences(device, 1, &fences[i], VK_TRUE, 0);
		if(result == VK_SUCCESS)
		{
			PVK_CHECK(vkResetFences(device, 1, &fences[i]));
			*outFence = fences[i];
			return true;
		}
		else if(result != VK_TIMEOUT) PVK_CHECK(result);
	}
	return false;
}
#endif

PVK_LINKAGE void pvkSubmit(VkCommandBuffer commandBuffer, VkQueue queue, VkSemaphore wait, VkSemaphore signal, VkFence signalFence);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkSubmit(VkCommandBuffer commandBuffer, VkQueue queue, VkSemaphore wait, VkSemaphore signal, VkFence signalFence)
{
	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo info = { };
	{
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &wait;
		info.pWaitDstStageMask = &waitStage;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &signal;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &commandBuffer;
	};
	PVK_CHECK(vkQueueSubmit(queue, 1, &info, signalFence));
}
#endif

PVK_LINKAGE bool pvkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore waitSemaphore, VkFence waitFence, uint32_t* outIndex);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE bool pvkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore waitSemaphore, VkFence waitFence, uint32_t* outIndex)
{
	uint32_t index;
	VkResult result = vkAcquireNextImageKHR(device, swapchain, timeout, waitSemaphore, waitFence, outIndex);
	if((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR))
		return false;
	else
		PVK_CHECK(result);
	return true;
}
#endif

PVK_LINKAGE bool pvkPresent(uint32_t index, VkSwapchainKHR  swapchain, VkQueue queue, uint32_t waitCount, VkSemaphore* waits);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE bool pvkPresent(uint32_t index, VkSwapchainKHR  swapchain, VkQueue queue, uint32_t waitCount, VkSemaphore* waits)
{
	VkResult result;
	VkPresentInfoKHR info = { };
	{
		info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		info.waitSemaphoreCount = waitCount;
		info.pWaitSemaphores = waits;
		info.swapchainCount = 1;
		info.pSwapchains = &swapchain;
		info.pImageIndices = &index;
		info.pResults = &result;
	};
	// PVK_ASSERT(result == VK_SUCCESS);
	result = vkQueuePresentKHR(queue, &info);
	if((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR))
		return false;
	else
		PVK_CHECK(result);
	return true;
}
#endif

PVK_LINKAGE void pvkBeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlagBits usageFlagBits);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkBeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlagBits usageFlagBits)
{
	PVK_CHECK(vkResetCommandBuffer(commandBuffer, 0));
	VkCommandBufferBeginInfo beginInfo = { };
	{ beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO; beginInfo.flags = usageFlagBits; };
	PVK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));
}
#endif

PVK_STATIC PVK_INLINE void pvkEndCommandBuffer(VkCommandBuffer commandBuffer)
{
	PVK_CHECK(vkEndCommandBuffer(commandBuffer));
}

PVK_LINKAGE void pvkBeginRenderPass(VkCommandBuffer commandBuffer, VkRenderPass renderPass, VkFramebuffer framebuffer, uint32_t width, uint32_t height, uint32_t clearValueCount, VkClearValue* clearValues);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkBeginRenderPass(VkCommandBuffer commandBuffer, VkRenderPass renderPass, VkFramebuffer framebuffer, uint32_t width, uint32_t height, uint32_t clearValueCount, VkClearValue* clearValues)
{
	VkRenderPassBeginInfo beginInfo = { };
	{
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderPass = renderPass;
		beginInfo.framebuffer = framebuffer;
		beginInfo.renderArea = (VkRect2D) { };
		{ beginInfo.renderArea.offset = (VkOffset2D) { 0, 0 }; beginInfo.renderArea.extent = (VkExtent2D) { width, height }; };
		beginInfo.clearValueCount = clearValueCount;
		beginInfo.pClearValues = clearValues;
	};
	vkCmdBeginRenderPass(commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
}
#endif

PVK_STATIC PVK_INLINE void pvkEndRenderPass(VkCommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}

PVK_LINKAGE VkRenderPass pvkCreateRenderPass(VkDevice device);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkRenderPass pvkCreateRenderPass(VkDevice device)
{
	VkAttachmentDescription colorAttachmentDescription = { };
	{
		colorAttachmentDescription.format = VK_FORMAT_B8G8R8A8_SRGB;
		colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	 	colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	 	colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	 	colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	 	colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	 	colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	 	colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	};

	VkAttachmentReference colorAttachmentReference = { };
	{
		colorAttachmentReference.attachment = 0;
		colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	};

	VkSubpassDescription subpass = { };
	{
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentReference;
	};

	VkRenderPassCreateInfo rInfo = { };
	{
		rInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		rInfo.attachmentCount = 1;
		rInfo.pAttachments = &colorAttachmentDescription;
		rInfo.subpassCount = 1;
		rInfo.pSubpasses = &subpass;
	};
	VkRenderPass renderPass;
	PVK_CHECK(vkCreateRenderPass(device, &rInfo, NULL, &renderPass));
	return renderPass;
}
#endif

/* Vulkan Device Memory Allocation */
PVK_LINKAGE VkDeviceMemory pvkAllocateMemory(VkDevice device, VkDeviceSize size, uint32_t memoryTypeIndex);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkDeviceMemory pvkAllocateMemory(VkDevice device, VkDeviceSize size, uint32_t memoryTypeIndex)
{
	VkMemoryAllocateInfo aInfo = { };
	{
		aInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		aInfo.allocationSize = size;
		aInfo.memoryTypeIndex = memoryTypeIndex;
	};

	VkDeviceMemory memory;
	PVK_CHECK(vkAllocateMemory(device, &aInfo, NULL, &memory));
	return memory;
}
#endif

PVK_LINKAGE uint32_t __pvkGetMemoryTypeIndexFromMemoryProperty(VkPhysicalDevice device, VkMemoryPropertyFlags propertyFlags);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE uint32_t __pvkGetMemoryTypeIndexFromMemoryProperty(VkPhysicalDevice device, VkMemoryPropertyFlags propertyFlags)
{
	VkPhysicalDeviceMemoryProperties properties;
	vkGetPhysicalDeviceMemoryProperties(device, &properties);
	uint32_t index = UINT32_MAX;
	for(int i = 0; i < properties.memoryTypeCount; i++)
	{
		VkMemoryPropertyFlags supportedFlags = properties.memoryTypes[i].propertyFlags;
		if((supportedFlags & propertyFlags) == propertyFlags)
			return i;
	}
	PVK_WARNING("Unable to find memory type with requested memory property flags");
	return index;
}
#endif

PVK_LINKAGE void __pvkCheckForMemoryTypesSupport(VkPhysicalDevice device, uint32_t bits);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void __pvkCheckForMemoryTypesSupport(VkPhysicalDevice device, uint32_t bits)
{
	/* TODO */
}
#endif

/* Vulkan Image & ImageView */
PVK_LINKAGE VkImage __pvkCreateImage(VkDevice device, VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags usageFlags, VkImageCreateFlags flags, uint32_t queueFamilyIndexCount, uint32_t* queueFamilyIndices);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkImage __pvkCreateImage(VkDevice device, VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags usageFlags, VkImageCreateFlags flags, uint32_t queueFamilyIndexCount, uint32_t* queueFamilyIndices)
{
	// union operation
	uint32_t uniqueQueueFamilyCount;
	uint32_t uniqueQueueFamilyIndices[queueFamilyIndexCount];
	__pvkUnionUInt32(queueFamilyIndexCount, queueFamilyIndices, &uniqueQueueFamilyCount, uniqueQueueFamilyIndices);
	
	VkImageCreateInfo cInfo = { };
	{
		cInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		cInfo.flags = flags;
		cInfo.imageType = VK_IMAGE_TYPE_2D;
		cInfo.format = format;
		cInfo.extent = (VkExtent3D) { };
		{ cInfo.extent.width = width; cInfo.extent.height = height; cInfo.extent.depth = 1; };
		cInfo.mipLevels = 1;
		cInfo.arrayLayers = 1;
		cInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		cInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		cInfo.usage = usageFlags;
		cInfo.sharingMode = (uniqueQueueFamilyCount > 1) ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;
		cInfo.queueFamilyIndexCount = uniqueQueueFamilyCount;
		cInfo.pQueueFamilyIndices = uniqueQueueFamilyIndices;
	};
	VkImage image;
	PVK_CHECK(vkCreateImage(device, &cInfo, NULL, &image));
	return image;
}
#endif

typedef struct PvkImage
{
	VkImage handle;
	VkDeviceMemory memory;
} PvkImage;

PVK_LINKAGE PvkImage pvkCreateImage(VkPhysicalDevice physicalDevice, VkDevice device, VkMemoryPropertyFlags mflags, VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags usageFlags, uint32_t queueFamilyIndexCount, uint32_t* queueFamilyIndices);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkImage pvkCreateImage(VkPhysicalDevice physicalDevice, VkDevice device, VkMemoryPropertyFlags mflags, VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags usageFlags, uint32_t queueFamilyIndexCount, uint32_t* queueFamilyIndices)
{
	VkImage image = __pvkCreateImage(device, format, width, height, usageFlags, 0, queueFamilyIndexCount, queueFamilyIndices);
	VkMemoryRequirements imageMemoryRequirements;
	vkGetImageMemoryRequirements(device, image, &imageMemoryRequirements);
	__pvkCheckForMemoryTypesSupport(physicalDevice, imageMemoryRequirements.memoryTypeBits);
	VkDeviceMemory memory = pvkAllocateMemory(device, imageMemoryRequirements.size, __pvkGetMemoryTypeIndexFromMemoryProperty(physicalDevice, mflags));
	PVK_CHECK(vkBindImageMemory(device, image, memory, 0));
	return (PvkImage) { image, memory };
}
#endif

PVK_LINKAGE PvkImage pvkCreateImage2(VkPhysicalDevice physicalDevice, VkDevice device, VkMemoryPropertyFlags mflags, VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags usageFlags, uint32_t queueFamilyIndexCount, uint32_t* queueFamilyIndices);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkImage pvkCreateImage2(VkPhysicalDevice physicalDevice, VkDevice device, VkMemoryPropertyFlags mflags, VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags usageFlags, uint32_t queueFamilyIndexCount, uint32_t* queueFamilyIndices)
{
	VkImage image = __pvkCreateImage(device, format, width, height, usageFlags, VK_IMAGE_CREATE_DISJOINT_BIT, queueFamilyIndexCount, queueFamilyIndices);
	
	VkImagePlaneMemoryRequirementsInfo imagePlaneRequirementInfo = { };
	imagePlaneRequirementInfo.sType = VK_STRUCTURE_TYPE_IMAGE_PLANE_MEMORY_REQUIREMENTS_INFO;
	imagePlaneRequirementInfo.planeAspect = (VkImageAspectFlagBits)(VK_IMAGE_ASPECT_PLANE_0_BIT)
	;
	VkImageMemoryRequirementsInfo2 info = { };
	info.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2;
	info.pNext = &imagePlaneRequirementInfo;
	info.image = image;
	VkMemoryRequirements2 imageMemoryRequirements = { };
	imageMemoryRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
	
	vkGetImageMemoryRequirements2(device, &info, &imageMemoryRequirements);
	__pvkCheckForMemoryTypesSupport(physicalDevice, imageMemoryRequirements.memoryRequirements.memoryTypeBits);
	uint32_t imageBits = imageMemoryRequirements.memoryRequirements.memoryTypeBits;
	VkDeviceSize imageSize = imageMemoryRequirements.memoryRequirements.size;
	uint32_t plane0Offset = 0;
	uint32_t plane1Offset = imageSize;

	imagePlaneRequirementInfo.planeAspect = (VkImageAspectFlagBits)(VK_IMAGE_ASPECT_PLANE_1_BIT);
	vkGetImageMemoryRequirements2(device, &info, &imageMemoryRequirements);
	__pvkCheckForMemoryTypesSupport(physicalDevice, imageMemoryRequirements.memoryRequirements.memoryTypeBits);
	imageBits |= imageMemoryRequirements.memoryRequirements.memoryTypeBits;
	imageSize += imageMemoryRequirements.memoryRequirements.size;

	VkDeviceMemory memory = pvkAllocateMemory(device, imageSize, __pvkGetMemoryTypeIndexFromMemoryProperty(physicalDevice, mflags));
	
	VkBindImagePlaneMemoryInfo bindPlane0Info = { };
	bindPlane0Info.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_PLANE_MEMORY_INFO;
	bindPlane0Info.planeAspect = VK_IMAGE_ASPECT_PLANE_0_BIT;
	VkBindImageMemoryInfo bindInfos[2] = { };
	bindInfos[0].sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO;
	bindInfos[0].pNext = &bindPlane0Info;
	bindInfos[0].image = image;
	bindInfos[0].memory = memory;
	bindInfos[0].memoryOffset = plane0Offset;
	VkBindImagePlaneMemoryInfo bindPlane1Info = { };
	bindPlane1Info.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_PLANE_MEMORY_INFO;
	bindPlane1Info.planeAspect = VK_IMAGE_ASPECT_PLANE_1_BIT;
	bindInfos[1].sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO;
	bindInfos[1].pNext = &bindPlane1Info;
	bindInfos[1].image = image;
	bindInfos[1].memory = memory;
	bindInfos[1].memoryOffset = plane1Offset;
	PVK_CHECK(vkBindImageMemory2(device, 2, bindInfos));
	return (PvkImage) { image, memory };
}
#endif

PVK_LINKAGE void pvkDestroyImage(VkDevice device, PvkImage image);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkDestroyImage(VkDevice device, PvkImage image)
{
	vkFreeMemory(device, image.memory, NULL);
	vkDestroyImage(device, image.handle, NULL);
}
#endif

PVK_LINKAGE VkImageView pvkCreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlagBits aspectMask);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkImageView pvkCreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlagBits aspectMask)
{
	VkImageViewCreateInfo cInfo = { };
	{
		cInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		cInfo.image = image;
		cInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		cInfo.format = format;
		cInfo.components = (VkComponentMapping) { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
		cInfo.subresourceRange = (VkImageSubresourceRange) { };
		{
			cInfo.subresourceRange.aspectMask = aspectMask;
			cInfo.subresourceRange.levelCount = 1;
			cInfo.subresourceRange.layerCount = 1;
		}
	};
	VkImageView imageView;
	PVK_CHECK(vkCreateImageView(device, &cInfo, NULL, &imageView));
	return imageView;
}
#endif

PVK_LINKAGE VkImageView pvkCreateImageView2(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlagBits aspectMask, VkSamplerYcbcrConversion conversion);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkImageView pvkCreateImageView2(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlagBits aspectMask, VkSamplerYcbcrConversion conversion)
{
	VkSamplerYcbcrConversionInfo conversionInfo	= { };
	conversionInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_INFO;
	conversionInfo.conversion = conversion;
	VkImageViewCreateInfo cInfo = { };
	{
		cInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		cInfo.pNext = &conversionInfo;
		cInfo.image = image;
		cInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		cInfo.format = format;
		cInfo.components = (VkComponentMapping) { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
		cInfo.subresourceRange = (VkImageSubresourceRange) { };
		{
			cInfo.subresourceRange.aspectMask = aspectMask;
			cInfo.subresourceRange.levelCount = 1;
			cInfo.subresourceRange.layerCount = 1;
		}
	};
	VkImageView imageView;
	PVK_CHECK(vkCreateImageView(device, &cInfo, NULL, &imageView));
	return imageView;
}
#endif

PVK_LINKAGE VkImageView* pvkCreateSwapchainImageViews(VkDevice device, VkSwapchainKHR swapchain, VkFormat format, uint32_t* outImageCount);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkImageView* pvkCreateSwapchainImageViews(VkDevice device, VkSwapchainKHR swapchain, VkFormat format, uint32_t* outImageCount)
{
	// get the swapchain images
	uint32_t imageCount;
	PVK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, NULL));
	if(outImageCount != NULL)
		*outImageCount = imageCount;
	VkImage* images = PVK_NEWV(VkImage, imageCount);
	PVK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images));

	VkImageView* imageViews = PVK_NEWV(VkImageView, imageCount);
	for(int i = 0; i < imageCount; i++)
		imageViews[i] = pvkCreateImageView(device, images[i], format, VK_IMAGE_ASPECT_COLOR_BIT);

	PVK_DELETE(images);
	return imageViews;
}
#endif

PVK_LINKAGE void pvkDestroySwapchainImageViews(VkDevice device, VkSwapchainKHR swapchain, VkImageView* imageViews);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkDestroySwapchainImageViews(VkDevice device, VkSwapchainKHR swapchain, VkImageView* imageViews)
{
	uint32_t imageCount;
	PVK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, NULL));
	for(int i = 0; i < imageCount; i++)
		vkDestroyImageView(device, imageViews[i], NULL);
	PVK_DELETE(imageViews);
}
#endif

/* Vulkan Frambuffer */
PVK_LINKAGE VkFramebuffer* pvkCreateFramebuffers(VkDevice device, VkRenderPass renderPass, uint32_t width, uint32_t height, uint32_t fbCount, uint32_t attachmentCount, VkImageView* imageViews);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkFramebuffer* pvkCreateFramebuffers(VkDevice device, VkRenderPass renderPass, uint32_t width, uint32_t height, uint32_t fbCount, uint32_t attachmentCount, VkImageView* imageViews)
{
	VkFramebuffer* framebuffers = PVK_NEWV(VkFramebuffer, fbCount);
	for(int i = 0; i < fbCount; i++)
	{
		VkFramebufferCreateInfo fInfo = { };
		{
			fInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			fInfo.renderPass = renderPass;
			fInfo.attachmentCount = attachmentCount;
			fInfo.pAttachments = &imageViews[attachmentCount * i];
			fInfo.width = width;
			fInfo.height = height;
			fInfo.layers = 1;
		};
		PVK_CHECK(vkCreateFramebuffer(device, &fInfo, NULL, &framebuffers[i]));
	}
	return framebuffers;
}
#endif

PVK_LINKAGE void pvkDestroyFramebuffers(VkDevice device, uint32_t fbCount, VkFramebuffer* framebuffers);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkDestroyFramebuffers(VkDevice device, uint32_t fbCount, VkFramebuffer* framebuffers)
{
	for(int i = 0; i < fbCount; i++)
		vkDestroyFramebuffer(device, framebuffers[i], NULL);
}
#endif

/* Shaders & Graphics Pipeline */
PVK_LINKAGE const char* __pvkLoadBinaryFile(const char* filePath, size_t* out_length);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE const char* __pvkLoadBinaryFile(const char* filePath, size_t* out_length)
{
	FILE* file = fopen(filePath, "rb");
	if(file == NULL)
		PVK_FETAL_ERROR("Unable to open the file at path \"%s\"", filePath);
	int result = fseek(file, 0, SEEK_END);
	PVK_ASSERT(result == 0);
	size_t length = ftell(file);
	if(length == 0)
		PVK_WARNING("File at path \"%s\" is empty", filePath);
	rewind(file);
	char* data = PVK_NEWV(char, length);
	size_t readLength = fread(data, 1, length, file);
	PVK_ASSERT(readLength == length);
	fclose(file);
	if(out_length != NULL)
		*out_length = length;
	return data;
}
#endif

PVK_LINKAGE VkShaderModule pvkCreateShaderModule(VkDevice device, const char* filePath);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkShaderModule pvkCreateShaderModule(VkDevice device, const char* filePath)
{
	size_t length;
	const char* bytes = __pvkLoadBinaryFile(filePath, &length);
	PVK_ASSERT((length % 4) == 0);
	VkShaderModuleCreateInfo cInfo = { };
	{
		cInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		cInfo.codeSize = (uint32_t)length;
		cInfo.pCode = (uint32_t*)bytes;
	};
	VkShaderModule shaderModule;
	PVK_CHECK(vkCreateShaderModule(device, &cInfo, NULL, &shaderModule));
	PVK_DELETE(bytes);
	return shaderModule;
}
#endif

typedef struct PvkShader
{
	VkShaderModule handle;
	PvkShaderType type;
} PvkShader;

PVK_LINKAGE VkShaderStageFlagBits __pkvShaderTypeToVulkanShaderStage(PvkShaderType type);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkShaderStageFlagBits __pkvShaderTypeToVulkanShaderStage(PvkShaderType type)
{
	VkShaderStageFlagBits flags = (VkShaderStageFlagBits)0;
	if(type & PVK_SHADER_TYPE_VERTEX)
		flags = (VkShaderStageFlagBits) (flags | VK_SHADER_STAGE_VERTEX_BIT);
	if(type & PVK_SHADER_TYPE_TESSELLATION)
		flags = (VkShaderStageFlagBits) (flags | VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT);
	if(type & PVK_SHADER_TYPE_GEOMETRY)
		flags = (VkShaderStageFlagBits) (flags | VK_SHADER_STAGE_GEOMETRY_BIT);
	if(type & PVK_SHADER_TYPE_FRAGMENT)
		flags = (VkShaderStageFlagBits) (flags | VK_SHADER_STAGE_FRAGMENT_BIT);
	return flags;
}
#endif

PVK_LINKAGE VkPipelineShaderStageCreateInfo* __pvkCreatePipelineShaderStageCreateInfos(uint32_t count, const PvkShader* const shaders);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkPipelineShaderStageCreateInfo* __pvkCreatePipelineShaderStageCreateInfos(uint32_t count, const PvkShader* const shaders)
{
	VkPipelineShaderStageCreateInfo* infos = PVK_NEWV(VkPipelineShaderStageCreateInfo, count);
	for(int i = 0; i < count; i++)
	{
		infos[i] = (VkPipelineShaderStageCreateInfo) { };
		{
			infos[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			infos[i].module = shaders[i].handle;
			infos[i].pName = "main";
			infos[i].stage = __pkvShaderTypeToVulkanShaderStage(shaders[i].type);	
		};
	}
	return infos;
}
#endif

typedef struct PvkVertex
{
	// attribute at location = 0 : position of the vertex
	PvkVec3 position;

	// attribute at location = 1 : normal of the vertex
	PvkVec3 normal;

	// attribute at location = 2 : texture coordinates of the vertex
	PvkVec2 texcoord;

	// attribute at location = 3 : color of the vertex
	PvkVec4 color;

} PvkVertex;

#define PVK_VERTEX_SIZE sizeof(PvkVertex)
#define PVK_VERTEX_ATTRIBUTE_COUNT 4
#define PVK_VERTEX_POSITION_OFFSET offsetof(PvkVertex, position)
#define PVK_VERTEX_NORMAL_OFFSET offsetof(PvkVertex, normal)
#define PVK_VERTEX_TEXCOORD_OFFSET offsetof(PvkVertex, texcoord)
#define PVK_VERTEX_COLOR_OFFSET offsetof(PvkVertex, color)

PVK_STATIC PVK_INLINE PVK_CONSTEXPR VkVertexInputAttributeDescription __pvkGetVertexInputAttributeDescription(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset)
{
	VkVertexInputAttributeDescription dsc = { };
	{
		dsc.location = location;
		dsc.binding = binding;
		dsc.format = format;
		dsc.offset = offset;
	};
	return dsc;
}

PVK_LINKAGE VkPipeline __pvkCreateGraphicsPipeline(VkDevice device, VkPipelineLayout layout, VkRenderPass renderPass, uint32_t subpassIndex, uint32_t width, uint32_t height, uint32_t vertInputBindCount, VkVertexInputBindingDescription* vertexBindingDescriptions, uint32_t vertInputAttrCount, VkVertexInputAttributeDescription* vertexAttributeDescriptions, VkPipelineColorBlendStateCreateInfo* colorBlend, bool enableDepth, uint32_t count, va_list args);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkPipeline __pvkCreateGraphicsPipeline(VkDevice device, VkPipelineLayout layout, VkRenderPass renderPass, uint32_t subpassIndex, uint32_t width, uint32_t height, uint32_t vertInputBindCount, VkVertexInputBindingDescription* vertexBindingDescriptions, uint32_t vertInputAttrCount, VkVertexInputAttributeDescription* vertexAttributeDescriptions, VkPipelineColorBlendStateCreateInfo* colorBlend, bool enableDepth, uint32_t count, va_list args)
{
	/* Shader modules */
	PvkShader shaderModules[count];
	for(int i = 0; i < count; i++)
		shaderModules[i] = va_arg(args, PvkShader);
	VkPipelineShaderStageCreateInfo* stageCInfos = __pvkCreatePipelineShaderStageCreateInfos(count, shaderModules);

	/* Vertex buffer layouts and their description */
	VkPipelineVertexInputStateCreateInfo vertexInputStateCInfo = { };
	{
		vertexInputStateCInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCInfo.vertexBindingDescriptionCount = vertInputBindCount;
		vertexInputStateCInfo.pVertexBindingDescriptions = vertexBindingDescriptions;
		vertexInputStateCInfo.vertexAttributeDescriptionCount = vertInputAttrCount;
		vertexInputStateCInfo.pVertexAttributeDescriptions = vertexAttributeDescriptions;
	};

	/* Primitive assembly */
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCInfo = { };
	{
		inputAssemblyStateCInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyStateCInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyStateCInfo.primitiveRestartEnable = VK_FALSE;
	};

	/* Viewport configuration */
	VkViewport viewport = { };
	{
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = width;
		viewport.height = height;
		viewport.minDepth = 0;
		viewport.maxDepth = 1.0f;
	};
	VkRect2D scissor = { };
	{
		scissor.offset = (VkOffset2D) { 0, 0 };
		scissor.extent = (VkExtent2D) { width, height };
	};

	VkPipelineViewportStateCreateInfo viewportStateCInfo = { };
	{
		viewportStateCInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateCInfo.viewportCount = 1;
		viewportStateCInfo.pViewports = &viewport;
		viewportStateCInfo.scissorCount = 1;
		viewportStateCInfo.pScissors = &scissor;
	};

	/* Rasterization configuration */
	VkPipelineRasterizationStateCreateInfo rasterizationStateCInfo = { };
	{
		rasterizationStateCInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationStateCInfo.depthClampEnable = VK_FALSE;
		rasterizationStateCInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizationStateCInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationStateCInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizationStateCInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizationStateCInfo.lineWidth = 1.0f;
	};

	/* Multisampling */
	VkPipelineMultisampleStateCreateInfo multisamplingStateCInfo = { };
	{
		multisamplingStateCInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisamplingStateCInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisamplingStateCInfo.sampleShadingEnable = VK_FALSE;
	};

	/* Depth stencil configuration */
	VkPipelineDepthStencilStateCreateInfo dephtStencilStateCInfo = { };
	{
		dephtStencilStateCInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		dephtStencilStateCInfo.depthTestEnable = enableDepth ? VK_TRUE : VK_FALSE;
		dephtStencilStateCInfo.depthWriteEnable = enableDepth ? VK_TRUE : VK_FALSE;
		dephtStencilStateCInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		dephtStencilStateCInfo.stencilTestEnable = VK_FALSE;
		dephtStencilStateCInfo.depthBoundsTestEnable = VK_FALSE;
	};

	VkGraphicsPipelineCreateInfo pipelineCInfo = { };
	{
		pipelineCInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCInfo.stageCount = count;
		pipelineCInfo.pStages = stageCInfos;
		pipelineCInfo.pVertexInputState = &vertexInputStateCInfo;
		pipelineCInfo.pInputAssemblyState = &inputAssemblyStateCInfo;
		pipelineCInfo.pViewportState = &viewportStateCInfo;
		pipelineCInfo.pMultisampleState = &multisamplingStateCInfo;
		pipelineCInfo.pRasterizationState = &rasterizationStateCInfo;
		pipelineCInfo.pDepthStencilState = &dephtStencilStateCInfo;
		pipelineCInfo.pColorBlendState = colorBlend;
		pipelineCInfo.layout = layout;
		pipelineCInfo.renderPass = renderPass;
		pipelineCInfo.subpass = subpassIndex;
	};

	VkPipeline pipeline;
	PVK_CHECK(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCInfo, NULL, &pipeline));

	PVK_DELETE(stageCInfos);
	return pipeline;
}
#endif

PVK_LINKAGE VkPipeline pvkCreateShadowMapGraphicsPipeline(VkDevice device, VkPipelineLayout layout, VkRenderPass renderPass, uint32_t subpassIndex, uint32_t width, uint32_t height, uint32_t count, ...);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkPipeline pvkCreateShadowMapGraphicsPipeline(VkDevice device, VkPipelineLayout layout, VkRenderPass renderPass, uint32_t subpassIndex, uint32_t width, uint32_t height, uint32_t count, ...)
{
	VkVertexInputBindingDescription vertexBindingDescription = { };
	{
		vertexBindingDescription.binding = 0;
		vertexBindingDescription.stride = PVK_VERTEX_SIZE;
		vertexBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	};
	VkVertexInputAttributeDescription* vertexAttributeDescriptions = PVK_NEWV(VkVertexInputAttributeDescription, PVK_VERTEX_ATTRIBUTE_COUNT);
	vertexAttributeDescriptions[0] = __pvkGetVertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, PVK_VERTEX_POSITION_OFFSET);
	vertexAttributeDescriptions[1] = __pvkGetVertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, PVK_VERTEX_NORMAL_OFFSET);
	vertexAttributeDescriptions[2] = __pvkGetVertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32_SFLOAT, PVK_VERTEX_TEXCOORD_OFFSET);
	vertexAttributeDescriptions[3] = __pvkGetVertexInputAttributeDescription(0, 3, VK_FORMAT_R32G32B32A32_SFLOAT, PVK_VERTEX_COLOR_OFFSET);

	va_list shaderModuleList;
	va_start(shaderModuleList, count);
	VkPipeline pipeline =  __pvkCreateGraphicsPipeline(device, layout, renderPass, subpassIndex, width, height, 1, &vertexBindingDescription, 4, vertexAttributeDescriptions, NULL, true, count, shaderModuleList);
	va_end(shaderModuleList);

	PVK_DELETE(vertexAttributeDescriptions);
	return pipeline;
}
#endif

PVK_LINKAGE VkPipeline pvkCreateGraphicsPipelineProfile0(VkDevice device, VkPipelineLayout layout, VkRenderPass renderPass, uint32_t width, uint32_t height, uint32_t count, ...);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkPipeline pvkCreateGraphicsPipelineProfile0(VkDevice device, VkPipelineLayout layout, VkRenderPass renderPass, uint32_t width, uint32_t height, uint32_t count, ...)
{
	va_list shaderModuleList;
	va_start(shaderModuleList, count);

	/* Color attachment configuration */
	VkPipelineColorBlendAttachmentState colorAttachment = { };
	{
		colorAttachment.blendEnable = VK_FALSE;
		colorAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	};

	VkPipelineColorBlendStateCreateInfo colorBlend = { };
	{
		colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlend.logicOpEnable = VK_FALSE;
		colorBlend.attachmentCount = 1;
		colorBlend.pAttachments = &colorAttachment;
	};

	VkPipeline pipeline =  __pvkCreateGraphicsPipeline(device, layout, renderPass, 0, width, height, 0, NULL, 0, NULL, &colorBlend, true, count, shaderModuleList);
	va_end(shaderModuleList);
	return pipeline;
}
#endif

PVK_LINKAGE VkPipeline pvkCreateGraphicsPipeline(VkDevice device, VkPipelineLayout layout, VkRenderPass renderPass, uint32_t subpassIndex, uint32_t colorAttachmentCount, uint32_t width, uint32_t height, uint32_t count, ...);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkPipeline pvkCreateGraphicsPipeline(VkDevice device, VkPipelineLayout layout, VkRenderPass renderPass, uint32_t subpassIndex, uint32_t colorAttachmentCount, uint32_t width, uint32_t height, uint32_t count, ...)
{
	va_list shaderModuleList;
	va_start(shaderModuleList, count);

	/* Color attachment configuration */
	VkPipelineColorBlendAttachmentState* colorAttachments = PVK_NEWV(VkPipelineColorBlendAttachmentState, colorAttachmentCount);
	for(uint32_t i = 0; i < colorAttachmentCount; i++)
	{
		VkPipelineColorBlendAttachmentState colorAttachment = { };
		{
			colorAttachment.blendEnable = VK_FALSE;
			colorAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		};
		colorAttachments[i] = colorAttachment;
	}

	VkPipelineColorBlendStateCreateInfo colorBlend = { };
	{
		colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlend.logicOpEnable = VK_FALSE;
		colorBlend.attachmentCount = colorAttachmentCount;
		colorBlend.pAttachments = colorAttachments;
	};

	VkVertexInputBindingDescription vertexBindingDescription = { };
	{
		vertexBindingDescription.binding = 0;
		vertexBindingDescription.stride = PVK_VERTEX_SIZE;
		vertexBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	};
	VkVertexInputAttributeDescription* vertexAttributeDescriptions = PVK_NEWV(VkVertexInputAttributeDescription, PVK_VERTEX_ATTRIBUTE_COUNT);
	vertexAttributeDescriptions[0] = __pvkGetVertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, PVK_VERTEX_POSITION_OFFSET);
	vertexAttributeDescriptions[1] = __pvkGetVertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, PVK_VERTEX_NORMAL_OFFSET);
	vertexAttributeDescriptions[2] = __pvkGetVertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32_SFLOAT, PVK_VERTEX_TEXCOORD_OFFSET);
	vertexAttributeDescriptions[3] = __pvkGetVertexInputAttributeDescription(0, 3, VK_FORMAT_R32G32B32A32_SFLOAT, PVK_VERTEX_COLOR_OFFSET);

	VkPipeline pipeline =  __pvkCreateGraphicsPipeline(device, layout, renderPass, subpassIndex, width, height, 1, &vertexBindingDescription, 4, vertexAttributeDescriptions, &colorBlend, true, count, shaderModuleList);
	va_end(shaderModuleList);
	PVK_DELETE(colorAttachments);
	PVK_DELETE(vertexAttributeDescriptions);
	return pipeline;
}
#endif

PVK_LINKAGE VkPipelineLayout pvkCreatePipelineLayout(VkDevice device, uint32_t setLayoutCount, VkDescriptorSetLayout* setLayouts);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkPipelineLayout pvkCreatePipelineLayout(VkDevice device, uint32_t setLayoutCount, VkDescriptorSetLayout* setLayouts)
{
	VkPipelineLayoutCreateInfo cInfo = { };
	{
		cInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		cInfo.setLayoutCount = setLayoutCount;
		cInfo.pSetLayouts = setLayouts;
	};
	VkPipelineLayout layout;
	PVK_CHECK(vkCreatePipelineLayout(device, &cInfo, NULL, &layout));
	return layout;
}
#endif

/* Vulkan Buffer */
PVK_LINKAGE VkBuffer __pvkCreateBuffer(VkDevice device, VkBufferUsageFlags usageFlags, VkDeviceSize size, uint32_t queueFamilyCount, uint32_t* queueFamilyIndices);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkBuffer __pvkCreateBuffer(VkDevice device, VkBufferUsageFlags usageFlags, VkDeviceSize size, uint32_t queueFamilyCount, uint32_t* queueFamilyIndices)
{
	// union operation
	uint32_t uniqueQueueFamilyCount;
	uint32_t uniqueQueueFamilyIndices[queueFamilyCount];
	__pvkUnionUInt32(queueFamilyCount, queueFamilyIndices, &uniqueQueueFamilyCount, uniqueQueueFamilyIndices);

	VkBufferCreateInfo cInfo = { };
	{
		cInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		cInfo.usage = usageFlags;
		cInfo.size = size;
		cInfo.queueFamilyIndexCount = uniqueQueueFamilyCount;
		cInfo.pQueueFamilyIndices = uniqueQueueFamilyIndices;
		cInfo.sharingMode = (uniqueQueueFamilyCount > 1) ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;
	};

	VkBuffer buffer;
	PVK_CHECK(vkCreateBuffer(device, &cInfo, NULL, &buffer));
	return buffer;
}
#endif

typedef struct PvkBuffer
{
	VkBuffer handle;
	VkDeviceMemory memory;
} PvkBuffer;


PVK_LINKAGE PvkBuffer pvkCreateBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkMemoryPropertyFlags mflags, VkBufferUsageFlags flags, VkDeviceSize size, uint32_t queueFamilyCount, uint32_t* queueFamilyIndices);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkBuffer pvkCreateBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkMemoryPropertyFlags mflags, VkBufferUsageFlags flags, VkDeviceSize size, uint32_t queueFamilyCount, uint32_t* queueFamilyIndices)
{
	VkBuffer buffer = __pvkCreateBuffer(device, flags, size, queueFamilyCount, queueFamilyIndices);
	VkMemoryRequirements bufferMemoryRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &bufferMemoryRequirements);
	__pvkCheckForMemoryTypesSupport(physicalDevice, bufferMemoryRequirements.memoryTypeBits);
	VkDeviceMemory memory = pvkAllocateMemory(device, bufferMemoryRequirements.size, 
												__pvkGetMemoryTypeIndexFromMemoryProperty(physicalDevice, mflags));	
	PVK_CHECK(vkBindBufferMemory(device, buffer, memory, 0));
	return (PvkBuffer) { buffer, memory };
}
#endif

PVK_LINKAGE void pvkDestroyBuffer(VkDevice device, PvkBuffer buffer);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkDestroyBuffer(VkDevice device, PvkBuffer buffer)
{
	vkDestroyBuffer(device, buffer.handle, NULL);
	vkFreeMemory(device, buffer.memory, NULL);
}
#endif

PVK_LINKAGE void pvkUploadToMemory(VkDevice device, VkDeviceMemory memory, void* data, size_t size);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkUploadToMemory(VkDevice device, VkDeviceMemory memory, void* data, size_t size)
{
	void* dst;
	PVK_CHECK(vkMapMemory(device, memory, 0, size, 0, &dst));
	memcpy(dst, data, size);
	vkUnmapMemory(device, memory);
}
#endif

/* Vulkan Descriptor sets */

PVK_LINKAGE VkDescriptorSet* pvkAllocateDescriptorSets(VkDevice device, VkDescriptorPool pool, uint32_t setCount, VkDescriptorSetLayout* setLayouts);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkDescriptorSet* pvkAllocateDescriptorSets(VkDevice device, VkDescriptorPool pool, uint32_t setCount, VkDescriptorSetLayout* setLayouts)
{
	VkDescriptorSetAllocateInfo allocInfo = { };
	{
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = setCount;
		allocInfo.pSetLayouts = setLayouts;
	};

	VkDescriptorSet* sets = PVK_NEWV(VkDescriptorSet, setCount);
	PVK_CHECK(vkAllocateDescriptorSets(device, &allocInfo, sets));
	return sets;
}
#endif

PVK_LINKAGE VkDescriptorPool pvkCreateDescriptorPool(VkDevice device, u32 maxSets, u32 poolSize, ...);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE VkDescriptorPool pvkCreateDescriptorPool(VkDevice device, u32 maxSets, u32 poolSize, ...)
{
	va_list args;
	va_start(args, poolSize);
	VkDescriptorPoolSize poolSizes[poolSize] = { };
	for(u32 i = 0; i < poolSize; i++)
	{
		poolSizes[i] = (VkDescriptorPoolSize)
		{
			(VkDescriptorType)va_arg(args, uint64_t),
			(uint32_t)va_arg(args, uint64_t)
		};
	};
	va_end(args);
	VkDescriptorPoolCreateInfo cInfo = { };
	{
		cInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		cInfo.maxSets = maxSets;
		cInfo.poolSizeCount = poolSize;
		cInfo.pPoolSizes = poolSizes;
	};

	VkDescriptorPool pool;
	PVK_CHECK(vkCreateDescriptorPool(device, &cInfo, NULL, &pool));
	return pool;
}
#endif

PVK_LINKAGE void pvkWriteImageViewToDescriptor(VkDevice device, VkDescriptorSet set, uint32_t binding, VkImageView imageView, VkSampler sampler, VkImageLayout layout, VkDescriptorType descriptorType);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkWriteImageViewToDescriptor(VkDevice device, VkDescriptorSet set, uint32_t binding, VkImageView imageView, VkSampler sampler, VkImageLayout layout, VkDescriptorType descriptorType)
{
	VkDescriptorImageInfo imageInfo = { };
	{
		imageInfo.imageView = imageView;
		imageInfo.sampler = sampler;
		imageInfo.imageLayout = layout;
	};

	VkWriteDescriptorSet writeInfo = { };
	{
		writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeInfo.dstSet = set;
		writeInfo.dstBinding = binding;
		writeInfo.dstArrayElement = 0;
		writeInfo.descriptorCount = 1;
		writeInfo.descriptorType = descriptorType;
		writeInfo.pImageInfo = &imageInfo;
	};

	vkUpdateDescriptorSets(device, 1, &writeInfo, 0, NULL);
}
#endif

PVK_LINKAGE void pvkWriteBufferToDescriptor(VkDevice device, VkDescriptorSet set, uint32_t binding, VkBuffer buffer, VkDescriptorType descriptorType);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkWriteBufferToDescriptor(VkDevice device, VkDescriptorSet set, uint32_t binding, VkBuffer buffer, VkDescriptorType descriptorType)
{
	VkDescriptorBufferInfo bufferInfo = { };
	{
		bufferInfo.buffer = buffer;
		bufferInfo.offset = 0;
		bufferInfo.range = VK_WHOLE_SIZE;
	};

	VkWriteDescriptorSet writeInfo = { };
	{
		writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeInfo.dstSet = set;
		writeInfo.dstBinding = binding;
		writeInfo.dstArrayElement = 0;
		writeInfo.descriptorCount = 1;
		writeInfo.descriptorType = descriptorType;
		writeInfo.pBufferInfo = &bufferInfo;
	};

	vkUpdateDescriptorSets(device, 1, &writeInfo, 0, NULL);
}
#endif

/* Geometry */
typedef uint16_t PvkIndex;

typedef struct PvkGeometryData
{
	PvkVertex* vertices;
	PvkIndex* indices;
	uint32_t vertexCount;
	uint32_t indexCount;
} PvkGeometryData;

typedef struct PvkGeometry
{
	PvkBuffer vertexBuffer;
	PvkBuffer indexBuffer;
	uint16_t indexCount;
	PvkMat4 transform;
} PvkGeometry;

PVK_LINKAGE PvkGeometry* __pvkCreateGeometry(VkPhysicalDevice physicalDevice, VkDevice device, uint16_t queueFamilyIndexCount, uint32_t* queueFamilyIndices, PvkGeometryData* data);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkGeometry* __pvkCreateGeometry(VkPhysicalDevice physicalDevice, VkDevice device, uint16_t queueFamilyIndexCount, uint32_t* queueFamilyIndices, PvkGeometryData* data)
{
	uint64_t vertexBufferSize = sizeof(PvkVertex) * data->vertexCount;
	uint64_t indexBufferSize = sizeof(PvkIndex) * data->indexCount;
	PvkBuffer vertexBuffer = pvkCreateBuffer(physicalDevice, device, 
												VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
												VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertexBufferSize, queueFamilyIndexCount, queueFamilyIndices);
	PvkBuffer indexBuffer = pvkCreateBuffer(physicalDevice, device, 
												VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
												VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexBufferSize, queueFamilyIndexCount, queueFamilyIndices);
	pvkUploadToMemory(device, vertexBuffer.memory, data->vertices, vertexBufferSize);
	pvkUploadToMemory(device, indexBuffer.memory, data->indices, indexBufferSize);
	PvkGeometry* geometry = PVK_NEW(PvkGeometry);
	geometry->vertexBuffer = vertexBuffer;
	geometry->indexBuffer = indexBuffer;
	geometry->indexCount = data->indexCount;
	geometry->transform = pvkMat4Identity();
	return geometry;
}
#endif

PVK_LINKAGE PvkGeometry* pvkCreatePlaneGeometry(VkPhysicalDevice physicalDevice, VkDevice device, uint32_t queueFamilyIndexCount, uint32_t* queueFamilyIndices, float size);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkGeometry* pvkCreatePlaneGeometry(VkPhysicalDevice physicalDevice, VkDevice device, uint32_t queueFamilyIndexCount, uint32_t* queueFamilyIndices, float size)
{
	PvkVertex vertices[4] = 
	{
		{ { -0.5f * size, 0, -0.5f * size }, { 0, 1.0f, 0 }, { 0, 1 }, { 1, 1, 1, 1 } },
		{ {  0.5f * size, 0, -0.5f * size }, { 0, 1.0f, 0 }, { 1, 1 }, { 1, 1, 1, 1 } },
		{ {  0.5f * size, 0,  0.5f * size }, { 0, 1.0f, 0 }, { 1, 0 }, { 1, 1, 1, 1 } },
		{ { -0.5f * size, 0,  0.5f * size }, { 0, 1.0f, 0 }, { 0, 0 }, { 1, 1, 1, 1 } }
	};

	PvkIndex indices[6] = 
	{
		2, 1, 0,
		0, 3, 2
	};

	PvkGeometryData geometryData = { };
	{ 
		geometryData.vertices = vertices; 
		geometryData.vertexCount = 4;
		geometryData.indices = indices;
		geometryData.indexCount = 6;
	};
	return __pvkCreateGeometry(physicalDevice, device, queueFamilyIndexCount, queueFamilyIndices, &geometryData);
}
#endif

PVK_LINKAGE PvkGeometry* pvkCreateBoxGeometry(VkPhysicalDevice physicalDevice, VkDevice device, uint32_t queueFamilyIndexCount, uint32_t* queueFamilyIndices, float size);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkGeometry* pvkCreateBoxGeometry(VkPhysicalDevice physicalDevice, VkDevice device, uint32_t queueFamilyIndexCount, uint32_t* queueFamilyIndices, float size)
{
	PvkVertex vertices[24] = 
	{
		// bottom
		{ { -0.5f * size, -0.5f * size, -0.5f * size }, { 0, -1.0f, 0 }, { 0, 1 }, { 1, 1, 1, 1 } },
		{ {  0.5f * size, -0.5f * size, -0.5f * size }, { 0, -1.0f, 0 }, { 1, 1 }, { 1, 1, 1, 1 } },
		{ {  0.5f * size, -0.5f * size,  0.5f * size }, { 0, -1.0f, 0 }, { 1, 0 }, { 1, 1, 1, 1 } },
		{ { -0.5f * size, -0.5f * size,  0.5f * size }, { 0, -1.0f, 0 }, { 0, 0 }, { 1, 1, 1, 1 } },

		// top
		{ { -0.5f * size, 0.5f * size, -0.5f * size }, { 0, 1.0f, 0 }, { 0, 1 }, { 1, 1, 1, 1 } },
		{ {  0.5f * size, 0.5f * size, -0.5f * size }, { 0, 1.0f, 0 }, { 1, 1 }, { 1, 1, 1, 1 } },
		{ {  0.5f * size, 0.5f * size,  0.5f * size }, { 0, 1.0f, 0 }, { 1, 0 }, { 1, 1, 1, 1 } },
		{ { -0.5f * size, 0.5f * size,  0.5f * size }, { 0, 1.0f, 0 }, { 0, 0 }, { 1, 1, 1, 1 } },

		// left
		{ { -0.5f * size,  0.5f * size, -0.5f * size }, { -1.0f, 0, 0 }, { 0, 1 }, { 1, 1, 1, 1 } },
		{ { -0.5f * size,  0.5f * size,  0.5f * size }, { -1.0f, 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 } },
		{ { -0.5f * size, -0.5f * size,  0.5f * size }, { -1.0f, 0, 0 }, { 1, 0 }, { 1, 1, 1, 1 } },
		{ { -0.5f * size, -0.5f * size, -0.5f * size }, { -1.0f, 0, 0 }, { 0, 0 }, { 1, 1, 1, 1 } },

		// right
		{ { 0.5f * size,  0.5f * size, -0.5f * size }, { 1.0f, 0, 0 }, { 0, 1 }, { 1, 1, 1, 1 } },
		{ { 0.5f * size,  0.5f * size,  0.5f * size }, { 1.0f, 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 } },
		{ { 0.5f * size, -0.5f * size,  0.5f * size }, { 1.0f, 0, 0 }, { 1, 0 }, { 1, 1, 1, 1 } },
		{ { 0.5f * size, -0.5f * size, -0.5f * size }, { 1.0f, 0, 0 }, { 0, 0 }, { 1, 1, 1, 1 } },

		// rear
		{ {  0.5f * size,  0.5f * size, -0.5f * size }, { 0, 0, 1.0f }, { 0, 1 }, { 1, 1, 1, 1 } },
		{ { -0.5f * size,  0.5f * size, -0.5f * size }, { 0, 0, 1.0f }, { 1, 1 }, { 1, 1, 1, 1 } },
		{ { -0.5f * size, -0.5f * size, -0.5f * size }, { 0, 0, 1.0f }, { 1, 0 }, { 1, 1, 1, 1 } },
		{ {  0.5f * size, -0.5f * size, -0.5f * size }, { 0, 0, 1.0f }, { 0, 0 }, { 1, 1, 1, 1 } },

		// fear
		{ {  0.5f * size,  0.5f * size, 0.5f * size }, { 0, 0, -1.0f }, { 0, 1 }, { 1, 1, 1, 1 } },
		{ { -0.5f * size,  0.5f * size, 0.5f * size }, { 0, 0, -1.0f }, { 1, 1 }, { 1, 1, 1, 1 } },
		{ { -0.5f * size, -0.5f * size, 0.5f * size }, { 0, 0, -1.0f }, { 1, 0 }, { 1, 1, 1, 1 } },
		{ {  0.5f * size, -0.5f * size, 0.5f * size }, { 0, 0, -1.0f }, { 0, 0 }, { 1, 1, 1, 1 } }
	};

	PvkIndex indices[36] = 
	{
		0, 1, 2,
		2, 3, 0,
		6, 5, 4,
		4, 7, 6,
		10, 9, 8,
		8, 11, 10,
		12, 13, 14,
		14, 15, 12,
		18, 17, 16,
		16, 19, 18,
		20, 21, 22,
		22, 23, 20
	};
	PvkGeometryData geometryData = { };
	{
		geometryData.vertices = vertices; 
		geometryData.vertexCount = 24;
		geometryData.indices = indices;
		geometryData.indexCount = 36;
	};
	return __pvkCreateGeometry(physicalDevice, device, queueFamilyIndexCount, queueFamilyIndices, &geometryData);
}
#endif

PVK_LINKAGE void pvkDrawGeometry(VkCommandBuffer cb, PvkGeometry* geometry);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkDrawGeometry(VkCommandBuffer cb, PvkGeometry* geometry)
{
	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(cb, 0, 1, &geometry->vertexBuffer.handle, &offset);
	vkCmdBindIndexBuffer(cb, geometry->indexBuffer.handle, 0, VK_INDEX_TYPE_UINT16);
	vkCmdDrawIndexed(cb, geometry->indexCount, 1, 0, 0, 0);
}
#endif

PVK_LINKAGE void pvkDestroyGeometry(VkDevice device, PvkGeometry* geometry);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE void pvkDestroyGeometry(VkDevice device, PvkGeometry* geometry)
{
	pvkDestroyBuffer(device, geometry->vertexBuffer);
	pvkDestroyBuffer(device, geometry->indexBuffer);
	PVK_DELETE(geometry);
}
#endif

/* Camera */

PVK_STATIC PVK_INLINE PVK_CONSTEXPR PvkMat4 pvkMat4OrthoProj(float height, float aspectRatio, float n, float f)
{
	return (PvkMat4)
	{
		2 / (height * aspectRatio), 0, 0, 0,
		0, -2 / height, 0, 0,								// negative sign due to vulkan clip space nature
		0, 0, -1 / (f - n), 0,								// negative sign due to the reversed direction of z-axis in our application
		0, 0, 0, 1
	};
}

PVK_LINKAGE PvkMat4 pvkMat4PerspProj(float vAngle, float aspectRatio, float n, float f);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkMat4 pvkMat4PerspProj(float vAngle, float aspectRatio, float n, float f)
{
	float height = tan(vAngle * 0.5f);
	return (PvkMat4)
	{
		1 / (height * aspectRatio), 0, 0, 0,
		0, -1 / height, 0, 0,								// negative sign due to vulkan clip space nature
		0, 0, -f / (f - n), -n * f / (f - n),				// negative sign due to the reversed direction of z-axis in our application
		0, 0, -1, 0
	};
	/*
	  f 		  n * f
	------  +    -------- = 
	(f - n) 	 (f - n)z

	 */

}
#endif

typedef struct PvkCamera
{
	PvkMat4 transform;			// model matrix of the camera
	PvkMat4 projection;			// projection matrix of the camera
	PvkMat4 view; 				// view matrix of the camera
} PvkCamera;

typedef enum PvkProjectionType
{
	PVK_PROJECTION_TYPE_ORTHOGRAPHIC,
	PVK_PROJECTION_TYPE_PERSPECTIVE
} PvkProjectionType;

PVK_LINKAGE PvkCamera* pvkCreateCamera(float aspectRatio, PvkProjectionType projectionType, float heightOrAngle);
#ifdef PVK_IMPLEMENTATION
PVK_LINKAGE PvkCamera* pvkCreateCamera(float aspectRatio, PvkProjectionType projectionType, float heightOrAngle)
{
	PvkCamera* cam = PVK_NEW(PvkCamera);
	cam->transform = pvkMat4Mul(pvkMat4Translate((PvkVec3) { 0, 2.0f, 6.0f }), pvkMat4Rotate((PvkVec3) { -20 PVK_DEG, 0, 0 }));
	cam->view = pvkMat4Inverse(cam->transform);
	switch(projectionType)
	{
		case PVK_PROJECTION_TYPE_PERSPECTIVE:
			cam->projection = pvkMat4PerspProj(heightOrAngle, aspectRatio, 1, 20);
			break;
		case PVK_PROJECTION_TYPE_ORTHOGRAPHIC:
			cam->projection = pvkMat4OrthoProj(heightOrAngle, aspectRatio, 1, 20);
			break;
		default:
			PVK_FETAL_ERROR("Unrecognized PvkProjectionType \"%d\"", projectionType);
	}
	return cam;
}
#endif


/* Lights */
typedef struct PvkAmbientLight
{
	PvkVec3 color;		// 12 bytes
	float intensity;	// 4 bytes
} PvkAmbientLight;

typedef struct PvkDirectionalLight
{
	PvkVec3 color;		// 12 bytes
	float intensity;	// 4 bytes
	PvkVec3 dir;		// 12 bytes
	float _;			// 4 bytes
} PvkDirectionalLight;

/* Global & Object Uniform Data */
typedef struct PvkGlobalData
{
	PvkMat4 projectionMatrix;		// 64 bytes
	PvkMat4 viewMatrix;				// 64 bytes
	PvkMat4 lightProjectionMatrix; 	// 64 bytes
	PvkMat4 lightViewMatrix; 		// 64 bytes
	PvkDirectionalLight dirLight;	// 32 bytes
	PvkAmbientLight ambLight;		// 16 bytes
} PvkGlobalData;					// total = 256 + 48 = 304 bytes

typedef struct PvkObjectData
{
	PvkMat4 modelMatrix;
	PvkMat4 normalMatrix;
} PvkObjectData;

#ifdef __cplusplus
}
#endif
