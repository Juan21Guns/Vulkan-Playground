#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Window.hpp"

#include <stdexcept>
#include <iostream>

Window::Window() {
    //redundant, but in glfw docs so including
    if (glfwVulkanSupported()) {
        std::cout << "creating window ... " << std::endl;

        // Vulkan is available, at least for compute
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(640, 480, "Window Title", NULL, NULL);
    
        if (!window) {
            std::cout << "window creation failed!" << std::endl;
            throw std::runtime_error("Failed to create GLFW Window!");
        }

        std::cout << "window created successfully" << std::endl;

        //glfwCreateWindowSurface later after creating instance
    } else {
        throw std::runtime_error("Vulkan is unavailable!");
    }
}

Window::~Window() {
    if (window) {
        glfwDestroyWindow(window);
    }
}

GLFWwindow* Window::getWindow() const {
    return window;
}