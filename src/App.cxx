#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "App.hpp"
#include "Window.hpp"
#include "Validation.hpp"

#include <stdexcept>
#include <memory>
#include <iostream>

#ifdef NDEBUG
    constexpr bool enableValidationLayer = false;
#else 
    constexpr bool enableValidationLayer = true;
#endif

App::App() 
{
    if (enableValidationLayer) {
        validation = std::make_unique<Validation>();
        if (!validation->checkValidationSupport()) {
            throw std::runtime_error("Validation layers unavailable");
        }
    }
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
    initVulkan();
    mainLoop();
}

void App::cleanup() {
    //cleanup in reverse order
    if (validation) {
        validation.reset();
    }
    
    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
        std::cout << "Instance cleaning up" << std::endl;
    }

    glfwTerminate();
    std::cout << "GLFW cleaning up" << std::endl;
}

void App::initVulkan() {
    uint32_t count = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + count);
    //IF DEBUG
    if (enableValidationLayer) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    } 

    //APPLICATION INFO
    VkApplicationInfo ai {};
        ai.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; 
        ai.pApplicationName = "Hello Triangle Playground";
        ai.applicationVersion = VK_MAKE_API_VERSION(0,1,0,0);
        ai.pEngineName = "Bebo Engine";
        ai.engineVersion = 1;
        ai.apiVersion = VK_API_VERSION_1_3;

    //INSTANCE CREATE INFO
    VkInstanceCreateInfo ici {};
    ici.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ici.pApplicationInfo = &ai;
    ici.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    ici.ppEnabledExtensionNames = extensions.data();

    if (enableValidationLayer) {
        ici.enabledLayerCount = validation->getLayerCount();
        ici.ppEnabledLayerNames = validation->getValidationLayers();
    }

    VkResult result = vkCreateInstance(&ici, nullptr, &instance);
    
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance!");
    }

    validation->createDebugUtilsMsgr(instance);
}

void App::mainLoop() {
    while (!glfwWindowShouldClose(window->getWindow())){
        glfwPollEvents();       
    }
}
