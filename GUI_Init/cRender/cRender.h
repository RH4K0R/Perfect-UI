#pragma once
///////////////////////////
#include <d3d9.h>        //
 #include <d3dx9.h>      //
///////////////////////////
#include <stdio.h>       //
///////////////////////////
#include "cColor.h"		 //
///////////////////////////	
#include "cFont\cFont.h" //
///////////////////////////
struct sScreen
{
	int Width, Height;
	sScreen(int Width, int Height)
	{
		this->Width = Width;
		this->Height = Height;
	}
};
////////////////////////////////////////
enum OrientationBox { horizontal, vertical };
////////////////////////////////////////
class cRender
{
public:
	cRender(IDirect3DDevice9* m_pDevice, const TCHAR* strFontName = __TEXT("Tahoma"), DWORD dwFontHeight = 8);
	~cRender();

	CD3DFont *m_pFont;
	HRESULT FontRestore();
	HRESULT FontReset(void);

	void FillRGB(int x, int y, int w, int h, Color color);

	////////////////////////////////////////

	void render_Rainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow);
	void render_String(float x, float y, Color color, DWORD Flag, const TCHAR *fmt, ...);
	void render_Box(float x, float y, float w, float h, Color color);
	void render_Border(float x, float y, float w, float h, Color color, float s = 1.f);
	void render_Border2(float x, float y, float w, float h, Color color, float s = 1.f);
	void render_FillRGB(int x, int y, int w, int h, DWORD color);
	void render_GradientBox(float x, float y, float width, float height, Color startCol, Color endCol, OrientationBox orientation = vertical);
	void render_Line(float startX, float startY, float endX, float endY, Color color);

	////////////////////////////////////////

	sScreen GetResolition()
	{
		DWORD Width, Height;
		this->m_pDevice->GetViewport(&m_ViewPort);
		Width = m_ViewPort.Width;
		Height = m_ViewPort.Height;
		sScreen ret = sScreen(Width, Height);
		return ret;
	}	

private:
	D3DVIEWPORT9 m_ViewPort;
	IDirect3DDevice9* m_pDevice;
	const TCHAR* strFontName;
	DWORD dwFontHeight;	
}; extern cRender *Render;
////////////////////////////////////////
struct D3DTLVERTEX
{
	float X, Y, Z, RHW;
	DWORD Color;
	float fU;
	float fV;
};
////////////////////////////////////////