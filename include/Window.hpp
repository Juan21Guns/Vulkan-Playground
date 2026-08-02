#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window {
    public:
        Window();
        ~Window();

        GLFWwindow* getWindow() const;
        VkExtent2D getFrameBufferSize();

    private:
        GLFWwindow* window;
        VkSurfaceKHR surface;
};