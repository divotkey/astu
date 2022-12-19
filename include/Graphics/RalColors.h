/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

namespace astu {

    /**
     * This enumeration defines values for color names according to 
     * the RAL color standard.
     * 
     * @ingroup gfx_group
     */
	class RalColors {
    public:
    
		enum {

			/** Color constant for color *GreenBeige* (0xBEBD7F). */
			GreenBeige = 0xBEBD7F,
		
			/** Color constant for color *Beige* (0xC2B078). */
			Beige = 0xC2B078,
		
			/** Color constant for color *SandYellow* (0xC6A664). */
			SandYellow = 0xC6A664,
		
			/** Color constant for color *SignalYellow* (0xE5BE01). */
			SignalYellow = 0xE5BE01,
		
			/** Color constant for color *GoldenYellow* (0xCDA434). */
			GoldenYellow = 0xCDA434,
		
			/** Color constant for color *HoneyYellow* (0xA98307). */
			HoneyYellow = 0xA98307,
		
			/** Color constant for color *MaizeYellow* (0xE4A010). */
			MaizeYellow = 0xE4A010,
		
			/** Color constant for color *DaffodilYellow* (0xDC9D00). */
			DaffodilYellow = 0xDC9D00,
		
			/** Color constant for color *BrownBeige* (0x8A6642). */
			BrownBeige = 0x8A6642,
		
			/** Color constant for color *LemonYellow* (0xC7B446). */
			LemonYellow = 0xC7B446,
		
			/** Color constant for color *OysterWhite* (0xEAE6CA). */
			OysterWhite = 0xEAE6CA,
		
			/** Color constant for color *Ivory* (0xE1CC4F). */
			Ivory = 0xE1CC4F,
		
			/** Color constant for color *LightIvory* (0xE6D690). */
			LightIvory = 0xE6D690,
		
			/** Color constant for color *SulfurYellow* (0xEDFF21). */
			SulfurYellow = 0xEDFF21,
		
			/** Color constant for color *SaffronYellow* (0xF5D033). */
			SaffronYellow = 0xF5D033,
		
			/** Color constant for color *ZincYellow* (0xF8F32B). */
			ZincYellow = 0xF8F32B,
		
			/** Color constant for color *GreyBeige* (0x9E9764). */
			GreyBeige = 0x9E9764,
		
			/** Color constant for color *OliveYellow* (0x999950). */
			OliveYellow = 0x999950,
		
			/** Color constant for color *RapeYellow* (0xF3DA0B). */
			RapeYellow = 0xF3DA0B,
		
			/** Color constant for color *TrafficYellow* (0xFAD201). */
			TrafficYellow = 0xFAD201,
		
			/** Color constant for color *OchreYellow* (0xAEA04B). */
			OchreYellow = 0xAEA04B,
		
			/** Color constant for color *LuminousYellow* (0xFFFF00). */
			LuminousYellow = 0xFFFF00,
		
			/** Color constant for color *Curry* (0x9D9101). */
			Curry = 0x9D9101,
		
			/** Color constant for color *MelonYellow* (0xF4A900). */
			MelonYellow = 0xF4A900,
		
			/** Color constant for color *BroomYellow* (0xD6AE01). */
			BroomYellow = 0xD6AE01,
		
			/** Color constant for color *DahliaYellow* (0xF3A505). */
			DahliaYellow = 0xF3A505,
		
			/** Color constant for color *PastelYellow* (0xEFA94A). */
			PastelYellow = 0xEFA94A,
		
			/** Color constant for color *PearlBeige* (0x6A5D4D). */
			PearlBeige = 0x6A5D4D,
		
			/** Color constant for color *PearlGold* (0x705335). */
			PearlGold = 0x705335,
		
			/** Color constant for color *SunYellow* (0xF39F18). */
			SunYellow = 0xF39F18,
		
			/** Color constant for color *YellowOrange* (0xED760E). */
			YellowOrange = 0xED760E,
		
			/** Color constant for color *RedOrange* (0xC93C20). */
			RedOrange = 0xC93C20,
		
			/** Color constant for color *Vermilion* (0xCB2821). */
			Vermilion = 0xCB2821,
		
			/** Color constant for color *PastelOrange* (0xFF7514). */
			PastelOrange = 0xFF7514,
		
			/** Color constant for color *PureOrange* (0xF44611). */
			PureOrange = 0xF44611,
		
			/** Color constant for color *LuminousOrange* (0xFF2301). */
			LuminousOrange = 0xFF2301,
		
			/** Color constant for color *LuminousBrightOrange* (0xFFA420). */
			LuminousBrightOrange = 0xFFA420,
		
			/** Color constant for color *BrightRedOrange* (0xF75E25). */
			BrightRedOrange = 0xF75E25,
		
			/** Color constant for color *TrafficOrange* (0xF54021). */
			TrafficOrange = 0xF54021,
		
			/** Color constant for color *SignalOrange* (0xD84B20). */
			SignalOrange = 0xD84B20,
		
			/** Color constant for color *DeepOrange* (0xEC7C26). */
			DeepOrange = 0xEC7C26,
		
			/** Color constant for color *SalmonRange* (0xE55137). */
			SalmonRange = 0xE55137,
		
			/** Color constant for color *PearlOrange* (0xC35831). */
			PearlOrange = 0xC35831,
		
			/** Color constant for color *FlameRed* (0xAF2B1E). */
			FlameRed = 0xAF2B1E,
		
			/** Color constant for color *SignalRed* (0xA52019). */
			SignalRed = 0xA52019,
		
			/** Color constant for color *CarmineRed* (0xA2231D). */
			CarmineRed = 0xA2231D,
		
			/** Color constant for color *RubyRed* (0x9B111E). */
			RubyRed = 0x9B111E,
		
			/** Color constant for color *PurpleRed* (0x75151E). */
			PurpleRed = 0x75151E,
		
			/** Color constant for color *WineRed* (0x5E2129). */
			WineRed = 0x5E2129,
		
			/** Color constant for color *BlackRed* (0x412227). */
			BlackRed = 0x412227,
		
			/** Color constant for color *OxideRed* (0x642424). */
			OxideRed = 0x642424,
		
			/** Color constant for color *BrownRed* (0x781F19). */
			BrownRed = 0x781F19,
		
			/** Color constant for color *BeigeRed* (0xC1876B). */
			BeigeRed = 0xC1876B,
		
			/** Color constant for color *TomatoRed* (0xA12312). */
			TomatoRed = 0xA12312,
		
			/** Color constant for color *AntiquePink* (0xD36E70). */
			AntiquePink = 0xD36E70,
		
			/** Color constant for color *LightPink* (0xEA899A). */
			LightPink = 0xEA899A,
		
			/** Color constant for color *CoralRed* (0xB32821). */
			CoralRed = 0xB32821,
		
			/** Color constant for color *Rose* (0xE63244). */
			Rose = 0xE63244,
		
			/** Color constant for color *StrawberryRed* (0xD53032). */
			StrawberryRed = 0xD53032,
		
			/** Color constant for color *TrafficRed* (0xCC0605). */
			TrafficRed = 0xCC0605,
		
			/** Color constant for color *SalmonPink* (0xD95030). */
			SalmonPink = 0xD95030,
		
			/** Color constant for color *LuminousRed* (0xF80000). */
			LuminousRed = 0xF80000,
		
			/** Color constant for color *LuminousBrightRed* (0xFE0000). */
			LuminousBrightRed = 0xFE0000,
		
			/** Color constant for color *RaspberryRed* (0xC51D34). */
			RaspberryRed = 0xC51D34,
		
			/** Color constant for color *PureRed* (0xCB3234). */
			PureRed = 0xCB3234,
		
			/** Color constant for color *OrientRed* (0xB32428). */
			OrientRed = 0xB32428,
		
			/** Color constant for color *PearlRubyRed* (0x721422). */
			PearlRubyRed = 0x721422,
		
			/** Color constant for color *PearlPink* (0xB44C43). */
			PearlPink = 0xB44C43,
		
			/** Color constant for color *RedLilac* (0x6D3F5B). */
			RedLilac = 0x6D3F5B,
		
			/** Color constant for color *RedViolet* (0x922B3E). */
			RedViolet = 0x922B3E,
		
			/** Color constant for color *HeatherViolet* (0xDE4C8A). */
			HeatherViolet = 0xDE4C8A,
		
			/** Color constant for color *ClaretViolet* (0x641C34). */
			ClaretViolet = 0x641C34,
		
			/** Color constant for color *BlueLilac* (0x6C4675). */
			BlueLilac = 0x6C4675,
		
			/** Color constant for color *TrafficPurple* (0xA03472). */
			TrafficPurple = 0xA03472,
		
			/** Color constant for color *PurpleViolet* (0x4A192C). */
			PurpleViolet = 0x4A192C,
		
			/** Color constant for color *SignalViolet* (0x924E7D). */
			SignalViolet = 0x924E7D,
		
			/** Color constant for color *PastelViolet* (0xA18594). */
			PastelViolet = 0xA18594,
		
			/** Color constant for color *Telemagenta* (0xCF3476). */
			Telemagenta = 0xCF3476,
		
			/** Color constant for color *PearlViolet* (0x8673A1). */
			PearlViolet = 0x8673A1,
		
			/** Color constant for color *PearlBlackBerry* (0x6C6874). */
			PearlBlackBerry = 0x6C6874,
		
			/** Color constant for color *VioletBlue* (0x354D73). */
			VioletBlue = 0x354D73,
		
			/** Color constant for color *GreenBlue* (0x1F3438). */
			GreenBlue = 0x1F3438,
		
			/** Color constant for color *UltramarineBlue* (0x20214F). */
			UltramarineBlue = 0x20214F,
		
			/** Color constant for color *SaphireBlue* (0x1D1E33). */
			SaphireBlue = 0x1D1E33,
		
			/** Color constant for color *BlackBlue* (0x18171C). */
			BlackBlue = 0x18171C,
		
			/** Color constant for color *SignalBlue* (0x1E2460). */
			SignalBlue = 0x1E2460,
		
			/** Color constant for color *BrillantBlue* (0x3E5F8A). */
			BrillantBlue = 0x3E5F8A,
		
			/** Color constant for color *GreyBlue* (0x26252D). */
			GreyBlue = 0x26252D,
		
			/** Color constant for color *AzureBlue* (0x025669). */
			AzureBlue = 0x025669,
		
			/** Color constant for color *GentianBlue* (0x0E294B). */
			GentianBlue = 0x0E294B,
		
			/** Color constant for color *SteelBlue* (0x231A24). */
			SteelBlue = 0x231A24,
		
			/** Color constant for color *LightBlue* (0x3B83BD). */
			LightBlue = 0x3B83BD,
		
			/** Color constant for color *CobaltBlue* (0x1E213D). */
			CobaltBlue = 0x1E213D,
		
			/** Color constant for color *PigeonBlue* (0x606E8C). */
			PigeonBlue = 0x606E8C,
		
			/** Color constant for color *SkyBlue* (0x2271B3). */
			SkyBlue = 0x2271B3,
		
			/** Color constant for color *TrafficBlue* (0x063971). */
			TrafficBlue = 0x063971,
		
			/** Color constant for color *TurquoiseBlue* (0x3F888F). */
			TurquoiseBlue = 0x3F888F,
		
			/** Color constant for color *CapriBlue* (0x1B5583). */
			CapriBlue = 0x1B5583,
		
			/** Color constant for color *OceanBlue* (0x1D334A). */
			OceanBlue = 0x1D334A,
		
			/** Color constant for color *WaterBlue* (0x256D7B). */
			WaterBlue = 0x256D7B,
		
			/** Color constant for color *NightBlue* (0x252850). */
			NightBlue = 0x252850,
		
			/** Color constant for color *DistantBlue* (0x49678D). */
			DistantBlue = 0x49678D,
		
			/** Color constant for color *PastelBlue* (0x5D9B9B). */
			PastelBlue = 0x5D9B9B,
		
			/** Color constant for color *PearlGentianBlue* (0x2A6478). */
			PearlGentianBlue = 0x2A6478,
		
			/** Color constant for color *PearlNightBlue* (0x102C54). */
			PearlNightBlue = 0x102C54,
		
			/** Color constant for color *PatinaGreen* (0x316650). */
			PatinaGreen = 0x316650,
		
			/** Color constant for color *EmeraldGreen* (0x287233). */
			EmeraldGreen = 0x287233,
		
			/** Color constant for color *LeafGreen* (0x2D572C). */
			LeafGreen = 0x2D572C,
		
			/** Color constant for color *OliveGreen* (0x424632). */
			OliveGreen = 0x424632,
		
			/** Color constant for color *BlueGreen* (0x1F3A3D). */
			BlueGreen = 0x1F3A3D,
		
			/** Color constant for color *MossGreen* (0x2F4538). */
			MossGreen = 0x2F4538,
		
			/** Color constant for color *GreyOlive* (0x3E3B32). */
			GreyOlive = 0x3E3B32,
		
			/** Color constant for color *BottleGreen* (0x343B29). */
			BottleGreen = 0x343B29,
		
			/** Color constant for color *BrownGreen* (0x39352A). */
			BrownGreen = 0x39352A,
		
			/** Color constant for color *FirGreen* (0x31372B). */
			FirGreen = 0x31372B,
		
			/** Color constant for color *GrassGreen* (0x35682D). */
			GrassGreen = 0x35682D,
		
			/** Color constant for color *ResedaGreen* (0x587246). */
			ResedaGreen = 0x587246,
		
			/** Color constant for color *BlackGreen* (0x343E40). */
			BlackGreen = 0x343E40,
		
			/** Color constant for color *ReedGreen* (0x6C7156). */
			ReedGreen = 0x6C7156,
		
			/** Color constant for color *YellowOlive* (0x47402E). */
			YellowOlive = 0x47402E,
		
			/** Color constant for color *BlackOlive* (0x3B3C36). */
			BlackOlive = 0x3B3C36,
		
			/** Color constant for color *TurquoiseGreen* (0x1E5945). */
			TurquoiseGreen = 0x1E5945,
		
			/** Color constant for color *MayGreen* (0x4C9141). */
			MayGreen = 0x4C9141,
		
			/** Color constant for color *YellowGreen* (0x57A639). */
			YellowGreen = 0x57A639,
		
			/** Color constant for color *PastelGreen* (0xBDECB6). */
			PastelGreen = 0xBDECB6,
		
			/** Color constant for color *ChromeGreen* (0x2E3A23). */
			ChromeGreen = 0x2E3A23,
		
			/** Color constant for color *PaleGreen* (0x89AC76). */
			PaleGreen = 0x89AC76,
		
			/** Color constant for color *OliveDrab* (0x25221B). */
			OliveDrab = 0x25221B,
		
			/** Color constant for color *TrafficGreen* (0x308446). */
			TrafficGreen = 0x308446,
		
			/** Color constant for color *FernGreen* (0x3D642D). */
			FernGreen = 0x3D642D,
		
			/** Color constant for color *OpalGreen* (0x015D52). */
			OpalGreen = 0x015D52,
		
			/** Color constant for color *LightGreen* (0x84C3BE). */
			LightGreen = 0x84C3BE,
		
			/** Color constant for color *PineGreen* (0x2C5545). */
			PineGreen = 0x2C5545,
		
			/** Color constant for color *MintGreen* (0x20603D). */
			MintGreen = 0x20603D,
		
			/** Color constant for color *SignalGreen* (0x317F43). */
			SignalGreen = 0x317F43,
		
			/** Color constant for color *MintTurquoise* (0x497E76). */
			MintTurquoise = 0x497E76,
		
			/** Color constant for color *PastelTurquoise* (0x7FB5B5). */
			PastelTurquoise = 0x7FB5B5,
		
			/** Color constant for color *PearlGreen* (0x1C542D). */
			PearlGreen = 0x1C542D,
		
			/** Color constant for color *PearlOpalGreen* (0x193737). */
			PearlOpalGreen = 0x193737,
		
			/** Color constant for color *PureGreen* (0x008F39). */
			PureGreen = 0x008F39,
		
			/** Color constant for color *LuminousGreen* (0x00BB2D). */
			LuminousGreen = 0x00BB2D,
		
			/** Color constant for color *SquirrelGrey* (0x78858B). */
			SquirrelGrey = 0x78858B,
		
			/** Color constant for color *SilverGrey* (0x8A9597). */
			SilverGrey = 0x8A9597,
		
			/** Color constant for color *OliveGrey* (0x7E7B52). */
			OliveGrey = 0x7E7B52,
		
			/** Color constant for color *MossGrey* (0x6C7059). */
			MossGrey = 0x6C7059,
		
			/** Color constant for color *SignalGrey* (0x969992). */
			SignalGrey = 0x969992,
		
			/** Color constant for color *MouseGrey* (0x646B63). */
			MouseGrey = 0x646B63,
		
			/** Color constant for color *BeigeGrey* (0x6D6552). */
			BeigeGrey = 0x6D6552,
		
			/** Color constant for color *KhakiGrey* (0x6A5F31). */
			KhakiGrey = 0x6A5F31,
		
			/** Color constant for color *GreenGrey* (0x4D5645). */
			GreenGrey = 0x4D5645,
		
			/** Color constant for color *TarpaulinGrey* (0x4C514A). */
			TarpaulinGrey = 0x4C514A,
		
			/** Color constant for color *IronGrey* (0x434B4D). */
			IronGrey = 0x434B4D,
		
			/** Color constant for color *BasaltGrey* (0x4E5754). */
			BasaltGrey = 0x4E5754,
		
			/** Color constant for color *BrownGrey* (0x464531). */
			BrownGrey = 0x464531,
		
			/** Color constant for color *SlateGrey* (0x434750). */
			SlateGrey = 0x434750,
		
			/** Color constant for color *AnthraciteGrey* (0x293133). */
			AnthraciteGrey = 0x293133,
		
			/** Color constant for color *BlackGrey* (0x23282B). */
			BlackGrey = 0x23282B,
		
			/** Color constant for color *UmbraGrey* (0x332F2C). */
			UmbraGrey = 0x332F2C,
		
			/** Color constant for color *ConcreteGrey* (0x686C5E). */
			ConcreteGrey = 0x686C5E,
		
			/** Color constant for color *GraphiteGrey* (0x474A51). */
			GraphiteGrey = 0x474A51,
		
			/** Color constant for color *GraniteGrey* (0x2F353B). */
			GraniteGrey = 0x2F353B,
		
			/** Color constant for color *StoneGrey* (0x8B8C7A). */
			StoneGrey = 0x8B8C7A,
		
			/** Color constant for color *BlueGrey* (0x474B4E). */
			BlueGrey = 0x474B4E,
		
			/** Color constant for color *PebbleGrey* (0xB8B799). */
			PebbleGrey = 0xB8B799,
		
			/** Color constant for color *CementGrey* (0x7D8471). */
			CementGrey = 0x7D8471,
		
			/** Color constant for color *YellowGrey* (0x8F8B66). */
			YellowGrey = 0x8F8B66,
		
			/** Color constant for color *LightGrey* (0xCBD0CC). */
			LightGrey = 0xCBD0CC,
		
			/** Color constant for color *PlatinumGrey* (0x7F7679). */
			PlatinumGrey = 0x7F7679,
		
			/** Color constant for color *DustyGrey* (0x7D7F7D). */
			DustyGrey = 0x7D7F7D,
		
			/** Color constant for color *AgateGrey* (0xB5B8B1). */
			AgateGrey = 0xB5B8B1,
		
			/** Color constant for color *QuartzGrey* (0x6C6960). */
			QuartzGrey = 0x6C6960,
		
			/** Color constant for color *WindowGrey* (0x9DA1AA). */
			WindowGrey = 0x9DA1AA,
		
			/** Color constant for color *TrafficGreyA* (0x8D948D). */
			TrafficGreyA = 0x8D948D,
		
			/** Color constant for color *TrafficGreyB* (0x4E5452). */
			TrafficGreyB = 0x4E5452,
		
			/** Color constant for color *SilkGrey* (0xCAC4B0). */
			SilkGrey = 0xCAC4B0,
		
			/** Color constant for color *Telegrey1* (0x909090). */
			Telegrey1 = 0x909090,
		
			/** Color constant for color *Telegrey2* (0x82898F). */
			Telegrey2 = 0x82898F,
		
			/** Color constant for color *Telegrey4* (0xD0D0D0). */
			Telegrey4 = 0xD0D0D0,
		
			/** Color constant for color *PearlMouseGrey* (0x898176). */
			PearlMouseGrey = 0x898176,
		
			/** Color constant for color *GreenBrown* (0x826C34). */
			GreenBrown = 0x826C34,
		
			/** Color constant for color *OchreBrown* (0x955F20). */
			OchreBrown = 0x955F20,
		
			/** Color constant for color *SignalBrown* (0x6C3B2A). */
			SignalBrown = 0x6C3B2A,
		
			/** Color constant for color *ClayBrown* (0x734222). */
			ClayBrown = 0x734222,
		
			/** Color constant for color *CopperBrown* (0x8E402A). */
			CopperBrown = 0x8E402A,
		
			/** Color constant for color *FawnBrown* (0x59351F). */
			FawnBrown = 0x59351F,
		
			/** Color constant for color *OliveBrown* (0x6F4F28). */
			OliveBrown = 0x6F4F28,
		
			/** Color constant for color *NutBrown* (0x5B3A29). */
			NutBrown = 0x5B3A29,
		
			/** Color constant for color *RedBrown* (0x592321). */
			RedBrown = 0x592321,
		
			/** Color constant for color *SepiaBrown* (0x382C1E). */
			SepiaBrown = 0x382C1E,
		
			/** Color constant for color *ChestnutBrown* (0x633A34). */
			ChestnutBrown = 0x633A34,
		
			/** Color constant for color *MahoganyBrown* (0x4C2F27). */
			MahoganyBrown = 0x4C2F27,
		
			/** Color constant for color *ChocolateBrown* (0x45322E). */
			ChocolateBrown = 0x45322E,
		
			/** Color constant for color *GreyBrown* (0x403A3A). */
			GreyBrown = 0x403A3A,
		
			/** Color constant for color *BlackBrown* (0x212121). */
			BlackBrown = 0x212121,
		
			/** Color constant for color *OrangeBrown* (0xA65E2E). */
			OrangeBrown = 0xA65E2E,
		
			/** Color constant for color *BeigeBrown* (0x79553D). */
			BeigeBrown = 0x79553D,
		
			/** Color constant for color *PaleBrown* (0x755C48). */
			PaleBrown = 0x755C48,
		
			/** Color constant for color *TerraBrown* (0x4E3B31). */
			TerraBrown = 0x4E3B31,
		
			/** Color constant for color *PearlCopper* (0x763C28). */
			PearlCopper = 0x763C28,
		
			/** Color constant for color *Cream* (0xFDF4E3). */
			Cream = 0xFDF4E3,
		
			/** Color constant for color *GreyWhite* (0xE7EBDA). */
			GreyWhite = 0xE7EBDA,
		
			/** Color constant for color *SignalWhite* (0xF4F4F4). */
			SignalWhite = 0xF4F4F4,
		
			/** Color constant for color *SignalBlack* (0x282828). */
			SignalBlack = 0x282828,
		
			/** Color constant for color *JetBlack* (0x0A0A0A). */
			JetBlack = 0x0A0A0A,
		
			/** Color constant for color *WhiteAluminium* (0xA5A5A5). */
			WhiteAluminium = 0xA5A5A5,
		
			/** Color constant for color *GreyAluminium* (0x8F8F8F). */
			GreyAluminium = 0x8F8F8F,
		
			/** Color constant for color *PureWhite* (0xFFFFFF). */
			PureWhite = 0xFFFFFF,
		
			/** Color constant for color *GraphiteBlack* (0x1C1C1C). */
			GraphiteBlack = 0x1C1C1C,
		
			/** Color constant for color *TrafficWhite* (0xF6F6F6). */
			TrafficWhite = 0xF6F6F6,
		
			/** Color constant for color *TrafficBlack* (0x1E1E1E). */
			TrafficBlack = 0x1E1E1E,
		
			/** Color constant for color *PapyrusWhite* (0xCFD3CD). */
			PapyrusWhite = 0xCFD3CD,
		
			/** Color constant for color *PearlLightGrey* (0x9C9C9C). */
			PearlLightGrey = 0x9C9C9C,
		
			/** Color constant for color *PearlDarkGrey* (0x828282). */
			PearlDarkGrey = 0x828282,
		
		};
	};

} // end of namespace