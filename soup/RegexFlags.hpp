#pragma once

#include <cstdint>

namespace soup
{
	enum RegexFlags : uint16_t
	{
		RE_MULTILINE = (1 << 0), // 'm' - '^' and '$' also match start and end of lines, respectively
		RE_DOTALL = (1 << 1), // 's' - '.' also matches '\n'
		RE_INSENSITIVE = (1 << 2), // 'i' - case insensitive match
		RE_UNICODE = (1 << 3), // 'u' - Treat pattern and strings-to-match as UTF-8 instead of binary data
		RE_UNGREEDY = (1 << 4), // 'U' - Quantifiers become lazy by default and are instead made greedy by a trailing '?'
		RE_DOLLAR_ENDONLY = (1 << 5), // 'D' - '$' only matches end of pattern, not '\n' - ignored if multi_line flag is set
		RE_EXPLICIT_CAPTURE = (1 << 6), // 'n' - only capture named groups (non-standard flag from .NET/C#)
	};
}
