/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#pragma once

#include <stdint.h>
#include "GamepadEnums.h"

#define GAMEPAD_BUTTON_COUNT 14

/*
	Gamepad button mapping table:

	+--------+--------+---------+----------+----------+--------+
	| MPG    | XInput | Switch  | PS3      | DInput   | Arcade |
	+--------+--------+---------+----------|----------+--------+
	| B1     | A      | B       | Cross    | 2        | K1     |
	| B2     | B      | A       | Circle   | 3        | K2     |
	| B3     | X      | Y       | Square   | 1        | P1     |
	| B4     | Y      | X       | Triangle | 4        | P2     |
	| L1     | LB     | L       | L1       | 5        | P4     |
	| R1     | RB     | R       | R1       | 6        | P3     |
	| L2     | LT     | ZL      | L2       | 7        | K4     |
	| R2     | RT     | ZR      | R2       | 8        | K3     |
	| S1     | Back   | -       | Select   | 9        | Coin   |
	| S2     | Start  | +       | Start    | 10       | Start  |
	| L3     | LS     | LS      | L3       | 11       | LS     |
	| R3     | RS     | RS      | R3       | 12       | RS     |
	| A1     | Guide  | Home    | -        | 13       | -      |
	| A2     | -      | Capture | -        | 14       | -      |
	+--------+--------+---------+----------+----------+--------+
*/

#define GAMEPAD_MASK_UP    (1U << 0)
#define GAMEPAD_MASK_DOWN  (1U << 1)
#define GAMEPAD_MASK_LEFT  (1U << 2)
#define GAMEPAD_MASK_RIGHT (1U << 3)

#define GAMEPAD_MASK_B1    (1U << 0)
#define GAMEPAD_MASK_B2    (1U << 1)
#define GAMEPAD_MASK_B3    (1U << 2)
#define GAMEPAD_MASK_B4    (1U << 3)
#define GAMEPAD_MASK_L1    (1U << 4)
#define GAMEPAD_MASK_R1    (1U << 5)
#define GAMEPAD_MASK_L2    (1U << 6)
#define GAMEPAD_MASK_R2    (1U << 7)
#define GAMEPAD_MASK_S1    (1U << 8)
#define GAMEPAD_MASK_S2    (1U << 9)
#define GAMEPAD_MASK_L3    (1U << 10)
#define GAMEPAD_MASK_R3    (1U << 11)
#define GAMEPAD_MASK_A1    (1U << 12)
#define GAMEPAD_MASK_A2    (1U << 13)

// For detecting dpad as buttons

#define GAMEPAD_MASK_DU    (1UL << 16)
#define GAMEPAD_MASK_DD    (1UL << 17)
#define GAMEPAD_MASK_DL    (1UL << 18)
#define GAMEPAD_MASK_DR    (1UL << 19)

// For detecting analog sticks as buttons

#define GAMEPAD_MASK_LX    (1UL << 20)
#define GAMEPAD_MASK_LY    (1UL << 21)
#define GAMEPAD_MASK_RX    (1UL << 22)
#define GAMEPAD_MASK_RY    (1UL << 23)

#define GAMEPAD_MASK_DPAD (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT)

#define GAMEPAD_JOYSTICK_MIN 0
#define GAMEPAD_JOYSTICK_MID 0x7FFF
#define GAMEPAD_JOYSTICK_MAX 0xFFFF

#define SOCD_INPUT_FLAG_UP     0
#define SOCD_INPUT_FLAG_DOWN   1
#define SOCD_INPUT_FLAG_LEFT   2
#define SOCD_INPUT_FLAG_RIGHT  3

const uint8_t dpadMasks[] =
{
	GAMEPAD_MASK_UP,
	GAMEPAD_MASK_DOWN,
	GAMEPAD_MASK_LEFT,
	GAMEPAD_MASK_RIGHT,
};

const uint16_t buttonMasks[] =
{
	GAMEPAD_MASK_B1,
	GAMEPAD_MASK_B2,
	GAMEPAD_MASK_B3,
	GAMEPAD_MASK_B4,
	GAMEPAD_MASK_L1,
	GAMEPAD_MASK_R1,
	GAMEPAD_MASK_L2,
	GAMEPAD_MASK_R2,
	GAMEPAD_MASK_S1,
	GAMEPAD_MASK_S2,
	GAMEPAD_MASK_L3,
	GAMEPAD_MASK_R3,
	GAMEPAD_MASK_A1,
	GAMEPAD_MASK_A2,
};

struct GamepadState
{
	uint8_t dpad {0};
	uint16_t buttons {0};
	uint16_t aux {0};
	uint16_t lx {GAMEPAD_JOYSTICK_MID};
	uint16_t ly {GAMEPAD_JOYSTICK_MID};
	uint16_t rx {GAMEPAD_JOYSTICK_MID};
	uint16_t ry {GAMEPAD_JOYSTICK_MID};
	uint8_t lt {0};
	uint8_t rt {0};
};

// Convert the horizontal GamepadState dpad axis value into an analog value
inline uint16_t dpadToAnalogX(uint8_t dpad)
{
	switch (dpad & (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT))
	{
		case GAMEPAD_MASK_LEFT:
			return GAMEPAD_JOYSTICK_MIN;

		case GAMEPAD_MASK_RIGHT:
			return GAMEPAD_JOYSTICK_MAX;

		default:
			return GAMEPAD_JOYSTICK_MID;
	}
}

// Convert the vertical GamepadState dpad axis value into an analog value
inline uint16_t dpadToAnalogY(uint8_t dpad)
{
	switch (dpad & (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN))
	{
		case GAMEPAD_MASK_UP:
			return GAMEPAD_JOYSTICK_MIN;

		case GAMEPAD_MASK_DOWN:
			return GAMEPAD_JOYSTICK_MAX;

		default:
			return GAMEPAD_JOYSTICK_MID;
	}
}

inline uint8_t socdLastInputCheck(uint8_t dpad,int *directionSetTime)
{
	uint8_t newDpad = 0;
	int maxvalue = 0;
	int maxindex = 0;
	int i;

	if((dpad & GAMEPAD_MASK_DPAD) != 0)
	{
		for(i=0 ; i<=3 ; i++)
		{
			if(maxvalue < directionSetTime[i])
			{
				maxvalue = directionSetTime[i];
				maxindex = i;
			}
		}
		newDpad = GAMEPAD_MASK_UP << maxindex;
	}
	else
	{
		newDpad = 0;
	}
	return newDpad;
}

inline void socdInputSet(int setDirection,int *directionSetTime)
{
	int maxvalue = 0;
	int maxindex = 0;
	int i;

		for(i=0 ; i<=3 ; i++)
		{
			if(maxvalue < directionSetTime[i])
			{
				maxvalue = directionSetTime[i];
				maxindex = i;
			}
		}
		if (maxvalue > 8)
		{
			if ((directionSetTime[SOCD_INPUT_FLAG_UP] != 1)   && (directionSetTime[SOCD_INPUT_FLAG_DOWN] != 1) &&
			    (directionSetTime[SOCD_INPUT_FLAG_LEFT] != 1) && (directionSetTime[SOCD_INPUT_FLAG_RIGHT] != 1))
			{
				for(i=0 ; i<=3 ; i++)
				{
					directionSetTime[i] = directionSetTime[i] - 1;
				}
			}
			else
			{
				for(i=0 ; i<=3 ; i++)
				{
					if(directionSetTime[i] > 5)
					{
						directionSetTime[i] = directionSetTime[i] - 1;
					}
				}
			}
			directionSetTime[setDirection] = maxvalue;
		}
		else
		{
			directionSetTime[setDirection] = maxvalue + 1;
		}
}

/**
 * @brief Run SOCD cleaning against a D-pad value.
 *
 * @param mode The SOCD cleaning mode.
 * @param dpad The GamepadState.dpad value.
 * @return uint8_t The clean D-pad value.
 */
inline uint8_t runSOCDCleaner(SOCDMode mode, uint8_t dpad)
{
	static DpadDirection lastUD = DIRECTION_NONE;
	static DpadDirection lastLR = DIRECTION_NONE;
	static uint8_t lastoutput = 0;
	static uint8_t lastinput  = 0;
	static int directionSetTime[4] = {0,0,0,0};

	uint8_t newDpad = 0;


	if (lastinput != (dpad & GAMEPAD_MASK_DPAD))
	{
		/* 4WAY JOYSTICK MODE */
		switch (mode)
		{
		case FOUR_MODE_SECOND_INPUT_PRIORITY: /* 4WAY LAST INPUT PRIORITY */
			if ((lastinput & GAMEPAD_MASK_UP) != (dpad & GAMEPAD_MASK_UP))	/* UP Change? */
			{
				if((dpad & GAMEPAD_MASK_UP) == GAMEPAD_MASK_UP)  /* UP TURN ON */
				{
					newDpad = GAMEPAD_MASK_UP;
					socdInputSet(SOCD_INPUT_FLAG_UP, directionSetTime);
				}
				else /* UP TURN OFF */
				{
					directionSetTime[SOCD_INPUT_FLAG_UP] = 0;
					if((lastoutput & GAMEPAD_MASK_UP) == GAMEPAD_MASK_UP)
					{
						newDpad = socdLastInputCheck(dpad, directionSetTime);
					}
					else
					{
						newDpad = lastoutput;
					}
				}
			}

			if ((lastinput & GAMEPAD_MASK_DOWN) != (dpad & GAMEPAD_MASK_DOWN))	/* DOWN Change? */
			{
				if((dpad & GAMEPAD_MASK_DOWN) == GAMEPAD_MASK_DOWN)
				{
					newDpad = GAMEPAD_MASK_DOWN;
					socdInputSet(SOCD_INPUT_FLAG_DOWN, directionSetTime);
				}
				else
				{
					directionSetTime[SOCD_INPUT_FLAG_DOWN] = 0;
					if((lastoutput & GAMEPAD_MASK_DOWN) == GAMEPAD_MASK_DOWN)
					{
						newDpad = socdLastInputCheck(dpad, directionSetTime);
					}
					else
					{
						newDpad = lastoutput;
					}
				}
			}
			if ((lastinput & GAMEPAD_MASK_LEFT) != (dpad & GAMEPAD_MASK_LEFT))	/* LEFT Change? */
			{
				if((dpad & GAMEPAD_MASK_LEFT) == GAMEPAD_MASK_LEFT)
				{
					newDpad = GAMEPAD_MASK_LEFT;
					socdInputSet(SOCD_INPUT_FLAG_LEFT, directionSetTime);

				}
				else
				{
					directionSetTime[SOCD_INPUT_FLAG_LEFT] = 0;
					if((lastoutput & GAMEPAD_MASK_LEFT) == GAMEPAD_MASK_LEFT)
					{
						newDpad = socdLastInputCheck(dpad, directionSetTime);
					}
					else
					{
						newDpad = lastoutput;
					}
				}
			}

			if ((lastinput & GAMEPAD_MASK_RIGHT) != (dpad & GAMEPAD_MASK_RIGHT))	/* RIGHT Change? */
			{
				if((dpad & GAMEPAD_MASK_RIGHT) == GAMEPAD_MASK_RIGHT)
				{
					newDpad = GAMEPAD_MASK_RIGHT;
					socdInputSet(SOCD_INPUT_FLAG_RIGHT, directionSetTime);
				}
				else
				{
					directionSetTime[SOCD_INPUT_FLAG_RIGHT] = 0;
					if((lastoutput & GAMEPAD_MASK_RIGHT) == GAMEPAD_MASK_RIGHT)
					{
						newDpad = socdLastInputCheck(dpad, directionSetTime);
					}
					else
					{
						newDpad = lastoutput;
					}
				}
			}
			break;
		
		case FOUR_MODE_Y_PRIORITY: /* 4WAY Y Priority */
			switch (dpad & (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT))
			{
				case GAMEPAD_MASK_LEFT:
					newDpad = GAMEPAD_MASK_LEFT;
					break;

				case GAMEPAD_MASK_RIGHT:
					newDpad = GAMEPAD_MASK_RIGHT;
					break;
			}
			switch (dpad & (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN))
			{
				case GAMEPAD_MASK_UP:
					newDpad = GAMEPAD_MASK_UP;
					break;

				case GAMEPAD_MASK_DOWN:
					newDpad = GAMEPAD_MASK_DOWN;
					break;
			}
			break;

		case FOUR_MODE_X_PRIORITY: /* 4WAY X Priority */
			switch (dpad & (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN))
			{
				case GAMEPAD_MASK_UP:
					newDpad = GAMEPAD_MASK_UP;
					break;

				case GAMEPAD_MASK_DOWN:
					newDpad = GAMEPAD_MASK_DOWN;
					break;
			}
			switch (dpad & (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT))
			{
				case GAMEPAD_MASK_LEFT:
					newDpad = GAMEPAD_MASK_LEFT;
					break;

				case GAMEPAD_MASK_RIGHT:
					newDpad = GAMEPAD_MASK_RIGHT;
					break;
			}
			break;

		default: /* Normal SOCD MODE */
			switch (dpad & (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN))
			{
				case (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN):
					if (mode == SOCD_MODE_UP_PRIORITY)
					{
						newDpad |= GAMEPAD_MASK_UP;
						lastUD = DIRECTION_UP;
					}
					else if (mode == SOCD_MODE_SECOND_INPUT_PRIORITY && lastUD != DIRECTION_NONE)
						newDpad |= (lastUD == DIRECTION_UP) ? GAMEPAD_MASK_DOWN : GAMEPAD_MASK_UP;
					else
						lastUD = DIRECTION_NONE;
					break;

				case GAMEPAD_MASK_UP:
					newDpad |= GAMEPAD_MASK_UP;
					lastUD = DIRECTION_UP;
					break;

				case GAMEPAD_MASK_DOWN:
					newDpad |= GAMEPAD_MASK_DOWN;
					lastUD = DIRECTION_DOWN;
					break;

				default:
					lastUD = DIRECTION_NONE;
					break;
			}
			switch (dpad & (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT))
			{
				case (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT):
					if (mode == SOCD_MODE_SECOND_INPUT_PRIORITY && lastLR != DIRECTION_NONE)
						newDpad |= (lastLR == DIRECTION_LEFT) ? GAMEPAD_MASK_RIGHT : GAMEPAD_MASK_LEFT;
					else
						lastLR = DIRECTION_NONE;
					break;

				case GAMEPAD_MASK_LEFT:
					newDpad |= GAMEPAD_MASK_LEFT;
					lastLR = DIRECTION_LEFT;
					break;

				case GAMEPAD_MASK_RIGHT:
					newDpad |= GAMEPAD_MASK_RIGHT;
					lastLR = DIRECTION_RIGHT;
					break;

				default:
					lastLR = DIRECTION_NONE;
					break;
			}
			break;
		}
	}
	else
	{
		newDpad = lastoutput;
	}

	lastinput  = (dpad & GAMEPAD_MASK_DPAD);
	lastoutput = newDpad;
	return newDpad;
}
