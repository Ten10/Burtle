#pragma once

#define RGB(r, g, b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

enum class eColor : COLORREF
{
	Aquamarine = RGB(0x70, 0xDB, 0x93),
	BakersChocolate = RGB(0x5C, 0x33, 0x17),
	Black = RGB(0x00, 0x00, 0x00),
	Blue = RGB(0x00, 0x00, 0xFF),
	BlueViolet = RGB(0x9F, 0x5F, 0x9F),
	Brass = RGB(0xB5, 0xA6, 0x42),
	BrightGold = RGB(0xD9, 0xD9, 0x19),
	Brown = RGB(0xA6, 0x2A, 0x2A),
	Bronze = RGB(0x8C, 0x78, 0x53),
	BronzeII = RGB(0xA6, 0x7D, 0x3D),
	CadetBlue = RGB(0x5F, 0x9F, 0x9F),
	CoolCopper = RGB(0xD9, 0x87, 0x19),
	Copper = RGB(0xB8, 0x73, 0x33),
	Coral = RGB(0xFF, 0x7F, 0x00),
	CornFlowerBlue = RGB(0x42, 0x42, 0x6F),
	Cyan = RGB(0x00, 0xFF, 0xFF),
	DarkBrown = RGB(0x5C, 0x40, 0x33),
	DarkGreen = RGB(0x2F, 0x4F, 0x2F),
	DarkGreenCopper = RGB(0x4A, 0x76, 0x6E),
	DarkOliveGreen = RGB(0x4F, 0x4F, 0x2F),
	DarkOrchid = RGB(0x99, 0x32, 0xCD),
	DarkPurple = RGB(0x87, 0x1F, 0x78),
	DarkSlateBlue = RGB(0x6B, 0x23, 0x8E),
	DarkSlateGrey = RGB(0x2F, 0x4F, 0x4F),
	DarkTan = RGB(0x97, 0x69, 0x4F),
	DarkTurquoise = RGB(0x70, 0x93, 0xDB),
	DarkWood = RGB(0x85, 0x5E, 0x42),
	DimGrey = RGB(0x54, 0x54, 0x54),
	DustyRose = RGB(0x85, 0x63, 0x63),
	Feldspar = RGB(0xD1, 0x92, 0x75),
	Firebrick = RGB(0x8E, 0x23, 0x23),
	ForestGreen= RGB(0x23, 0x8E, 0x23),
	Gold = RGB(0xCD, 0x7F, 0x32),
	Goldenrod = RGB(0xDB, 0xDB, 0x70),
	Grey = RGB(0xC0, 0xC0, 0xC0),
	Green = RGB(0x00, 0xFF, 0x00),
	GreenCopper = RGB(0x52, 0x7F, 0x76),
	GreenYellow = RGB(0x93, 0xDB, 0x70),
	HunterGreen= RGB(0x21, 0x5E, 0x21),
	IndianRed = RGB(0x4E, 0x2F, 0x2F),
	Khaki = RGB(0x9F, 0x9F, 0x5F),
	LightBlue = RGB(0xC0, 0xD9, 0xD9),
	LightGrey = RGB(0xC8, 0xC8, 0xC8),
	LightSteelBlue = RGB(0x8F, 0x8F, 0xBD),
	LightWood = RGB(0xE9, 0xC2, 0xA6),
	LimeGreen = RGB(0x32, 0xCD, 0x32),
	Magenta = RGB(0xFF, 0x00, 0xFF),
	MandarianOrange = RGB(0xE4, 0x78, 0x33),
	Maroon = RGB(0x8E, 0x23, 0x6B),
	MediumAquamarine = RGB(0x32, 0xCD, 0x99),
	MediumBlue = RGB(0x32, 0x32, 0xCD),
	MediumForestGreen = RGB(0x6B, 0x8E, 0x23),
	MediumGoldenrod = RGB(0xEA, 0xEA, 0xAE),
	MediumOrchid = RGB(0x93, 0x70, 0xDB),
	MediumSeaGreen = RGB(0x42, 0x6F, 0x42),
	MediumSlateBlue = RGB(0x7F, 0x00, 0xFF),
	MediumSpringGreen = RGB(0x7F, 0xFF, 0x00),
	MediumTurquoise = RGB(0x70, 0xDB, 0xDB),
	MediumVioletRed = RGB(0xDB, 0x70, 0x93),
	MediumWood = RGB(0xA6, 0x80, 0x64),
	MidnightBlue = RGB(0x2F, 0x2F, 0x4F),
	NavyBlue = RGB(0x23, 0x23, 0x8E),
	NeonBlue = RGB(0x4D, 0x4D, 0xFF),
	NeonPink = RGB(0xFF, 0x6E, 0xC7),
	NewMidnightBlue = RGB(0x00, 0x00, 0x9C),
	NewTan = RGB(0xEB, 0xC7, 0x9E),
	OldGold = RGB(0xCF, 0xB5, 0x3B),
	Orange = RGB(0xFF, 0x7F, 0x00),
	OrangeRed = RGB(0xFF, 0x24, 0x00),
	Orchid = RGB(0xDB, 0x70, 0xDB),
	PaleGreen = RGB(0x8F, 0xBC, 0x8F),
	Pink = RGB(0xBC, 0x8F, 0x8F),
	Plum = RGB(0xEA, 0xAD, 0xEA),
	Quartz = RGB(0xD9, 0xD9, 0xF3),
	Red = RGB(0xFF, 0x00, 0x00),
	RichBlue = RGB(0x59, 0x59, 0xAB),
	Salmon = RGB(0x6F, 0x42, 0x42),
	Scarlet = RGB(0x8C, 0x17, 0x17),
	SeaGreen = RGB(0x23, 0x8E, 0x68),
	SemiSweetChocolate = RGB(0x6B, 0x42, 0x26),
	Sienna = RGB(0x8E, 0x6B, 0x23),
	Silver = RGB(0xE6, 0xE8, 0xFA),
	SkyBlue = RGB(0x32, 0x99, 0xCC),
	SlateBlue = RGB(0x00, 0x7F, 0xFF),
	SpicyPink = RGB(0xFF, 0x1C, 0xAE),
	SpringGreen = RGB(0x00, 0xFF, 0x7F),
	SteelBlue = RGB(0x23, 0x6B, 0x8E),
	SummerSky = RGB(0x38, 0xB0, 0xDE),
	Tan = RGB(0xDB, 0x93, 0x70),
	Thistle = RGB(0xD8, 0xBF, 0xD8),
	Turquoise = RGB(0xAD, 0xEA, 0xEA),
	VeryDarkBrown = RGB(0x5C, 0x40, 0x33),
	VeryLightGrey = RGB(0xCD, 0xCD, 0xCD),
	Violet = RGB(0x4F, 0x2F, 0x4F),
	VioletRed = RGB(0xCC, 0x32, 0x99),
	Wheat = RGB(0xD8, 0xD8, 0xBF),
	White = RGB(0xFF, 0xFF, 0xFF),
	Yellow = RGB(0xFF, 0xFF, 0x00),
	YellowGreen = RGB(0x99, 0xCC, 0x32),
	DarkYellow = RGB(0x99, 0xCC, 0x32),
};