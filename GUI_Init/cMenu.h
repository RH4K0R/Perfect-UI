#define WINDOWNAME "D3D Test"
#define TITLENAME "D3D Test

#define MENUH 100

void DrawUIFrame(int x, int y, LPDIRECT3DDEVICE9 pD3D9)
{	
	static float rainbow;

	if (AlphaFade)
	{
		X::alpha++;

		if (X::alpha > 255)
			X::alpha = 255;
	}
	else
		X::alpha = 255;

	Render->render_FillRGB(x + 2, y + 2, 602, 220 + MENUH, D3DCOLOR_ARGB(X::alpha, 19, 19, 19));
	Render->render_Border2(x - 2, y - 2, 602 + 6, 220 + 6 + MENUH, Color(255, 30, 30, 30)); // Outline Outside
	
	UI::Tab(x + 2, y + 7, tab[0], "Aimbot");
	UI::Tab(x + 2 + 150, y + 7, tab[1], "Visuals");
	UI::Tab(x + 2 + 150 * 2, y + 7, tab[2], "Misc");
	UI::Tab(x + 2 + 150 * 3, y + 7, tab[3], "Colors");

	Render->render_String(5, 5, Color(255, 218, 218, 218), DT_LEFT | DT_NOCLIP, WINDOWNAME);

	Render->render_Rainbow(x + 2, y + 4, 602, 2, 0.001f, rainbow); // Rainbow Bar

	if (tab[0])
	{
		UI::Slider(x + 10, y + 50, 0, 180, &example_value[0], "Example Slider");

		UI::ListView(x + 10, y + 70, example_int_value, example_char, 2, "Example Listview", pD3D9);

		UI::CheckBox(x + 10, y + 90, example_bool_value[0], "Example Checkbox");
	}
}
