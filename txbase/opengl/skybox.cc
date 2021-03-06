#include "skybox.h"
#include "txbase/scene/camera.h"

namespace TX {
	namespace GL {
		const char * const SKYBOX_VS = R"(
			#version 330 core
			layout (location = 0) in vec3 vertPos;
			out vec3 texCoord;

			uniform mat4 iMVP;

			void main()
			{
				vec4 fragPos = iMVP * vec4(vertPos, 1.0);
				gl_Position = fragPos.xyww;		// maximum depth value = 1.0
				texCoord = vertPos;
			}
		)";
		const char * const SKYBOX_FS = R"(
			#version 330 core
			in vec3 texCoord;
			out vec4 color;

			uniform samplerCube iSkybox;

			void main()
			{
				color = texture(iSkybox, texCoord);
			}
		)";

		struct Skybox::Impl {
			Cubemap cubemap;
			VertexArray vao;
			VertexBuffer vbo;
			IndexBuffer ebo;

			Impl() {
				// Cube mesh
				TX::Mesh cube;
				cube.LoadCube(1);
				vao.Bind();
				{
					vbo.Data(cube.vertices.size() * sizeof(cube.vertices[0]), cube.vertices.data());
					glEnableVertexAttribArray(ATTRIB_POS);
					glVertexAttribPointer(ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
					ebo.Data(cube.indices.size() * sizeof(cube.indices[0]), cube.indices.data());
				}
				vao.Unbind();
			}
		};

		Skybox::Skybox(): p(new Impl)
		{
			Compile(
				GL::Shader(GL_VERTEX_SHADER, SKYBOX_VS),
				GL::Shader(GL_FRAGMENT_SHADER, SKYBOX_FS));
		}

		Skybox::~Skybox() {}

		Cubemap& Skybox::GetCubemap() {
			return p->cubemap;
		}

		void Skybox::Draw(const Camera& camera) const {
			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CW);

			Use();

			// Remove translation so that camera is always centered at skybox
			Matrix4x4 V = Matrix4x4(Matrix3x3(camera.transform.WorldToLocalMatrix()));
			SetUniform("iMVP", camera.CameraToViewport() * V);

			p->cubemap.Bind();
			p->vao.Bind();
			glDrawElements(
				GL_TRIANGLES,
				36,
				GL_UNSIGNED_INT,
				(void *)0
			);
			p->vao.Unbind();
			p->cubemap.Unbind();

			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);
		}
	}
}