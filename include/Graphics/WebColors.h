/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

namespace astu {

    /**
     * This enumeration defines values for color names according to 
     * the W3C specification.
     * 
     * @ingroup gfx_group
     */
    enum WebColors {
	 
		///////////////////////////
		/////// Pink colors ///////
		///////////////////////////
		
		/** Color constant for color *MediumVioletRed* (0xc71585). */
		MediumVioletRed = 0xc71585,
	
		/** Color constant for color *DeepPink* (0xff1493). */
		DeepPink = 0xff1493,
	
		/** Color constant for color *PaleVioletRed* (0xdb7093). */
		PaleVioletRed = 0xdb7093,
	
		/** Color constant for color *HotPink* (0xff69b4). */
		HotPink = 0xff69b4,
	
		/** Color constant for color *LightPink* (0xffb6c1). */
		LightPink = 0xffb6c1,
	
		/** Color constant for color *Pink* (0xffc0cb). */
		Pink = 0xffc0cb,
	
	 
		//////////////////////////
		/////// Red colors ///////
		//////////////////////////
		
		/** Color constant for color *DarkRed* (0x8b0000). */
		DarkRed = 0x8b0000,
	
		/** Color constant for color *Red* (0xff0000). */
		Red = 0xff0000,
	
		/** Color constant for color *Firebrick* (0xb22222). */
		Firebrick = 0xb22222,
	
		/** Color constant for color *Crimson* (0xdc143c). */
		Crimson = 0xdc143c,
	
		/** Color constant for color *IndianRed* (0xcd5c5c). */
		IndianRed = 0xcd5c5c,
	
		/** Color constant for color *LightCoral* (0xf08080). */
		LightCoral = 0xf08080,
	
		/** Color constant for color *Salmon* (0xfa8072). */
		Salmon = 0xfa8072,
	
		/** Color constant for color *DarkSalmon* (0xe9967a). */
		DarkSalmon = 0xe9967a,
	
		/** Color constant for color *LightSalmon* (0xffa07a). */
		LightSalmon = 0xffa07a,
	
	 
		/////////////////////////////
		/////// Orange colors ///////
		/////////////////////////////
		
		/** Color constant for color *OrangeRed* (0xff4500). */
		OrangeRed = 0xff4500,
	
		/** Color constant for color *Tomato* (0xff6347). */
		Tomato = 0xff6347,
	
		/** Color constant for color *DarkOrange* (0xff8c00). */
		DarkOrange = 0xff8c00,
	
		/** Color constant for color *Coral* (0xff7f50). */
		Coral = 0xff7f50,
	
		/** Color constant for color *Orange* (0xffa500). */
		Orange = 0xffa500,
	
	 
		/////////////////////////////
		/////// Yellow colors ///////
		/////////////////////////////
		
		/** Color constant for color *DarkKhaki* (0xbdb76b). */
		DarkKhaki = 0xbdb76b,
	
		/** Color constant for color *Gold* (0xffd700). */
		Gold = 0xffd700,
	
		/** Color constant for color *Khaki* (0xf0e68c). */
		Khaki = 0xf0e68c,
	
		/** Color constant for color *PeachPuff* (0xffdab9). */
		PeachPuff = 0xffdab9,
	
		/** Color constant for color *Yellow* (0xffff00). */
		Yellow = 0xffff00,
	
		/** Color constant for color *PaleGoldenrod* (0xeee8aa). */
		PaleGoldenrod = 0xeee8aa,
	
		/** Color constant for color *Moccasin* (0xffe4b5). */
		Moccasin = 0xffe4b5,
	
		/** Color constant for color *PapayaWhip* (0xffefd5). */
		PapayaWhip = 0xffefd5,
	
		/** Color constant for color *LightGoldenrodYellow* (0xfafad2). */
		LightGoldenrodYellow = 0xfafad2,
	
		/** Color constant for color *LemonChiffon* (0xfffacd). */
		LemonChiffon = 0xfffacd,
	
		/** Color constant for color *LightYellow* (0xffffe0). */
		LightYellow = 0xffffe0,
	
	 
		////////////////////////////
		/////// Brown colors ///////
		////////////////////////////
		
		/** Color constant for color *Maroon* (0x800000). */
		Maroon = 0x800000,
	
		/** Color constant for color *Brown* (0xa52a2a). */
		Brown = 0xa52a2a,
	
		/** Color constant for color *SaddleBrown* (0x8b4513). */
		SaddleBrown = 0x8b4513,
	
		/** Color constant for color *Sienna* (0xa0522d). */
		Sienna = 0xa0522d,
	
		/** Color constant for color *Chocolate* (0xd2691e). */
		Chocolate = 0xd2691e,
	
		/** Color constant for color *DarkGoldenrod* (0xb8860b). */
		DarkGoldenrod = 0xb8860b,
	
		/** Color constant for color *Peru* (0xcd853f). */
		Peru = 0xcd853f,
	
		/** Color constant for color *RosyBrown* (0xbc8f8f). */
		RosyBrown = 0xbc8f8f,
	
		/** Color constant for color *Goldenrod* (0xdaa520). */
		Goldenrod = 0xdaa520,
	
		/** Color constant for color *SandyBrown* (0xf4a460). */
		SandyBrown = 0xf4a460,
	
		/** Color constant for color *Tan* (0xd2b48c). */
		Tan = 0xd2b48c,
	
		/** Color constant for color *Burlywood* (0xdeb887). */
		Burlywood = 0xdeb887,
	
		/** Color constant for color *Wheat* (0xf5deb3). */
		Wheat = 0xf5deb3,
	
		/** Color constant for color *NavajoWhite* (0xffdead). */
		NavajoWhite = 0xffdead,
	
		/** Color constant for color *Bisque* (0xffe4c4). */
		Bisque = 0xffe4c4,
	
		/** Color constant for color *BlanchedAlmond* (0xffebcd). */
		BlanchedAlmond = 0xffebcd,
	
		/** Color constant for color *Cornsilk* (0xfff8dc). */
		Cornsilk = 0xfff8dc,
	
	 
		////////////////////////////
		/////// Green Colors ///////
		////////////////////////////
		
		/** Color constant for color *DarkGreen* (0x006400). */
		DarkGreen = 0x006400,
	
		/** Color constant for color *Green* (0x008000). */
		Green = 0x008000,
	
		/** Color constant for color *DarkOliveGreen* (0x556b2f). */
		DarkOliveGreen = 0x556b2f,
	
		/** Color constant for color *ForestGreen* (0x228b22). */
		ForestGreen = 0x228b22,
	
		/** Color constant for color *SeaGreen* (0x2e8b57). */
		SeaGreen = 0x2e8b57,
	
		/** Color constant for color *Olive* (0x808000). */
		Olive = 0x808000,
	
		/** Color constant for color *OliveDrab* (0x6b8e23). */
		OliveDrab = 0x6b8e23,
	
		/** Color constant for color *MediumSeaGreen* (0x3cb371). */
		MediumSeaGreen = 0x3cb371,
	
		/** Color constant for color *LimeGreen* (0x32cd32). */
		LimeGreen = 0x32cd32,
	
		/** Color constant for color *Lime* (0x00ff00). */
		Lime = 0x00ff00,
	
		/** Color constant for color *SpringGreen* (0x00ff7f). */
		SpringGreen = 0x00ff7f,
	
		/** Color constant for color *MediumSpringGreen* (0x00fa9a). */
		MediumSpringGreen = 0x00fa9a,
	
		/** Color constant for color *DarkSeaGreen* (0x8fbc8f). */
		DarkSeaGreen = 0x8fbc8f,
	
		/** Color constant for color *MediumAquamarine* (0x66cdaa). */
		MediumAquamarine = 0x66cdaa,
	
		/** Color constant for color *YellowGreen* (0x9acd32). */
		YellowGreen = 0x9acd32,
	
		/** Color constant for color *LawnGreen* (0x7cfc00). */
		LawnGreen = 0x7cfc00,
	
		/** Color constant for color *Chartreuse* (0x7fff00). */
		Chartreuse = 0x7fff00,
	
		/** Color constant for color *LightGreen* (0x90ee90). */
		LightGreen = 0x90ee90,
	
		/** Color constant for color *GreenYellow* (0xadff2f). */
		GreenYellow = 0xadff2f,
	
		/** Color constant for color *PaleGreen* (0x98fb98). */
		PaleGreen = 0x98fb98,
	
	 
		///////////////////////////
		/////// Cyan colors ///////
		///////////////////////////
		
		/** Color constant for color *Teal* (0x008080). */
		Teal = 0x008080,
	
		/** Color constant for color *DarkCyan* (0x008b8b). */
		DarkCyan = 0x008b8b,
	
		/** Color constant for color *LightSeaGreen* (0x20b2aa). */
		LightSeaGreen = 0x20b2aa,
	
		/** Color constant for color *CadetBlue* (0x5f9ea0). */
		CadetBlue = 0x5f9ea0,
	
		/** Color constant for color *DarkTurquoise* (0x00ced1). */
		DarkTurquoise = 0x00ced1,
	
		/** Color constant for color *MediumTurquoise* (0x48d1cc). */
		MediumTurquoise = 0x48d1cc,
	
		/** Color constant for color *Turquoise* (0x40e0d0). */
		Turquoise = 0x40e0d0,
	
		/** Color constant for color *Aqua* (0x00ffff). */
		Aqua = 0x00ffff,
	
		/** Color constant for color *Cyan* (0x00ffff). */
		Cyan = 0x00ffff,
	
		/** Color constant for color *Aquamarine* (0x7fffd4). */
		Aquamarine = 0x7fffd4,
	
		/** Color constant for color *PaleTurquoise* (0xafeeee). */
		PaleTurquoise = 0xafeeee,
	
		/** Color constant for color *LightCyan* (0xe0ffff). */
		LightCyan = 0xe0ffff,
	
	 
		///////////////////////////
		/////// Blue colors ///////
		///////////////////////////
		
		/** Color constant for color *Navy* (0x000080). */
		Navy = 0x000080,
	
		/** Color constant for color *DarkBlue* (0x00008b). */
		DarkBlue = 0x00008b,
	
		/** Color constant for color *MediumBlue* (0x0000cd). */
		MediumBlue = 0x0000cd,
	
		/** Color constant for color *Blue* (0x0000ff). */
		Blue = 0x0000ff,
	
		/** Color constant for color *MidnightBlue* (0x191970). */
		MidnightBlue = 0x191970,
	
		/** Color constant for color *RoyalBlue* (0x4169e1). */
		RoyalBlue = 0x4169e1,
	
		/** Color constant for color *SteelBlue* (0x4682b4). */
		SteelBlue = 0x4682b4,
	
		/** Color constant for color *DodgerBlue* (0x1e90ff). */
		DodgerBlue = 0x1e90ff,
	
		/** Color constant for color *DeepSkyBlue* (0x00bfff). */
		DeepSkyBlue = 0x00bfff,
	
		/** Color constant for color *CornflowerBlue* (0x6495ed). */
		CornflowerBlue = 0x6495ed,
	
		/** Color constant for color *SkyBlue* (0x87ceeb). */
		SkyBlue = 0x87ceeb,
	
		/** Color constant for color *LightSkyBlue* (0x87cefa). */
		LightSkyBlue = 0x87cefa,
	
		/** Color constant for color *LightSteelBlue* (0xb0c4de). */
		LightSteelBlue = 0xb0c4de,
	
		/** Color constant for color *LightBlue* (0xadd8e6). */
		LightBlue = 0xadd8e6,
	
		/** Color constant for color *PowderBlue* (0xb0e0e6). */
		PowderBlue = 0xb0e0e6,
	
	 
		//////////////////////////////////////////////////
		/////// Purple, violet, and magenta colors ///////
		//////////////////////////////////////////////////
		
		/** Color constant for color *Indigo* (0x4b0082). */
		Indigo = 0x4b0082,
	
		/** Color constant for color *Purple* (0x800080). */
		Purple = 0x800080,
	
		/** Color constant for color *DarkMagenta* (0x8b008b). */
		DarkMagenta = 0x8b008b,
	
		/** Color constant for color *DarkViolet* (0x9400d3). */
		DarkViolet = 0x9400d3,
	
		/** Color constant for color *DarkSlateBlue* (0x483d8b). */
		DarkSlateBlue = 0x483d8b,
	
		/** Color constant for color *BlueViolet* (0x8a2be2). */
		BlueViolet = 0x8a2be2,
	
		/** Color constant for color *DarkOrchid* (0x9932cc). */
		DarkOrchid = 0x9932cc,
	
		/** Color constant for color *Fuchsia* (0xff00ff). */
		Fuchsia = 0xff00ff,
	
		/** Color constant for color *Magenta* (0xff00ff). */
		Magenta = 0xff00ff,
	
		/** Color constant for color *SlateBlue* (0x6a5acd). */
		SlateBlue = 0x6a5acd,
	
		/** Color constant for color *MediumSlateBlue* (0x7b68ee). */
		MediumSlateBlue = 0x7b68ee,
	
		/** Color constant for color *MediumOrchid* (0xba55d3). */
		MediumOrchid = 0xba55d3,
	
		/** Color constant for color *MediumPurple* (0x9370db). */
		MediumPurple = 0x9370db,
	
		/** Color constant for color *Orchid* (0xda70d6). */
		Orchid = 0xda70d6,
	
		/** Color constant for color *Violet* (0xee82ee). */
		Violet = 0xee82ee,
	
		/** Color constant for color *Plum* (0xdda0dd). */
		Plum = 0xdda0dd,
	
		/** Color constant for color *Thistle* (0xd8bfd8). */
		Thistle = 0xd8bfd8,
	
		/** Color constant for color *Lavender* (0xe6e6fa). */
		Lavender = 0xe6e6fa,
	
	 
		////////////////////////////
		/////// White colors ///////
		////////////////////////////
		
		/** Color constant for color *MistyRose* (0xffe4e1). */
		MistyRose = 0xffe4e1,
	
		/** Color constant for color *AntiqueWhite* (0xfaebd7). */
		AntiqueWhite = 0xfaebd7,
	
		/** Color constant for color *Linen* (0xfaf0e6). */
		Linen = 0xfaf0e6,
	
		/** Color constant for color *Beige* (0xf5f5dc). */
		Beige = 0xf5f5dc,
	
		/** Color constant for color *WhiteSmoke* (0xf5f5f5). */
		WhiteSmoke = 0xf5f5f5,
	
		/** Color constant for color *LavenderBlush* (0xfff0f5). */
		LavenderBlush = 0xfff0f5,
	
		/** Color constant for color *OldLace* (0xfdf5e6). */
		OldLace = 0xfdf5e6,
	
		/** Color constant for color *AliceBlue* (0xf0f8ff). */
		AliceBlue = 0xf0f8ff,
	
		/** Color constant for color *Seashell* (0xfff5ee). */
		Seashell = 0xfff5ee,
	
		/** Color constant for color *GhostWhite* (0xf8f8ff). */
		GhostWhite = 0xf8f8ff,
	
		/** Color constant for color *Honeydew* (0xf0fff0). */
		Honeydew = 0xf0fff0,
	
		/** Color constant for color *FloralWhite* (0xfffaf0). */
		FloralWhite = 0xfffaf0,
	
		/** Color constant for color *Azure* (0xf0ffff). */
		Azure = 0xf0ffff,
	
		/** Color constant for color *MintCream* (0xf5fffa). */
		MintCream = 0xf5fffa,
	
		/** Color constant for color *Snow* (0xfffafa). */
		Snow = 0xfffafa,
	
		/** Color constant for color *Ivory* (0xfffff0). */
		Ivory = 0xfffff0,
	
		/** Color constant for color *White* (0xffffff). */
		White = 0xffffff,
	
	 
		/////////////////////////////////////
		/////// Gray and black colors ///////
		/////////////////////////////////////
		
		/** Color constant for color *Black* (0x000000). */
		Black = 0x000000,
	
		/** Color constant for color *DarkSlateGray* (0x2f4f4f). */
		DarkSlateGray = 0x2f4f4f,
	
		/** Color constant for color *DimGray* (0x696969). */
		DimGray = 0x696969,
	
		/** Color constant for color *SlateGray* (0x708090). */
		SlateGray = 0x708090,
	
		/** Color constant for color *Gray* (0x808080). */
		Gray = 0x808080,
	
		/** Color constant for color *LightSlateGray* (0x778899). */
		LightSlateGray = 0x778899,
	
		/** Color constant for color *DarkGray* (0xa9a9a9). */
		DarkGray = 0xa9a9a9,
	
		/** Color constant for color *Silver* (0xc0c0c0). */
		Silver = 0xc0c0c0,
	
		/** Color constant for color *LightGray* (0xd3d3d3). */
		LightGray = 0xd3d3d3,
	
		/** Color constant for color *Gainsboro* (0xdcdcdc). */
		Gainsboro = 0xdcdcdc,
	
	};

} // end of namespace