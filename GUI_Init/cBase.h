#pragma once
#include "cRender\cRender.h"

#include "cHook.h"

#include <Shlobj.h>

namespace X {
	int menu_x = 30;
	int menu_y = 20;
	int menu_w = 604;
	int menu_h = 30;

	bool move_index[8][2];
	POINT move_cursor;

	static int alpha;
	int tab_count;
}
typedef UINT(__stdcall *threadFunction_t)(void*);

typedef unsigned char      uint8_t;

typedef IDirect3D9 * (WINAPI *oDirect3DCreate9)(UINT SDKVersion);
typedef IDirect3D9* (WINAPI*D3D9CREATEPROC)(UINT);

cHook chPresent;

typedef HRESULT(WINAPI*D3D9CREATEEXPROC)(UINT, IDirect3D9Ex**);

namespace Movement
{
	POINT GetMousePosition() {
		POINT mouse;
		RECT rect;
		GetWindowRect(GetActiveWindow(), &rect);
		POINT p;
		GetCursorPos(&p);
		if ((p.y > rect.top && p.y < rect.bottom) && (p.x > rect.left && p.x < rect.right)) {
			mouse.y = (p.y - rect.top) - 30;
			mouse.x = (p.x - rect.left) - 5;
		}
		else {
			mouse.x = 0;
			mouse.y = 0;
		}
		return mouse;
	}

	bool MenuMovement(int &x, int &y, int w, int h, int index, int relx, int rely) {
		for (int i = 0; i < 8; i++)
			if (X::move_index[i][0] && i != index)
				return false;

		POINT Cursor = GetMousePosition();

		if (GetAsyncKeyState(VK_LBUTTON) < 0)
		{
			if (Cursor.x > (relx) && Cursor.x < (relx + w) && Cursor.y >(rely) && Cursor.y < (rely + h) || X::move_index[index][0])
			{
				X::move_index[index][0] = true;

				if (!X::move_index[index][1])
				{
					X::move_cursor.x = Cursor.x - relx;
					X::move_cursor.y = Cursor.y - rely;
					X::move_index[index][1] = true;

				}
			}
			else
			{
				X::move_index[index][0] = false;
				X::move_index[index][1] = false;
			}
		}

		if (GetAsyncKeyState(VK_LBUTTON) == 0 && X::move_index[index][0])
		{
			X::move_index[index][0] = false;
			X::move_index[index][1] = false;
		}

		if (X::move_index[index][0])
		{
			x = Cursor.x - X::move_cursor.x;
			y = Cursor.y - X::move_cursor.y;
		}

		return true;

	}
}

namespace Globals
{
	//////////////////////

	static struct _Keys 
	{ 
		bool bPressed;  
		DWORD dwStartTime; 

	} kPressingKeys[256];

	//////////////////////

	BOOL State_Key(INT Key, DWORD dwTimeOut)
	{
		if (HIWORD(GetKeyState(Key)))
		{
			if (!kPressingKeys[Key].bPressed || (kPressingKeys[Key].dwStartTime && (kPressingKeys[Key].dwStartTime + dwTimeOut) <= GetTickCount()))
			{
				kPressingKeys[Key].bPressed = TRUE;
				if (dwTimeOut > NULL)
					kPressingKeys[Key].dwStartTime = GetTickCount();
				return TRUE;
			}
		}
		else
			kPressingKeys[Key].bPressed = FALSE;
		return FALSE;
	}

	//////////////////////

	BOOL IsInBox(INT x, INT y, INT w, INT h)
	{
		POINT MousePosition;
		GetCursorPos(&MousePosition);
		ScreenToClient(GetForegroundWindow(), &MousePosition);
		return(MousePosition.x >= x && MousePosition.x <= x + w && MousePosition.y >= y && MousePosition.y <= y + h);
	}

	//////////////////////
}

// Modules

typedef struct _LDR_MODULE
{
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID BaseAddress;
	PVOID EntryPoint;
	ULONG SizeOfImage;
} LDR_MODULE, *PLDR_MODULE;

//////////////////////

typedef struct _PEB_LDR_DATA
{
	ULONG Length;
	BOOLEAN Initialized;
	PVOID SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

//////////////////////

typedef struct _PEB
{
	BYTE Reserved1[2];
	BYTE BeingDebugged;
	BYTE Reserved2[1];
	PVOID Reserved3[2];
	PPEB_LDR_DATA Ldr;
} PEB, *PPEB;

//////////////////////





