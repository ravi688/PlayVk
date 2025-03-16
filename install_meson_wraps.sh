#! /usr/bin/bash

if [ ! -f 'subprojects/glfw.wrap' ]; then
	meson wrap install glfw
fi
if [ ! -f 'subprojects/vulkan-headers.wrap' ]; then
	meson wrap install vulkan-headers
fi
