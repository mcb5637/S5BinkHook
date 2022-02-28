#include "pch.h"
#include "s5data.h"

void EScr::CScriptTriggerSystem::RunTrigger(BB::CEvent* ev)
{
	PostEvent(ev);
}

