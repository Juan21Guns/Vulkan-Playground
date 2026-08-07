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
[x] Create image view for each swapchain image
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


Command Infrastructure
 x Identify graphics queue family
 x Create command pool
 Allocate primary command buffers
 Decide number of frames in flight
 Associate command buffers with frames in flight
Synchronization
 Create image-available semaphores
 Create render-finished semaphores
 Create in-flight fences
 Wait for frame fence before reusing frame resources
 Reset fence before submitting frame
 Use synchronization2 for queue submission/barriers
Begin Frame
 Acquire swapchain image
 Handle VK_ERROR_OUT_OF_DATE_KHR
 Handle VK_SUBOPTIMAL_KHR
 Reset/reuse the appropriate command buffer
 Begin command buffer recording
Swapchain Image → Rendering
 Transition swapchain image from PRESENT_SRC_KHR
 Transition to COLOR_ATTACHMENT_OPTIMAL
 Use an image memory barrier
 Set appropriate source/destination pipeline stages
 Set appropriate source/destination access masks
Dynamic Rendering Setup
 Enable dynamic rendering device feature
 Create VkRenderingAttachmentInfo
 Set swapchain image view
 Set COLOR_ATTACHMENT_OPTIMAL layout
 Choose color loadOp
 Choose color storeOp
 Set clear color if using CLEAR
 Set resolve mode to NONE unless using MSAA
 Create VkRenderingInfo
 Set render area
 Set layer count
 Set color attachment count
 Provide color attachments
Begin Rendering
 Begin dynamic rendering
 Bind graphics pipeline
 Set viewport
 Set scissor
 Bind vertex/index buffers when applicable
 Bind descriptor sets when applicable
 Push constants when applicable
 Issue draw commands
End Rendering
 End dynamic rendering
Rendering → Swapchain Present
 Transition swapchain image from COLOR_ATTACHMENT_OPTIMAL
 Transition to PRESENT_SRC_KHR
 Use an image memory barrier
 Set appropriate source/destination pipeline stages
 Set appropriate source/destination access masks
 End command buffer recording
Queue Submission
 Wait for image-available semaphore
 Submit command buffer
 Signal render-finished semaphore
 Signal in-flight fence
Presentation
 Present swapchain image
 Wait on render-finished semaphore
 Handle VK_ERROR_OUT_OF_DATE_KHR
 Handle VK_SUBOPTIMAL_KHR
Swapchain Recreation
 Wait for GPU/device as appropriate
 Destroy old swapchain image views
 Recreate swapchain
 Retrieve new swapchain images
 Recreate image views
 Update extent
 Update viewport/scissor as needed
 Update pipeline/rendering formats if necessary
 Reset/reinitialize swapchain-dependent resources
Explicitly Not Needed with Dynamic Rendering
 Create VkRenderPass
 Create VkFramebuffer
 Define subpasses
 Define render-pass dependencies
 vkCmdBeginRenderPass
 vkCmdEndRenderPass
 Render-pass attachment descriptions


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
[x] Destroy image views
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
