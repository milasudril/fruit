//@	{"dependencies_extra":[{"ref":"./fill_ops.o", "rel":"implementation"}]}

#ifndef FRUIT_LIB_FILLOPS_HPP
#define FRUIT_LIB_FILLOPS_HPP

#include "./image_view.hpp"
#include "./pixel.hpp"
#include "./point.hpp"

namespace fruit::fill_ops
{
	void source_over(image_span<Pixel> target_buffer, Point<int> begin, Point<int> end, Pixel source);
}

#endif