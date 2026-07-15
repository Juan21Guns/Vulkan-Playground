#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "App.hpp"
#include "Window.hpp"
#include "VulkanContext.hpp"

#include <stdexcept>
#include <memory>
#include <iostream>

App::App() 
{
}

App::~App() {
    std::cout << "cleaning up" << std::endl;
    cleanup();
}

void App::run() {
    if (!glfwInit()){
        throw std::runtime_error("Failed to initialize GLFW!");
    };

    window = std::make_unique<Window>();
    vk = std::make_unique<VulkanContext>(*window);
    vk->initVulkan();
    mainLoop();
}

void App::cleanup() {
    //will destroy VulkanContext
                // Validation
                // Instance
    vk.reset();

    glfwTerminate();
    std::cout << "GLFW cleaning up" << std::endl;
}

void App::mainLoop() {
    while (!glfwWindowShouldClose(window->getWindow())){
        glfwPollEvents();       
    }
}
