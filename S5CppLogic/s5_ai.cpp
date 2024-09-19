#include "pch.h"
#include "s5_ai.h"

inline GAI::CPlayer* (__thiscall* const aiholder_getplayer)(GAI::AIHolder* th, shok::PlayerId i) = reinterpret_cast<GAI::CPlayer * (__thiscall*)(GAI::AIHolder*, shok::PlayerId)>(0x444D0C);
GAI::CPlayer* GAI::AIHolder::GetPlayer(shok::PlayerId i)
{
	return aiholder_getplayer(this, i);
}
