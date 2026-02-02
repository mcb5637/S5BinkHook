function InitExtraPlayers()
	local max = CppLogic.ModLoader.GetMaxPlayers()
	for i = 9, max do
		Score.Player[i] = {}
		Score.Player[i]["all"] = 0
		Score.Player[i]["resources"] = 0
		Score.Player[i]["buildings"] = 0
		Score.Player[i]["technology"] = 0
		Score.Player[i]["settlers"] = 0
		Score.Player[i]["battle"] = 0
	end
end

Trigger.RequestTrigger(Events.CPPLOGIC_EVENT_ON_MAP_STARTED, nil, "InitExtraPlayers", 1)
