
#include <PlayVk/PlayVk.h>
#include <bufferlib/buffer.h>

#define FENCE_WAIT_TIME 5 /* nano seconds */

static VkRenderPass pvkCreateShadowMapRenderPass(VkDevice device)
{
	// depth attachment only
	VkAttachmentDescription depthAttachment = 
	{
		.format = VK_FORMAT_D32_SFLOAT,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,			// we need to access it after rendering
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
	};

	VkAttachmentReference depthAttachmentReference = 
	{
		.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	};

	VkSubpassDescription subpass = 
	{
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.pDepthStencilAttachment = &depthAttachmentReference,
		.colorAttachmentCount = 0
	};

	VkSubpassDependency* dependencies = newv(VkSubpassDependency, 2);

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkRenderPassCreateInfo cInfo = 
	{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.attachmentCount = 1,
		.pAttachments = &depthAttachment,
		.subpassCount = 1,
		.pSubpasses = &subpass,
		.dependencyCount = 2,
		.pDependencies = dependencies
	};

	VkRenderPass renderPass;
	PVK_CHECK(vkCreateRenderPass(device, &cInfo, NULL, &renderPass));
	// delete(dependencies);	
	return renderPass;
}

static VkSampler pvkCreateShadowMapSampler(VkDevice device)
{
	VkSamplerCreateInfo cInfo = 
	{
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.magFilter = VK_FILTER_LINEAR,
		.minFilter = VK_FILTER_LINEAR,
		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.mipLodBias = 0,
		.anisotropyEnable = VK_FALSE,
		.maxAnisotropy = 1.0f, // Optional
		.compareEnable = VK_FALSE,
		.compareOp = VK_COMPARE_OP_ALWAYS,	// Optional
		.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.mipLodBias = 0.0f,
		.minLod = 0.0f,
		.maxLod = 1.0f
	};

	VkSampler sampler;
	PVK_CHECK(vkCreateSampler(device, &cInfo, NULL, &sampler));
	return sampler;
}

static VkRenderPass pvkCreateRenderPass2(VkDevice device)
{
	VkAttachmentDescription depthAttachment = 
	{
		.format = VK_FORMAT_D32_SFLOAT,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	};

	VkAttachmentReference depthAttachmentReference = 
	{
		.attachment = 2,
		.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	};

	/* Attachment for the sub pass 1 */
	VkAttachmentDescription colorAttachment1 = 
	{
		.format = VK_FORMAT_B8G8R8A8_SRGB,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	/* Attachment for the sub pass 2 */
	VkAttachmentDescription colorAttachment2 = 
	{
		.format = VK_FORMAT_B8G8R8A8_SRGB,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};

	VkAttachmentReference attachmentReference1 = 
	{
		.attachment = 1,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	VkAttachmentReference inputAttachmentReference2 = 
	{
		.attachment = 1,
		.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	};

	VkAttachmentReference attachmentReference2 = 
	{
		.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	VkSubpassDescription subpass1 = 
	{
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount = 1,
		.pColorAttachments = &attachmentReference1,
		.pDepthStencilAttachment = &depthAttachmentReference
	};

	VkSubpassDescription subpass2 = 
	{
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount = 1,
		.pColorAttachments = &attachmentReference2,
		.inputAttachmentCount = 1,
		.pInputAttachments = &inputAttachmentReference2,
		.pDepthStencilAttachment = &depthAttachmentReference
	};		
	VkSubpassDescription* subpasses = newv(VkSubpassDescription, 2);
	subpasses[0] = subpass1;
	subpasses[1] = subpass2;

	VkAttachmentDescription attachments[3] = { colorAttachment2, colorAttachment1, depthAttachment };

	VkSubpassDependency* dependencies = newv(VkSubpassDependency, 3);

	// dependency 1
	dependencies[0].srcSubpass = 0;
	dependencies[0].dstSubpass = 1;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	VkRenderPassCreateInfo cInfo = 
	{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.attachmentCount = 3, 
		.pAttachments = &attachments[0],
		.subpassCount = 2,
		.pSubpasses = subpasses,
		.dependencyCount = 1,
		.pDependencies = dependencies
	};
	VkRenderPass renderPass;
	PVK_CHECK(vkCreateRenderPass(device, &cInfo, NULL, &renderPass));
	delete(dependencies);
	delete(subpasses);
	return renderPass;
}

static VkDescriptorSetLayout pvkCreateShadowMapDescriptorSetLayout(VkDevice device)
{
	VkDescriptorSetLayoutBinding binding = 
	{
		.binding = 3,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT
	};

	VkDescriptorSetLayoutCreateInfo cInfo = 
	{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = 1,
		.pBindings = &binding
	};

	VkDescriptorSetLayout setLayout;
	PVK_CHECK(vkCreateDescriptorSetLayout(device, &cInfo, NULL, &setLayout));
	return setLayout;
}

static VkDescriptorSetLayout pvkCreateDescriptorSetLayout(VkDevice device)
{
	VkDescriptorSetLayoutBinding binding = 
	{
		.binding = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
	};

	VkDescriptorSetLayoutCreateInfo cInfo = 
	{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = 1,
		.pBindings = &binding
	};

	VkDescriptorSetLayout setLayout;
	PVK_CHECK(vkCreateDescriptorSetLayout(device, &cInfo, NULL, &setLayout));
	return setLayout;
}

static VkDescriptorSetLayout pvkCreateGlobalDescriptorSetLayout(VkDevice device)
{
	VkDescriptorSetLayoutBinding* bindings = newv(VkDescriptorSetLayoutBinding, 2);
	bindings[0] = (VkDescriptorSetLayoutBinding)
	{
		.binding = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT		
	};

	VkDescriptorSetLayoutCreateInfo cInfo = 
	{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = 1,
		.pBindings = bindings
	};

	VkDescriptorSetLayout setLayout;
	PVK_CHECK(vkCreateDescriptorSetLayout(device, &cInfo, NULL, &setLayout));
	delete(bindings);
	return setLayout;
}

static VkDescriptorSetLayout pvkCreateObjectSetLayout(VkDevice device)
{
	VkDescriptorSetLayoutBinding binding = 
	{
		.binding = 2,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
	};

	VkDescriptorSetLayoutCreateInfo cInfo = 
	{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = 1,
		.pBindings = &binding
	};

	VkDescriptorSetLayout setLayout;
	PVK_CHECK(vkCreateDescriptorSetLayout(device, &cInfo, NULL, &setLayout));
	return setLayout;
}

typedef buffer_t VkSemaphoreList;

typedef struct PvkSemaphoreFIFOPool
{
	VkSemaphoreList semaphores;
	uint32_t acquiredIndex;
} PvkSemaphoreFIFOPool;

PvkSemaphoreFIFOPool* pvkCreateSemaphoreFIFOPool(VkDevice device, uint32_t reserveCount)
{
	PvkSemaphoreFIFOPool* pool = (PvkSemaphoreFIFOPool*)malloc(sizeof(PvkSemaphoreFIFOPool));
	pool->semaphores = buf_create(sizeof(VkSemaphore), reserveCount, 0);
	pool->acquiredIndex = 0;
	for(uint32_t i = 0; i < reserveCount; i++)
	{
		VkSemaphore semaphore = pvkCreateSemaphore(device);
		buf_push(&pool->semaphores, &semaphore);
	}
	return pool;
}

void pvkDestroySemaphoreFIFOPool(VkDevice device, PvkSemaphoreFIFOPool* pool)
{
	uint32_t reserveCount = (uint32_t)buf_get_capacity(&pool->semaphores);
	for(uint32_t i = 0; i < reserveCount; i++)
		vkDestroySemaphore(device, buf_get_value_at_typeof(&pool->semaphores, VkSemaphore, i), NULL);
	buf_free(&pool->semaphores);
	memset((void*)pool, 0, sizeof(pool));
	free(pool);
}

VkSemaphore pvkSemaphoreFIFOPoolAcquire(PvkSemaphoreFIFOPool* pool)
{
	VkSemaphore semaphore = buf_get_value_at_typeof(&pool->semaphores, VkSemaphore, pool->acquiredIndex);
	uint32_t reserveCount = (uint32_t)buf_get_capacity(&pool->semaphores);
	pool->acquiredIndex = (pool->acquiredIndex + 1) % reserveCount;
	return semaphore;
}


static void recordCommandBuffers(u32 width, u32 height, VkCommandBuffer* commandBuffers,
							    VkClearValue* clearValues,
								VkRenderPass renderPass, 
								VkRenderPass shadowMapRenderPass, 
								VkFramebuffer* shadowMapFramebuffer,
								VkFramebuffer* framebuffers,
								VkPipeline shadowMapPipeline,
								VkPipeline pipeline,
								VkPipeline pipeline2,
								VkPipelineLayout shadowMapPipelineLayout,
								VkPipelineLayout pipelineLayout,
								VkPipelineLayout pipelineLayout2,
								VkDescriptorSet* set,
								PvkGeometry* planeGeometry,
								PvkGeometry* boxGeometry)
{
	for(int index = 0; index < 3; index++)
	{
		pvkBeginCommandBuffer(commandBuffers[index], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

		/* shadow map renderpass */
		VkClearValue shadowMapClearValue = { .depthStencil = { .depth = 1.0f, .stencil = 0.0f } };
		pvkBeginRenderPass(commandBuffers[index], shadowMapRenderPass, *shadowMapFramebuffer, width, height, 1, &shadowMapClearValue);
		vkCmdBindPipeline(commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, shadowMapPipeline);
		vkCmdBindDescriptorSets(commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, shadowMapPipelineLayout, 0, 2, &set[1], 0, NULL);
		pvkDrawGeometry(commandBuffers[index], planeGeometry);
		pvkDrawGeometry(commandBuffers[index], boxGeometry);
		pvkEndRenderPass(commandBuffers[index]);

		/* color renderpass */
		pvkBeginRenderPass(commandBuffers[index], renderPass, framebuffers[index], width, height, 3, clearValues);

		/* first subpass */
		vkCmdBindPipeline(commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		vkCmdBindDescriptorSets(commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 3, &set[1], 0, NULL);
		pvkDrawGeometry(commandBuffers[index], planeGeometry);
		pvkDrawGeometry(commandBuffers[index], boxGeometry);

		vkCmdNextSubpass(commandBuffers[index], VK_SUBPASS_CONTENTS_INLINE);

		/* second subpass */
		vkCmdBindPipeline(commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline2);
		vkCmdBindDescriptorSets(commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout2, 0, 3, &set[0], 0, NULL);
		pvkDrawGeometry(commandBuffers[index], planeGeometry);
		pvkDrawGeometry(commandBuffers[index], boxGeometry);

		pvkEndRenderPass(commandBuffers[index]);

		pvkEndCommandBuffer(commandBuffers[index]);
	}
}

int main()
{
	VkInstance instance = pvkCreateVulkanInstanceWithExtensions(2, "VK_KHR_win32_surface", "VK_KHR_surface");
	PvkWindow* window = pvkWindowCreate(800, 800, "Vulkan Multipass Rendering", false, true);
	VkSurfaceKHR surface = pvkWindowCreateVulkanSurface(window, instance);
	VkPhysicalDevice physicalGPU = pvkGetPhysicalDevice(instance, surface, 
														VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU, 
														VK_FORMAT_B8G8R8A8_SRGB, 
														VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, 
														VK_PRESENT_MODE_FIFO_KHR);
	uint32_t graphicsQueueFamilyIndex = pvkFindQueueFamilyIndex(physicalGPU, VK_QUEUE_GRAPHICS_BIT);
	uint32_t presentQueueFamilyIndex = pvkFindQueueFamilyIndexWithPresentSupport(physicalGPU, surface);
	uint32_t queueFamilyIndices[2] = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };
	VkDevice logicalGPU = pvkCreateLogicalDeviceWithExtensions(instance, 
																physicalGPU,
																2, queueFamilyIndices,
																1, VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	VkQueue graphicsQueue, presentQueue;
	vkGetDeviceQueue(logicalGPU, graphicsQueueFamilyIndex, 0, &graphicsQueue);
	vkGetDeviceQueue(logicalGPU, presentQueueFamilyIndex, 0, &presentQueue);

	VkSwapchainKHR swapchain = pvkCreateSwapchain(logicalGPU, surface, 
													800, 800, 
													VK_FORMAT_B8G8R8A8_SRGB, 
													VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, 
													VK_PRESENT_MODE_FIFO_KHR,
													2, queueFamilyIndices, VK_NULL_HANDLE);

	VkCommandPool commandPool = pvkCreateCommandPool(logicalGPU, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, graphicsQueueFamilyIndex);
	VkCommandBuffer* commandBuffers = pvkAllocateCommandBuffers(logicalGPU, commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	VkSemaphore imageAvailableSemaphore = pvkCreateSemaphore(logicalGPU);
	VkSemaphore renderFinishSemaphore = pvkCreateSemaphore(logicalGPU);

	/* Render Pass & Framebuffer attachments */
	VkRenderPass shadowMapRenderPass = pvkCreateShadowMapRenderPass(logicalGPU);
	VkRenderPass renderPass = pvkCreateRenderPass2(logicalGPU);
	VkImageView* swapchainImageViews = pvkCreateSwapchainImageViews(logicalGPU, swapchain, VK_FORMAT_B8G8R8A8_SRGB);
	PvkImage auxImage = pvkCreateImage(physicalGPU, logicalGPU, 
										VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
										VK_FORMAT_B8G8R8A8_SRGB, 800, 800, 
										VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, 
										2, queueFamilyIndices);
	VkImageView auxAttachment = pvkCreateImageView(logicalGPU, auxImage.handle, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	PvkImage depthImage = pvkCreateImage(physicalGPU, logicalGPU, 
										VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
										VK_FORMAT_D32_SFLOAT, 800, 800,
										VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
										2, queueFamilyIndices);
	VkImageView depthAttachment = pvkCreateImageView(logicalGPU, depthImage.handle, VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);
	VkImageView attachments[9] = 
	{
		swapchainImageViews[0], auxAttachment, depthAttachment,				// framebuffer for swapchain image 0
		swapchainImageViews[1], auxAttachment, depthAttachment,				// framebuffer for swapchain image 1
		swapchainImageViews[2], auxAttachment, depthAttachment				// framebuffer for swapchain image 2
	};
	VkFramebuffer* framebuffers = pvkCreateFramebuffers(logicalGPU, renderPass, 800, 800, 3, 3, attachments);
	PvkImage shadowMapImage = pvkCreateImage(physicalGPU, logicalGPU,
												VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
												VK_FORMAT_D32_SFLOAT, 800, 800,
												VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
												2, queueFamilyIndices);
	VkSampler shadowMapSampler = pvkCreateShadowMapSampler(logicalGPU);
	VkImageView shadowMapAttachment = pvkCreateImageView(logicalGPU, shadowMapImage.handle, VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);
	VkFramebuffer* shadowMapFramebuffer = pvkCreateFramebuffers(logicalGPU, shadowMapRenderPass, 800, 800, 1, 1, &shadowMapAttachment);

	/* Uniform Buffers */
	PvkBuffer globalUniformBuffer = pvkCreateBuffer(physicalGPU, logicalGPU, 
													VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
													VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
													sizeof(PvkGlobalData),
													2, queueFamilyIndices);
	PvkBuffer objectUniformBuffer = pvkCreateBuffer(physicalGPU, logicalGPU,
													VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
													VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
													sizeof(PvkObjectData),
													2, queueFamilyIndices);

	/* Resource Descriptors */
	VkDescriptorPool descriptorPool = pvkCreateDescriptorPool(logicalGPU, 4, 3, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1,
																		  	 VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2,
																		  	 VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1);
	VkDescriptorSetLayout setLayouts[4] = 
	{ 
		pvkCreateDescriptorSetLayout(logicalGPU),				// input_attachment (binding = 0)
		pvkCreateGlobalDescriptorSetLayout(logicalGPU),			// uniform buffer (PvkGlobalData) (binding = 1)
		pvkCreateObjectSetLayout(logicalGPU),					// uniform buffer (PvkObjectData) (binding = 2)
		pvkCreateShadowMapDescriptorSetLayout(logicalGPU) 		// shadow map sampler (binding = 3)
	};
	VkDescriptorSet* set = pvkAllocateDescriptorSets(logicalGPU, descriptorPool, 4, setLayouts);
	pvkWriteImageViewToDescriptor(logicalGPU, set[0], 0, auxAttachment, VK_NULL_HANDLE, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT);
	pvkWriteBufferToDescriptor(logicalGPU, set[1], 1, globalUniformBuffer.handle, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	pvkWriteBufferToDescriptor(logicalGPU, set[2], 2, objectUniformBuffer.handle, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	pvkWriteImageViewToDescriptor(logicalGPU, set[3], 3, shadowMapAttachment, shadowMapSampler, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

	PvkCamera* camera = pvkCreateCamera((float)window->width / window->height, PVK_PROJECTION_TYPE_PERSPECTIVE, 65 DEG);	
	PvkGlobalData* globalData = new(PvkGlobalData);
	PvkObjectData* objectData = new(PvkObjectData);
	globalData->projectionMatrix = pvkMat4Transpose(camera->projection);
	globalData->viewMatrix = pvkMat4Transpose(camera->view);
	globalData->dirLight.dir = pvkVec3Normalize((PvkVec3) { 1, -1, 0 });
	globalData->dirLight.intensity = 1.0f;
	globalData->dirLight.color = (PvkVec3) { 1, 1, 1 };
	globalData->lightProjectionMatrix = pvkMat4Transpose(pvkMat4OrthoProj(10, 1, 1, 20));
	globalData->lightViewMatrix = pvkMat4Transpose(pvkMat4Inverse(pvkMat4Mul(pvkMat4Translate((PvkVec3) { -4.0f, 4.0f, 0 }), pvkMat4Rotate((PvkVec3) { -20 DEG, -90 DEG, 0 }))));
	globalData->ambLight.color = (PvkVec3) { 0.3f, 0.3f, 0.3f };
	globalData->ambLight.intensity = 1.0f;
	objectData->modelMatrix = pvkMat4Transpose(pvkMat4Rotate((PvkVec3) { 0 DEG, 0, 0 }));
	objectData->normalMatrix = pvkMat4Inverse(objectData->modelMatrix);
	pvkUploadToMemory(logicalGPU, globalUniformBuffer.memory, globalData, sizeof(PvkGlobalData));
	pvkUploadToMemory(logicalGPU, objectUniformBuffer.memory, objectData, sizeof(PvkObjectData));
	delete(globalData);

	/* Graphics Pipeline & Shaders */
	VkShaderModule fragmentShader = pvkCreateShaderModule(logicalGPU, "shaders/shader.frag.spv");
	VkShaderModule vertexShader = pvkCreateShaderModule(logicalGPU, "shaders/shader.vert.spv");

	VkShaderModule fragmentShaderPass2 = pvkCreateShaderModule(logicalGPU, "shaders/shader.pass2.frag.spv");
	VkShaderModule vertexShaderPass2 = pvkCreateShaderModule(logicalGPU, "shaders/shader.pass2.vert.spv");

	VkShaderModule shadowMapFragmentShader = pvkCreateShaderModule(logicalGPU, "shaders/shadowMapShader.frag.spv");
	VkShaderModule shadowMapVertexShader = pvkCreateShaderModule(logicalGPU, "shaders/shadowMapShader.vert.spv");

	VkPipelineLayout pipelineLayout = pvkCreatePipelineLayout(logicalGPU, 3, &setLayouts[1]);
	VkPipelineLayout pipelineLayout2 = pvkCreatePipelineLayout(logicalGPU, 3, &setLayouts[0]);
	VkPipelineLayout shadowMapPipelineLayout = pvkCreatePipelineLayout(logicalGPU, 2, &setLayouts[1]);
	VkPipeline pipeline = pvkCreateGraphicsPipeline(logicalGPU, pipelineLayout, renderPass, 0, 1, 800, 800, 2,
													(PvkShader) { fragmentShader, PVK_SHADER_TYPE_FRAGMENT },
													(PvkShader) { vertexShader, PVK_SHADER_TYPE_VERTEX });
	VkPipeline pipeline2 = pvkCreateGraphicsPipeline(logicalGPU, pipelineLayout2, renderPass, 1, 1, 800, 800, 2,
													(PvkShader) { fragmentShaderPass2, PVK_SHADER_TYPE_FRAGMENT },
													(PvkShader) { vertexShaderPass2, PVK_SHADER_TYPE_VERTEX });
	VkPipeline shadowMapPipeline = pvkCreateShadowMapGraphicsPipeline(logicalGPU, shadowMapPipelineLayout, shadowMapRenderPass, 0, 800, 800, 1,
													(PvkShader) { shadowMapVertexShader, PVK_SHADER_TYPE_VERTEX });
	PvkGeometry* planeGeometry = pvkCreatePlaneGeometry(physicalGPU, logicalGPU, 2, queueFamilyIndices, 6);
	PvkGeometry* boxGeometry = pvkCreateBoxGeometry(physicalGPU, logicalGPU, 2, queueFamilyIndices, 3);

	VkClearValue* clearValues = newv(VkClearValue, 3);
	for(int i = 0; i < 2; i++)
	{
		clearValues[i].color.float32[0] = 0.1f;
		clearValues[i].color.float32[1] = 0.3f;
		clearValues[i].color.float32[2] = 0;
		clearValues[i].color.float32[3] = 1;
	}
	clearValues[2].depthStencil.depth = 1;

	/* Command buffer recording */
	recordCommandBuffers(800, 800, commandBuffers,
								clearValues,
								renderPass, 
								shadowMapRenderPass, 
								shadowMapFramebuffer,
								framebuffers,
								shadowMapPipeline,
								pipeline,
								pipeline2,
								shadowMapPipelineLayout,
								pipelineLayout,
								pipelineLayout2,
								set,
								planeGeometry,
								boxGeometry);

	PvkSemaphoreFIFOPool* semaphorePool = pvkCreateSemaphoreFIFOPool(logicalGPU, 20);

	VkFence fences[3];
	for(uint32_t i = 0; i < 3; i++)
		fences[i] = pvkCreateFence(logicalGPU);

	float angle = 0;
	uint32_t fIndex = 0;
	uint32_t fUnsignaledCount = 3;
	/* Rendering & Presentation */
	while(!pvkWindowShouldClose(window))
	{
		VkSemaphore imageAvailableSemaphore = pvkSemaphoreFIFOPoolAcquire(semaphorePool);
		uint32_t index = 0;
		PVK_CHECK(vkAcquireNextImageKHR(logicalGPU, swapchain, UINT64_MAX, imageAvailableSemaphore, fences[fIndex++], &index));
		if(fIndex >= fUnsignaledCount)
		{
			PVK_CHECK(vkWaitForFences(logicalGPU, 1, &fences[0], VK_TRUE, UINT64_MAX));
			PVK_CHECK(vkResetFences(logicalGPU, 1, &fences[0]));
			fUnsignaledCount = 1;
			VkResult result = vkWaitForFences(logicalGPU, 1, &fences[1], VK_TRUE, FENCE_WAIT_TIME);
			if(result != VK_TIMEOUT)
			{
				PVK_CHECK(result);
				PVK_CHECK(vkResetFences(logicalGPU, 1, &fences[1]));
				fUnsignaledCount = 2;
				result = vkWaitForFences(logicalGPU, 1, &fences[2], VK_TRUE, FENCE_WAIT_TIME);
				if(result != VK_TIMEOUT)
				{
					PVK_CHECK(result);
					PVK_CHECK(vkResetFences(logicalGPU, 1, &fences[2]));
					fUnsignaledCount = 3;
				}
			}
			fIndex = 0;
		}

		angle += 0.1f DEG;
		objectData->modelMatrix = pvkMat4Transpose(pvkMat4Transform((PvkVec3) { 0, 0, 0 }, (PvkVec3) { 0, angle, 0 }));
		objectData->normalMatrix = pvkMat4Inverse(objectData->modelMatrix);
		pvkUploadToMemory(logicalGPU, objectUniformBuffer.memory, objectData, sizeof(PvkObjectData));
		
		VkSemaphore renderFinishSemaphore = pvkSemaphoreFIFOPoolAcquire(semaphorePool);
		// execute commands
		pvkSubmit(commandBuffers[index], graphicsQueue, imageAvailableSemaphore, renderFinishSemaphore);

		// present the output image
		if(!pvkPresent(index, swapchain, presentQueue, renderFinishSemaphore))
		{
			vkDestroySurfaceKHR(instance, surface, NULL);;
			vkDestroyPipeline(logicalGPU, shadowMapPipeline, NULL);
			vkDestroyPipeline(logicalGPU, pipeline2, NULL);
			vkDestroyPipeline(logicalGPU, pipeline, NULL);
			delete(shadowMapFramebuffer);
			vkDestroyImageView(logicalGPU, shadowMapAttachment, NULL);
			pvkDestroyImage(logicalGPU, shadowMapImage);
			delete(framebuffers);
			vkDestroyImageView(logicalGPU, depthAttachment, NULL);
			pvkDestroyImage(logicalGPU, depthImage);
			vkDestroyImageView(logicalGPU, auxAttachment, NULL);
			pvkDestroyImage(logicalGPU, auxImage);
			pvkDestroySwapchainImageViews(logicalGPU, swapchain, swapchainImageViews);
			vkDestroySwapchainKHR(logicalGPU, swapchain, NULL);

			surface = pvkWindowCreateVulkanSurface(window, instance);
			swapchain = pvkCreateSwapchain(logicalGPU, surface, 
													window->width, window->height, 
													VK_FORMAT_B8G8R8A8_SRGB, 
													VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, 
													VK_PRESENT_MODE_FIFO_KHR,
													2, queueFamilyIndices, VK_NULL_HANDLE);
			swapchainImageViews = pvkCreateSwapchainImageViews(logicalGPU, swapchain, VK_FORMAT_B8G8R8A8_SRGB);

			auxImage = pvkCreateImage(physicalGPU, logicalGPU, 
										VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
										VK_FORMAT_B8G8R8A8_SRGB, window->width, window->height, 
										VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, 
										2, queueFamilyIndices);
			auxAttachment = pvkCreateImageView(logicalGPU, auxImage.handle, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
			
			depthImage = pvkCreateImage(physicalGPU, logicalGPU, 
										VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
										VK_FORMAT_D32_SFLOAT, window->width, window->height,
										VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
										2, queueFamilyIndices);
			depthAttachment = pvkCreateImageView(logicalGPU, depthImage.handle, VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);
			
			attachments[0] = swapchainImageViews[0];
			attachments[1] = auxAttachment;
			attachments[2] = depthAttachment;				// framebuffer for swapchain image 0
			attachments[3] = swapchainImageViews[1];
			attachments[4] = auxAttachment;
			attachments[5] = depthAttachment;				// framebuffer for swapchain image 1
			attachments[6] = swapchainImageViews[2];
			attachments[7] = auxAttachment;
			attachments[8] = depthAttachment;				// framebuffer for swapchain image 2
			framebuffers = pvkCreateFramebuffers(logicalGPU, renderPass, window->width, window->height, 3, 3, attachments);

			shadowMapImage = pvkCreateImage(physicalGPU, logicalGPU,
												VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
												VK_FORMAT_D32_SFLOAT, window->width, window->height,
												VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
												2, queueFamilyIndices);
			shadowMapAttachment = pvkCreateImageView(logicalGPU, shadowMapImage.handle, VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);
			shadowMapFramebuffer = pvkCreateFramebuffers(logicalGPU, shadowMapRenderPass, window->width, window->height, 1, 1, &shadowMapAttachment);

			pipeline = pvkCreateGraphicsPipeline(logicalGPU, pipelineLayout, renderPass, 0, 1, window->width, window->height, 2,
													(PvkShader) { fragmentShader, PVK_SHADER_TYPE_FRAGMENT },
													(PvkShader) { vertexShader, PVK_SHADER_TYPE_VERTEX });
			pipeline2 = pvkCreateGraphicsPipeline(logicalGPU, pipelineLayout2, renderPass, 1, 1, window->width, window->height, 2,
													(PvkShader) { fragmentShaderPass2, PVK_SHADER_TYPE_FRAGMENT },
													(PvkShader) { vertexShaderPass2, PVK_SHADER_TYPE_VERTEX });
			shadowMapPipeline = pvkCreateShadowMapGraphicsPipeline(logicalGPU, shadowMapPipelineLayout, shadowMapRenderPass, 0, window->width, window->height, 1,
													(PvkShader) { shadowMapVertexShader, PVK_SHADER_TYPE_VERTEX });

			delete(camera);
			camera = pvkCreateCamera((float)window->width / window->height, PVK_PROJECTION_TYPE_PERSPECTIVE, 65 DEG);	
			PvkGlobalData* globalData = new(PvkGlobalData);
			globalData->projectionMatrix = pvkMat4Transpose(camera->projection);
			globalData->viewMatrix = pvkMat4Transpose(camera->view);
			globalData->dirLight.dir = pvkVec3Normalize((PvkVec3) { 1, -1, 0 });
			globalData->dirLight.intensity = 1.0f;
			globalData->dirLight.color = (PvkVec3) { 1, 1, 1 };
			globalData->lightProjectionMatrix = pvkMat4Transpose(pvkMat4OrthoProj(10, 1, 1, 20));
			globalData->lightViewMatrix = pvkMat4Transpose(pvkMat4Inverse(pvkMat4Mul(pvkMat4Translate((PvkVec3) { -4.0f, 4.0f, 0 }), pvkMat4Rotate((PvkVec3) { -20 DEG, -90 DEG, 0 }))));
			globalData->ambLight.color = (PvkVec3) { 0.3f, 0.3f, 0.3f };
			globalData->ambLight.intensity = 1.0f;
			objectData->modelMatrix = pvkMat4Transpose(pvkMat4Rotate((PvkVec3) { 0 DEG, 0, 0 }));
			objectData->normalMatrix = pvkMat4Inverse(objectData->modelMatrix);
			pvkUploadToMemory(logicalGPU, globalUniformBuffer.memory, globalData, sizeof(PvkGlobalData));
			pvkUploadToMemory(logicalGPU, objectUniformBuffer.memory, objectData, sizeof(PvkObjectData));
			delete(globalData);

			pvkWriteImageViewToDescriptor(logicalGPU, set[0], 0, auxAttachment, VK_NULL_HANDLE, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT);
			pvkWriteImageViewToDescriptor(logicalGPU, set[3], 3, shadowMapAttachment, shadowMapSampler, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);


			recordCommandBuffers(window->width, window->height, commandBuffers,
								clearValues,
								renderPass, 
								shadowMapRenderPass, 
								shadowMapFramebuffer,
								framebuffers,
								shadowMapPipeline,
								pipeline,
								pipeline2,
								shadowMapPipelineLayout,
								pipelineLayout,
								pipelineLayout2,
								set,
								planeGeometry,
								boxGeometry);
		}

		pvkWindowPollEvents(window);
	}

	PVK_CHECK(vkDeviceWaitIdle(logicalGPU));

	delete(clearValues);
	delete(objectData);
	delete(camera);
	pvkDestroyGeometry(logicalGPU, planeGeometry);
	pvkDestroyGeometry(logicalGPU, boxGeometry);
	vkDestroyShaderModule(logicalGPU, shadowMapFragmentShader, NULL);
	vkDestroyShaderModule(logicalGPU, shadowMapVertexShader, NULL);
	vkDestroyShaderModule(logicalGPU, fragmentShaderPass2, NULL);
	vkDestroyShaderModule(logicalGPU, vertexShaderPass2, NULL);
	vkDestroyPipeline(logicalGPU, shadowMapPipeline, NULL);
	vkDestroyPipeline(logicalGPU, pipeline2, NULL);
	vkDestroyPipeline(logicalGPU, pipeline, NULL);
	vkDestroyPipelineLayout(logicalGPU, shadowMapPipelineLayout, NULL);
	vkDestroyPipelineLayout(logicalGPU, pipelineLayout2, NULL);
	vkDestroyPipelineLayout(logicalGPU, pipelineLayout, NULL);
	vkDestroyShaderModule(logicalGPU, fragmentShader, NULL);
	vkDestroyShaderModule(logicalGPU, vertexShader, NULL);
	delete(set);
	for(int i = 0; i < 4; i++)
		vkDestroyDescriptorSetLayout(logicalGPU, setLayouts[i], NULL);
	pvkDestroyBuffer(logicalGPU, objectUniformBuffer);
	pvkDestroyBuffer(logicalGPU, globalUniformBuffer);
	vkDestroyDescriptorPool(logicalGPU, descriptorPool, NULL);
	pvkDestroyFramebuffers(logicalGPU, 1, shadowMapFramebuffer);
	delete(shadowMapFramebuffer);
	vkDestroyImageView(logicalGPU, shadowMapAttachment, NULL);
	pvkDestroyImage(logicalGPU, shadowMapImage);
	vkDestroySampler(logicalGPU, shadowMapSampler, NULL);
	pvkDestroyFramebuffers(logicalGPU, 3, framebuffers);
	delete(framebuffers);
	vkDestroyImageView(logicalGPU, depthAttachment, NULL);
	pvkDestroyImage(logicalGPU, depthImage);
	vkDestroyImageView(logicalGPU, auxAttachment, NULL);
	pvkDestroyImage(logicalGPU, auxImage);
	pvkDestroySwapchainImageViews(logicalGPU, swapchain, swapchainImageViews);
	vkDestroyRenderPass(logicalGPU, renderPass, NULL);
	vkDestroyRenderPass(logicalGPU, shadowMapRenderPass, NULL);
	vkDestroySemaphore(logicalGPU, imageAvailableSemaphore, NULL);
	vkDestroySemaphore(logicalGPU, renderFinishSemaphore, NULL);
	delete(commandBuffers);
	vkDestroyCommandPool(logicalGPU, commandPool, NULL);
	vkDestroySwapchainKHR(logicalGPU, swapchain, NULL);
	vkDestroyDevice(logicalGPU, NULL);
	vkDestroySurfaceKHR(instance, surface, NULL);
	pvkWindowDestroy(window);
	vkDestroyInstance(instance, NULL);
	return 0;
}
