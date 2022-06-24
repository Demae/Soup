#pragma once

#include "Mixed.hpp"

namespace soup
{
	struct Token
	{
		enum ReservedId : int
		{
			VAL = -1,
			LITERAL = -2,
			SPACE = -3,
		};

		int id;
		Mixed val;
	};
}
