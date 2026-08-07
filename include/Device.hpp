#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional> 

struct queueStruct {
    std::optional<uint32_t> graphicsQueue; 
    std::optional<uint32_t> presentationQueue;

    bool isComplete() {
        return (graphicsQueue.has_value() && presentationQueue.has_value());
    }
};

class Device {
    public:
        bool setupDevice(); 
        Device(VkSurfaceKHR sf, VkInstance is);
        ~Device();

        VkDevice getDevice();
        VkPhysicalDevice getPhysDevice();
        queueStruct getQueueIndices();
        uint32_t getGraphicsQueueIndex();
        uint32_t getPresentQueueIndex();
    
    private:
        bool createDevice(queueStruct qfCount, const VkPhysicalDevice& pd, std::vector<const char*>& requiredDeviceExtensions);
        bool getPhysicalDeviceSupport(VkPhysicalDevice pd, uint32_t i);
        bool checkDeviceExtensions(VkPhysicalDevice physicalDevice, const std::vector<const char*>& extensions);

        VkInstance instance;
        VkSurfaceKHR surface;
        VkQueue graphicsQueue;
        VkQueue presentationQueue;
        VkDevice device;

        VkPhysicalDevice vkpd;

        queueStruct queueIndiceStruct {}; 
};