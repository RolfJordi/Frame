#include "frame/opengl/window_test.h"

#include "frame/opengl/window.h"

namespace test {

TEST_F(WindowTest, CreateWindowTest) {
    EXPECT_FALSE(window_);
    window_ = frame::opengl::CreateNoneOpenGL({ 640, 512 });
    EXPECT_TRUE(window_);
}

TEST_F(WindowTest, GetSizeWindowTest) {
    ASSERT_FALSE(window_);
    window_ = frame::opengl::CreateNoneOpenGL({ 640, 512 });
    ASSERT_TRUE(window_);
    std::pair<std::uint32_t, std::uint32_t> pair = { 640, 512 };
    EXPECT_EQ(pair, window_->GetSize());
}

TEST_F(WindowTest, CreateDeviceWindowTest) {
    ASSERT_FALSE(window_);
    window_ = frame::opengl::CreateNoneOpenGL({ 320, 200 });
    ASSERT_TRUE(window_);
    EXPECT_TRUE(window_->GetUniqueDevice());
}

}  // End namespace test.
