#pragma once

#include <memory>

#include "Window.hpp"
#include "VulkanContext.hpp"

class App {
    public:
        //constructor
        App();

        //destructor
        ~App();

        //copy constructor forbidden
        App(const App&) = delete;

        //copy assignment operator forbidden
        App& operator=(const App&) = delete;

        void run();
        
    private:
        void mainLoop();
        void cleanup();

        std::unique_ptr<Window> window;
        std::unique_ptr<VulkanContext> vk;
};