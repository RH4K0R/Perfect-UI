#include <vector>
#include <string>
#include <sstream>
#include <vector>

#include "cBase.h"
#include "cVar.h"
#include "cDrawMenu.h"
#include "cMenu.h"

/////////////////////

void SetupFont(CHAR* FontName, DWORD Stile, LPDIRECT3DDEVICE9 pD3D9)
{
	D3DXCreateFont(pD3D9, 13, 0, Stile, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, FontName, &pFont);
}
 
/////////////////////

typedef HRESULT(WINAPI *tPresent)(LPDIRECT3DDEVICE9 pDevice, const RECT *a, const RECT *b, HWND c, const RGNDATA *d);
tPresent oPresent;

/////////////////////

bool Show = 1;

/////////////////////

HRESULT WINAPI Present_HK(LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	chPresent.Unhook();
	
	if (InitOne)
	{
		Render = new cRender(pDevice);

		InitOne = false;

		SetupFont("Tahoma", FW_BOLD, pDevice);

		D3DXCreateLine(pDevice, &pLine);
		
	}

	(MenuItems).x = x;
	(MenuItems).y = y;
	
	Render->FontRestore();

	if (GetAsyncKeyState(VK_INSERT) & 1) Show = (!Show);

	Movement::MenuMovement(X::menu_x, X::menu_y, X::menu_w,X::menu_h, 0, X::menu_x, X::menu_y);

	if (Show)
		DrawUIFrame(X::menu_x, X::menu_y, pDevice);
	else
		X::alpha = 0;

	pLine->OnLostDevice();
	pFont->OnLostDevice();

	Render->FontReset();
	
	HRESULT r_Present = pDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	chPresent.Rehook();
	return r_Present;
}
 
bool Interface()
{
	HMODULE hD3D9 = NULL;
	bool bResult = false;

	char szPath[256];

	SHGetFolderPathA(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, szPath);
	strcat_s(szPath, "\\d3d9.dll");
	
	if (hD3D9 = GetModuleHandleA(szPath))
	{
		D3D9CREATEEXPROC CreateEx = (D3D9CREATEEXPROC)GetProcAddress(hD3D9, "Direct3DCreate9Ex");

		if (CreateEx)
		{
			HRESULT hr = D3D_OK;
			
			LPDIRECT3D9EX d3d9ex = nullptr;

			if (SUCCEEDED(hr = CreateEx(D3D_SDK_VERSION, &d3d9ex)))
			{
				D3DPRESENT_PARAMETERS dp;

				ZeroMemory(&dp, sizeof(dp));

				/////////////////////////////
				//// Presentation Flags /////
				/////////////////////////////

				dp.Windowed = 1;
				dp.SwapEffect = D3DSWAPEFFECT_FLIP;
				dp.BackBufferFormat = D3DFMT_A8R8G8B8;
				dp.BackBufferCount = 1;
				dp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
				
				IDirect3DDevice9Ex *MyDevice = nullptr;
				
				if (SUCCEEDED(hr = d3d9ex->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dp, NULL, &MyDevice)))
				{

					/////////////////////////////
					////// Hooking System ///////
					/////////////////////////////

					bResult = true;

					UPARAM * vtbl = *(UPARAM**)MyDevice;
					chPresent.Hook((FARPROC)*(vtbl + 17), (FARPROC)Present_HK);

					MyDevice->Release();
					
					chPresent.Rehook();
					
				}

				d3d9ex->Release();
				
			}
		}
	}

	return bResult;
}

HMODULE hMod;
DWORD WINAPI MyThread(HANDLE hThread)
{	
	while (!Interface())
		Sleep(200);

	while (!GetKeyState(VK_END))
		Sleep(1);

	chPresent.Unhook();

	Sleep(200);

	FreeLibraryAndExitThread(hMod, 0);

	return 0;
}
#define StartRoutine(Function) CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Function, 0, 0, 0);
BOOL __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	if (dwReason == DLL_PROCESS_ATTACH)
	{		
		hMod = hModule;

		StartRoutine(MyThread);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		chPresent.Unhook();
	}
	return TRUE;
}
