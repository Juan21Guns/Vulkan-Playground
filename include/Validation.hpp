#pragma once

#include <vector>

class Validation {
    public: 
        Validation();
        ~Validation();

        bool checkValidationSupport(); 
        const char* const* getValidationLayers() const;
        uint32_t getLayerCount() const;
        VkResult createDebugUtilsMsgr(VkInstance vInstance); 

    private:
        VkInstance instance = VK_NULL_HANDLE;
        const std::vector<const char*> validationLayers;
        VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
};