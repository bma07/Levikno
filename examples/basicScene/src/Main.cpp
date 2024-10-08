﻿#include <levikno/levikno.h>

#include <cstdint>
#include <chrono>
#include <vector>

const static float s_CubemapVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f, // 0       7--------6
	 1.0f, -1.0f,  1.0f, // 1      /|       /|
	 1.0f, -1.0f, -1.0f, // 2     4--------5 |
	-1.0f, -1.0f, -1.0f, // 3     | |      | |
	-1.0f,  1.0f,  1.0f, // 4     | 3------|-2
	 1.0f,  1.0f,  1.0f, // 5     |/       |/
	 1.0f,  1.0f, -1.0f, // 6     0--------1
	-1.0f,  1.0f, -1.0f  // 7
};

const static uint32_t s_CubemapIndices[] =
{
	// Right
	6, 2, 1,
	6, 1, 5,
	// Left
	4, 0, 3,
	4, 3, 7,
	// Top
	5, 4, 7,
	5, 7, 6,
	// Bottom
	3, 0, 1,
	3, 1, 2,
	// Front
	5, 1, 0,
	5, 0, 4,
	// Back
	7, 3, 2,
	7, 2, 6
};

class Timer
{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Time;

public:
	void start() { m_Time = std::chrono::high_resolution_clock::now(); }
	void reset() { m_Time = std::chrono::high_resolution_clock::now(); }
	float elapsed() { return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Time).count() * 0.001f * 0.001f * 0.001f; }
	float elapsedms() { return elapsed() * 1000.0f; }
};

struct SoundData
{
	LvnSound* sound;
	float vol, pan, pitch;
};

bool windowMoved(LvnWindowMovedEvent* e, void* userData)
{
	LVN_TRACE("%s: (x:%d,y:%d)", e->name, e->x, e->y);
	return true;
}

bool windowResize(LvnWindowResizeEvent* e, void* userData)
{
	LVN_TRACE("%s: (x:%d,y:%d)", e->name, e->width, e->height);
	return true;
}

bool mousePos(LvnMouseMovedEvent* e, void* userData)
{
	LVN_TRACE("%s: (x:%d,y:%d)", e->name, e->x, e->y);
	return true;
}

bool keyPress(LvnKeyPressedEvent* e, void* userData)
{
	LVN_TRACE("%s: code: %d", e->name, e->keyCode);


	return true;
}

bool keyRelease(LvnKeyReleasedEvent* e, void* userData)
{
	LVN_TRACE("%s: code: %d", e->name, e->keyCode);
	return true;
}

bool keyHold(LvnKeyHoldEvent* e, void* userData)
{
	LVN_TRACE("%s: code: %d (%d)", e->name, e->keyCode, e->repeat);
	return true;
}

bool keyTyped(LvnKeyTypedEvent* e, void* userData)
{
	LVN_TRACE("%s: key: %c", e->name, e->key);

	SoundData* sndData = (SoundData*)userData;

	if (e->key == 'p')
	{
		lvn::soundTogglePause(sndData->sound);
	}

	if (e->key == 'j')
	{
		sndData->vol -= 0.05f;
		if (sndData->vol < 0.0f) sndData->vol = 0.0f;
		LVN_INFO("sound volume: %f", sndData->vol);

		lvn::soundSetVolume(sndData->sound, sndData->vol);
	}
	if (e->key == 'k')
	{
		sndData->vol += 0.05f;
		if (sndData->vol > 1.0f) sndData->vol = 1.0f;
		LVN_INFO("sound volume: %f", sndData->vol);

		lvn::soundSetVolume(sndData->sound, sndData->vol);

	}
	if (e->key == 'h')
	{
		sndData->pan -= 0.05f;
		if (sndData->pan < -1.0f) sndData->pan = -1.0f;
		LVN_INFO("sound pan: %f", sndData->pan);

		lvn::soundSetPan(sndData->sound, sndData->pan);
	}
	if (e->key == 'l')
	{
		sndData->pan += 0.05f;
		if (sndData->pan > 1.0f) sndData->pan = 1.0f;
		LVN_INFO("sound pan: %f", sndData->pan);

		lvn::soundSetPan(sndData->sound, sndData->pan);
	}
	if (e->key == 'u')
	{
		sndData->pitch -= 0.05f;
		if (sndData->pitch < 0.0f) sndData->pitch = 0.0f;
		LVN_INFO("sound pitch: %f", sndData->pitch);

		lvn::soundSetPitch(sndData->sound, sndData->pitch);
	}
	if (e->key == 'i')
	{
		sndData->pitch += 0.05f;
		LVN_INFO("sound pitch: %f", sndData->pitch);

		lvn::soundSetPitch(sndData->sound, sndData->pitch);
	}

	return true;
}

bool mousePress(LvnMouseButtonPressedEvent* e, void* userData)
{
	LVN_TRACE("%s: button: %d", e->name, e->buttonCode);
	return true;
}

bool mouseScroll(LvnMouseScrolledEvent* e, void* userData)
{
	LVN_TRACE("%s: scroll: (x:%f, y:%f)", e->name, e->x, e->y);
	return true;
}

void eventsCallbackFn(LvnEvent* e)
{
	// lvn::dispatchKeyPressedEvent(e, keyPress);
	// lvn::dispatchKeyHoldEvent(e, keyHold);
	// lvn::dispatchKeyReleasedEvent(e, keyRelease);
	// lvn::dispatchKeyTypedEvent(e, keyTyped);
	// lvn::dispatchMouseMovedEvent(e, mousePos);
	// lvn::dispatchMouseButtonPressedEvent(e, mousePress);
	// lvn::dispatchMouseScrolledEvent(e, mouseScroll);
}

float vertices[] =
{
	/*      pos        |       color     |   TexUV    */
	-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
};

std::vector<LvnVertex> lvnVertices = 
{
	{ {-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} },
	{ { 1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} },
	{ { 1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} },
	{ {-1.0f,  1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} },
};

uint32_t indices[] =
{
	0, 1, 2, 0, 2, 3,
};

struct UniformData
{
	lvn::mat4 matrix;
};

struct PbrStorageData
{
	lvn::mat4 matrix;
	lvn::mat4 model;
};

struct PbrUniformData
{
	lvn::vec3 campPos;
	alignas(16) lvn::vec3 lightPos;
	float metalic;
	float roughness;
	float ambientOcclusion;
};

static float s_CameraSpeed = 5.0f;
static bool s_CameraFirstClick = true;
static float m_LastMouseX = 0.0f, m_LastMouseY = 0.0f;
static float s_CameraSensitivity = 50.0f;

void cameraMovment(LvnWindow* window, LvnCamera* camera, float dt)
{
	if (lvn::keyPressed(window, Lvn_KeyCode_W))
		camera->position += (s_CameraSpeed * camera->orientation) * dt;

	if (lvn::keyPressed(window, Lvn_KeyCode_A))
		camera->position += (s_CameraSpeed * -lvn::normalize(lvn::cross(camera->orientation, camera->upVector))) * dt;

	if (lvn::keyPressed(window, Lvn_KeyCode_S))
		camera->position += (s_CameraSpeed * -camera->orientation) * dt;

	if (lvn::keyPressed(window, Lvn_KeyCode_D))
		camera->position += (s_CameraSpeed * lvn::normalize(lvn::cross(camera->orientation, camera->upVector))) * dt;

	if (lvn::keyPressed(window, Lvn_KeyCode_Space))
		camera->position += (s_CameraSpeed * camera->upVector) * dt;

	if (lvn::keyPressed(window, Lvn_KeyCode_LeftControl))
		camera->position += (s_CameraSpeed * -camera->upVector) * dt;

	if (lvn::keyPressed(window, Lvn_KeyCode_LeftShift))
		s_CameraSpeed = 10.0f;
	else if (lvn::keyReleased(window, Lvn_KeyCode_LeftShift))
		s_CameraSpeed = 5.0f;

	if (lvn::mouseButtonPressed(window, Lvn_MouseButton_1))
	{
		auto mousePos = lvn::mouseGetPos(window);
		
		if (s_CameraFirstClick)
		{
			m_LastMouseX = mousePos.x;
			m_LastMouseY = mousePos.y;
			s_CameraFirstClick = false;
			lvn::mouseSetInputMode(window, Lvn_MouseInputMode_Disable);
		}

		float xoffset = mousePos.x - m_LastMouseX;
		float yoffset = m_LastMouseY - mousePos.y;
		m_LastMouseX = mousePos.x;
		m_LastMouseY = mousePos.y;
		xoffset *= s_CameraSensitivity * dt;
		yoffset *= s_CameraSensitivity * dt;

		lvn::vec3 newOrientation = lvn::rotate(camera->orientation, lvn::radians(yoffset), lvn::normalize(lvn::cross(camera->orientation, camera->upVector)));
		if (abs(lvn::angle(lvn::normalize(newOrientation), camera->upVector) - lvn::radians(90.0f)) <= lvn::radians(85.0f))
		{
			if (abs(lvn::angle(lvn::normalize(newOrientation), lvn::normalize(camera->upVector)) - lvn::radians(90.0f)) <= lvn::radians(85.0f))
				camera->orientation = newOrientation;
		}

		// Rotates the orientation left and right
		camera->orientation = lvn::normalize(lvn::rotate(camera->orientation, lvn::radians(-xoffset), camera->upVector));


	}
	else if (lvn::mouseButtonReleased(window, Lvn_MouseButton_1))
	{
		s_CameraFirstClick = true;
		lvn::mouseSetInputMode(window, Lvn_MouseInputMode_Normal);
	}

	int width, height;
	lvn::windowGetSize(window, &width, &height);
	camera->projectionMatrix = lvn::perspective(lvn::radians(60.0f), (float)width / (float)height, 0.01f, 1000.0f);
	camera->viewMatrix = lvn::lookAt(camera->position, camera->position + camera->orientation, camera->upVector);
	camera->matrix = camera->projectionMatrix * camera->viewMatrix;
}

static float s_Radius = 5.0f;
static float s_AngleX = 0.0f, s_AngleY = 0.0f;
static float s_PanSpeed = 5.0f, s_MoveShiftSpeed = 5.0f;
static LvnVec3 s_Pos = LvnVec3(0.0f);
static LvnMat4 s_Model = LvnMat4(1.0f);

void orbitMovment(LvnWindow* window, LvnCamera* camera, float dt)
{
	LvnMat4 view = camera->viewMatrix;
	camera->position = LvnVec3(view[3][0], view[3][1], view[3][2]);

	bool mouse1 = lvn::mouseButtonPressed(window, Lvn_MouseButton_1);
	bool mouse2 = lvn::mouseButtonPressed(window, Lvn_MouseButton_2);

	if (lvn::keyPressed(window, Lvn_KeyCode_LeftShift) && mouse1 && !mouse2)
	{
		auto mousePos = lvn::mouseGetPos(window);
		
		if (s_CameraFirstClick)
		{
			m_LastMouseX = mousePos.x;
			m_LastMouseY = mousePos.y;
			s_CameraFirstClick = false;
			lvn::mouseSetInputMode(window, Lvn_MouseInputMode_Disable);
		}

		float xoffset = mousePos.x - m_LastMouseX;
		float yoffset = mousePos.y - m_LastMouseY;
		m_LastMouseX = mousePos.x;
		m_LastMouseY = mousePos.y;
		xoffset *= s_MoveShiftSpeed * dt;
		yoffset *= s_MoveShiftSpeed * dt;

		if (lvn::keyPressed(window, Lvn_KeyCode_LeftControl))
		{
			xoffset *= 0.1f;
			yoffset *= 0.1f;
		}

		LvnVec3 right = LvnVec3(view[0][0], view[1][0], view[2][0]);
		LvnVec3 up = LvnVec3(view[0][1], view[1][1], view[2][1]);

		s_Model = lvn::translate(LvnMat4(s_Model), right * -xoffset);
		s_Model = lvn::translate(LvnMat4(s_Model), up * -yoffset);
	}
	else if (mouse1 && !mouse2)
	{
		auto mousePos = lvn::mouseGetPos(window);
		
		if (s_CameraFirstClick)
		{
			m_LastMouseX = mousePos.x;
			m_LastMouseY = mousePos.y;
			s_CameraFirstClick = false;
			lvn::mouseSetInputMode(window, Lvn_MouseInputMode_Disable);
		}

		float xoffset = mousePos.x - m_LastMouseX;
		float yoffset = mousePos.y - m_LastMouseY;
		m_LastMouseX = mousePos.x;
		m_LastMouseY = mousePos.y;
		xoffset *= s_CameraSensitivity * dt;
		yoffset *= s_CameraSensitivity * dt;

		s_AngleX -= xoffset;
		s_AngleY -= yoffset;
	}

	if (mouse2 && !mouse1)
	{
		auto mousePos = lvn::mouseGetPos(window);
		if (s_CameraFirstClick)
		{
			m_LastMouseY = mousePos.y;
			s_CameraFirstClick = false;
			lvn::mouseSetInputMode(window, Lvn_MouseInputMode_Disable);
		}

		float yoffset = mousePos.y - m_LastMouseY;
		m_LastMouseY = mousePos.y;
		yoffset *= s_PanSpeed * dt;

		if (lvn::keyPressed(window, Lvn_KeyCode_LeftControl))
			yoffset *= 0.1f;

		s_Radius += yoffset;
		s_Radius = lvn::max(s_Radius, 0.0f);
	}

	if (lvn::mouseButtonReleased(window, Lvn_MouseButton_1) && lvn::mouseButtonReleased(window, Lvn_MouseButton_2))
	{
		s_CameraFirstClick = true;
		lvn::mouseSetInputMode(window, Lvn_MouseInputMode_Normal);
	}

	int width, height;
	lvn::windowGetSize(window, &width, &height);
	camera->projectionMatrix = lvn::perspective(lvn::radians(60.0f), (float)width / (float)height, 0.01f, 1000.0f);
	camera->viewMatrix =
		  lvn::translate(LvnMat4(1.0f), LvnVec3(0.0f, 0.0f, s_Radius))
		* lvn::rotate(LvnMat4(1.0f), lvn::radians(s_AngleY), LvnVec3(1.0f, 0.0f, 0.0f))
		* lvn::rotate(LvnMat4(1.0f), lvn::radians(s_AngleX), LvnVec3(0.0f, 1.0f, 0.0f))
		* s_Model;

	camera->matrix = camera->projectionMatrix * camera->viewMatrix;
}

LvnDescriptorBinding textureBinding(uint32_t binding, uint32_t maxAllocations)
{
	LvnDescriptorBinding combinedImageDescriptorBinding{};
	combinedImageDescriptorBinding.binding = binding;
	combinedImageDescriptorBinding.descriptorType = Lvn_DescriptorType_CombinedImageSampler;
	combinedImageDescriptorBinding.shaderStage = Lvn_ShaderStage_Fragment;
	combinedImageDescriptorBinding.descriptorCount = 1;
	combinedImageDescriptorBinding.maxAllocations = maxAllocations;

	return combinedImageDescriptorBinding;
}


int main()
{
	LvnContextCreateInfo lvnCreateInfo{};
	lvnCreateInfo.logging.enableLogging = true;
	lvnCreateInfo.logging.enableVulkanValidationLayers = true;
	lvnCreateInfo.windowapi = Lvn_WindowApi_glfw;
	lvnCreateInfo.graphicsapi = Lvn_GraphicsApi_vulkan;
	lvnCreateInfo.frameBufferColorFormat = Lvn_TextureFormat_Srgb;

	lvn::createContext(&lvnCreateInfo);

	LVN_TRACE("testing trace log");
	LVN_DEBUG("testing debug log");
	LVN_INFO("testing info log");
	LVN_WARN("testing warn log");
	LVN_ERROR("testing error log");
	LVN_FATAL("testing fatal log");

	uint32_t deviceCount = 0;
	std::vector<LvnPhysicalDevice*> devices;
	lvn::getPhysicalDevices(nullptr, &deviceCount);

	devices.resize(deviceCount);
	lvn::getPhysicalDevices(devices.data(), &deviceCount);

	LvnPhysicalDevice* selectedPhysicalDevice = nullptr;

	for (uint32_t i = 0; i < deviceCount; i++)
	{
		LvnPhysicalDeviceInfo deviceInfo = lvn::getPhysicalDeviceInfo(devices[i]);
		if (lvn::checkPhysicalDeviceSupport(devices[i]) == Lvn_Result_Success)
		{
			selectedPhysicalDevice = devices[i];
			break;
		}
	}

	if (selectedPhysicalDevice == nullptr)
	{
		LVN_TRACE("no physical device supported");
		return -1;
	}

	LvnRenderInitInfo renderInfo{};
	renderInfo.physicalDevice = selectedPhysicalDevice;
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
	windowInfo.eventCallBack = eventsCallbackFn;
	windowInfo.userData = nullptr;

	LvnWindow* window;
	lvn::createWindow(&window, &windowInfo);


	LvnFrameBufferColorAttachment frameBufferColorAttachment = { 0, Lvn_ColorImageFormat_RGBA32F };
	LvnFrameBufferDepthAttachment frameBufferDepthAttachment = { 1, Lvn_DepthImageFormat_Depth32Stencil8 };

	LvnFrameBufferCreateInfo frameBufferCreateInfo{};
	frameBufferCreateInfo.width = 800;
	frameBufferCreateInfo.height = 600;
	frameBufferCreateInfo.sampleCount = Lvn_SampleCount_8_Bit;
	frameBufferCreateInfo.pColorAttachments = &frameBufferColorAttachment;
	frameBufferCreateInfo.colorAttachmentCount = 1;
	frameBufferCreateInfo.depthAttachment = &frameBufferDepthAttachment;
	frameBufferCreateInfo.textureMode = Lvn_TextureMode_ClampToEdge;
	frameBufferCreateInfo.textureFilter = Lvn_TextureFilter_Linear;

	LvnFrameBuffer* frameBuffer;
	lvn::createFrameBuffer(&frameBuffer, &frameBufferCreateInfo);

	LvnShaderCreateInfo shaderCreateInfo{};
	shaderCreateInfo.vertexSrc = "/home/bma/Documents/dev/levikno/examples/basicScene/res/shaders/pbr.vert";
	shaderCreateInfo.fragmentSrc = "/home/bma/Documents/dev/levikno/examples/basicScene/res/shaders/pbr.frag";

	LvnShader* shader;
	lvn::createShaderFromFileSrc(&shader, &shaderCreateInfo);


	LvnVertexBindingDescription vertexBindingDescroption{};
	vertexBindingDescroption.stride = 8 * sizeof(float);
	vertexBindingDescroption.binding = 0;

	LvnVertexBindingDescription lvnVertexBindingDescription{};
	lvnVertexBindingDescription.stride = sizeof(LvnVertex);
	lvnVertexBindingDescription.binding = 0;

	LvnVertexAttribute attributes[3] =
	{
		{ 0, 0, Lvn_VertexDataType_Vec3f, 0 },
		{ 0, 1, Lvn_VertexDataType_Vec3f, (3 * sizeof(float)) },
		{ 0, 2, Lvn_VertexDataType_Vec2f, (6 * sizeof(float)) },
	};

	LvnVertexAttribute lvnAttributes[6] =
	{
		{ 0, 0, Lvn_VertexDataType_Vec3f, 0 },                   // pos
		{ 0, 1, Lvn_VertexDataType_Vec4f, 3 * sizeof(float) },   // color
		{ 0, 2, Lvn_VertexDataType_Vec2f, 7 * sizeof(float) },   // texUV
		{ 0, 3, Lvn_VertexDataType_Vec3f, 9 * sizeof(float) },   // normal
		{ 0, 4, Lvn_VertexDataType_Vec3f, 12 * sizeof(float) },  // tangent
		{ 0, 5, Lvn_VertexDataType_Vec3f, 15 * sizeof(float) },  // bitangent
	};

	LvnDescriptorBinding uniformDescriptorBinding{};
	uniformDescriptorBinding.binding = 0;
	uniformDescriptorBinding.descriptorType = Lvn_DescriptorType_UniformBuffer;
	uniformDescriptorBinding.shaderStage = Lvn_ShaderStage_Vertex;
	uniformDescriptorBinding.descriptorCount = 1;
	uniformDescriptorBinding.maxAllocations = 1;

	LvnDescriptorBinding pbrUniformDescriptorBinding{};
	pbrUniformDescriptorBinding.binding = 1;
	pbrUniformDescriptorBinding.descriptorType = Lvn_DescriptorType_UniformBuffer;
	pbrUniformDescriptorBinding.shaderStage = Lvn_ShaderStage_All;
	pbrUniformDescriptorBinding.descriptorCount = 1;
	pbrUniformDescriptorBinding.maxAllocations = 256;

	LvnDescriptorBinding storageDescriptorBinding{};
	storageDescriptorBinding.binding = 0;
	storageDescriptorBinding.descriptorType = Lvn_DescriptorType_StorageBuffer;
	storageDescriptorBinding.shaderStage = Lvn_ShaderStage_Vertex;
	storageDescriptorBinding.descriptorCount = 1;
	storageDescriptorBinding.maxAllocations = 256;

	LvnDescriptorBinding combinedImageDescriptorBinding{};
	combinedImageDescriptorBinding.binding = 1;
	combinedImageDescriptorBinding.descriptorType = Lvn_DescriptorType_CombinedImageSampler;
	combinedImageDescriptorBinding.shaderStage = Lvn_ShaderStage_Fragment;
	combinedImageDescriptorBinding.descriptorCount = 1;
	combinedImageDescriptorBinding.maxAllocations = 256;

	std::vector<LvnDescriptorBinding> descriptorBindings =
	{
		storageDescriptorBinding, pbrUniformDescriptorBinding,
		textureBinding(2, 256),
		textureBinding(3, 256),
		textureBinding(4, 256),
		// textureBinding(5, 256),
	};

	LvnDescriptorLayoutCreateInfo descriptorLayoutCreateInfo{};
	descriptorLayoutCreateInfo.pDescriptorBindings = descriptorBindings.data();
	descriptorLayoutCreateInfo.descriptorBindingCount = descriptorBindings.size();
	descriptorLayoutCreateInfo.maxSets = 256;

	LvnDescriptorLayout* descriptorLayout;
	lvn::createDescriptorLayout(&descriptorLayout, &descriptorLayoutCreateInfo);

	LvnDescriptorSet* descriptorSet;
	lvn::createDescriptorSet(&descriptorSet, descriptorLayout);

	LvnPipelineSpecification pipelineSpec = lvn::pipelineSpecificationGetConfig();
	pipelineSpec.depthstencil.enableDepth = true;
	pipelineSpec.depthstencil.depthOpCompare = Lvn_CompareOperation_Less;
	// pipelineSpec.rasterizer.cullMode = Lvn_CullFaceMode_Back;
	// pipelineSpec.rasterizer.frontFace = Lvn_CullFrontFace_CCW;

	LvnPipelineCreateInfo pipelineCreateInfo{};
	pipelineCreateInfo.pipelineSpecification = &pipelineSpec;
	pipelineCreateInfo.pVertexBindingDescriptions = &lvnVertexBindingDescription;
	pipelineCreateInfo.vertexBindingDescriptionCount = 1;
	pipelineCreateInfo.pVertexAttributes = lvnAttributes;
	pipelineCreateInfo.vertexAttributeCount = 6;
	pipelineCreateInfo.pDescriptorLayouts = &descriptorLayout;
	pipelineCreateInfo.descriptorLayoutCount = 1;
	pipelineCreateInfo.shader = shader;
	pipelineCreateInfo.renderPass = lvn::frameBufferGetRenderPass(frameBuffer);
	pipelineCreateInfo.pipelineSpecification->multisampling.rasterizationSamples = Lvn_SampleCount_8_Bit;

	LvnPipeline* pipeline;
	lvn::createPipeline(&pipeline, &pipelineCreateInfo);

	lvn::destroyShader(shader);


	// framebuffer pipeline
	LvnShaderCreateInfo fbShaderCreateInfo{};
	fbShaderCreateInfo.vertexSrc = "/home/bma/Documents/dev/levikno/examples/basicScene/res/shaders/vkFB.vert";
	fbShaderCreateInfo.fragmentSrc = "/home/bma/Documents/dev/levikno/examples/basicScene/res/shaders/vkFB.frag";

	LvnShader* fbShader;
	lvn::createShaderFromFileSrc(&fbShader, &fbShaderCreateInfo);

	std::vector<LvnDescriptorBinding> fbDescriptorBinding =
	{
		uniformDescriptorBinding, combinedImageDescriptorBinding,
	};

	LvnDescriptorLayoutCreateInfo fbDescriptorLayoutCreateInfo{};
	fbDescriptorLayoutCreateInfo.pDescriptorBindings = fbDescriptorBinding.data();
	fbDescriptorLayoutCreateInfo.descriptorBindingCount = fbDescriptorBinding.size();
	fbDescriptorLayoutCreateInfo.maxSets = 1;

	LvnDescriptorLayout* fbDescriptorLayout;
	lvn::createDescriptorLayout(&fbDescriptorLayout, &fbDescriptorLayoutCreateInfo);

	LvnDescriptorSet* fbDescriptorSet;
	lvn::createDescriptorSet(&fbDescriptorSet, fbDescriptorLayout);

	pipelineCreateInfo.pDescriptorLayouts = &fbDescriptorLayout;
	pipelineCreateInfo.pVertexBindingDescriptions = &vertexBindingDescroption;
	pipelineCreateInfo.vertexBindingDescriptionCount = 1;
	pipelineCreateInfo.pVertexAttributes = attributes;
	pipelineCreateInfo.vertexAttributeCount = 3;
	pipelineCreateInfo.shader = fbShader;
	pipelineCreateInfo.renderPass = lvn::windowGetRenderPass(window);
	pipelineCreateInfo.pipelineSpecification->multisampling.rasterizationSamples = Lvn_SampleCount_1_Bit;
	// pipelineCreateInfo.pipelineSpecification->rasterizer.cullMode = Lvn_CullFaceMode_Back;
	// pipelineSpec.rasterizer.frontFace = Lvn_CullFrontFace_CW;

	LvnPipeline* fbPipeline;
	lvn::createPipeline(&fbPipeline, &pipelineCreateInfo);

	lvn::destroyShader(fbShader);


	// cubemap pipeline
	LvnVertexBindingDescription cubemapBindingDescription{};
	cubemapBindingDescription.stride = 3 * sizeof(float);
	cubemapBindingDescription.binding = 0;
	
	LvnVertexAttribute cubemapAttributes[1] = 
	{
		{ 0, 0, Lvn_VertexDataType_Vec3f, 0 },
	};
	
	LvnShaderCreateInfo cubemapShaderCreateInfo{};
	cubemapShaderCreateInfo.vertexSrc = "/home/bma/Documents/dev/levikno/examples/basicScene/res/shaders/vkcubemap.vert";
	cubemapShaderCreateInfo.fragmentSrc = "/home/bma/Documents/dev/levikno/examples/basicScene/res/shaders/vkcubemap.frag";
	
	LvnShader* cubemapShader;
	lvn::createShaderFromFileSrc(&cubemapShader, &cubemapShaderCreateInfo);
	
	std::vector<LvnDescriptorBinding> cubemapDescriptorBinding =
	{
		uniformDescriptorBinding, combinedImageDescriptorBinding,
	};
	
	LvnDescriptorLayoutCreateInfo cubemapDescriptorLayoutCreateInfo{};
	cubemapDescriptorLayoutCreateInfo.pDescriptorBindings = cubemapDescriptorBinding.data();
	cubemapDescriptorLayoutCreateInfo.descriptorBindingCount = cubemapDescriptorBinding.size();
	cubemapDescriptorLayoutCreateInfo.maxSets = 1;
	
	LvnDescriptorLayout* cubemapDescriptorLayout;
	lvn::createDescriptorLayout(&cubemapDescriptorLayout, &cubemapDescriptorLayoutCreateInfo);

	LvnDescriptorSet* cubemapDescriptorSet;
	lvn::createDescriptorSet(&cubemapDescriptorSet, cubemapDescriptorLayout);

	pipelineCreateInfo.pDescriptorLayouts = &cubemapDescriptorLayout;
	pipelineCreateInfo.pVertexBindingDescriptions = &cubemapBindingDescription;
	pipelineCreateInfo.vertexBindingDescriptionCount = 1;
	pipelineCreateInfo.pVertexAttributes = cubemapAttributes;
	pipelineCreateInfo.vertexAttributeCount = 1;
	pipelineCreateInfo.shader = cubemapShader;
	pipelineCreateInfo.renderPass = lvn::frameBufferGetRenderPass(frameBuffer);
	pipelineCreateInfo.pipelineSpecification->depthstencil.depthOpCompare = Lvn_CompareOperation_LessOrEqual;
	pipelineCreateInfo.pipelineSpecification->multisampling.rasterizationSamples = Lvn_SampleCount_8_Bit;
	// pipelineCreateInfo.pipelineSpecification->rasterizer.cullMode = Lvn_CullFaceMode_Front;
	// pipelineSpec.rasterizer.frontFace = Lvn_CullFrontFace_CW;
	
	LvnPipeline* cubemapPipeline;
	lvn::createPipeline(&cubemapPipeline, &pipelineCreateInfo);

	lvn::destroyShader(cubemapShader);


	// vertex/index buffer
	LvnBufferCreateInfo bufferCreateInfo{};
	bufferCreateInfo.type = Lvn_BufferType_Vertex | Lvn_BufferType_Index;
	bufferCreateInfo.pVertexBindingDescriptions = &vertexBindingDescroption;
	bufferCreateInfo.vertexBindingDescriptionCount = 1;
	bufferCreateInfo.pVertexAttributes = attributes;
	bufferCreateInfo.vertexAttributeCount = 3;

	bufferCreateInfo.pVertices = vertices;
	bufferCreateInfo.vertexBufferSize = sizeof(vertices);

	bufferCreateInfo.pIndices = indices;
	bufferCreateInfo.indexBufferSize = sizeof(indices);

	LvnBuffer* buffer;
	lvn::createBuffer(&buffer, &bufferCreateInfo);

	LvnBufferCreateInfo cubemapBufferCreateInfo{};
	cubemapBufferCreateInfo.type = Lvn_BufferType_Vertex | Lvn_BufferType_Index;
	cubemapBufferCreateInfo.pVertexBindingDescriptions = &cubemapBindingDescription;
	cubemapBufferCreateInfo.vertexBindingDescriptionCount = 1;
	cubemapBufferCreateInfo.pVertexAttributes = cubemapAttributes;
	cubemapBufferCreateInfo.vertexAttributeCount = 1;

	cubemapBufferCreateInfo.pVertices = s_CubemapVertices;
	cubemapBufferCreateInfo.vertexBufferSize = sizeof(s_CubemapVertices);

	cubemapBufferCreateInfo.pIndices = s_CubemapIndices;
	cubemapBufferCreateInfo.indexBufferSize = sizeof(s_CubemapIndices);

	LvnBuffer* cubemapBuffer;
	lvn::createBuffer(&cubemapBuffer, &cubemapBufferCreateInfo);


	// uniform buffer
	LvnUniformBufferCreateInfo uniformBufferInfo{};
	uniformBufferInfo.binding = 0;
	uniformBufferInfo.type = Lvn_BufferType_Storage;
	uniformBufferInfo.size = sizeof(PbrStorageData) * 64;

	LvnUniformBuffer* uniformBuffer;
	lvn::createUniformBuffer(&uniformBuffer, &uniformBufferInfo);

	LvnUniformBufferCreateInfo pbrUniformBufferInfo{};
	pbrUniformBufferInfo.binding = 1;
	pbrUniformBufferInfo.type = Lvn_BufferType_Uniform;
	pbrUniformBufferInfo.size = sizeof(PbrUniformData);

	LvnUniformBuffer* pbrUniformBuffer;
	lvn::createUniformBuffer(&pbrUniformBuffer, &pbrUniformBufferInfo);

	LvnUniformBufferCreateInfo fbUniformBufferInfo{};
	fbUniformBufferInfo.binding = 0;
	fbUniformBufferInfo.type = Lvn_BufferType_Uniform;
	fbUniformBufferInfo.size = sizeof(UniformData);

	LvnUniformBuffer* fbUniformBuffer;
	lvn::createUniformBuffer(&fbUniformBuffer, &fbUniformBufferInfo);


	LvnUniformBufferCreateInfo cubemapUniformBufferInfo{};
	cubemapUniformBufferInfo.binding = 0;
	cubemapUniformBufferInfo.type = Lvn_BufferType_Uniform;
	cubemapUniformBufferInfo.size = sizeof(UniformData);

	LvnUniformBuffer* cubemapUniformBuffer;
	lvn::createUniformBuffer(&cubemapUniformBuffer, &cubemapUniformBufferInfo);

	// texture
	LvnTextureCreateInfo textureCreateInfo{};
	textureCreateInfo.imageData = lvn::loadImageData("/home/bma/Documents/dev/levikno/examples/basicScene/res/images/grass.png", 4);
	textureCreateInfo.minFilter = Lvn_TextureFilter_Linear;
	textureCreateInfo.magFilter = Lvn_TextureFilter_Linear;
	textureCreateInfo.wrapMode = Lvn_TextureMode_Repeat;

	LvnTexture* texture;
	lvn::createTexture(&texture, &textureCreateInfo);

	textureCreateInfo.imageData = lvn::loadImageData("/home/bma/Documents/dev/levikno/examples/basicScene/res/images/debug.png", 4);
	textureCreateInfo.minFilter = Lvn_TextureFilter_Linear;
	textureCreateInfo.magFilter = Lvn_TextureFilter_Linear;
	textureCreateInfo.wrapMode = Lvn_TextureMode_Repeat;

	LvnTexture* texture2;
	lvn::createTexture(&texture2, &textureCreateInfo);


	// cubemap
	LvnCubemapCreateInfo cubemapCreateInfo{};
	cubemapCreateInfo.posx = lvn::loadImageData("/home/bma/Documents/dev/levikno/examples/basicScene/res/cubemaps/Chapel/posx.jpg", 4);
	cubemapCreateInfo.negx = lvn::loadImageData("/home/bma/Documents/dev/levikno/examples/basicScene/res/cubemaps/Chapel/negx.jpg", 4);
	cubemapCreateInfo.posy = lvn::loadImageData("/home/bma/Documents/dev/levikno/examples/basicScene/res/cubemaps/Chapel/posy.jpg", 4);
	cubemapCreateInfo.negy = lvn::loadImageData("/home/bma/Documents/dev/levikno/examples/basicScene/res/cubemaps/Chapel/negy.jpg", 4);
	cubemapCreateInfo.posz = lvn::loadImageData("/home/bma/Documents/dev/levikno/examples/basicScene/res/cubemaps/Chapel/posz.jpg", 4);
	cubemapCreateInfo.negz = lvn::loadImageData("/home/bma/Documents/dev/levikno/examples/basicScene/res/cubemaps/Chapel/negz.jpg", 4);

	LvnCubemap* cubemap;
	lvn::createCubemap(&cubemap, &cubemapCreateInfo);

	// update descriptor sets

	LvnDescriptorUpdateInfo fbDescriptorUniformUpdateInfo{};
	fbDescriptorUniformUpdateInfo.descriptorType = Lvn_DescriptorType_UniformBuffer;
	fbDescriptorUniformUpdateInfo.binding = 0;
	fbDescriptorUniformUpdateInfo.descriptorCount = 1;
	fbDescriptorUniformUpdateInfo.bufferInfo = fbUniformBuffer;

	LvnDescriptorUpdateInfo fbDescriptorTextureUpdateInfo{};
	fbDescriptorTextureUpdateInfo.descriptorType = Lvn_DescriptorType_CombinedImageSampler;
	fbDescriptorTextureUpdateInfo.binding = 1;
	fbDescriptorTextureUpdateInfo.descriptorCount = 1;
	fbDescriptorTextureUpdateInfo.textureInfo = lvn::frameBufferGetImage(frameBuffer, 0);

	std::vector<LvnDescriptorUpdateInfo> fbDescriptorUpdateInfo =
	{
		fbDescriptorUniformUpdateInfo, fbDescriptorTextureUpdateInfo,
	};

	lvn::updateDescriptorSetData(fbDescriptorSet, fbDescriptorUpdateInfo.data(), fbDescriptorUpdateInfo.size());

	LvnDescriptorUpdateInfo cubemapDescriptorUniformUpdateInfo{};
	cubemapDescriptorUniformUpdateInfo.descriptorType = Lvn_DescriptorType_UniformBuffer;
	cubemapDescriptorUniformUpdateInfo.binding = 0;
	cubemapDescriptorUniformUpdateInfo.descriptorCount = 1;
	cubemapDescriptorUniformUpdateInfo.bufferInfo = cubemapUniformBuffer;

	LvnDescriptorUpdateInfo cubemapDescriptorTextureUpdateInfo{};
	cubemapDescriptorTextureUpdateInfo.descriptorType = Lvn_DescriptorType_CombinedImageSampler;
	cubemapDescriptorTextureUpdateInfo.binding = 1;
	cubemapDescriptorTextureUpdateInfo.descriptorCount = 1;
	cubemapDescriptorTextureUpdateInfo.textureInfo = lvn::cubemapGetTextureData(cubemap);

	std::vector<LvnDescriptorUpdateInfo> cubemapDescriptorUpdateInfo =
	{
		cubemapDescriptorUniformUpdateInfo, cubemapDescriptorTextureUpdateInfo,
	};

	lvn::updateDescriptorSetData(cubemapDescriptorSet, cubemapDescriptorUpdateInfo.data(), cubemapDescriptorUpdateInfo.size());


	// mesh
	LvnBufferCreateInfo meshBufferInfo{};
	meshBufferInfo.type = Lvn_BufferType_Vertex | Lvn_BufferType_Index;
	meshBufferInfo.pVertexBindingDescriptions = &lvnVertexBindingDescription;
	meshBufferInfo.vertexBindingDescriptionCount = 1;
	meshBufferInfo.pVertexAttributes = lvnAttributes;
	meshBufferInfo.vertexAttributeCount = 6;
	meshBufferInfo.pVertices = lvnVertices.data();
	meshBufferInfo.vertexBufferSize = lvnVertices.size() * sizeof(LvnVertex);
	meshBufferInfo.pIndices = indices;
	meshBufferInfo.indexBufferSize = sizeof(indices);

	LvnMeshCreateInfo meshCreateInfo{};
	meshCreateInfo.bufferInfo = &meshBufferInfo;

	LvnMesh mesh = lvn::createMesh(&meshCreateInfo);

	LvnCameraCreateInfo cameraCreateInfo{};
	cameraCreateInfo.width = lvn::windowGetSize(window).width;
	cameraCreateInfo.height = lvn::windowGetSize(window).height;
	cameraCreateInfo.position = LvnVec3(0.0f, 0.0f, -1.0f);
	cameraCreateInfo.orientation = LvnVec3(0.0f, 0.0f, 1.0f);
	cameraCreateInfo.upVector = LvnVec3(0.0f, 1.0f, 0.0f);
	cameraCreateInfo.fovDeg = 60.0f;
	cameraCreateInfo.nearPlane = 0.1f;
	cameraCreateInfo.farPlane = 100.0f;
	LvnCamera camera = lvn::cameraConfigInit(&cameraCreateInfo);


	LvnModel lvnmodel = lvn::loadModel("/home/bma/Documents/models/gltf/gameboy/scene.gltf");

	LvnDescriptorUpdateInfo descriptorUniformUpdateInfo{};
	descriptorUniformUpdateInfo.descriptorType = Lvn_DescriptorType_StorageBuffer;
	descriptorUniformUpdateInfo.binding = 0;
	descriptorUniformUpdateInfo.descriptorCount = 1;
	descriptorUniformUpdateInfo.bufferInfo = uniformBuffer;

	LvnDescriptorUpdateInfo descriptorPbrUniformUpdateInfo{};
	descriptorPbrUniformUpdateInfo.descriptorType = Lvn_DescriptorType_UniformBuffer;
	descriptorPbrUniformUpdateInfo.binding = 1;
	descriptorPbrUniformUpdateInfo.descriptorCount = 1;
	descriptorPbrUniformUpdateInfo.bufferInfo = pbrUniformBuffer;

	std::vector<LvnDescriptorUpdateInfo> pbrDescriptorUpdateInfo = 
	{
		descriptorUniformUpdateInfo,
		descriptorPbrUniformUpdateInfo,
		{ 2, Lvn_DescriptorType_CombinedImageSampler, 1, nullptr, lvnmodel.meshes[0].material.albedo },
		{ 3, Lvn_DescriptorType_CombinedImageSampler, 1, nullptr, lvnmodel.meshes[0].material.metallicRoughnessOcclusion },
		{ 4, Lvn_DescriptorType_CombinedImageSampler, 1, nullptr, lvnmodel.meshes[0].material.normal },
	};

	lvn::updateDescriptorSetData(descriptorSet, pbrDescriptorUpdateInfo.data(), pbrDescriptorUpdateInfo.size());


	LvnSoundCreateInfo soundCreateInfo{};
	soundCreateInfo.filepath = "/home/bma/Downloads/Soundtrack - Red Alert 2 - Blow It Up.mp3"; // your audio filepath here
	soundCreateInfo.volume = 1.0f;
	soundCreateInfo.pitch = 1.0f;
	soundCreateInfo.pan = 0.0f;
	soundCreateInfo.looping = false;

	LvnSound* sound;
	lvn::createSoundFromFile(&sound, &soundCreateInfo);

	SoundData sndData = { .sound = sound, .vol = 1.0f, .pan = 0.0f, .pitch = 1.0f };
	lvn::windowSetEventCallback(window, eventsCallbackFn, &sndData);


	LvnFont font = lvn::loadFontFromFileTTF("/home/bma/Documents/dev/JetBrainsMonoNerdFont-Regular.ttf", 32, { 32, 126 });


	lvn::soundPlayStart(sound);

	auto startTime = std::chrono::high_resolution_clock::now();

	Timer timer;
	int fps;
	timer.start();

	float oldTime = 0.0f;
	Timer deltaTime;
	deltaTime.start();

	UniformData uniformData{};
	PbrUniformData pbrData{};
	std::vector<PbrStorageData> objectData = {};
	int winWidth, winHeight;

	while (lvn::windowOpen(window))
	{
		lvn::windowUpdate(window);

		// auto [x, y] = lvn::getWindowDimensions(window);
		// LVN_TRACE("(x:%d,y:%d)", x, y);
		auto windowSize = lvn::windowGetDimensions(window);
		int width = windowSize.width, height = windowSize.height;

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		lvn::mat4 model = lvn::scale(lvn::mat4(1.0f), lvn::vec3(1.01f)); // * lvn::rotate(lvn::mat4(1.0f), time * lvn::radians(30.0f), lvn::vec3(0.0f, 1.0f, 0.0f));

		camera.width = width;
		camera.height = height;

		float timeNow = deltaTime.elapsed();
		float dt = timeNow - oldTime;
		oldTime = timeNow;
		orbitMovment(window, &camera, dt);
		
		if (winWidth != width || winHeight != height)
		{
			lvn::frameBufferResize(frameBuffer, width, height);

			fbDescriptorUniformUpdateInfo.descriptorType = Lvn_DescriptorType_UniformBuffer;
			fbDescriptorUniformUpdateInfo.binding = 0;
			fbDescriptorUniformUpdateInfo.descriptorCount = 1;
			fbDescriptorUniformUpdateInfo.bufferInfo = fbUniformBuffer;

			fbDescriptorTextureUpdateInfo.descriptorType = Lvn_DescriptorType_CombinedImageSampler;
			fbDescriptorTextureUpdateInfo.binding = 1;
			fbDescriptorTextureUpdateInfo.descriptorCount = 1;
			fbDescriptorTextureUpdateInfo.textureInfo = lvn::frameBufferGetImage(frameBuffer, 0);

			fbDescriptorUpdateInfo =
			{
				fbDescriptorUniformUpdateInfo, fbDescriptorTextureUpdateInfo,
			};

			lvn::updateDescriptorSetData(fbDescriptorSet, fbDescriptorUpdateInfo.data(), fbDescriptorUpdateInfo.size());
		}

		lvn::renderBeginNextFrame(window);
		lvn::renderBeginCommandRecording(window);
		lvn::renderCmdBeginFrameBuffer(window, frameBuffer);

		objectData.resize(lvnmodel.meshes.size());
		for (uint32_t i = 0; i < lvnmodel.meshes.size(); i++)
		{
			objectData[i].matrix = camera.matrix;
			objectData[i].model = lvnmodel.meshes[i].modelMatrix * model;
		}

		pbrData.campPos = lvn::cameraGetPos(&camera);
		pbrData.lightPos = lvn::vec3(cos(time) * 3.0f, 0.0f, sin(time) * 3.0f);
		pbrData.metalic = 0.5f;
		pbrData.roughness = abs(sin(time));
		pbrData.ambientOcclusion = 1.0f;

		lvn::updateUniformBufferData(window, pbrUniformBuffer, &pbrData, sizeof(PbrUniformData));

		lvn::renderCmdBindPipeline(window, pipeline);
		lvn::updateUniformBufferData(window, uniformBuffer, objectData.data(), sizeof(PbrStorageData) * lvnmodel.meshes.size());
		lvn::renderCmdBindDescriptorSets(window, pipeline, 0, 1, &descriptorSet);

		for (uint32_t i = 0; i < lvnmodel.meshes.size(); i++)
		{
			lvn::renderCmdBindVertexBuffer(window, lvn::meshGetBuffer(&lvnmodel.meshes[i]));
			lvn::renderCmdBindIndexBuffer(window, lvn::meshGetBuffer(&lvnmodel.meshes[i]));

			lvn::renderCmdDrawIndexedInstanced(window, lvnmodel.meshes[i].indexCount, 1, i);
		}

		// draw cubemap
		lvn::mat4 projection = camera.projectionMatrix;
		lvn::mat4 view = lvn::mat4(lvn::mat3(camera.viewMatrix));
		uniformData.matrix = projection * view;

		lvn::renderCmdBindPipeline(window, cubemapPipeline);
		lvn::updateUniformBufferData(window, cubemapUniformBuffer, &uniformData, sizeof(UniformData));
		lvn::renderCmdBindDescriptorSets(window, cubemapPipeline, 0, 1, &cubemapDescriptorSet);

		lvn::renderCmdBindVertexBuffer(window, cubemapBuffer);
		lvn::renderCmdBindIndexBuffer(window, cubemapBuffer);

		lvn::renderCmdDrawIndexed(window, 36);

		lvn::renderCmdEndFrameBuffer(window, frameBuffer);


		// begin main render pass
		lvn::renderClearColor(window, abs(sin(time)) * 0.1f, 0.0f, abs(cos(time)) * 0.1f, 1.0f);
		lvn::renderCmdBeginRenderPass(window);

		uniformData.matrix = lvn::mat4(1.0f);

		lvn::renderCmdBindPipeline(window, fbPipeline);
		lvn::updateUniformBufferData(window, fbUniformBuffer, &uniformData, sizeof(UniformData));

		lvn::renderCmdBindVertexBuffer(window, buffer);
		lvn::renderCmdBindIndexBuffer(window, buffer);

		lvn::renderCmdBindDescriptorSets(window, fbPipeline, 0, 1, &fbDescriptorSet);
		lvn::renderCmdDrawIndexed(window, sizeof(indices) / sizeof(indices[0]));

		lvn::renderCmdEndRenderPass(window);
		lvn::renderEndCommandRecording(window);
		lvn::renderDrawSubmit(window);

		winWidth = width;
		winHeight = height;

		fps++;
		if (timer.elapsed() >= 1.0f)
		{
			LVN_TRACE("FPS: %d", fps);
			timer.reset();
			fps = 0;
		}
	}

	lvn::destroySound(sound);

	lvn::freeModel(&lvnmodel);
	lvn::destroyMesh(&mesh);
	lvn::destroyCubemap(cubemap);
	lvn::destroyFrameBuffer(frameBuffer);

	lvn::destroyTexture(texture);
	lvn::destroyTexture(texture2);

	lvn::destroyBuffer(buffer);
	lvn::destroyBuffer(cubemapBuffer);

	lvn::destroyUniformBuffer(cubemapUniformBuffer);
	lvn::destroyUniformBuffer(fbUniformBuffer);
	lvn::destroyUniformBuffer(uniformBuffer);
	lvn::destroyUniformBuffer(pbrUniformBuffer);

	lvn::destroyDescriptorSet(descriptorSet);
	lvn::destroyDescriptorSet(fbDescriptorSet);
	lvn::destroyDescriptorSet(cubemapDescriptorSet);

	lvn::destroyDescriptorLayout(cubemapDescriptorLayout);
	lvn::destroyDescriptorLayout(fbDescriptorLayout);
	lvn::destroyDescriptorLayout(descriptorLayout);

	lvn::destroyPipeline(cubemapPipeline);
	lvn::destroyPipeline(fbPipeline);
	lvn::destroyPipeline(pipeline);

	lvn::destroyWindow(window);
	lvn::terminateContext();

}
