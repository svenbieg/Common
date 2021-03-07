//=========
// Color.h
//=========

#pragma once


//===========
// Namespace
//===========

namespace Graphics {


//========
// Colors
//========

enum class Colors: UINT
{
Black=0xFF000000U,
Blue=0xFFFF0000U,
Green=0xFF00FF00U,
LightGray=0xFFD0D0D0U,
Orange=0xFF00A5FF,
Red=0xFF0000FFU,
Transparent=0U,
Violett=0xFFFF0088U,
Yellow=0xFF00FFFFU,
White=0xFFFFFFFFU
};


//=======
// Color
//=======

class COLOR
{
public:
	// Con-/Destructors
	COLOR(UINT Color=0): uColor(Color) {}
	COLOR(Colors Color): uColor((UINT)Color) {}
	COLOR(BYTE R, BYTE G, BYTE B, BYTE A=0xFF) { Set(R, G, B, A); }
	#ifdef __cplusplus_winrt
	COLOR(Windows::UI::Color Color) { Set(Color.R, Color.G, Color.B, Color.A); }
	#endif

	// Static Access
	static inline BYTE GetAlpha(COLOR Color) { return (BYTE)(Color>>24); }
	static inline BYTE GetBlue(COLOR Color) { return (BYTE)(Color>>16); }
	static inline BYTE GetGreen(COLOR Color) { return (BYTE)(Color>>8); }
	static inline BOOL GetMonochrome(COLOR Color) { return Color>=0xFF808080; }
	static inline BYTE GetRed(COLOR Color) { return (BYTE)Color; }
	static UINT Reverse(COLOR Color);
	static Handle<String> ToString(COLOR Color);

	// Access
	operator UINT()const { return uColor; }
	inline BYTE GetAlpha()const { return (BYTE)(uColor>>24); }
	inline BYTE GetBlue()const { return (BYTE)(uColor>>16); }
	inline BYTE GetGreen()const { return (BYTE)(uColor>>8); }
	inline BOOL GetMonochrome()const { return uColor>=0xFF808080; }
	inline BYTE GetRed()const { return (BYTE)uColor; }
	inline UINT Reverse()const { return Reverse(uColor); }
	inline Handle<String> ToString() { return ToString(uColor); }

	// Modification
	COLOR& operator+=(COLOR Add);
	VOID Set(BYTE R, BYTE G, BYTE B, BYTE A=0xFF);
	VOID SetAlpha(BYTE Alpha);
	VOID SetBlue(BYTE Blue);
	VOID SetBrightness(FLOAT Brightness);
	VOID SetGreen(BYTE Green);
	VOID SetMonochrome(BOOL Monochrome);
	VOID SetRed(BYTE Red);

private:
	// Common
	UINT uColor;
};

}
