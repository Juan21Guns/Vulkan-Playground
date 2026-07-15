#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanContext.hpp"
#include "Validation.hpp"
#include "Window.hpp"

#include <vector>
#include <stdexcept>
#include <memory>

#ifdef NDEBUG
    constexpr bool enableValidationLayer = false;
#else 
    constexpr bool enableValidationLayer = true;
#endif

VulkanContext::VulkanContext(Window& window) : wnd(window) 
{

    if (enableValidationLayer) {
        validation = std::make_unique<Validation>();
        if (!validation->checkValidationSupport()) {
            throw std::runtime_error("Validation layers unavailable");
        }
    }
}

VulkanContext::~VulkanContext() {
        //cleanup in reverse order
    std::cout << "Vulkan Context cleaning up" << std::endl;
    if (validation) {
        validation.reset();
    }
    
    if (instance != VK_NULL_HANDLE) {
        if (surface) {
            vkDestroySurfaceKHR(instance, surface, nullptr);
        }
        vkDestroyInstance(instance, nullptr);
        std::cout << "Instance cleaning up" << std::endl;
    }
}

void VulkanContext::initVulkan() {
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
    } else {
        std::cout << "instance created" << std::endl;
    }

    if (enableValidationLayer) {
        std::cout << "Debug messenger created" << std::endl;
        validation->createDebugUtilsMsgr(instance);
    }

    VkResult err = glfwCreateWindowSurface(instance, wnd.getWindow(), nullptr, &surface);
    if (err) {
        throw std::runtime_error("Failed to create window surface!");
    }
}