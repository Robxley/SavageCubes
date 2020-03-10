#pragma once

#ifndef _BH3D_SDL_ENGINE_H_
#define _BH3D_SDL_ENGINE_H_

#include <any>
#include <type_traits>

#include "BH3D_Camera.hpp"
#include "BH3D_Event.hpp"
#include "BH3D_SDLTextureManager.hpp"
#include "BH3D_TinyEngine.hpp"

//Redefine some SDL opengl
struct SDL_Window;
typedef union SDL_Event SDL_Event;
typedef void* SDL_GLContext;

namespace bh3d
{

	struct WindowInfo
	{
		int width = 800;
		int height = 600;
		std::string title = "SDL GL window - TinyEngine - Biohazard3d";
		int glContextMajorVersion = 4;
		int glContextMinorVersion = 3;
		int glDoubleBuffer = 1;
		int glDepthSize = 24;
		bool fullscreen = false;
		bool borderless = false;
		int vsync = 1;
		int glMultiSamples = 4;
	};

	class SDL_Windows_GL_Context
	{
	public:
		SDL_Window *m_SDL_Window = nullptr;
		SDL_GLContext m_SDL_GLContext = nullptr;

		operator SDL_Window *() { return m_SDL_Window; }
		operator SDL_GLContext() { return m_SDL_GLContext; }

		~SDL_Windows_GL_Context();
	};



	class SDLEngine : public TinyEngine
	{

	public:

		using FProcessEvent = std::function<int(const SDL_Event*)>;

		WindowInfo m_windowInfo;

	public:

		template<typename TWindowInfo>
		SDLEngine(TWindowInfo&& windowInfo) :
			m_windowInfo(std::forward<TWindowInfo>(windowInfo))
		{}

		void Init() override;

		// Create a SDL windows with a opengl context
		int CreateWindow();

		// Window events managed by SDL (windows resize, mouse, keyboard....)
		int PollEvents();

		//Main loop of the opengl application
		virtual void Run();

		// Call when the windows is resized
		virtual void Resize();

		// The width and height are ignored with fullscreen flag or with negative values.
		void SetWindowFullScreen(bool borderless = true); 

		// Leave the fullscreen mode. The width and height are ignored with negative values.
		void SetWindowedMode(int width = -1, int height = -1);

		// Swap our buffer to display the current contents of buffer on screen
		inline void SwapWindow();

		//Input update functions
		virtual void Update();

		//Bind the opengl viewport with the size of the windows
		inline void GLViewport() {
			glViewport(0, 0, m_windowInfo.width, m_windowInfo.height);
		}
		//Bind the opengl viewport with the size of the windows
		inline void GLViewportScissor() {
			glViewport(0, 0, m_windowInfo.width, m_windowInfo.height);
			glScissor(0, 0, m_windowInfo.width, m_windowInfo.height);
		}


		template<class Func>
		/// <summary>
		/// Add a function inside the SDL loop for event handling
		/// </summary>
		/// <param name="func">Function to add</param>
		void AddProcessEventFunction(Func && func);

		auto * Get_SDL_Window() const { return m_SDL_Windows_GL_Context.m_SDL_Window; }
		auto Get_SDL_GLContext() const { return m_SDL_Windows_GL_Context.m_SDL_GLContext; }
		auto& Get_SDL_Windows_GL_Context() const { return m_SDL_Windows_GL_Context; }

	protected:

		Mouse m_mouse;
		FProcessEvent m_FProcessEvent; 	//! if valid, the function is called inside the internal loop of PollEvent. Break the event loop in the PollEvents function if the return value of the function differs from BH3D_OK

	private:

		SDL_Windows_GL_Context m_SDL_Windows_GL_Context;
		SDLTextureManager m_textureManager;

	};


	template<class Func>
	void SDLEngine::AddProcessEventFunction(Func && func) {
		//Add to process event function
		if (m_FProcessEvent) {
			FProcessEvent fPushProcessEvent;
			fPushProcessEvent = [lastProcessEvent = std::move(m_FProcessEvent), new_func = std::forward<Func>(func)](const SDL_Event * event) -> int  {
				int result = BH3D_OK;
				if ((result = lastProcessEvent(event)) == BH3D_OK) {
					result = new_func(event);
				}
				return result;
			};
			m_FProcessEvent = std::move(fPushProcessEvent);
		}
		else
		{
			m_FProcessEvent = [new_func = std::forward<Func>(func)](const SDL_Event * event) -> int {
				return new_func(event);
			};
		}

	}

}
#endif //BHD_SDL_ENGINE_H_