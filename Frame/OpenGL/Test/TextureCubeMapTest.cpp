#include "TextureCubeMapTest.h"

#include <GL/glew.h>

#include <algorithm>

#include "Frame/File/FileSystem.h"
#include "Frame/OpenGL/File/LoadTexture.h"
#include "Frame/OpenGL/TextureCubeMap.h"
#include "Frame/Proto/ParseTexture.h"

namespace test {

TEST_F(TextureCubeMapTest, CreateTextureCubeMapTest) {
    ASSERT_FALSE(texture_);
    EXPECT_NO_THROW(
        texture_ = frame::opengl::file::LoadCubeMapTextureFromFiles(
            std::array<std::string, 6>{ frame::file::FindFile("Asset/CubeMap/PositiveX.png"),
                                        frame::file::FindFile("Asset/CubeMap/NegativeX.png"),
                                        frame::file::FindFile("Asset/CubeMap/PositiveY.png"),
                                        frame::file::FindFile("Asset/CubeMap/NegativeY.png"),
                                        frame::file::FindFile("Asset/CubeMap/PositiveZ.png"),
                                        frame::file::FindFile("Asset/CubeMap/NegativeZ.png") }));
    ASSERT_TRUE(texture_);
    auto* opengl_texture = dynamic_cast<frame::opengl::TextureCubeMap*>(texture_.get());
    ASSERT_NE(nullptr, opengl_texture);
    ASSERT_NE(0, opengl_texture->GetId());
    EXPECT_EQ(1024, texture_->GetSize().first);
    EXPECT_EQ(1024, texture_->GetSize().second);
    auto vec8 = texture_->GetTextureByte();
    auto p    = std::minmax_element(vec8.begin(), vec8.end());
    EXPECT_EQ(0x49, *p.first);
    EXPECT_EQ(0xff, *p.second);
}

// FIXME(anirul): There is a problem in there...
// FIXME(anirul): Note that the issue is slightly different with card and
// FIXME(anirul): if you execute it stand alone.
TEST_F(TextureCubeMapTest, DISABLED_CreateEquirectangularTextureCubeMapTest) {
    ASSERT_FALSE(texture_);
    EXPECT_NO_THROW(texture_ = frame::opengl::file::LoadCubeMapTextureFromFile(
                        frame::file::FindFile("Asset/CubeMap/Hamarikyu.hdr"),
                        frame::proto::PixelElementSize_FLOAT()));
    ASSERT_TRUE(texture_);
    auto* opengl_texture = dynamic_cast<frame::opengl::TextureCubeMap*>(texture_.get());
    ASSERT_NE(nullptr, opengl_texture);
    ASSERT_NE(0, opengl_texture->GetId());
    EXPECT_EQ(512, texture_->GetSize().first);
    EXPECT_EQ(512, texture_->GetSize().second);
    auto vecf = texture_->GetTextureFloat();
    // Image size time the number of color per pixel time 6 (cubemap).
    EXPECT_EQ(512 * 512 * 3 * 6, vecf.size());
    auto p = std::minmax_element(vecf.begin(), vecf.end());
    // Should probably make a GT and LT...
    EXPECT_EQ(0.0f, *p.first);
    EXPECT_FLOAT_EQ(20.413086f, *p.second);
}

}  // End namespace test.
