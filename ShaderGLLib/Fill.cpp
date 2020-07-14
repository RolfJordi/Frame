#include "Fill.h"
#include <assert.h>

namespace sgl {

	namespace {
		// Get the 6 view for the cube map.
		const std::array<glm::mat4, 6> views_cubemap =
		{
			glm::lookAt(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)),
			glm::lookAt(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, -1.0f)),
			glm::lookAt(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, -1.0f),
				glm::vec3(0.0f, -1.0f, 0.0f))
		};
	}

	void FillProgramMultiTexture(
		std::vector<std::shared_ptr<Texture>>& out_textures,
		const std::map<std::string, std::shared_ptr<Texture>>& in_textures,
		const std::shared_ptr<Program>& program)
	{
		FillProgramMultiTextureMipmap(
			out_textures,
			in_textures,
			program,
			0,
			[](const int, const std::shared_ptr<Program>&) {});
	}

	void FillProgramMultiTextureMipmap(
		std::vector<std::shared_ptr<Texture>>& out_textures,
		const std::map<std::string, std::shared_ptr<Texture>>& in_textures,
		const std::shared_ptr<Program>& program,
		const int mipmap,
		const std::function<void(
			const int mipmap,
			const std::shared_ptr<Program>& program)> func /*=
				[](const int, const std::shared_ptr<sgl::Program>&) {}*/)
	{
		auto& error = Error::GetInstance();
		assert(out_textures.size());
		auto size = out_textures[0]->GetSize();
		Frame frame{};
		Render render{};
		ScopedBind scoped_frame(frame);
		ScopedBind scoped_render(render);
		frame.AttachRender(render);
		render.CreateStorage(size);
		frame.DrawBuffers(static_cast<std::uint32_t>(out_textures.size()));
		int max_mipmap = (mipmap <= 0) ? 1 : mipmap;
		if (max_mipmap > 1) {
			for (const auto& texture : out_textures)
			{
				texture->BindEnableMipmap();
			}
		}
		glm::mat4 projection = glm::perspective(
			glm::radians(90.0f),
			1.0f,
			0.1f,
			10.0f);
		auto quad = CreateQuadMesh(program);
		auto material = std::make_shared<Material>();
		for (const auto& p : in_textures)
		{
			material->AddTexture(p.first, p.second);
		}
		quad->SetMaterial(material);
		std::pair<uint32_t, uint32_t> temporary_size = size;
		for (int mipmap_level = 0; mipmap_level < max_mipmap; ++mipmap_level)
		{
			func(mipmap_level, program);
			double fact = std::pow(0.5, mipmap_level);
			temporary_size.first =
				static_cast<std::uint32_t>(size.first * fact);
			temporary_size.second =
				static_cast<std::uint32_t>(size.second * fact);
			glViewport(0, 0, temporary_size.first, temporary_size.second);
			error.Display(__FILE__, __LINE__ - 1);
			for (int i = 0; i < out_textures.size(); ++i)
			{
				frame.AttachTexture(
					*out_textures[i],
					Frame::GetFrameColorAttachment(i),
					mipmap_level);
			}
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			error.Display(__FILE__, __LINE__ - 1);
			quad->Draw(projection);
		}
	}

	void FillProgramMultiTextureCubeMap(
		std::vector<std::shared_ptr<Texture>>& out_textures,
		const std::map<std::string, std::shared_ptr<Texture>>& in_textures,
		const std::shared_ptr<Program>& program)
	{
		FillProgramMultiTextureCubeMapMipmap(
			out_textures,
			in_textures,
			program,
			0,
			[](const int, const std::shared_ptr<Program>&) {});
	}

	void FillProgramMultiTextureCubeMapMipmap(
		std::vector<std::shared_ptr<Texture>>& out_textures,
		const std::map<std::string, std::shared_ptr<Texture>>& in_textures,
		const std::shared_ptr<Program>& program,
		const int mipmap,
		const std::function<void(
			const int mipmap,
			const std::shared_ptr<sgl::Program>& program)> func /*=
				[](const int, const std::shared_ptr<sgl::Program>&) {}*/)
	{
		auto& error = Error::GetInstance();
		assert(out_textures.size());
		auto size = out_textures[0]->GetSize();
		Frame frame{};
		Render render{};
		ScopedBind scoped_frame(frame);
		ScopedBind scoped_render(render);
		frame.AttachRender(render);
		frame.DrawBuffers(static_cast<std::uint32_t>(out_textures.size()));
		int max_mipmap = (mipmap <= 0) ? 1 : mipmap;
		if (max_mipmap > 1)
		{
			for (const auto& texture : out_textures)
			{
				texture->BindEnableMipmap();
			}
		}
		glm::mat4 projection = glm::perspective(
			glm::radians(90.0f),
			1.0f,
			0.1f,
			10.0f);
		auto cube = CreateCubeMesh(program);
		auto material = std::make_shared<Material>();
		for (const auto& p : in_textures)
		{
			material->AddTexture(p.first, p.second);
		}
		cube->SetMaterial(material);
		std::pair<std::uint32_t, std::uint32_t> temporary_size = { 0, 0 };
		for (int mipmap_level = 0; mipmap_level < max_mipmap; ++mipmap_level)
		{
			func(mipmap_level, program);
			double fact = std::pow(0.5, mipmap_level);
			temporary_size.first =
				static_cast<std::uint32_t>(size.first * fact);
			temporary_size.second =
				static_cast<std::uint32_t>(size.second * fact);
			render.CreateStorage(temporary_size);
			glViewport(0, 0, temporary_size.first, temporary_size.second);
			error.Display(__FILE__, __LINE__ - 1);
			int cubemap_element = 0;
			for (glm::mat4 view : views_cubemap)
			{
				for (int i = 0; i < out_textures.size(); ++i)
				{
					frame.AttachTexture(
						*out_textures[i],
						Frame::GetFrameColorAttachment(i),
						mipmap_level,
						Frame::GetFrameTextureType(cubemap_element));
				}
				cubemap_element++;
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				error.Display(__FILE__, __LINE__ - 1);
				cube->Draw(projection, view);
			}
		}
	}

} // End namespace sgl.