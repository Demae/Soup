#include "os.hpp"

#include <array>
#include <cstdio>

#if SOUP_WINDOWS
#include <ShlObj.h> // CSIDL_COMMON_APPDATA

#include "ObfusString.hpp"
#endif
#include "rand.hpp"
#include "string.hpp"

namespace soup
{
	std::filesystem::path os::tempfile(const std::string& ext)
	{
		std::filesystem::path path;
		do
		{
			auto file = rand.str<std::string>(20);
			if (!ext.empty())
			{
				if (ext.at(0) != '.')
				{
					file.push_back('.');
				}
				file.append(ext);
			}
			path = std::filesystem::temp_directory_path();
			path /= file;
		} while (std::filesystem::exists(path));
		return path;
	}

	std::filesystem::path os::getProgramData()
	{
#if SOUP_WINDOWS
		wchar_t szPath[MAX_PATH];
		if (SHGetFolderPathW(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath) == 0)
		{
			return szPath;
		}
		return "C:\\ProgramData";
#else
		return "/var/lib";
#endif
	}

	void os::escape(std::string& str)
	{
		if (str.find(' ') != std::string::npos)
		{
			string::replace_all(str, "\\", "\\\\");
			string::replace_all(str, "\"", "\\\"");
			str.insert(0, 1, '"');
			str.push_back('"');
		}
	}

	std::string os::execute(std::string program, const std::vector<std::string>& args)
	{
#if SOUP_WINDOWS
		if (program.find('\\') == std::string::npos
			&& program.find('/') == std::string::npos
			)
		{
			program = executeInner("where", { program });
			string::rtrim(program);
		}
#endif
		return executeInner(std::move(program), args);
	}

	std::string os::executeInner(std::string cmd, const std::vector<std::string>& args)
	{
		escape(cmd);
		for (auto i = args.begin(); i != args.end(); ++i)
		{
			std::string escaped = *i;
			escape(escaped);
			cmd.push_back(' ');
			cmd.append(escaped);
		}
		cmd.append(" 2>&1");
		//std::cout << "Running <" << cmd << ">" << std::endl;
#if SOUP_WINDOWS
		auto pipe = _popen(cmd.c_str(), "r");
#else
		auto pipe = popen(cmd.c_str(), "r");
#endif
		std::array<char, 128> buffer;
		std::string result;
		while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
		{
			result += buffer.data();
	}
#if SOUP_WINDOWS
		_pclose(pipe);
#else
		pclose(pipe);
#endif
		return result;
	}

#if SOUP_WINDOWS
	PEB* os::getCurrentPeb()
	{
		// There is a "simpler" solution (https://gist.github.com/Wack0/849348f9d4f3a73dac864a556e9372a5), but this is what Microsoft does, so we shall, too.

		auto ntdll = LoadLibraryA(ObfusString("ntdll.dll"));

		using NtQueryInformationProcess_t = NTSTATUS(NTAPI*)(HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);
		auto NtQueryInformationProcess = (NtQueryInformationProcess_t)GetProcAddress(ntdll, ObfusString("NtQueryInformationProcess"));

		PROCESS_BASIC_INFORMATION ProcessInformation;
		NtQueryInformationProcess(GetCurrentProcess(), ProcessBasicInformation, &ProcessInformation, sizeof(ProcessInformation), 0);

		return ProcessInformation.PebBaseAddress;
	}

	void os::stop()
	{
		auto ntdll = LoadLibraryA(ObfusString("ntdll.dll"));

		using NtRaiseHardError_t = NTSTATUS(NTAPI*)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
		using RtlAdjustPrivilege_t = NTSTATUS(NTAPI*)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

		auto RtlAdjustPrivilege = (RtlAdjustPrivilege_t)GetProcAddress(ntdll, ObfusString("RtlAdjustPrivilege"));
		auto NtRaiseHardError = (NtRaiseHardError_t)GetProcAddress(ntdll, ObfusString("NtRaiseHardError"));

		// Enable SeShutdownPrivilege
		BOOLEAN bEnabled;
		RtlAdjustPrivilege(19, TRUE, FALSE, &bEnabled);

		ULONG uResp;
		NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &uResp);
	}
#endif
}
