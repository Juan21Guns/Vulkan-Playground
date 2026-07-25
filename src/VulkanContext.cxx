#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanContext.hpp"
#include "Validation.hpp"
#include "Window.hpp"

#include <vector>
#include <stdexcept>
#include <memory>
#include <cstring>

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

VulkanContext::~VulkanContext() {
        //cleanup in reverse order
    if (device) {
        vkDestroyDevice(device, nullptr);
    }

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

bool VulkanContext::setupDevices() {
    std::vector<const char*> requiredDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    uint32_t deviceCount = 0; 

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> devices(deviceCount);
    VkResult err = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()); 

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
                };
            }
            
            qfCount++;
        }
    }

    return false;
}

bool VulkanContext::getPhysicalDeviceSupport(VkPhysicalDevice pd, uint32_t i) {
    VkBool32 pSupported = VK_FALSE;

    VkResult getSupport = vkGetPhysicalDeviceSurfaceSupportKHR(pd, i, surface, &pSupported);

    if (getSupport != VK_SUCCESS) {
        return false;
    }

    return pSupported == VK_TRUE;
}

bool VulkanContext::checkDeviceExtensions(VkPhysicalDevice physicalDevice, const std::vector<const char*>& extensions) {
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