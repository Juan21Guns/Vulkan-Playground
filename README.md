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
[x] Create debug callback function
[x] Create VkDebugUtilsMessengerCreateInfoEXT
[x] Dynamically load extension functions
[x] Create debug messenger


SURFACE
-------
[x] Create VkSurfaceKHR
    - glfwCreateWindowSurface()


PHYSICAL DEVICE SELECTION
-------------------------
[x] Enumerate physical devices
    - vkEnumeratePhysicalDevices()
[x] For each GPU:
    [x] Check graphics queue support
    [x] Check presentation queue support
    [x] Check required device extensions
    [x] Check swapchain support
[x] Select suitable GPU


QUEUE FAMILIES
--------------
[x] Find graphics queue family
[x] Find presentation queue family
    - vkGetPhysicalDeviceQueueFamilyProperties()
    - vkGetPhysicalDeviceSurfaceSupportKHR()


LOGICAL DEVICE
--------------
[x] Create queue create infos
[x] Enable device extensions
    - VK_KHR_SWAPCHAIN_EXTENSION_NAME
[x] vkCreateDevice()
[x] Retrieve graphics queue
    - vkGetDeviceQueue()
[x] Retrieve presentation queue
    - vkGetDeviceQueue()


SWAPCHAIN SUPPORT
-----------------
[x] Query surface capabilities
    - vkGetPhysicalDeviceSurfaceCapabilitiesKHR()
[x] Query surface formats
    - vkGetPhysicalDeviceSurfaceFormatsKHR()
[x] Query present modes
    - vkGetPhysicalDeviceSurfacePresentModesKHR()


SWAPCHAIN
---------
[x] Choose surface format
[x] Choose present mode
[x] Choose swap extent
[x] Create VkSwapchainKHR
    - vkCreateSwapchainKHR()
[x] Retrieve swapchain images
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
[x] Destroy swapchain
[x] Destroy logical device
[x] Destroy debug messenger
[x] Destroy surface
[x] Destroy Vulkan instance
[x] Destroy GLFW window
[x] glfwTerminate()


MILESTONES
----------
[x] Window opens
[x] Vulkan instance created
[x] Validation layers working

[x] Physical device selected
[x] Logical device created

[x] Swapchain created
[ ] Image views created

[ ] Graphics pipeline created

[ ] First triangle rendered
