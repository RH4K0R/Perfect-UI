#include "cRender.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

cRender *Render;

cRender::cRender(IDirect3DDevice9* m_pDevice, const TCHAR* strFontName, DWORD dwFontHeight)
{
	ZeroMemory(this, sizeof(cRender));

	this->m_pDevice    = m_pDevice;
	this->strFontName  = strFontName;
	this->dwFontHeight = dwFontHeight;
}
///////////////////////////////////////////////////////////////
HRESULT  cRender::FontRestore()
{
	this->m_pFont = new CD3DFont(this->strFontName, this->dwFontHeight, D3DFONT_BOLD);
	this->m_pFont->InitDeviceObjects(this->m_pDevice);
	this->m_pFont->RestoreDeviceObjects();
	return S_OK;
}
///////////////////////////////////////////////////////////////
HRESULT  cRender::FontReset(void)
{
	this->m_pFont->InvalidateDeviceObjects();
	this->m_pFont->DeleteDeviceObjects();
	delete m_pFont;
	this->m_pFont = NULL;
	return S_OK;
}
///////////////////////////////////////////////////////////////
void     cRender::render_String(float x, float y, Color color, DWORD Flag, const char *fmt, ...)
{
	char buf[1024] = { '\0' };
	va_list va_alist;
	va_start(va_alist, fmt);
	vsprintf_s(buf, fmt, va_alist);
	va_end(va_alist);
	m_pFont->DrawText(x, y, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b), buf, Flag | DT_SHADOW);
}
///////////////////////////////////////////////////////////////
void cRender::FillRGB(int x, int y, int w, int h, Color color)
{
	D3DRECT rec = { x, y, x + w, y + h };
	m_pDevice->Clear(1, &rec, D3DCLEAR_TARGET, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b), 0, 0);
}
///////////////////////////////////////////////////////////////
struct vertex
{
	FLOAT x, y, z, rhw;
	DWORD color;
};
///////////////////////////////////////////////////////////////

LPDIRECT3DINDEXBUFFER9  g_pIB;
LPDIRECT3DVERTEXBUFFER9 g_pVB;

void cRender::render_FillRGB(int x, int y, int w, int h, DWORD color) {

	vertex V[4];

	V[0].color = V[1].color = V[2].color = V[3].color = color;

	V[0].z = V[1].z = V[2].z = V[3].z = 0;
	V[0].rhw = V[1].rhw = V[2].rhw = V[3].rhw = 0;

	V[0].x = x;
	V[0].y = y;
	V[1].x = x + w;
	V[1].y = y;
	V[2].x = x + w;
	V[2].y = y + h;
	V[3].x = x;
	V[3].y = y + h;

	unsigned short indexes[] = { 0, 1, 3, 1, 2, 3 };

	m_pDevice->CreateVertexBuffer(4 * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);
	m_pDevice->CreateIndexBuffer(2 * sizeof(short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL);

	VOID* pVertices;
	g_pVB->Lock(0, sizeof(V), (void**)&pVertices, 0);
	memcpy(pVertices, V, sizeof(V));
	g_pVB->Unlock();

	VOID* pIndex;
	g_pIB->Lock(0, sizeof(indexes), (void**)&pIndex, 0);
	memcpy(pIndex, indexes, sizeof(indexes));
	g_pIB->Unlock();

	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetPixelShader(NULL);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->SetIndices(g_pIB);

	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	g_pVB->Release();
	g_pIB->Release();
}

///////////////////////////////////////////////////////////////
void cRender::render_Rainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
{
	Color colColor(255, 0, 0, 0);

	flRainbow += flSpeed;
	if (flRainbow > 1.0f) flRainbow = 0.0f;

	for (int i = 0; i < (width); i++)
	{
		float hue = (1.f / (float)(width)) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1;
		if (hue > 1.0f) hue -= 1;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		FillRGB(x + i, y, 1, height, colRainbow);
	}
}
///////////////////////////////////////////////////////////////
void     cRender::render_Box(float x, float y, float w, float h, Color color)
{
	FillRGB(x, y, w, h, color);
}
///////////////////////////////////////////////////////////////
void     cRender::render_Border(float x, float y, float w, float h, Color color, float s)
{
	render_Box(x, y, s, h, color);
	render_Box(x, y + h, w, s, color);
	render_Box(x, y, w, s, color);
	render_Box(x + w, y, s, h + s, color);
}
///////////////////////////////////////////////////////////////
void     cRender::render_Border2(float x, float y, float w, float h, Color color, float s)
{
	render_Box(x, y, s, h, color);
	render_Box(x, y + h, w, s, color);
	render_Box(x, y, w, s, color);
	render_Box(x + w, y, s, h + s, color);
}
///////////////////////////////////////////////////////////////
void     cRender::render_GradientBox(float x, float y, float width, float height, Color startCol, Color endCol, OrientationBox orientation)
{
	static struct D3DVERTEX
	{
		float x,
		y,
		z,
		rhw;
		DWORD color;
	}
	vertices[4] = {
		{ 0, 0, 0, 1.0f, 0 },
		{ 0, 0, 0, 1.0f, 0 },
		{ 0, 0, 0, 1.0f, 0 },
		{ 0, 0, 0, 1.0f, 0 }
	};
	vertices[0].x = x;
	vertices[0].y = y;
	vertices[0].color = D3DCOLOR_ARGB(startCol.a, startCol.r, startCol.g, startCol.b);
	vertices[1].x = x + width;
	vertices[1].y = y;
	vertices[1].color = orientation == horizontal ? D3DCOLOR_ARGB(endCol.a, endCol.r, endCol.g, endCol.b) : 
		                                            D3DCOLOR_ARGB(startCol.a, startCol.r, startCol.g, startCol.b);
	vertices[2].x = x;
	vertices[2].y = y + height;
	vertices[2].color = orientation == horizontal ? D3DCOLOR_ARGB(startCol.a, startCol.r, startCol.g, startCol.b) : 
		                                            D3DCOLOR_ARGB(endCol.a, endCol.r, endCol.g, endCol.b);
	vertices[3].x = x + width;
	vertices[3].y = y + height;
	vertices[3].color = D3DCOLOR_ARGB(endCol.a, endCol.r, endCol.g, endCol.b);

	this->m_pDevice->SetTexture(0, NULL);
	this->m_pDevice->SetPixelShader(0);
	this->m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	this->m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	this->m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	this->m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	this->m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(D3DVERTEX));
}
///////////////////////////////////////////////////////////////
void     cRender::render_Line(float startX, float startY, float endX, float endY, Color color)
{
	D3DTLVERTEX Vertex[2];
	Vertex[0].X = startX;
	Vertex[0].Y = startY;
	Vertex[0].Color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
	Vertex[1].X = endX;
	Vertex[1].Y = endY;
	Vertex[1].Color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
	const DWORD FVFTL = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
	IDirect3DBaseTexture9* pTexture = NULL;

	DWORD dwLighting = 0, dwZEnable = 0, dwZFunc = 0, dwFVF = 0, dwColorOp = 0, dwColorArg = 0;

	this->m_pDevice->GetTexture(0, &pTexture);
	this->m_pDevice->GetRenderState(D3DRS_LIGHTING, &dwLighting);
	this->m_pDevice->GetRenderState(D3DRS_ZENABLE, &dwZEnable);
	this->m_pDevice->GetRenderState(D3DRS_ZFUNC, &dwZFunc);
	this->m_pDevice->GetFVF(&dwFVF);
	this->m_pDevice->GetTextureStageState(0, D3DTSS_COLOROP, &dwColorOp);
	this->m_pDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &dwColorArg);

	this->m_pDevice->SetTexture(0, NULL);
	this->m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	this->m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	this->m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	this->m_pDevice->SetFVF(FVFTL);
	this->m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	this->m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

	this->m_pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &Vertex, sizeof(D3DTLVERTEX));

	this->m_pDevice->SetTexture(0, pTexture);
	this->m_pDevice->SetRenderState(D3DRS_LIGHTING, dwLighting);
	this->m_pDevice->SetRenderState(D3DRS_ZENABLE, dwZEnable);
	this->m_pDevice->SetRenderState(D3DRS_ZFUNC, dwZFunc);
	this->m_pDevice->SetFVF(dwFVF);
	this->m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp);
	this->m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dwColorArg);
	return;
}
///////////////////////////////////////////////////////////////
cRender::~cRender()
{
	ZeroMemory(this, sizeof(cRender));
}
