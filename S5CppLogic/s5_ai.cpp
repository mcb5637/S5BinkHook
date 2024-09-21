#include "pch.h"
#include "s5_ai.h"

inline GAI::CPlayer* (__thiscall* const aiholder_getplayer)(GAI::AIHolder* th, shok::PlayerId i) = reinterpret_cast<GAI::CPlayer * (__thiscall*)(GAI::AIHolder*, shok::PlayerId)>(0x444D0C);
GAI::CPlayer* GAI::AIHolder::GetPlayer(shok::PlayerId i)
{
	return aiholder_getplayer(this, i);
}

inline GAI::CEntity* (__thiscall* const aiholder_getentity)(GAI::AIHolder* th, shok::EntityId i) = reinterpret_cast<GAI::CEntity * (__thiscall*)(GAI::AIHolder*, shok::EntityId)>(0x444D32);
GAI::CEntity* GAI::AIHolder::GetEntity(shok::EntityId id)
{
	return aiholder_getentity(this, id);
}
