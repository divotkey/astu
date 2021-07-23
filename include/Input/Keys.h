/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "InputMappingService.h"

namespace astu {

	/**
	 * This class defines common input controls which are called Keys for the
	 * sake of shortness.
	 * 
     * @ingroup input_group
	 */
    class Keys {
    public:

		/////////////////////////////////////////////////
		/////// Keyboard Keys
		/////////////////////////////////////////////////

		/** Constant for key 'A' */
		static const Key A;

		/** Constant for key 'B' */
		static const Key B;

		/** Constant for key 'C' */
		static const Key C;

		/** Constant for key 'D' */
		static const Key D;

		/** Constant for key 'E' */
		static const Key E;

		/** Constant for key 'F' */
		static const Key F;

		/** Constant for key 'G' */
		static const Key G;

		/** Constant for key 'H' */
		static const Key H;

		/** Constant for key 'I' */
		static const Key I;

		/** Constant for key 'J' */
		static const Key J;

		/** Constant for key 'K' */
		static const Key K;

		/** Constant for key 'L' */
		static const Key L;

		/** Constant for key 'M' */
		static const Key M;

		/** Constant for key 'N' */
		static const Key N;

		/** Constant for key 'O' */
		static const Key O;

		/** Constant for key 'P' */
		static const Key P;

		/** Constant for key 'Q' */
		static const Key Q;

		/** Constant for key 'R' */
		static const Key R;

		/** Constant for key 'S' */
		static const Key S;

		/** Constant for key 'T' */
		static const Key T;

		/** Constant for key 'U' */
		static const Key U;

		/** Constant for key 'V' */
		static const Key V;

		/** Constant for key 'W' */
		static const Key W;

		/** Constant for key 'X' */
		static const Key X;

		/** Constant for key 'Y' */
		static const Key Y;

		/** Constant for key 'Z' */
		static const Key Z;

		/** Constant for key 'One' */
		static const Key One;

		/** Constant for key 'Two' */
		static const Key Two;

		/** Constant for key 'Three' */
		static const Key Three;

		/** Constant for key 'Four' */
		static const Key Four;

		/** Constant for key 'Five' */
		static const Key Five;

		/** Constant for key 'Six' */
		static const Key Six;

		/** Constant for key 'Seven' */
		static const Key Seven;

		/** Constant for key 'Eight' */
		static const Key Eight;

		/** Constant for key 'Nine' */
		static const Key Nine;

		/** Constant for key 'Zero' */
		static const Key Zero;

		/** Constant for key 'Return' */
		static const Key Return;

		/** Constant for key 'Escape' */
		static const Key Escape;

		/** Constant for key 'Backspace' */
		static const Key Backspace;

		/** Constant for key 'Tab' */
		static const Key Tab;

		/** Constant for key 'SpaceBar' */
		static const Key SpaceBar;

		/** Constant for key 'F1' */
		static const Key F1;

		/** Constant for key 'F2' */
		static const Key F2;

		/** Constant for key 'F3' */
		static const Key F3;

		/** Constant for key 'F4' */
		static const Key F4;

		/** Constant for key 'F5' */
		static const Key F5;

		/** Constant for key 'F6' */
		static const Key F6;

		/** Constant for key 'F7' */
		static const Key F7;

		/** Constant for key 'F8' */
		static const Key F8;

		/** Constant for key 'F9' */
		static const Key F9;

		/** Constant for key 'F10' */
		static const Key F10;

		/** Constant for key 'F11' */
		static const Key F11;

		/** Constant for key 'F12' */
		static const Key F12;

		/** Constant for key 'Home' */
		static const Key Home;

		/** Constant for key 'PageUp' */
		static const Key PageUp;

		/** Constant for key 'Delete' */
		static const Key Delete;

		/** Constant for key 'End' */
		static const Key End;

		/** Constant for key 'PageDown' */
		static const Key PageDown;

		/** Constant for key 'Right' */
		static const Key Right;

		/** Constant for key 'Left' */
		static const Key Left;

		/** Constant for key 'Down' */
		static const Key Down;

		/** Constant for key 'Up' */
		static const Key Up;

		/** Constant for key 'LeftControl' */
		static const Key LeftControl;

		/** Constant for key 'LeftShift' */
		static const Key LeftShift;

		/** Constant for key 'LeftAlt' */
		static const Key LeftAlt;

		/** Constant for key 'LeftGui' */
		static const Key LeftGui;

		/** Constant for key 'RightControl' */
		static const Key RightControl;

		/** Constant for key 'RightShift' */
		static const Key RightShift;

		/** Constant for key 'RightAlt' */
		static const Key RightAlt;

		/** Constant for key 'RightGui' */
		static const Key RightGui;

		/////////////////////////////////////////////////
		/////// Special Keys
		/////////////////////////////////////////////////

		/** Constant for 'Unknown key' */
		static const Key Unknown;

		/** Constant for 'Gamepad Face Button Bottom' */
		static const Key GamepadFaceButtonBottom;

		/** Constant for 'Gamepad Face Button Top' */
		static const Key GamepadFaceButtonTop;

		/** Constant for 'Gamepad Face Button Right' */
		static const Key GamepadFaceButtonRight;

		/** Constant for 'Gamepad Face Button Left' */
		static const Key GamepadFaceButtonLeft;

		/** Constant for 'Gamepad Left Thumbstick X-Achis' */
		static const Key GamepadLeftThumbstickX;

		/** Constant for 'Gamepad Left Thumbstick Y-Achis' */
		static const Key GamepadLeftThumbstickY;

		/** Constant for 'Gamepad Right Thumbstick X-Achis' */
		static const Key GamepadRightThumbstickX;

		/** Constant for 'Gamepad Right Thumbstick Y-Achis' */
		static const Key GamepadRightThumbstickY;

		/** Constant for 'Gamepad Keft Shoulder' */
		static const Key GamepadLeftShoulder;

		/** Constant for 'Gamepad Right Shoulder' */
		static const Key GamepadRightShoulder;
	};

} // end of namespace
