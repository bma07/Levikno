﻿#include <levikno/levikno.h>
#include <vector>

bool windowMoved(LvnWindowMovedEvent* e)
{
	LVN_TRACE("%s: (x:%d,y:%d)", e->name, e->x, e->y);
	return true;
}

bool windowResize(LvnWindowResizeEvent* e)
{
	LVN_TRACE("%s: (x:%d,y:%d)", e->name, e->width, e->height);
	return true;
}

bool mousePos(LvnMouseMovedEvent* e)
{
	LVN_TRACE("%s: (x:%d,y:%d)", e->name, e->x, e->y);
	return true;
}

bool keyPress(LvnKeyPressedEvent* e)
{
	LVN_TRACE("%s: code: %d", e->name, e->keyCode);
	return true;
}

bool keyRelease(LvnKeyReleasedEvent* e)
{
	LVN_TRACE("%s: code: %d", e->name, e->keyCode);
	return true;
}

bool keyHold(LvnKeyHoldEvent* e)
{
	LVN_TRACE("%s: code: %d (%d)", e->name, e->keyCode, e->repeat);
	return true;
}

bool keyTyped(LvnKeyTypedEvent* e)
{
	LVN_TRACE("%s: key: %c", e->name, e->key);
	return true;
}

void eventsCallbackFn(LvnEvent* e)
{
	lvn::dispatchKeyPressedEvent(e, keyPress);
	lvn::dispatchKeyHoldEvent(e, keyHold);
	lvn::dispatchKeyReleasedEvent(e, keyRelease);
	lvn::dispatchKeyTypedEvent(e, keyTyped);
	lvn::dispatchMouseMovedEvent(e, mousePos);
}

float vertices[] = 
{
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
};

uint32_t indices[] = 
{
	0, 1, 2, 2, 3, 0
};

struct UniformData
{
	lvn::mat4 matrix;
};

int main()
{
	LvnContextCreateInfo lvnCreateInfo{};
	lvnCreateInfo.enableLogging = true;
	lvnCreateInfo.enableVulkanValidationLayers = true;
	lvnCreateInfo.windowapi = Lvn_WindowApi_glfw;
	lvnCreateInfo.graphicsapi = Lvn_GraphicsApi_vulkan;

	lvn::createContext(&lvnCreateInfo);

	uint32_t deviceCount = 0;
	std::vector<LvnPhysicalDevice*> devices;
	lvn::getPhysicalDevices(nullptr, &deviceCount);

	devices.resize(deviceCount);
	lvn::getPhysicalDevices(devices.data(), &deviceCount);

	for (uint32_t i = 0; i < deviceCount; i++)
	{
		LvnPhysicalDeviceInfo deviceInfo = lvn::getPhysicalDeviceInfo(devices[i]);
		LVN_TRACE("name: %s, version: %d", deviceInfo.name, deviceInfo.driverVersion);
	}

	LvnRenderInitInfo renderInfo{};
	renderInfo.physicalDevice = devices[0];
	lvn::renderInit(&renderInfo);


	LvnWindowCreateInfo windowInfo{};
	windowInfo.width = 800;
	windowInfo.height = 600;
	windowInfo.title = "window";
	windowInfo.fullscreen = false;
	windowInfo.resizable = true;
	windowInfo.minWidth = 300;
	windowInfo.minHeight = 200;
	windowInfo.maxWidth = -1;
	windowInfo.maxHeight = -1;
	windowInfo.pIcons = nullptr;
	windowInfo.iconCount = 0;

	LvnWindow* window;
	lvn::createWindow(&window, &windowInfo);
	lvn::setWindowEventCallback(window, eventsCallbackFn);


	LvnRenderPassAttachment colorAttachment{};
	colorAttachment.type = Lvn_AttachmentType_Color;
	colorAttachment.format = Lvn_ImageFormat_RGBA8;
	colorAttachment.loadOp = Lvn_AttachmentLoadOp_Clear;
	colorAttachment.storeOp = Lvn_AttachmentStoreOp_Store;
	colorAttachment.stencilLoadOp = Lvn_AttachmentLoadOp_DontCare;
	colorAttachment.stencilStoreOp = Lvn_AttachmentStoreOp_DontCare;
	colorAttachment.samples = Lvn_SampleCount_1_Bit;
	colorAttachment.initialLayout = Lvn_ImageLayout_Undefined;
	colorAttachment.finalLayout = Lvn_ImageLayout_Present;

	LvnRenderPassCreateInfo renderPassCreateInfo{};
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &colorAttachment;

	LvnRenderPass* renderPass;
	lvn::createRenderPass(&renderPass, &renderPassCreateInfo);


	LvnShaderCreateInfo shaderCreateInfo{};
	shaderCreateInfo.vertexSrc = "/home/bma/Documents/dev/levikno/LeviknoEditor/res/shaders/vkvert.spv";
	shaderCreateInfo.fragmentSrc = "/home/bma/Documents/dev/levikno/LeviknoEditor/res/shaders/vkfrag.spv";

	LvnShader* shader;
	lvn::createShaderFromFileBin(&shader, &shaderCreateInfo);


	LvnVertexBindingDescription vertexBindingDescroption{};
	vertexBindingDescroption.stride = 6 * sizeof(float);
	vertexBindingDescroption.binding = 0;

	LvnVertexAttribute attributes[2] = 
	{
		{ 0, 0, Lvn_VertexDataType_Vec3f, 0 },
		{ 0, 1, Lvn_VertexDataType_Vec3f, (3 * sizeof(float)) },
	};


	LvnDescriptorBinding descriptorBinding{};
	descriptorBinding.binding = 0;
	descriptorBinding.descriptorType = Lvn_DescriptorType_UniformBuffer;
	descriptorBinding.shaderStage = Lvn_ShaderStage_Vertex;
	descriptorBinding.descriptorCount = 1;

	LvnDescriptorLayoutCreateInfo descriptorLayoutCreateInfo{};
	descriptorLayoutCreateInfo.pDescriptorBindings = &descriptorBinding;
	descriptorLayoutCreateInfo.descriptorBindingCount = 1;

	LvnDescriptorLayout* descriptorLayout;
	lvn::createDescriptorLayout(&descriptorLayout, &descriptorLayoutCreateInfo);


	LvnPipelineSpecification pipelineSpec = lvn::getDefaultPipelineSpecification();
	LvnPipelineCreateInfo pipelineCreateInfo{};
	pipelineCreateInfo.pipelineSpecification = &pipelineSpec;
	pipelineCreateInfo.pVertexBindingDescriptions = &vertexBindingDescroption;
	pipelineCreateInfo.vertexBindingDescriptionCount = 1;
	pipelineCreateInfo.pVertexAttributes = attributes;
	pipelineCreateInfo.vertexAttributeCount = 2;
	pipelineCreateInfo.pDescriptorLayouts = &descriptorLayout;
	pipelineCreateInfo.descriptorLayoutCount = 1;
	pipelineCreateInfo.shader = shader;
	pipelineCreateInfo.renderPass = nullptr;
	pipelineCreateInfo.window = window;

	LvnPipeline* pipeline;
	lvn::createPipeline(&pipeline, &pipelineCreateInfo);

	lvn::destroyShader(shader);

	LvnBufferCreateInfo bufferCreateInfo{};
	bufferCreateInfo.type = Lvn_BufferType_Vertex | Lvn_BufferType_Index;
	bufferCreateInfo.pVertexBindingDescriptions = &vertexBindingDescroption;
	bufferCreateInfo.vertexBindingDescriptionCount = 1;
	bufferCreateInfo.pVertexAttributes = attributes;
	bufferCreateInfo.vertexAttributeCount = 2;

	bufferCreateInfo.pVertices = vertices;
	bufferCreateInfo.vertexBufferSize = sizeof(vertices);

	bufferCreateInfo.pIndices = indices;
	bufferCreateInfo.indexBufferSize = sizeof(indices);

	LvnBuffer* buffer;
	lvn::createBuffer(&buffer, &bufferCreateInfo);


	LvnUniformBufferCreateInfo uniformBufferInfo{};
	uniformBufferInfo.binding = 0;
	uniformBufferInfo.type = Lvn_BufferType_Uniform;
	uniformBufferInfo.size = sizeof(UniformData);
	uniformBufferInfo.descriptorLayout = descriptorLayout;

	LvnUniformBuffer* uniformBuffer;
	lvn::createUniformBuffer(&uniformBuffer, &uniformBufferInfo);

	UniformData uniformData{};

	while (lvn::windowOpen(window))
	{
		lvn::updateWindow(window);

		// auto [x, y] = lvn::getWindowDimensions(window);
		// LVN_TRACE("(x:%d,y:%d)", x, y);


		auto [width, height] = lvn::getWindowDimensions(window);
		lvn::mat4 proj = lvn::perspective(lvn::radians(60.0f), (float)width / (float)height, 0.01f, 1000.0f);
		lvn::mat4 view = lvn::lookAt(lvn::vec3(0.0f, 0.0f, 10.0f), lvn::vec3(0.0f, 0.0f, 0.0f), lvn::vec3(0.0f, 1.0f, 0.0f));
		lvn::mat4 model = lvn::mat4(1.0f);
		lvn::mat4 camera = proj * view * model;
		
		uniformData.matrix = camera;

		lvn::renderBeginNextFrame(window);
		lvn::renderBeginCommandRecording(window);
		lvn::renderCmdBeginRenderPass(window);

		lvn::renderCmdBindPipeline(window, pipeline);

		lvn::updateUniformBufferData(window, uniformBuffer, &uniformData, sizeof(UniformData));

		lvn::renderCmdBindVertexBuffer(window, buffer);
		lvn::renderCmdBindIndexBuffer(window, buffer);

		lvn::renderCmdBindDescriptorLayout(window, pipeline, descriptorLayout);
		lvn::renderCmdDrawIndexed(window, 6);

		lvn::renderCmdEndRenderPass(window);
		lvn::renderEndCommandRecording(window);
		lvn::renderDrawSubmit(window);

	}

	lvn::destroyUniformBuffer(uniformBuffer);
	lvn::destroyBuffer(buffer);
	lvn::destroyDescriptorLayout(descriptorLayout);
	lvn::destroyPipeline(pipeline);
	lvn::destroyRenderPass(renderPass);

	lvn::destroyWindow(window);
	lvn::terminateContext();

	return 0;
}
