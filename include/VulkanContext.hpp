#pragma once 

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "Window.hpp"
#include "Validation.hpp"
#include "Device.hpp"
#include "Swapchain.hpp"

class VulkanContext { 
    public: 
        VulkanContext(Window& window);
        ~VulkanContext();

    private:
        Window& wnd;
        VkInstance instance{};

        void createCommandPool(uint32_t graphicsQueueIndex);
        void allocCommandBuffer();

        std::unique_ptr<Validation> validation;
        std::unique_ptr<Device> device;
        std::unique_ptr<Swapchain> swapchain;
        std::vector<VkCommandPool> pCommandPool;
        std::vector<VkCommandBuffer> pCommandBuffers;

        VkSurfaceKHR surface;
        const uint32_t MAX_FRAMES_IN_FLIGHT = 2;
};