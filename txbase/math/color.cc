#include "txbase/stdafx.h"
#include "txbase/math/color.h"

namespace TX{
	const Color Color::ZERO = Color(0, 0, 0, 0);
	const Color Color::BLACK = Color(0);
	const Color Color::WHITE = Color(1);
	const Color Color::RED = Color(1, 0, 0);
	const Color Color::GREEN = Color(0, 1, 0);
	const Color Color::BLUE = Color(0, 0, 1);

	Color Color::RGBA(uint32_t rgba){
		return Color((rgba >> 24) / 255.f,
			((rgba >> 16) & 0xFF) / 255.f,
			((rgba >> 8) & 0xFF) / 255.f,
			(rgba & 0xFF) / 255.f);
	}
	Color Color::RGB(uint32_t rgb){
		return RGBA((rgb << 8) | 0xFF);
	}
}
