#pragma once

#include <gtest/gtest.h>
#include "Frame/Window.h"
#include "Frame/BufferInterface.h"

namespace test {

	class BufferTest : public testing::Test
	{
	public:
		BufferTest() : window_(frame::CreateSDLOpenGL(size_)) {}

	protected:
		const std::pair<std::uint32_t, std::uint32_t> size_ = { 330, 200 };
		std::unique_ptr<frame::WindowInterface> window_ = nullptr;
		std::unique_ptr<frame::BufferInterface> buffer_ = nullptr;
	};

} // End namespace test.
