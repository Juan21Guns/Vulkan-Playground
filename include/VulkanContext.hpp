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

        bool setupDevices();

    private:
        Window wnd;
        VkInstance instance{};
        std::unique_ptr<Validation> validation;
        VkSurfaceKHR surface;
        uint32_t graphicsQueueFamilyIndex;
        VkQueue pQueue;
        VkDevice device;
        VkPhysicalDevice vkpd;

        bool getPhysicalDeviceSupport(VkPhysicalDevice pd, uint32_t i);
        bool checkDeviceExtensions(VkPhysicalDevice physicalDevice, const std::vector<const char*>& extensions);
};