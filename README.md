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

* **pvkGetPhysicalDevice**: Finds a VkPhysicalDevice with the specified requirements
  * Param 1 (VkInstance): vulkan instance
  * Param 2 (VkSurfaceKHR): vulkan surface
  * Param 3 (VkPhysicalDeviceType): type of the physical device
  * Param 4 (VkFormat): surface format
  * Param 5 (VkColorSpace): color space
  * Param 6 (VkPresentModeKHR): present mode
  * Param 7 (uint32_t): number of swapchain images
  * Param 8 (bool): whether samplerYcbcrConversion is required? Enables
```C
	VkPhysicalDevice physicalGPU = pvkGetPhysicalDevice(instance, surface,
                                                     VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU, 
                                                     VK_FORMAT_B8G8R8A8_SRGB, 
                                                     VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, 
                                                     VK_PRESENT_MODE_FIFO_KHR, 3, false);
```

* **pvkFindQueueFamilyIndex**: Finds queuy family index with specified VkQueueFlagBits set
  * Param 1 (VkPhysicalDevice): vulkan physical device
  * Param 2 (VkQueueFlagBits): vulkan queue flag bits
```C
	uint32_t graphicsQueueFamilyIndex = pvkFindQueueFamilyIndex(physicalGPU, VK_QUEUE_GRAPHICS_BIT);
```

* **pvkFindQueueFamilyIndexWithPresentSupport**: Finds queue family index with present support on the specified surface
  * Param 1 (VkPhysicalDevice): vulkan physical device
  * Param 2 (VkSurfaceKHR): vulkan surface
```
	uint32_t presentQueueFamilyIndex = pvkFindQueueFamilyIndexWithPresentSupport(physicalGPU, surface);
```

## About Me
I'm a software engineer working on GPU development toolchains and GPU application development frameworks. 
You can always find me on [LinkedIn](https://www.linkedin.com/in/ravi-prakash-singh/), [Github](https://github.com/ravi688), or [Youtube](https://www.youtube.com/@phymacillustrator/videos).
Or just drop me an email at rp0412204@gmail.com
