#pragma once
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "Texture.h"

	class FrameBuffer
	{
		GLuint sceneFramebufferId;
		GLuint depthBufferName;

		GE::Texture* sceneTexture;

	public:
		FrameBuffer(glm::vec2 widthHeight) {
			width = widthHeight.x;
			height = widthHeight.y;

			sceneFramebufferId = -1;
			sceneTexture = nullptr;
			depthBufferName = -1;
		}

		~FrameBuffer() {
			// Release the texture, depth buffer and framebuffer objects
			delete sceneTexture;
			glDeleteRenderbuffers(1, &depthBufferName);
			glDeleteFramebuffers(1, &sceneFramebufferId);
		}

		bool init();
	
		void beginRender() {
			// 11. Bind the framebuffer to the pipeline
			// All objects rendered from this point go to the
			// framebuffer until glBindBuffer is used with a different framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, this->sceneFramebufferId);
		}

		void endRender() {
			// 12. Bind the default framebuffer.  No more objects
			// will be rendered to the framebuffer encapsulated in this class
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// Provides access to the texture object
		// associated with the framebuffer
		GE::Texture* getTexture() {
			return sceneTexture;
		}

	private:
		// 1. Member variables


		int width, height;
	};