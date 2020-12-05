#pragma once

#include "../ShaderGLLib/Window.h"
#include "../ShaderGLLib/Device.h"

class Draw : public sgl::DrawInterface
{
public:
	Draw(const std::shared_ptr<sgl::Device>& device) :
		device_(device) {}

public:
	void Startup(const std::pair<std::uint32_t, std::uint32_t> size) override;
	void RunDraw(const double dt) override;

protected:
	std::shared_ptr<sgl::Mesh> CreateAppleMesh();
	std::shared_ptr<sgl::Mesh> CreateCubeMapMesh(
		const std::shared_ptr<sgl::TextureCubeMap>& texture);

private:
	std::vector<std::shared_ptr<sgl::Texture>> out_textures_ = {};
	std::shared_ptr<sgl::Device> device_ = nullptr;
	std::shared_ptr<sgl::Mesh> cube_map_mesh_ = nullptr;
	std::shared_ptr<sgl::Mesh> apple_mesh_ = nullptr;
	std::shared_ptr<sgl::ProgramInterface> simple_program_ = nullptr;
	std::shared_ptr<sgl::ProgramInterface> cubemap_program_ = nullptr;
};
