#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "App.hpp"

int main() {
    App app;
    app.run();
    return 0;
}