#include "ffi.hpp"

#include "base.hpp"

#if SOUP_WINDOWS
#include <Windows.h>
#undef CDECL
#endif

// Generated by codegen/ffi.php

#define DO_FFI_CALL(cc) \
switch (args.size()) { \
case 0: return reinterpret_cast<uintptr_t(cc*)()>(func)(); \
case 1: return reinterpret_cast<uintptr_t(cc*)(uintptr_t)>(func)(args.at(0)); \
case 2: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1)); \
case 3: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2)); \
case 4: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3)); \
case 5: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4)); \
case 6: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5)); \
case 7: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6)); \
case 8: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6), args.at(7)); \
case 9: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6), args.at(7), args.at(8)); \
case 10: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6), args.at(7), args.at(8), args.at(9)); \
case 11: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6), args.at(7), args.at(8), args.at(9), args.at(10)); \
case 12: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6), args.at(7), args.at(8), args.at(9), args.at(10), args.at(11)); \
case 13: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6), args.at(7), args.at(8), args.at(9), args.at(10), args.at(11), args.at(12)); \
case 14: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6), args.at(7), args.at(8), args.at(9), args.at(10), args.at(11), args.at(12), args.at(13)); \
case 15: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6), args.at(7), args.at(8), args.at(9), args.at(10), args.at(11), args.at(12), args.at(13), args.at(14)); \
case 16: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6), args.at(7), args.at(8), args.at(9), args.at(10), args.at(11), args.at(12), args.at(13), args.at(14), args.at(15)); \
case 17: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6), args.at(7), args.at(8), args.at(9), args.at(10), args.at(11), args.at(12), args.at(13), args.at(14), args.at(15), args.at(16)); \
case 18: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6), args.at(7), args.at(8), args.at(9), args.at(10), args.at(11), args.at(12), args.at(13), args.at(14), args.at(15), args.at(16), args.at(17)); \
case 19: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6), args.at(7), args.at(8), args.at(9), args.at(10), args.at(11), args.at(12), args.at(13), args.at(14), args.at(15), args.at(16), args.at(17), args.at(18)); \
case 20: return reinterpret_cast<uintptr_t(cc*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(func)(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4), args.at(5), args.at(6), args.at(7), args.at(8), args.at(9), args.at(10), args.at(11), args.at(12), args.at(13), args.at(14), args.at(15), args.at(16), args.at(17), args.at(18), args.at(19)); \
} \
SOUP_THROW(BadCall());

namespace soup
{
	bool ffi::isSafeToCall(void* func) noexcept
	{
#if SOUP_WINDOWS
		auto ntdll = LoadLibraryA("ntdll.dll");
		if (GetProcAddress(ntdll, "NtRaiseHardError") == func)
		{
			return false;
		}
#endif
		return true;
	}

	uintptr_t ffi::call(CallConv conv, void* func, const std::vector<uintptr_t>& args)
	{
		switch (conv) {
		case CDECL: return cdeclCall(func, args);
		case FASTCALL: return fastcall(func, args);
		case STDCALL: return stdcall(func, args);
		case THISCALL: return thiscall(func, args);
		case VECTORCALL: return vectorcall(func, args);
		}
		SOUP_THROW(BadCall());
	}
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-attributes"
#endif
	uintptr_t ffi::cdeclCall(void* func, const std::vector<uintptr_t>& args)
	{
		DO_FFI_CALL(__cdecl)
	}
	uintptr_t ffi::fastcall(void* func, const std::vector<uintptr_t>& args)
	{
		DO_FFI_CALL(__fastcall)
	}
	uintptr_t ffi::stdcall(void* func, const std::vector<uintptr_t>& args)
	{
		DO_FFI_CALL(__stdcall)
	}
	uintptr_t ffi::thiscall(void* func, const std::vector<uintptr_t>& args)
	{
		DO_FFI_CALL(__thiscall)
	}
	uintptr_t ffi::vectorcall(void* func, const std::vector<uintptr_t>& args)
	{
		DO_FFI_CALL(__vectorcall)
	}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
}
