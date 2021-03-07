//=========
// Color.h
//=========

#pragma once


//=======
// Using
//=======

#include "Color.h"
#include "Variable.h"


//===========
// Namespace
//===========

namespace Graphics {


//================
// Color-Variable
//================

class ColorVariable: public TypedVariable<COLOR>
{
public:
	// Con-/Destructors
	ColorVariable(COLOR Color=0);
	ColorVariable(Colors Colors);
	ColorVariable(Handle<String> Name, COLOR Color=0);

	// Common
	Property<ColorVariable, BYTE> Alpha;
	Property<ColorVariable, BYTE> Blue;
	Property<ColorVariable, BYTE> Green;
	Property<ColorVariable, BYTE> Red;
	Handle<String> ToString()override;

	// Modification
	VOID Set(COLOR const& Color)override;
	VOID Set(Colors Colors) { Set((UINT)Colors); }
	VOID Set(BYTE R, BYTE G, BYTE B, BYTE A=0xFF) { Set(COLOR(R, G, B, A)); }

private:
	// Flags
	enum class ColorFlags: UINT
		{
		None=0,
		Setting=1
		};

	// Common
	VOID OnAlphaChanged(BYTE Alpha);
	VOID OnBlueChanged(BYTE Blue);
	VOID OnGreenChanged(BYTE Green);
	VOID OnRedChanged(BYTE Red);
	ColorFlags uFlags;
};

}
