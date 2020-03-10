#include <sstream>

#include <SDL2/SDL.h>

#include "BH3D_Common.hpp"
#include "BH3D_SDLEngine.hpp"

#define		AUTO_CAST(var) operator decltype(var)&(){return var;}

namespace bh3d
{

	namespace
	{

		bool MouseEvent(const SDL_Event & event, Mouse & mouse)
		{
			auto SDL_2_BH3D_BUTTON = [](auto bouton)
			{
				switch (bouton)
				{
				case SDL_BUTTON_LEFT:
					return	Mouse::Button::LEFT;
				case SDL_BUTTON_MIDDLE:
					return	Mouse::Button::MIDDLE;
				case SDL_BUTTON_RIGHT:
					return	Mouse::Button::RIGHT;
				case SDL_BUTTON_X1:
					return	Mouse::Button::X1;
				case SDL_BUTTON_X2:
					return	Mouse::Button::X2;
				}
				assert(0 && "Something strange - Maybe update the switch case with the last SDL features ?");
				return Mouse::Button::NONE;
			};

			switch (event.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				mouse = Mouse::Event::DOWN;
				mouse << SDL_2_BH3D_BUTTON(event.button.button);	//Set down status
				break;
			case SDL_MOUSEBUTTONUP:
				mouse = Mouse::Event::UP;
				mouse >> SDL_2_BH3D_BUTTON(event.button.button);	//remove down status
				break;
			case SDL_MOUSEMOTION:
				mouse << Mouse::Event::MOVE;
				mouse << glm::u32vec2{ event.motion.x, event.motion.y };
				break;
			case SDL_MOUSEWHEEL:
				mouse << Mouse::Event::WHEEL;
				mouse = event.wheel.y;
				break;
			default:
				return false;  // No mouse event
			}

			return true;
		}



		bool ThreadSafeInitSDL()
		{
			class CInitSDL
			{
				bool m_ok = false;
			public:
				CInitSDL() {
					// SDL initialisation
					if (SDL_Init(SDL_INIT_VIDEO) < 0)
					{
						std::stringstream sstream;
						sstream << "Error during the initialisation of SDL library : " << SDL_GetError() << std::endl;
						BH3D_LOGGER_ERROR(sstream.str());
						SDL_Quit();
						return;
					}
					BH3D_LOGGER("SDL_init : OK");
					m_ok = true;
				}

				bool IsInit() { return m_ok	; }
			};
			static CInitSDL initSDL;

			return initSDL.IsInit();
		}

	} // Anonymous namespace


	SDL_Windows_GL_Context::~SDL_Windows_GL_Context()
	{
		// Destroy the opengl and sdl context
		if (m_SDL_GLContext != nullptr)
			SDL_GL_DeleteContext(m_SDL_GLContext);
		if (m_SDL_Window != nullptr)
			SDL_DestroyWindow(m_SDL_Window);

		SDL_Quit();
	}



	void SDLEngine::Init()
	{
		CreateWindow();		//Create a SDL windows with a opengl context
		InitOpenGL();		//Init opengl stuff (Some GLad and Opengl default values)
	}

	//Main loop of the opengl application
	void SDLEngine::Run() 
	{	
		Resize();			//Call the resize function once before the first display
		
		while (PollEvents())	//Collect overall event (return false when the program have to exist)
		{
			Update();				//Event processing and stuff like that
			Display();				//Display function
			SDL_GL_SwapWindow(m_SDL_Windows_GL_Context);	// Swap our buffer to display the current contents of buffer on screen 
		}
	}

	void SDLEngine::SwapWindow() {
		// Swap our buffer to display the current contents of buffer on screen
		SDL_GL_SwapWindow(m_SDL_Windows_GL_Context);
	}

	// The width and height are ignored with fullscreen flag or with negative values.
	void SDLEngine::SetWindowFullScreen(bool borderless) {
		SDL_SetWindowFullscreen(m_SDL_Windows_GL_Context, borderless ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_FULLSCREEN);
	}

	// Set the windowed mode. The width and height are ignored with negative values.
	void SDLEngine::SetWindowedMode(int w, int h) {
		SDL_SetWindowFullscreen(m_SDL_Windows_GL_Context, 0);	//Leave the full screen mode
		if (w > 0 && h > 0)		//! Window mode (sdl_flags == 0) and valid value for w and h
			SDL_SetWindowSize(m_SDL_Windows_GL_Context, w, h);
	}

	int SDLEngine::CreateWindow()
	{

		// SDL initialisation
		ThreadSafeInitSDL();

		// OpenGL Version
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, m_windowInfo.glContextMajorVersion);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, m_windowInfo.glContextMinorVersion);


		// Double Buffer
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, m_windowInfo.glDoubleBuffer);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, m_windowInfo.glDepthSize);

		//Multisample
		if (m_windowInfo.glMultiSamples > 0) {
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, m_windowInfo.glMultiSamples);
		}

		// Create a windows supporting opengl
		auto & window = m_SDL_Windows_GL_Context.m_SDL_Window;
		
		
		Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
		if (m_windowInfo.fullscreen)
			windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		if (m_windowInfo.borderless)
			windowFlags |= SDL_WINDOW_BORDERLESS;
		
		window = SDL_CreateWindow(m_windowInfo.title.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			m_windowInfo.width,
			m_windowInfo.height,
			windowFlags
		);
		if (window == nullptr)
		{
			std::stringstream sstream;
			sstream << "Error during the window creation : " << SDL_GetError() << std::endl;
			BH3D_LOGGER_ERROR(sstream.str());
			SDL_Quit();
			return BH3D_ERROR;
		}
		BH3D_LOGGER("SDL_CreateWindow : OK");

		// Creation du contexte OpenGL
		auto & glcontext = m_SDL_Windows_GL_Context.m_SDL_GLContext;
		glcontext = SDL_GL_CreateContext(window);
		if (glcontext == nullptr)
		{
			std::stringstream sstream;
			sstream << "Error during the GL context creation : " << SDL_GetError() << std::endl;
			BH3D_LOGGER_ERROR(sstream.str());

			SDL_DestroyWindow(window);
			SDL_Quit();
			return BH3D_ERROR;
		}
		BH3D_LOGGER("SDL_GL_CreateContext : OK");

		//synchronized with the vertical retrace ?
		SDL_GL_SetSwapInterval(m_windowInfo.vsync);
		

		return BH3D_OK;
	}

	int SDLEngine::PollEvents()
	{
		int returnValue = BH3D_OK;
		SDL_Event event;

		m_mouse.StatusSwap();

		while (SDL_PollEvent(&event))  //get the next event
		{
			if(m_FProcessEvent)
				if ((returnValue = m_FProcessEvent(&event)) != BH3D_OK) {
					return returnValue;
				}

			switch (event.type)
			{
				//it's the end, so sad
			case SDL_QUIT:
				return BH3D_EXIT;
				break;

				//window event
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_CLOSE: //it's the end, so sad
					return BH3D_EXIT;
					break;
				case SDL_WINDOWEVENT_RESIZED: //window resize
					Resize();
					break;
				case SDL_WINDOWEVENT_RESTORED:
					Resize();
					break;
				}
				break;

				//keyboard event
			case SDL_KEYDOWN:   //ESCAPE to quit
				returnValue = (event.key.keysym.sym == SDLK_ESCAPE) ? BH3D_EXIT : returnValue;
				break;
			default:
				MouseEvent(event, m_mouse);

				break;
			} // switch end

		} // while end

		return returnValue;
	}

	void SDLEngine::Update() {
		//Update the camera with the mouse deplacement/events
		//ex m_cameraEngine.CameraFreeFlight(m_mouse);
	}

	void SDLEngine::Resize()
	{
		SDL_GL_GetDrawableSize(m_SDL_Windows_GL_Context, &m_windowInfo.width, &m_windowInfo.height);
		bh3d::TinyEngine::Resize(m_windowInfo.width, m_windowInfo.height);
	}



}