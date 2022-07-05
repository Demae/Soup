#pragma once

#include "fwd.hpp"

#include <filesystem>
#include <stack>
#include <string>
#include <vector>

namespace soup
{
	struct PhpState
	{
		std::filesystem::path cwd;
		std::string request_uri;

		[[nodiscard]] static const LangDesc& getLangDesc();

		static void processPhpmode(std::vector<Lexeme>& ls);

		[[nodiscard]] std::string evaluate(const std::string& code, unsigned int max_require_depth = 10) const;

		void execute(std::string& output, Reader& r, unsigned int max_require_depth = 10, std::stack<std::shared_ptr<Mixed>>&& stack = {}) const;
	};
}
