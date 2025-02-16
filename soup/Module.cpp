#include "Module.hpp"

#if SOUP_WINDOWS

#include "Exception.hpp"
#include "HandlePlain.hpp"
#include "HandleRaii.hpp"
#include "MemoryBuffer.hpp"
#include "Pattern.hpp"
#include "Pointer.hpp"
#include "AllocRaiiRemote.hpp"

namespace soup
{
	Module::Module(UniquePtr<HandleBase>&& h, Range&& range)
		: h(std::move(h)), range(std::move(range))
	{
	}

	Module::Module(UniquePtr<HandleBase>&& h)
		: h(std::move(h)), range(*this->h, 0)
	{
		SOUP_IF_UNLIKELY (range.base.as<void*>() == nullptr)
		{
			SOUP_THROW(Exception("Module not found"));
		}
		auto dosHeader = range.base.as<IMAGE_DOS_HEADER*>();
		auto ntHeader = range.base.add(dosHeader->e_lfanew).as<IMAGE_NT_HEADERS*>();
		range.size = ntHeader->OptionalHeader.SizeOfImage;
	}

	Module::Module(HANDLE h)
		: Module((UniquePtr<HandleBase>)make_unique<HandlePlain>(h))
	{
	}

	Module::Module(std::nullptr_t)
		: Module(GetModuleHandle(nullptr))
	{
	}

	Module::Module(const char* name)
		: Module(GetModuleHandleA(name))
	{
	}

	Pointer Module::getExport(const char* name) const noexcept
	{
		return Pointer((void*)GetProcAddress(*h, name));
	}

	size_t Module::externalRead(Pointer p, void* out, size_t size) const noexcept
	{
		SIZE_T read = 0;
		ReadProcessMemory(*h, p.as<void*>(), out, size, &read);
		return read;
	}

	Pointer Module::externalScan(const Pattern& sig) const
	{
		MemoryBuffer buf{};
		size_t matched_bytes = 0;
		Pointer p = range.base;
		size_t end = p.as<uintptr_t>() + range.size - 1;
		for (; p.as<size_t>() < end; p = p.add(1))
		{
			if (!buf.covers(p))
			{
				buf.updateRegion(*this, p.as<void*>(), 0x10000);
			}
			if (!sig.bytes.at(matched_bytes).has_value() || buf.read<uint8_t>(p) == sig.bytes.at(matched_bytes).value())
			{
				if (++matched_bytes >= sig.bytes.size())
				{
					return p.sub(sig.bytes.size() - 1);
				}
			}
			else
			{
				matched_bytes = 0;
			}
		}
		return nullptr;
	}

	UniquePtr<AllocRaiiRemote> Module::allocate(size_t size, DWORD type, DWORD protect) const
	{
		auto res = make_unique<AllocRaiiRemote>();
		res->h = *h;
		res->size = size;
		res->p = VirtualAllocEx(*h, nullptr, size, type, protect);
		return res;
	}

	UniquePtr<AllocRaiiRemote> Module::copyInto(const void* data, size_t size) const
	{
		auto res = allocate(size);
		externalWrite(res->p, data, size);
		return res;
	}

	size_t Module::externalWrite(Pointer p, const void* data, size_t size) const noexcept
	{
		SIZE_T written = 0;
		DWORD oldprotect;
		VirtualProtectEx(*h, p.as<void*>(), size, PAGE_EXECUTE_READWRITE, &oldprotect);
		WriteProcessMemory(*h, p.as<void*>(), data, size, &written);
		VirtualProtectEx(*h, p.as<void*>(), size, oldprotect, &oldprotect);
		return written;
	}

	UniquePtr<HandleRaii> Module::executeAsync(void* rip, uintptr_t rcx) const noexcept
	{
		return make_unique<HandleRaii>(CreateRemoteThread(*h, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(rip), reinterpret_cast<LPVOID>(rcx), 0, nullptr));
	}

	void Module::executeSync(void* rip, uintptr_t rcx) const noexcept
	{
		auto t = executeAsync(rip, rcx);
		WaitForSingleObject(*t, INFINITE);
	}
}

#endif
