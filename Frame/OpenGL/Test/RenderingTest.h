#pragma once

#include <gtest/gtest.h>
#include "Frame/DeviceInterface.h"
#include "Frame/Level.h"
#include "Frame/Window.h"
#include "Frame/OpenGL/Rendering.h"
#include "Frame/Proto/ParseLevel.h"
#include "Frame/Proto/ProtoLevelCreate.h"

namespace test {

	class RenderingTest : public testing::Test
	{
	public:
		RenderingTest() 
		{ 
			const auto size =
				std::make_pair<std::uint32_t, std::uint32_t>(320, 200);
			window_ = frame::CreateSDLOpenGL(size);
			device_ = window_->GetUniqueDevice();
			level_ = frame::proto::ParseLevelOpenGL(
				size,
				frame::proto::GetLevel(),
				frame::proto::GetProgramFile(),
				frame::proto::GetSceneFile(),
				frame::proto::GetTextureFile(),
				frame::proto::GetMaterialFile());
		}

	protected:
		std::shared_ptr<frame::WindowInterface> window_ = nullptr;
		std::shared_ptr<frame::LevelInterface> level_ = nullptr;
		std::shared_ptr<frame::DeviceInterface> device_ = nullptr;
		std::shared_ptr<frame::opengl::Rendering> rendering_ = nullptr;
	};

} // End namespace test.
