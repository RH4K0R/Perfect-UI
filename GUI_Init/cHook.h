#pragma once
#include <Windows.h>

#ifdef _WIN64
typedef unsigned __int64 UPARAM;
#else
typedef unsigned long UPARAM;
#endif

class cHook
{
	BYTE data[14];
	FARPROC func;
	FARPROC hookFunc;
	bool bHooked;
	bool b64bitJump;
	DWORD oldProtect;
public:
	inline cHook() : bHooked(false), func(NULL), hookFunc(NULL), b64bitJump(false), oldProtect(0)
	{
	}

	inline ~cHook()
	{
		if (bHooked)
			Unhook();

		if (func && oldProtect)
			VirtualProtect((LPVOID)func, 14, oldProtect, &oldProtect);
	}

	inline bool Hook(FARPROC funcIn, FARPROC hookFuncIn)
	{
		if (bHooked)
		{
			if (funcIn == func)
			{
				if (hookFunc != hookFuncIn)
				{
					hookFunc = hookFuncIn;
					Rehook();
					return true;
				}
			}

			Unhook();
		}

		func = funcIn;
		hookFunc = hookFuncIn;

		if (!VirtualProtect((LPVOID)func, 14, PAGE_EXECUTE_READWRITE, &oldProtect))
			return false;

		memcpy(data, (const void*)func, 14);

		return true;
	}

	inline void Rehook(bool bForce = false)
	{
		if ((!bForce && bHooked) || !func)
			return;

		UPARAM startAddr = UPARAM(func);
		UPARAM targetAddr = UPARAM(hookFunc);
		UPARAM offset = targetAddr - (startAddr + 5);

#ifdef _WIN64
		b64bitJump = (offset > 0x7fff0000);

		if (b64bitJump)
		{
			LPBYTE addrData = (LPBYTE)func;
			*(addrData++) = 0xFF;
			*(addrData++) = 0x25;
			*((LPDWORD)(addrData)) = 0;
			*((unsigned __int64*)(addrData + 4)) = targetAddr;
		}
		else
#endif
		{
			LPBYTE addrData = (LPBYTE)func;
			*addrData = 0xE9;
			*(DWORD*)(addrData + 1) = DWORD(offset);
		}

		bHooked = true;
	}

	inline void Unhook()
	{
		if (!bHooked || !func)
			return;

		UINT count = b64bitJump ? 14 : 5;

		memcpy((void*)func, data, count);

		bHooked = false;
	}
};


