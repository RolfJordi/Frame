#include "Window.h"

#include <exception>
#include <stdexcept>
#include <chrono>
#include <utility>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL_syswm.h>
#include <gl/glew.h>

namespace sgl {

	// Private space.
	namespace {

		class SDLWindow : public Window
		{
		public:
			SDLWindow(std::pair<int, int> size) : size_(size)
			{
				if (SDL_Init(SDL_INIT_VIDEO) != 0)
				{
					ErrorMessageDisplay("Couldn't initialize SDL2.");
					throw std::runtime_error("Couldn't initialize SDL2.");
				}
				sdl_window_ = SDL_CreateWindow(
					"Shader OpenGL",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					size_.first,
					size_.second,
					SDL_WINDOW_OPENGL);
				if (!sdl_window_)
				{
					ErrorMessageDisplay("Couldn't start a window in SDL2.");
					throw std::runtime_error(
						"Couldn't start a window in SDL2.");
				}
#if defined(_WIN32) || defined(_WIN64)
				SDL_SysWMinfo wmInfo;
				SDL_VERSION(&wmInfo.version);
				SDL_GetWindowWMInfo(sdl_window_, &wmInfo);
				hwnd_ = wmInfo.info.win.window;
#endif
				// Create a new device.
				device_ = std::make_shared<sgl::Device>(sdl_window_);
			}

			virtual ~SDLWindow()
			{
				SDL_DestroyWindow(sdl_window_);
				SDL_Quit();
			}

			bool Startup() override
			{
#if _DEBUG && !defined(__APPLE__)
				// Enable error message.
				glEnable(GL_DEBUG_OUTPUT);
				glDebugMessageCallback(
					SDLWindow::ErrorMessageHandler,
					nullptr);
#endif
				// Device Startup call.
				device_->Startup(size_);

				return true;
			}

			void Run() override
			{
				// While Run return true continue.
				bool loop = true;
				double previous_count = 0.0f;
				// Timing counter.
				static auto start = std::chrono::system_clock::now();
				do
				{
					// Compute the time difference from previous frame.
					auto end = std::chrono::system_clock::now();
					std::chrono::duration<double> time = end - start;
					// Process events
					SDL_Event event;
					if (SDL_PollEvent(&event))
					{
						if (!RunEvent(event))
						{
							loop = false;
							continue;
						}
					}

					device_->Draw(time.count());
					previous_count = time.count();
					SDL_GL_SwapWindow(sdl_window_);
				} while (loop);
			}

			void SetDraw(std::function<void(const double)> draw_func) override
			{
				draw_func_ = draw_func;
			}

			std::shared_ptr<Device> GetDevice() override
			{
				return device_;
			}

		protected:
			bool RunEvent(const SDL_Event& event)
			{
				if (event.type == SDL_QUIT)
				{
					return false;
				}
				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						return false;
					}
				}
				return true;
			}

			void ErrorMessageDisplay(const std::string& message)
			{
#if defined(_WIN32) || defined(_WIN64)
				MessageBox(hwnd_, message.c_str(), "OpenGL Error", 0);
#else
				std::cerr << "OpenGL Error: " << message << std::endl;
#endif
			}

#if !defined(__APPLE__)
			static void GLAPIENTRY ErrorMessageHandler(
				GLenum source,
				GLenum type,
				GLuint id,
				GLenum severity,
				GLsizei length,
				const GLchar* message,
				const void* userParam)
			{
				// Remove notifications.
				if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
					return;
				// Remove medium
				if (severity == GL_DEBUG_SEVERITY_MEDIUM)
					return;
				std::ostringstream oss;
				oss << "message\t: " << message << std::endl;
				oss << "type\t: ";
				switch (type)
				{
				case GL_DEBUG_TYPE_ERROR:
					oss << "ERROR";
					break;
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
					oss << "DEPRECATED_BEHAVIOR";
					break;
				case GL_DEBUG_TYPE_PORTABILITY:
					oss << "PORABILITY";
					break;
				case GL_DEBUG_TYPE_PERFORMANCE:
					oss << "PERFORMANCE";
					break;
				case GL_DEBUG_TYPE_OTHER:
					oss << "OTHER";
					break;
				}
				oss << std::endl;
				oss << "id\t: " << id << std::endl;
				oss << "severity\t: ";
				switch (severity)
				{
				case GL_DEBUG_SEVERITY_LOW:
					oss << "LOW";
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
					oss << "MEDIUM";
					break;
				case GL_DEBUG_SEVERITY_HIGH:
					oss << "HIGH";
					break;
				case GL_DEBUG_SEVERITY_NOTIFICATION:
					oss << "NOTIFICATION";
					break;
				}
				oss << std::endl;
#if defined(_WIN32) || defined(_WIN64)
				MessageBox(nullptr, oss.str().c_str(), "OpenGL Error", 0);
#else
				std::cerr << "OpenGL Error: " << oss.str() << std::endl;
#endif
			}
#endif

		private:
			std::pair<int, int> size_;
			std::shared_ptr<sgl::Device> device_ = nullptr;
			std::function<void(const double)> draw_func_ = nullptr;
			SDL_Window* sdl_window_ = nullptr;
#if defined(_WIN32) || defined(_WIN64)
			HWND hwnd_ = nullptr;
#endif
		};

	}

	std::shared_ptr<sgl::Window> MakeSDLOpenGL(std::pair<int, int> size)
	{
		return std::make_shared<SDLWindow>(size);
	}

} // End namespace sgl.