#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Device {
    public:
        bool setupDevice(); 
        Device(VkSurfaceKHR* sf, VkInstance* is);
        ~Device();
    
    private:
        bool createDevice(uint32_t& qfCount, const VkPhysicalDevice& pd, std::vector<const char*>& requiredDeviceExtensions);
        bool getPhysicalDeviceSupport(VkPhysicalDevice pd, uint32_t i);
        bool checkDeviceExtensions(VkPhysicalDevice physicalDevice, const std::vector<const char*>& extensions);

        VkInstance* instance;
        VkSurfaceKHR* surface;
        VkQueue pQueue;
        VkDevice device;
        uint32_t graphicsQueueFamilyIndex;
        VkPhysicalDevice vkpd;
};