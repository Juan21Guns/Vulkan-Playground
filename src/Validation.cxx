#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Validation.hpp"

#include <vector>
#include <cstring>
#include <iostream>

static VkBool32 callbackFunc (
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void*                                       pUserData ) 
{
    const char* messageError; 
    switch (messageSeverity) {
        case 0x00000001: 
            messageError = "Verbose";
        case 0x00000010: 
            messageError = "Info";
        case 0x00000100:
            messageError = "Warning";
        case 0x00001000: 
            messageError = "Error";
    }

    const char* messageType;
    switch (messageTypes) {
        case 0x00000001: 
            messageType = "GENERAL";
        case 0x00000002: 
            messageType = "VALIDATION";
        case 0x00000004:
            messageType = "PERFORMANCE";
        case 0x00000008: 
            messageType = "DEVICE ADDRESS BINDING";
    }

    std::cerr << "Message Severity: " << messageError << std::endl
              << "Message Type: " << messageType << std::endl
              << "Callback Data: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

Validation::Validation() 
    : validationLayers {
        "VK_LAYER_KHRONOS_validation"
    }
{

}

Validation::~Validation() {
    if (debugMessenger != VK_NULL_HANDLE) { 
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        func(instance, debugMessenger, nullptr);
        std::cout << "Validation cleaning up" << std::endl;
    }
}

VkResult Validation::createDebugUtilsMsgr(VkInstance vInstance) {
    this->instance = vInstance;

    VkDebugUtilsMessengerCreateInfoEXT debugStruct {};
    debugStruct.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugStruct.flags = 0;
    debugStruct.messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugStruct.messageType = 
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    debugStruct.pfnUserCallback = callbackFunc;

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    
    VkResult result = func(
        instance,
        &debugStruct,
        NULL,
        &debugMessenger
    );

    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to initialize debug utils!");
    }

    return VK_SUCCESS;
};

const char* const* Validation::getValidationLayers() const {
    return validationLayers.data();
}

uint32_t Validation::getLayerCount() const {
    return static_cast<uint32_t>(validationLayers.size());
};

bool Validation::checkValidationSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const VkLayerProperties& property : availableLayers) {
            if (strcmp(property.layerName, layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}