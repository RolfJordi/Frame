#include "Frame.h"
#include <stdexcept>
#include <gl/glew.h>

namespace sgl {

	Frame::Frame()
	{
		glGenFramebuffers(1, &frame_id_);
		if (frame_id_ == 0)
		{
			throw std::runtime_error("Couldn't create a Frame Buffer.");
		}
	}

	Frame::~Frame()
	{
		UnBind();
		glDeleteFramebuffers(1, &frame_id_);
	}

	void Frame::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frame_id_);
	}

	void Frame::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Frame::BindAttach(const Render& render) const
	{
		Bind();
		render.Bind();
		glFramebufferRenderbuffer(
			GL_FRAMEBUFFER, 
			GL_DEPTH_ATTACHMENT, 
			GL_RENDERBUFFER, 
			render.GetId());
	}

	void Frame::BindTexture2D(
		const int texture_id,
		const FrameTextureType frame_texture_type 
			/*= FrameTextureType::TEXTURE_2D*/) const
	{
		Bind();
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GetFrameTextureType(frame_texture_type),
			texture_id,
			0);
	}

	const int Frame::GetFrameTextureType(
		const FrameTextureType frame_texture_type) const
	{
		int value = static_cast<int>(frame_texture_type);
		if (value >= 0)
		{
			return GL_TEXTURE_CUBE_MAP_POSITIVE_X + value;
		}
		return GL_TEXTURE_2D;
	}

} // End namespace sgl.
