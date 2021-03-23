#pragma once

#include "luaimport.h"
#include "s5data.h"

void l_ua_init(lua_State* L);

class UnlimitedArmy {
public:
	int Player;
	std::vector<int> Entities;
	shok_position LastPos;
	int PosLastUpdatedTick = -1;;

	UnlimitedArmy(int p);
	~UnlimitedArmy();
	void CalculatePos();
	void Tick();

private:
	void CleanDead();
};
