#pragma once

#include <cstdint>

namespace soup
{
	struct netAs
	{
		uint32_t number;
		const char* handle;
		const char* name;

		netAs() = default;

		netAs(uint32_t number, const char* name)
			: number(number), handle(name), name(name)
		{
		}

		// Does the AS belong to a hosting provider? Can be used to tell if this is a VPN.
		[[nodiscard]] bool isHosting() const noexcept;
	};
}
