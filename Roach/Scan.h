#pragma once
#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <WinINet.h>
#include <algorithm>
#include <random>
#include <ctime>
#include <typeinfo>
#include <fcntl.h>
#include <io.h>
#include <TlHelp32.h>
#include <WinInet.h>
#include <istream>
#include <string>
#include <TlHelp32.h>
#include <numeric>
#include <sstream>
#include <tchar.h>
#define WRITABLE (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)
namespace MemScan {
	bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask) return 0;
		return (*szMask) == NULL;
	}

	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
	{
		for (int i = 0; i < (int)dwLen; i++)
			if (Compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
		return 0;
	}

	int Scan(DWORD mode, const char* content, char* mask)
	{

		SYSTEM_INFO SystemInfo;
		GetSystemInfo(&SystemInfo);
		DWORD PageSize = SystemInfo.dwPageSize;
		MEMORY_BASIC_INFORMATION meminfo;
		DWORD Start = (DWORD)SystemInfo.lpMinimumApplicationAddress;
		DWORD End = (DWORD)SystemInfo.lpMaximumApplicationAddress;
		for (DWORD lpAddress = Start; lpAddress <= End; lpAddress += PageSize)
		{
			VirtualQuery((void*)lpAddress, &meminfo, sizeof(MEMORY_BASIC_INFORMATION));
			if (meminfo.Type == MEM_MAPPED) continue;
			if (meminfo.Protect == PAGE_READWRITE) {
				int addr = FindPattern(lpAddress, PageSize, (PBYTE)content, mask);
				if (addr != 0)
				{
					return addr;
				}
			}
		}

	}

}


namespace aobscan {
	BOOL compare(const BYTE* location, const BYTE* aob, const char* mask) {
		for (; *mask; ++aob, ++mask, ++location) {
			__try {
				if (*mask == 'x' && *location != *aob)
					return 0;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return 0;
			}
		}
		return 1;
	};

	DWORD scan(const char* aob, const char* mask) {
		for (DWORD i = (DWORD)GetModuleHandle(NULL); i <= 0xF000000; ++i) {

			if (compare((BYTE*)i, (BYTE*)aob, mask))
				return i;
		}
		return 0;
	};

	DWORD scanWritable(const char* aob, const char* mask) {
		MEMORY_BASIC_INFORMATION memoryInfo;
		DWORD regionEnd = 0;
		DWORD maskLen = strlen(mask);
		for (DWORD i = (DWORD)GetModuleHandle(NULL); i <= 0x4FFFFFFF; ++i) {
			if (i + maskLen > regionEnd) {
				i += maskLen;
			}

			if (i > regionEnd || !regionEnd) {
				VirtualQuery((DWORD*)i, &memoryInfo, sizeof(memoryInfo));
				regionEnd = i + memoryInfo.RegionSize;
				if (!((memoryInfo.State & MEM_COMMIT) && (memoryInfo.Protect & WRITABLE))) {
					i = regionEnd + 1;
					continue;
				}
			}

			if (compare((BYTE*)i, (BYTE*)aob, mask))
				return i;
		}
		return 0;
	}
};
#pragma once
