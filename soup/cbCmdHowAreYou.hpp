#pragma once

#include "cbCmd.hpp"

namespace soup
{
	struct cbCmdHowAreYou : public cbCmd
	{
		[[nodiscard]] std::vector<std::string> getTriggers() const noexcept final
		{
			return { "how are you" };
		}

		[[nodiscard]] cbResult process(cbParser& p) const noexcept final
		{
			return "Running smoothly.";
		}
	};
}
