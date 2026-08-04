#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include "Window.hpp"
#include "Device.hpp"

class Swapchain {
    public:
        Swapchain(VkDevice device, queueStruct queueIndices, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, Window& window);
        ~Swapchain();
        
        void createSwapchain();
        void getSwapchainImages();
        void createImageViews();

    private:
        bool isSwapchainAccessible();

        Window& window;

        VkSwapchainKHR pSwapchain;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        queueStruct queueIndices;

        VkSurfaceKHR surface;
        VkSurfaceCapabilitiesKHR pSurfaceCapabilities;

        std::vector<VkSurfaceFormatKHR> pSurfaceFormats;
        std::vector<VkPresentModeKHR> pPresentModes;
        std::vector<VkImage> pSwapchainImages;
        std::vector<VkImageView> pImageViews;

        VkSurfaceFormatKHR surfaceFormat{};
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

        VkSurfaceFormatKHR chooseSurfaceFormat();
        VkPresentModeKHR choosePresentMode();
        VkExtent2D chooseSwapExtent();
        uint32_t chooseMinImageCount();
};