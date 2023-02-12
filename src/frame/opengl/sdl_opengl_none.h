#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#if defined(_WIN32) || defined(_WIN64)
#include <SDL2/SDL_syswm.h>
#endif
#include <fmt/core.h>

#include <stdexcept>

#include "frame/logger.h"
#include "frame/window_interface.h"

namespace frame::opengl {

class SDLOpenGLNone : public WindowInterface {
   public:
    SDLOpenGLNone(glm::uvec2 size);
    virtual ~SDLOpenGLNone();

   public:
    void Run(std::function<void()> lambda) override;
    void* GetGraphicContext() const override;

   public:
    void SetInputInterface(std::unique_ptr<InputInterface>&& input_interface) override {
        input_interface_ = std::move(input_interface);
    }
    void SetUniqueDevice(std::unique_ptr<DeviceInterface>&& device) override {
        device_ = std::move(device);
    }
    DeviceInterface* GetUniqueDevice() override { return device_.get(); }
    glm::uvec2 GetSize() const override { return size_; }
    glm::uvec2 GetDesktopSize() const override { return {0, 0}; }
    void* GetWindowContext() const override { return sdl_window_; }
    void SetWindowTitle(const std::string& title) const override {}
    void Resize(glm::uvec2 size, FullScreenEnum fullscreen_enum) override {
		size_ = size;
        device_->Resize(size);
    }
    FullScreenEnum GetFullScreenEnum() const override { return FullScreenEnum::WINDOW; }
	
   private:
    glm::uvec2 size_;
    std::unique_ptr<DeviceInterface> device_         = nullptr;
    std::unique_ptr<InputInterface> input_interface_ = nullptr;
    SDL_Window* sdl_window_                          = nullptr;
    frame::Logger& logger_ = frame::Logger::GetInstance();
};

}  // namespace frame::opengl
