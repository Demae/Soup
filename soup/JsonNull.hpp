#pragma once

#include "JsonNode.hpp"

namespace soup
{
	struct JsonNull : public JsonNode
	{
		explicit JsonNull() noexcept;

		[[nodiscard]] std::string encode() const SOUP_EXCAL final;

		bool binaryEncode(Writer& w) const final;
	};
}
