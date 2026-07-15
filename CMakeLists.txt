cmake_minimum_required(VERSION 3.23)

project(VulkanPlayground)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(HelloTriangle
  src/main.cxx 
  src/App.cxx
  src/Window.cxx
  src/Validation.cxx
  src/VulkanContext.cxx
)

target_include_directories(HelloTriangle
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(HelloTriangle
  PRIVATE 
    glfw
    Vulkan::Vulkan
)