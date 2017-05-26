using namespace std;

//////////////////////

#define C_Text (DT_CENTER|DT_NOCLIP)

//////////////////////
#include <string>
//////////////////////

bool InitOne = true;

//////////////////////

namespace UI
{
	namespace Tools
	{
		string ConvertIntToString(int a) {
			char out[500];
			itoa(a, out, 10);
			return out;
		}
	}
	
	void Slider(int x, int y, int min, int max, void *value, char *slider_text)
	{
		int width = 100,
			height = 9;
		POINT CursorPos;
		GetCursorPos(&CursorPos);
		ScreenToClient(GetForegroundWindow(), &CursorPos);

		if (*(int*)value > max) *(int*)value = max;
		if (*(int*)value < min) *(int*)value = min;
		int valueX = x + ((*(int*)value - min) * width / (max - min));

		if ((CursorPos.x >= x && CursorPos.x <= x + width && CursorPos.y >= y && CursorPos.y <= y + height) && GetKeyState(VK_LBUTTON) < 0)
			*(int*)value = (CursorPos.x - x) * (max - min) / width;

		//////////////////////

		Render->render_Box(x, y - 1, width, 6, Color(255, 100, 100, 100)); // Background
		Render->render_Box(x, y - 1, valueX - x, 6, Color(255, 123, 220, 117)); // Slider
		
		//////////////////////

		string iValue;
		string fValue = slider_text;

		fValue += " (";
		fValue += Tools::ConvertIntToString(*(int*)value);
		fValue += ")";

		//////////////////////

		Render->render_String(x + width + 7, y - 5, Color(255, 255, 255, 255), DT_LEFT | DT_NOCLIP, fValue.c_str());
	}

	void __fastcall Box(int x, int y, int w, int h, D3DCOLOR Color, IDirect3DDevice9* pD3D9)
	{
		struct Vertex
		{
			float x, y, z, ht;
			DWORD Color;
		}

		//////////////////////

		V[4] = { { x, y + h, 0.0f, 0.0f, Color },{ x, y, 0.0f, 0.0f, Color },{ x + w, y + h, 0.0f, 0.0f, Color },{ x + w, y, 0.0f, 0.0f, Color } };

		pD3D9->SetTexture(0, NULL);
		pD3D9->SetPixelShader(0);
		pD3D9->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		pD3D9->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		pD3D9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pD3D9->SetRenderState(D3DRS_ZENABLE, FALSE);
		pD3D9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		pD3D9->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, V, sizeof(Vertex));

		//////////////////////

		return;
	}

	//////////////////////

	void  String(int x, int y, DWORD color, LPSTR text, DWORD ST)
	{
		RECT rect, rect_2;

		//////////////////////

		SetRect(&rect, x, y, x, y); SetRect(&rect_2, x - 0.1, y + 0.2, x - 0.1, y + 0.1);

		//////////////////////

		pFont->DrawTextA(NULL, text, -1, &rect_2, ST, 0xff000000); pFont->DrawTextA(NULL, text, -1, &rect, ST, color);
	}

	//////////////////////

	void Tab(int x, int y, int &Var, char *Button_Text)
	{
		int w = 10;
		int h = 10;
		int s = 150;

		Render->render_Box(x, y, s, 30, Color(255, 31, 28, 31));
		Render->render_String(x + (s / 2) - 20, y + 6, Color(255, 255, 255, 255), DT_LEFT | DT_NOCLIP, Button_Text);

		if (Globals::IsInBox(x, y - 1, s, 30))
		{
			Render->render_Box(x, y, s, 30, Color(255, 81, 79, 80));
			Render->render_String(x + (s / 2) - 20, y + 6, Color(255, 255, 255, 255), DT_LEFT | DT_NOCLIP, Button_Text);

			if (Globals::State_Key(VK_LBUTTON, 5000))
			{
				tab[0] = tab[1] = tab[2] = tab[3] = tab[4] = false;

				if (Var == 1 && Var != 0)
				{
					Var = 0;
				}
				else if (Var == 0)
				{
					Var = 1;
				}
			}
		}

		if (Var == 1)
		{

			Render->render_Box(x, y, s, 30, Color(255, 95, 93, 91));
			Render->render_String(x + (s / 2) - 20, y + 6, Color(255, 255, 255, 255), DT_LEFT | DT_NOCLIP, Button_Text);
		}
	}

	//////////////////////

	void CheckBox(int x, int y, int &Var, char *Button_Text)
	{
		int w = 10;
		int h = 10;

		//////////////////////

		Render->render_GradientBox(x, y, w + 5, h + 5, Color(255, 68, 68, 68), Color(255, 73, 73, 73));
		Render->render_Border(x - 1, y - 1, w + 5, h + 5, Color(255, 35, 35, 35));
		Render->render_String(x + 25, y + 1, Color(255, 255, 255, 255), DT_LEFT | DT_NOCLIP, Button_Text);

		//////////////////////

		if (Globals::IsInBox(x - 1, y - 1, w + 5, h + 5))
		{
			Render->render_GradientBox(x, y, w + 5, h + 5, Color(255, 73, 73, 73), Color(255, 78, 78, 78));
			Render->render_String(x + 25, y + 1, Color(255, 255, 255, 255), DT_LEFT | DT_NOCLIP, Button_Text);

			if (Globals::State_Key(VK_LBUTTON, 5000))
			{
				if (Var == 1 && Var != 0)
				{
					Var = 0;
				}
				else if (Var == 0)
				{
					Var = 1;
				}
			}
		}

		//////////////////////

		if (Var == 1)
		{
			Render->render_Border(x - 1, y - 1, w + 5, h + 5, Color(255, 35, 35, 35));
			Render->render_GradientBox(x, y, w + 5, h + 5, Color(255, 125, 198, 112), Color(255, 72, 125, 64));
			Render->render_String(x + 25, y + 1, Color(255, 255, 255, 255), DT_LEFT | DT_NOCLIP, Button_Text);
		}
	}

	//////////////////////

	void ListView(int x, int y, int &Var, char **typ, int max, char* text, LPDIRECT3DDEVICE9 pDevice)
	{
		Render->render_String(x + 4, y - 1, Color(255, 255, 255, 255), DT_LEFT | DT_NOCLIP, "-");
		Render->render_String(x + 70 + 2 + 25, y, Color(255, 255, 255, 255), DT_LEFT | DT_NOCLIP, "+");

		//////////////////////

		if (Globals::IsInBox(x, y, 10, 10))
		{
			if (Globals::State_Key(VK_LBUTTON, 5000))
			{
				if (Var != 0)
				{
					Var--;
				}
			}
		}

		//////////////////////

		if (Globals::IsInBox(x + 80 + 15, y, 10, 10))
		{
			if (Globals::State_Key(VK_LBUTTON, 5000))
			{
				if (Var >= 0 && Var<max)
				{
					Var++;
				}
			}
		}

		//////////////////////

		Render->render_String(x + 55, y, Color(255, 255, 255, 255), DT_CENTER, typ[Var]);
		Render->render_String(x + 110 + 25, y - 1, Color(255, 255, 255, 255), DT_LEFT | DT_NOCLIP, text);
	}
}

//////////////////////

FLOAT x = 40, y = 40, w = 150, h = 30;

//////////////////////

struct MyItems
{
	FLOAT x;
	FLOAT y;
} MenuItems;

//////////////////////

class sButton;

//////////////////////

std::vector<sButton*> virtual_var;

//////////////////////

class sButton
{
public:

	sButton()
	{
		virtual_var.push_back(this);
	};

	int fValue;
};

//////////////////////

sButton  cVar1;
sButton  cVar2;
sButton  cVar3;
sButton  cVar4;
sButton  cVar5;

//////////////////////
