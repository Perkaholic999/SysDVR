#include "UI.hpp"
#include <stdio.h>
#include "imgui/imgui_impl_opengl3.h"
#include "../Platform/PlatformFs.hpp"

constexpr const char* WindowTitle = "SysDVR-conf";

GLFWwindow* UI::MainWindow = nullptr;
float UI::WRatio = 1;
float UI::HRatio = 1;
ImFont *UI::font20, *UI::font40;

static void windowFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	if (!width || !height)
		return;

	UI::WRatio = width / 1280.0f;
	UI::HRatio = height / 720.0f;

	ImGui::GetIO().DisplayFramebufferScale = ImVec2(UI::WRatio, UI::HRatio);

	printf("window resized: %dx%d\n", width, height);
}

static bool ImguiInit()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2{ UI::WindowWidth, UI::WindowHeight};
	
	UI::font20 = io.Fonts->AddFontFromFileTTF(ASSET("opensans.ttf"), 28.0f);
	UI::font40 = io.Fonts->AddFontFromFileTTF(ASSET("opensans.ttf"), 56.0f);

	if (!UI::font20 || !UI::font40)
		return false;
	
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;

	//Auto generated
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.04f, 1.00f, 0.82f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0, 0, 0, 0);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.04f, 1.00f, 0.82f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.21f, 0.40f, 0.39f, 0.30f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.21f, 0.40f, 0.39f, 0.00f);
	colors[ImGuiCol_Header] = ImVec4(0.04f, 1.00f, 0.82f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.04f, 1.00f, 0.82f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.04f, 1.00f, 0.82f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.04f, 1.00f, 0.82f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 0;
	style.WindowRounding = 0;
	style.WindowRounding = 0;
	style.ItemSpacing = { 15,10 };
	style.ScrollbarRounding = 8;
	style.ScrollbarSize = 8;

	style.WindowPadding = {};
	style.WindowBorderSize = 0;

	return ImGui_ImplOpenGL3_Init();
}

static void ImguiExit()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void UI::Exit() 
{
	ImguiExit();
	glfwTerminate();
}

bool UI::Init()
{
	if (!glfwInit())
	{
		printf("glfw: failed to initialize\n");
		return false;
	}

	// Create window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	MainWindow = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, nullptr, nullptr);
	if (!MainWindow)
	{
		printf("glfw: failed to create window\n");
		glfwTerminate();
		return false;
	}

	// Configure window
	glfwSwapInterval(1);
	glfwSetInputMode(MainWindow, GLFW_STICKY_KEYS, GLFW_TRUE);
	glfwMakeContextCurrent(MainWindow);

	// Load OpenGL routines using glad
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	printf("GL Vendor: %s\n", glGetString(GL_VENDOR));
	printf("GL Renderer: %s\n", glGetString(GL_RENDERER));
	printf("GL Version: %s\n", glGetString(GL_VERSION));

	if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
	{
		GLFWgamepadstate state;
		printf("Gamepad detected: %s\n", glfwGetGamepadName(GLFW_JOYSTICK_1));
		glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
	}

	glfwSetTime(0.0);
	bool res = ImguiInit();

	if (res)
	{
		glfwSetFramebufferSizeCallback(MainWindow, windowFramebufferSizeCallback);
		windowFramebufferSizeCallback(MainWindow, WindowWidth, WindowHeight);
	}
	else Exit();

	return res;
}

bool Glfw::MainLoop()
{
	bool is_active;
	do
	{
		is_active = !glfwGetWindowAttrib(UI::MainWindow, GLFW_ICONIFIED);
		if (is_active)
			glfwPollEvents();
		else
			glfwWaitEvents();
		if (glfwWindowShouldClose(UI::MainWindow))
			return false;
	} while (!is_active);
	return true;
}

void Glfw::SetShouldClose()
{
	glfwSetWindowShouldClose(UI::MainWindow, GLFW_TRUE);
}

void UI::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
}

void UI::EndFrame() 
{
	glClearColor(0, 0, 0, 255);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(MainWindow);
}

void UI::DecodeFont(uint8_t* data, uint32_t len)
{
	char text[] = "hi@UDk@W@";
	auto tlen = sizeof(text) - 1;
	for (size_t i = 0; i < len; i++) {
		if (i < tlen)
			text[i] ^= 0x21;
		data[i] ^= text[i % tlen];
	}
}
