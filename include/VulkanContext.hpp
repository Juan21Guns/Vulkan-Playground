#pragma once 

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "Window.hpp"
#include "Validation.hpp"

class VulkanContext { 
    public: 
        VulkanContext(Window& window);
        ~VulkanContext();
        void initVulkan(); 

    private:
        Window wnd;
        VkInstance instance{};
        std::unique_ptr<Validation> validation;
        VkSurfaceKHR surface;
};