#include "Pointer.hpp"

#include "Module.hpp"

namespace soup
{
	Pointer Pointer::rip() const noexcept
	{
		return ripT<int32_t>();
	}

#if SOUP_WINDOWS
	Pointer Pointer::externalRip(const Module& mod) const noexcept
	{
		return add(mod.externalRead<int32_t>(*this)).add(sizeof(int32_t));
	}

	bool Pointer::isInModule() const noexcept
	{
		return isInModule(nullptr);
	}

	bool Pointer::isInModule(const Module& mod) const noexcept
	{
		return *this >= mod.base()
			&& *this < mod.base().add(mod.size())
			;
	}

	Pointer Pointer::rva() const noexcept
	{
		return rva(nullptr);
	}

	Pointer Pointer::rva(const Module& mod) const noexcept
	{
		return Pointer(as<int32_t&>()).add(mod.base().as<uintptr_t>());
	}

	std::vector<Pointer> Pointer::getJumps() const noexcept
	{
		auto ptr = *this;
		std::vector<Pointer> res{ ptr };
		__try
		{
			while (ptr.as<uint8_t&>() == 0xE9)
			{
				ptr = ptr.add(1).rip();
				res.emplace_back(ptr);
			}
		}
		__except (GetExceptionInformation()->ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
		}
		return res;
	}

	Pointer Pointer::followJumps() const noexcept
	{
		auto jumps = getJumps();
		return jumps.back();
	}
#endif
}
