#pragma once

#include "vector_base.hpp"

namespace soup
{
#pragma pack(push, 1)
	struct vector4 : public vector_base<4, vector4>
	{
		float x;
		float y;
		float z;
		float w;
	};
	static_assert(sizeof(vector4) == 4 * sizeof(float));
#pragma pack(pop)
}
