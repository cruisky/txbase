#pragma once

#include "txbase/fwddecl.h"
#include "txbase/opengl/opengl.h"
#include "txbase/misc/const.h"
#include "txbase/shader/base.h"

namespace TX {
	class Phong : public ShaderProgram {
	public:
		struct LightSource {
			Color ambient;
			Color diffuse;
			Color specular;
			Vec4 position;
			Vec3 spotDirection;
			float spotExponent;
			float spotCutoff;
			float constantAttenuation;
			float linearAttenuation;
			float quadraticAttenuation;
			LightSource() :
				ambient(0.4f, 0.4f, 0.4f, 1.f),
				diffuse(Color::WHITE),
				specular(Color::WHITE),
				position(0, 0, 1, 0),
				spotDirection(0, 0, -1),
				spotCutoff(180),
				constantAttenuation(1),
				linearAttenuation(0),
				quadraticAttenuation(0) {
			}
		};
	private:
		virtual void CompileProgram();
	public:
		void SetLight(const LightSource& light) const;
		void SetMaterial(
			const Color& ambient,
			const Color& diffuse,
			const Color& specular,
			float shininess) const;
	private:
		/// <summary>
		/// Vertex shader code.
		/// </summary>
		constexpr static ConstString VertShaderSrc =
#include "default.vs.glsl"
			;
		/// <summary>
		/// Fragment shader code.
		/// </summary>
		constexpr static ConstString FragShaderSrc =
#include "phong/phong.fs.glsl"
			;
	};
}
