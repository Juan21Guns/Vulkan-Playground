#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Device.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <optional>
#include <set>

Device::Device(VkSurfaceKHR sf, VkInstance is) : surface(sf), instance(is) {
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

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("No devices found!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    VkResult err = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()); 

    if (err < 0) {
        throw std::runtime_error("Failed to gather physical devices!");
    }

    for (auto const& pd : devices) {
        if (!checkDeviceExtensions(pd, requiredDeviceExtensions)) {
            continue;
        }

        queueStruct tempStruct{};
        uint32_t queueCount = 0;

        vkGetPhysicalDeviceQueueFamilyProperties(pd, &queueCount, nullptr);
        std::vector<VkQueueFamilyProperties> qProperties(queueCount);
        vkGetPhysicalDeviceQueueFamilyProperties(pd, &queueCount, qProperties.data());

        for (uint32_t i = 0; i < qProperties.size(); i++) 
        {
            //SELECTING GRAPHICS QUEUE
            if ((!tempStruct.graphicsQueue.has_value()) && (qProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                tempStruct.graphicsQueue = i;
            }

            // SELECTING PRESENTATION QUEUE
            if ((!tempStruct.presentationQueue.has_value()) && getPhysicalDeviceSupport(pd, i)) {
                tempStruct.presentationQueue = i;
            }

            if (tempStruct.isComplete()) {
                break;
            }
        }

        if (tempStruct.isComplete()) {
            if (createDevice(tempStruct, pd, requiredDeviceExtensions)) {
                queueIndiceStruct = tempStruct;
                return true;
            }
        }
    }

    return false;
}

//THIS IS LOGICAL DEVICE
bool Device::createDevice(queueStruct qfCount, const VkPhysicalDevice& pd, std::vector<const char*>& requiredDeviceExtensions) {
    float queuePriority = 1.0f;

    std::set<uint32_t> uniqueQueues = {
        qfCount.graphicsQueue.value(),
        qfCount.presentationQueue.value()
    }; 

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    for (const auto uq : uniqueQueues) {
        VkDeviceQueueCreateInfo dqci{};
        dqci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        dqci.queueFamilyIndex = uq;
        dqci.queueCount = 1;
        dqci.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(dqci);
    }

    VkDeviceCreateInfo dci{};
    dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    dci.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    dci.pQueueCreateInfos = queueCreateInfos.data();
    dci.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtensions.size());
    dci.ppEnabledExtensionNames = requiredDeviceExtensions.data();

    VkResult createDevice = vkCreateDevice(pd, &dci, nullptr, &device);
    if (createDevice != VK_SUCCESS) {
        throw std::runtime_error("failed to create device!");
    } else {
        std::cout << "created device!" << std::endl;  
        vkpd = pd;
        
        vkGetDeviceQueue(device, qfCount.graphicsQueue.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, qfCount.presentationQueue.value(), 0, &presentationQueue);

        return true; 
    }
}

bool Device::getPhysicalDeviceSupport(VkPhysicalDevice pd, uint32_t i) {
    VkBool32 pSupported = VK_FALSE;

    VkResult getSupport = vkGetPhysicalDeviceSurfaceSupportKHR(pd, i, surface, &pSupported);

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

VkPhysicalDevice Device::getPhysDevice() {
    return vkpd;
}

VkDevice Device::getDevice() {
    return device;
}

queueStruct Device::getQueueIndices() {
    return queueIndiceStruct;
};

uint32_t Device::getGraphicsQueueIndex() {
    if (queueIndiceStruct.graphicsQueue.has_value()) {
        return queueIndiceStruct.graphicsQueue.value();
    } 

    throw std::runtime_error("no queue index found!");
}

uint32_t Device::getPresentQueueIndex() {
    if (queueIndiceStruct.presentationQueue.has_value()) {
        return queueIndiceStruct.presentationQueue.value();
    }

    throw std::runtime_error("no queue index found!");
}