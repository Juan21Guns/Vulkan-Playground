#pragma once 

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "Window.hpp"
#include "Validation.hpp"
#include "Device.hpp"

class VulkanContext { 
    public: 
        VulkanContext(Window& window);
        ~VulkanContext();

    private:
        Window& wnd;
        VkInstance instance{};
    
        std::unique_ptr<Validation> validation;
        std::unique_ptr<Device> device;

        VkSurfaceKHR surface;
};