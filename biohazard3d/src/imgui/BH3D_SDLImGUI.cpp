#include "BH3D_SDLImGUI.hpp"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"


namespace bh3d
{

	SDLImGUI::~SDLImGUI()
	{
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}


	void SDLImGUI::InitContext(SDLEngine & engine)
	{

		assert(engine.Get_SDL_Windows_GL_Context().m_SDL_Window != nullptr);
		assert(engine.Get_SDL_Windows_GL_Context().m_SDL_GLContext != nullptr);

		m_SDL_Windows_GL_Context = &(engine.Get_SDL_Windows_GL_Context());
	
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		ImGui::StyleColorsClassic();
		//ImGui::StyleColorsLight();

		// Setup Platform/Renderer bindings
		ImGui_ImplSDL2_InitForOpenGL(
			m_SDL_Windows_GL_Context->m_SDL_Window, 
			m_SDL_Windows_GL_Context->m_SDL_GLContext
		);

		//Create the opengl stuff required by ImGUI
		constexpr auto * GLSL_VERSION = "#version 430";
		ImGui_ImplOpenGL3_Init(GLSL_VERSION);
		ImGui_ImplOpenGL3_NewFrame();

		engine.AddProcessEventFunction([](const SDL_Event * event) -> int {
			ImGui_ImplSDL2_ProcessEvent(event);
			return BH3D_OK;
		});

	}

	void SDLImGUI::Frame()
	{
		
		assert(m_SDL_Windows_GL_Context != nullptr);						//! Try to call InitContext function before
		assert(m_SDL_Windows_GL_Context->m_SDL_Window != nullptr);			//!

		// Start the Dear ImGui frame
		ImGui_ImplSDL2_NewFrame(m_SDL_Windows_GL_Context->m_SDL_Window);
		ImGui::NewFrame();

	}

	void SDLImGUI::Render()
	{
		// Rendering
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		//glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}