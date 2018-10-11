#include "TextureMSAA.h"
#include "OpenGL.h"
#include "Application.h"
#include "ModuleWindow.h"

TextureMSAA::TextureMSAA()
{
}


TextureMSAA::~TextureMSAA()
{
}

bool TextureMSAA::Create(uint width, uint height, int msa_lvl)
{
	max_msaa_samples = 0;
	glGetIntegerv(GL_MAX_SAMPLES, &max_msaa_samples);
	if (msa_lvl > max_msaa_samples) msa_lvl = max_msaa_samples;
	current_msaa_samples = msa_lvl;

	// create a texture object
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a MSAA framebuffer object
	glGenFramebuffers(1, &fbo_msaa_id);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_msaa_id);

	// create a MSAA renderbuffer object to store color info
	glGenRenderbuffers(1, &rbo_color_id);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_color_id);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, msa_lvl, GL_RGB8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// create a MSAA renderbuffer object to store depth info
	glGenRenderbuffers(1, &rbo_depth_id);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_id);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, msa_lvl, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach msaa RBOs to FBO attachment points
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo_color_id);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth_id);

	// create a normal (no MSAA) FBO to hold a render-to-texture
	glGenFramebuffers(1, &fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

	glGenRenderbuffers(1, &rbo_id);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach a texture to FBO color attachement point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

	// attach a rbo to FBO depth attachement point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_id);

	// check FBO status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->width = width;
	this->height = height;

	return true;
}

void TextureMSAA::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_msaa_id);
	glViewport(0, 0, width, height);
}

void TextureMSAA::Render()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_msaa_id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_id);
	glBlitFramebuffer(0, 0, width, height,  // src rect
		0, 0, width, height,  // dst rect
		GL_COLOR_BUFFER_BIT, // buffer mask
		GL_LINEAR); // scale filter
}

void TextureMSAA::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, App->window->screen_surface->w, App->window->screen_surface->h);
}

void TextureMSAA::SetTextureID()
{
}

unsigned int TextureMSAA::GetTextureID() const
{
	return texture_id;
}

void TextureMSAA::SetWidth()
{
}

unsigned int TextureMSAA::GetWidth() const
{
	return width;
}

void TextureMSAA::SetHeight()
{
}

unsigned int TextureMSAA::GetHeight() const
{
	return height;
}

void TextureMSAA::LoadToMemory()
{
}

void TextureMSAA::UnloadFromMemory()
{
}

void TextureMSAA::Clear()
{
}
