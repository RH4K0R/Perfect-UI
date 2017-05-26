#pragma once

#pragma warning(disable: 4244)

class Color
{
public:

	int a, r, g, b;

	Color()
	{ 
		Color(0, 0, 0, 0);
	}

	Color(int color32)
	{
		*((int *)this) = color32;
	}

	Color(int _a, int _r, int _g, int _b)
	{
		SetColor(_a, _r, _g, _b);
	}

	void SetColor(int a, int r, int g, int b)
	{
		this->a = a;
		this->r = r;
		this->g = g;
		this->b = b;
	}

	Color operator+(const Color& c) const
	{
		return Color(a + c.a, r + c.r, g + c.g, b + c.b);
	}

	Color& operator+=(const Color& c)
	{
		a += c.a;
		r += c.r;
		g += c.g;
		b += c.b;
		return *this;
	}

	Color operator*(float s) const
	{
		return Color(a * s, r * s, g * s, b * s);
	}

	Color& operator*=(float s)
	{
		a *= s;
		r *= s;
		g *= s;
		b *= s;
		return *this;
	}

	Color operator/(float s) const
	{
		return Color(a / s, r / s, g / s, b / s);
	}

	Color& operator/=(float s)
	{
		a /= s;
		r /= s;
		g /= s;
		b /= s;
		return *this;
	}

	static Color FromHSB(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return Color(
				255,
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 2)
		{
			return Color(
				255,
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 3)
		{
			return Color(
				255,
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		else if (h < 4)
		{
			return Color(
				255,
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else if (h < 5)
		{
			return Color(
				255,
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else
		{
			return Color(
				255,
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
			);
		}
	}
};
