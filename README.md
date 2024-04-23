# PlayVk

[![C/C++ CI](https://github.com/ravi688/PlayVk/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/ravi688/PlayVk/actions/workflows/c-cpp.yml)

PlayVk is a single header file library for simplifying vulkan objects creation, initializing vulkan, and rendering basic geometries. <br><br>

<a href="https://www.buymeacoffee.com/raviprakashsingh" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="41" width="174"></a>

## Documentation

### Functions
* **pvkCreateVulkanInstanceWithExtensions**: Creates VkInstance instance
  * Param 1 (integer): is the number of extensions we require
  * Param 2 (variadic): is a variadic argument consisting of list of `const char*` strings representing the extensions required.
```C
VkInstance instance = pvkCreateVulkanInstanceWithExtensions(2, "VK_KHR_win32_surface", "VK_KHR_surface");
````

* **pvkWindowCreate**: Creates Win32 Window
  * Param 1 (integer): width of the window
  * Param 2 (integer): height of the window
  * Param 3 (const char*): Name of the window (which is displayed in the title bar)
  * Param 4 (bool): whether the window need to be created in full screen mode
  * Param 5 (bool): whether the window should be resizable
```C
PvkWindow* window = pvkWindowCreate(800, 800, "Vulkan Multipass Rendering", false, true);
```

* **pvkWindowCreateVulkanSurface**: Creates Vulkan surface (VkSurfaceKHR)
  * Param 1 (PvkWindow*): pointer to PvkWindow object, created with `pvkWindowCreate` function
  * Param 2 (VkInstance): vulkan instance object
```C
VkInstance instance = ...
PvkWindow* window = ...
VkSurfaceKHR surface = pvkWindowCreateVulkanSurface(window, instance);
```
