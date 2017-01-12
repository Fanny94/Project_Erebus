#pragma once

enum PlayerStates
{
	NOSTATE = 0x0,
	IDLE = 0x1,
	RUNFORWARD = 0x2,
	RUNLEFT = 0x4,
	RUNRIGHT = 0x5,
	RUNBACK = 0x6,
	JUMP = 0x7,
	ATTACK = 0x8,
	CHARGE = 0x9,
};