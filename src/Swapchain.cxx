#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Swapchain.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <algorithm>

// [x] Choose surface format
// [x] Choose present mode
// [x] Choose swap extent
// [ ] Create VkSwapchainKHR
//     - vkCreateSwapchainKHR()
// [ ] Retrieve swapchain images
//     - vkGetSwapchainImagesKHR()

Swapchain::Swapchain(VkDevice device, queueStruct queueIndices, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, Window& window) 
: device(device), queueIndices(queueIndices), surface(surface), physicalDevice(physicalDevice), window(window) {
    isSwapchainAccessible();
    createSwapchain();
    getSwapchainImages();
}

Swapchain::~Swapchain() {
    if (pSwapchain) {
        std::cout << "cleaning up swapchain" << std::endl;
        vkDestroySwapchainKHR(device, pSwapchain, nullptr);
    }
}

bool Swapchain::isSwapchainAccessible() {
    //physical device surface capabilities
    if (
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &pSurfaceCapabilities) 
        != VK_SUCCESS
    ) {
        throw std::runtime_error("Failed to get surface capabilities!");
    }

    //physical device surface formats
    uint32_t surfaceCounts = 0;

    if (
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceCounts, nullptr)
        != VK_SUCCESS
    ) {
        throw std::runtime_error("Failed to get surface capabilities!");
    } 
    
    pSurfaceFormats.resize(surfaceCounts);

    if (
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceCounts, pSurfaceFormats.data())
        != VK_SUCCESS
    ) {
        throw std::runtime_error("Failed to gather surface formats!");
    }
    

    //present modes
    
    uint32_t presentModeCount = 0;

    if (
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr)
        != VK_SUCCESS
    ) {
        throw std::runtime_error("Failed to gather surface present modes!");
    } 
    
    pPresentModes.resize(presentModeCount);

    if (
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, pPresentModes.data())
        != VK_SUCCESS
    ) {
        throw std::runtime_error("Failed to gather surface present modes in array!");
    } 

    return true;
}

void Swapchain::createSwapchain() {
    chooseSurfaceFormat();
    choosePresentMode();

    bool qSame = queueIndices.graphicsQueue.value() == queueIndices.presentationQueue.value();

    VkSwapchainCreateInfoKHR sci{};
    sci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    sci.pNext = nullptr;
    sci.flags = 0;
    sci.surface = surface;
    sci.minImageCount = chooseMinImageCount();
    sci.imageFormat = surfaceFormat.format;
    sci.imageColorSpace = surfaceFormat.colorSpace;
    sci.imageExtent = chooseSwapExtent();
    sci.imageArrayLayers = 1;
    sci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (qSame) {
        sci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        sci.queueFamilyIndexCount = 0;
        sci.pQueueFamilyIndices = nullptr;
    } else {
        uint32_t queueFamilyIndices[] {
            queueIndices.graphicsQueue.value(),
            queueIndices.presentationQueue.value()
        };

        sci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        sci.queueFamilyIndexCount = 2;
        sci.pQueueFamilyIndices = queueFamilyIndices; 
    }

    sci.preTransform = pSurfaceCapabilities.currentTransform;
    sci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    sci.presentMode = presentMode;
    sci.clipped = VK_TRUE;
    sci.oldSwapchain = VK_NULL_HANDLE;
    
    VkResult err = vkCreateSwapchainKHR(device, &sci, nullptr, &pSwapchain);
    if (err != VK_SUCCESS) {
        throw std::runtime_error("failed to create swapchain!");
    } 

    std::cout << "Created swapchain!" << std::endl;
}

VkSurfaceFormatKHR Swapchain::chooseSurfaceFormat() {
    //surface format: for beginning purposes will look for: 
    // VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
    // VK_FORMAT_R8G8B8A8_SRGB

    if (pSurfaceFormats.size() == 0) {
        throw std::runtime_error("failed to get any surface formats!");
    }

    //VkSurfaceFormatKHR
    for (const auto pm : pSurfaceFormats) {
        if (pm.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR && pm.format == VK_FORMAT_B8G8R8A8_SRGB) {
            surfaceFormat = pm;
            return surfaceFormat;
        }
    }

    surfaceFormat = pSurfaceFormats[0];
    return surfaceFormat;
};

VkPresentModeKHR Swapchain::choosePresentMode() {
    // PRESENT MODE
    if (pPresentModes.size() == 0) {
        throw std::runtime_error("No present modes to queue through!");
    }
    
    //VkPresentModeKHR
    for (const auto pm : pPresentModes) {
        if (pm == VK_PRESENT_MODE_MAILBOX_KHR) {
            presentMode = pm;
            break;
        }
    }

    return presentMode;
};

VkExtent2D Swapchain::chooseSwapExtent() {
    if ((pSurfaceCapabilities.currentExtent.width == UINT32_MAX)
        && (pSurfaceCapabilities.currentExtent.height == UINT32_MAX)    
    ) {
        VkExtent2D glfwWindow = window.getFrameBufferSize();
        uint32_t clampedExtentHeight = std::clamp(glfwWindow.height, pSurfaceCapabilities.minImageExtent.height, pSurfaceCapabilities.maxImageExtent.height);
        uint32_t clampedExtentWidth = std::clamp(glfwWindow.width, pSurfaceCapabilities.minImageExtent.width, pSurfaceCapabilities.maxImageExtent.width);

        return {clampedExtentWidth, clampedExtentHeight};
    }

    return pSurfaceCapabilities.currentExtent;
};

uint32_t Swapchain::chooseMinImageCount() {
    // HALF-ASSED LOGIC, REDO LATER
    if (pSurfaceCapabilities.maxImageCount != 0) {
        uint32_t midCount = pSurfaceCapabilities.minImageCount + 1; 

        if (midCount >= pSurfaceCapabilities.maxImageCount) {
            return pSurfaceCapabilities.minImageCount;
        } else {
            return midCount;
        }

    } else {
        return pSurfaceCapabilities.minImageCount + 1;
    }
}

void Swapchain::getSwapchainImages() {
    uint32_t imageCount = 0;

    vkGetSwapchainImagesKHR(device, pSwapchain, &imageCount, nullptr);
    
    if (imageCount == 0) {
        throw std::runtime_error("no images on swapchain!");
    }

    pSwapchainImages.resize(imageCount);

    VkResult err = vkGetSwapchainImagesKHR(device, pSwapchain, &imageCount, pSwapchainImages.data());

    if (err != VK_SUCCESS) {
        throw std::runtime_error("failed to get swapchain images!");
    }
}