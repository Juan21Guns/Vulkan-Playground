#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Device.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>

Device::Device(VkSurfaceKHR* sf, VkInstance* is) : surface(sf), instance(is) {
    if (!instance || !surface) {
        throw std::runtime_error("instance or surface not created!");
    }
}

Device::~Device() {
    if (device) {
        vkDestroyDevice(device, nullptr);
    }
}

bool Device::setupDevice() {
    std::vector<const char*> requiredDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    uint32_t deviceCount = 0; 

    vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("No devices found!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    VkResult err = vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data()); 

    if (err < 0) {
        throw std::runtime_error("Failed to gather physical devices!");
    }

    for (auto const& pd : devices) {
        uint32_t queueCount = 0;
        uint32_t qfCount = 0;

        vkGetPhysicalDeviceQueueFamilyProperties(pd, &queueCount, nullptr);
        std::vector<VkQueueFamilyProperties> qProperties(queueCount);
        vkGetPhysicalDeviceQueueFamilyProperties(pd, &queueCount, qProperties.data());

        for (auto const& qp : qProperties) {
            // //001, 010, 100   111 
            // // 111
            // bool qCompare = (qp.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT)) == (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT);
            bool qCompare = qp.queueFlags & VK_QUEUE_GRAPHICS_BIT;
            if (qCompare) {
                if (checkDeviceExtensions(pd, requiredDeviceExtensions) && getPhysicalDeviceSupport(pd, qfCount)) {
                    if (createDevice(qfCount, pd, requiredDeviceExtensions)) {
                        return true;
                    }
                };
            }
            
            qfCount++;
        }
    }

    return false;
}

bool Device::createDevice(uint32_t& qfCount, const VkPhysicalDevice& pd, std::vector<const char*>& requiredDeviceExtensions) {
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo dqci{};
    dqci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    dqci.queueFamilyIndex = qfCount;
    dqci.queueCount = 1;
    dqci.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo dci{};
    dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    dci.queueCreateInfoCount = 1;
    dci.pQueueCreateInfos = &dqci;
    dci.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtensions.size());
    dci.ppEnabledExtensionNames = requiredDeviceExtensions.data();

    VkResult createDevice = vkCreateDevice(pd, &dci, nullptr, &device);
    if (createDevice != VK_SUCCESS) {
        throw std::runtime_error("failed to create device!");
    } else {
        std::cout << "created device!" << std::endl;  
        graphicsQueueFamilyIndex = qfCount;
        vkpd = pd;
        vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &pQueue);
        return true; 
    }
}

bool Device::getPhysicalDeviceSupport(VkPhysicalDevice pd, uint32_t i) {
    VkBool32 pSupported = VK_FALSE;

    VkResult getSupport = vkGetPhysicalDeviceSurfaceSupportKHR(pd, i, *surface, &pSupported);

    if (getSupport != VK_SUCCESS) {
        return false;
    }

    return pSupported == VK_TRUE;
}

bool Device::checkDeviceExtensions(VkPhysicalDevice physicalDevice, const std::vector<const char*>& extensions) {
    uint32_t propertyCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &propertyCount, nullptr);
    std::vector<VkExtensionProperties> pProperties(propertyCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &propertyCount, pProperties.data());

    for (auto const deviceExtension : extensions) {
        bool extensionFound = false;
        for (const auto& pp : pProperties) {
            if (std::strcmp(deviceExtension, pp.extensionName) == 0) {
                extensionFound = true;
                break;
            }
        }

        if (!extensionFound) {
            return false;
        }
    }

    return true;
}
