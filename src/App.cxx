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

    /* 
VULKAN INITIALIZATION CHECKLIST
==============================

APP STRUCTURE
-------------
[x] Create App class
[x] Create run()
[x] Create initWindow()
[x] Create initVulkan()
[x] Create mainLoop()
[x] Create cleanup()


WINDOW SETUP
------------
[x] glfwInit()
[x] glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API)
[x] Create GLFW window


VALIDATION LAYERS
-----------------
[x] Define validationLayers array
[x] Check validation layer support
    - vkEnumerateInstanceLayerProperties()
[x] Enable validation layers in debug builds


INSTANCE EXTENSIONS
-------------------
[x] Get GLFW required extensions
    - glfwGetRequiredInstanceExtensions()
[x] Add VK_EXT_DEBUG_UTILS_EXTENSION_NAME if validation enabled
[x] Combine all required extensions into one list


VULKAN INSTANCE
---------------
[x] Create VkApplicationInfo
[x] Create VkInstanceCreateInfo
[x] Add required extensions
[x] Add validation layers
[x] vkCreateInstance()


DEBUG MESSENGER
---------------
[ ] Create debug callback function
[ ] Create VkDebugUtilsMessengerCreateInfoEXT
[ ] Dynamically load extension functions
[ ] Create debug messenger


SURFACE
-------
[ ] Create VkSurfaceKHR
    - glfwCreateWindowSurface()


PHYSICAL DEVICE SELECTION
-------------------------
[ ] Enumerate physical devices
    - vkEnumeratePhysicalDevices()
[ ] For each GPU:
    [ ] Check graphics queue support
    [ ] Check presentation queue support
    [ ] Check required device extensions
    [ ] Check swapchain support
[ ] Select suitable GPU


QUEUE FAMILIES
--------------
[ ] Find graphics queue family
[ ] Find presentation queue family
    - vkGetPhysicalDeviceQueueFamilyProperties()
    - vkGetPhysicalDeviceSurfaceSupportKHR()


LOGICAL DEVICE
--------------
[ ] Create queue create infos
[ ] Enable device extensions
    - VK_KHR_SWAPCHAIN_EXTENSION_NAME
[ ] vkCreateDevice()
[ ] Retrieve graphics queue
    - vkGetDeviceQueue()
[ ] Retrieve presentation queue
    - vkGetDeviceQueue()


SWAPCHAIN SUPPORT
-----------------
[ ] Query surface capabilities
    - vkGetPhysicalDeviceSurfaceCapabilitiesKHR()
[ ] Query surface formats
    - vkGetPhysicalDeviceSurfaceFormatsKHR()
[ ] Query present modes
    - vkGetPhysicalDeviceSurfacePresentModesKHR()


SWAPCHAIN
---------
[ ] Choose surface format
[ ] Choose present mode
[ ] Choose swap extent
[ ] Create VkSwapchainKHR
    - vkCreateSwapchainKHR()
[ ] Retrieve swapchain images
    - vkGetSwapchainImagesKHR()


IMAGE VIEWS
-----------
[ ] Create image view for each swapchain image
    - vkCreateImageView()


RENDER PASS
-----------
[ ] Define color attachment
[ ] Define subpass
[ ] Define dependency
[ ] Create render pass
    - vkCreateRenderPass()


GRAPHICS PIPELINE
-----------------
Shaders:
    [ ] Vertex shader
    [ ] Fragment shader
    [ ] Compile to SPIR-V

Pipeline:
    [ ] Shader stages
    [ ] Vertex input state
    [ ] Input assembly
    [ ] Viewport
    [ ] Scissor
    [ ] Rasterizer
    [ ] Multisampling
    [ ] Color blending
    [ ] Pipeline layout
    [ ] Create graphics pipeline


FRAMEBUFFERS
------------
[ ] Create framebuffer for each swapchain image view
    - vkCreateFramebuffer()


COMMAND POOL
------------
[ ] Create command pool
    - vkCreateCommandPool()


COMMAND BUFFERS
---------------
[ ] Allocate command buffers
    - vkAllocateCommandBuffers()
[ ] Record commands
    - vkBeginCommandBuffer()
    - vkCmdBeginRenderPass()
    - vkCmdBindPipeline()
    - vkCmdDraw()
    - vkEndCommandBuffer()


SYNCHRONIZATION
---------------
[ ] Create imageAvailableSemaphore
[ ] Create renderFinishedSemaphore
[ ] Create inFlightFence
    - vkCreateSemaphore()
    - vkCreateFence()


DRAW LOOP
---------
Every frame:

[ ] Wait for fence
    - vkWaitForFences()

[ ] Acquire swapchain image
    - vkAcquireNextImageKHR()

[ ] Submit command buffer
    - vkQueueSubmit()

[ ] Present image
    - vkQueuePresentKHR()


CLEANUP
-------
[ ] Destroy semaphores
[ ] Destroy fences
[ ] Destroy command pool
[ ] Destroy framebuffers
[ ] Destroy graphics pipeline
[ ] Destroy pipeline layout
[ ] Destroy render pass
[ ] Destroy image views
[ ] Destroy swapchain
[ ] Destroy logical device
[ ] Destroy debug messenger
[ ] Destroy surface
[ ] Destroy Vulkan instance
[ ] Destroy GLFW window
[ ] glfwTerminate()


MILESTONES
----------
[x] Window opens
[ ] Vulkan instance created
[ ] Validation layers working

[ ] Physical device selected
[ ] Logical device created

[ ] Swapchain created
[ ] Image views created

[ ] Graphics pipeline created

[ ] First triangle rendered

    */
