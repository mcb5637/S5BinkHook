if not AutoScroll then
	Script.Load("Data\\Script\\InterfaceTools\\AutoScroll.lua")
end

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

	ExtraPlayers.GUIUpdate_MinimapInDiplomacyMenuOrig = GUIUpdate_MinimapInDiplomacyMenu
	GUIUpdate_MinimapInDiplomacyMenu = ExtraPlayers.GUIUpdate_MinimapInDiplomacyMenu

	ExtraPlayers_InitUI()
end

ExtraPlayers = {}

---@type CPPLAutoScroll<number>
ExtraPlayers.DiplomacyPlayerScroll = AutoScroll.Init("DiplomacyWindowScroll", nil, nil, nil, true)

function ExtraPlayers.GUIUpdate_MinimapInDiplomacyMenu()
	ExtraPlayers.GUIUpdate_MinimapInDiplomacyMenuOrig()
	ExtraPlayers.InitDiplomacyWindow()
end

function ExtraPlayers.InitDiplomacyWindow()
	local pids = {}
	for i = 1, Logic.GetMaximumNumberOfPlayer() do
		if CppLogic.Logic.GetPlayerName(i) ~= "" or i == GUI.GetPlayerID() then
			table.insert(pids, i)
		end
	end
	ExtraPlayers.DiplomacyPlayerScroll:SetDataToScrollOver(pids, true, true)
end

---@type table<number, table<string, number>>
ExtraPlayers.WidgetIdCache = {}
---@param containerwid number
---@param name string
---@param extra nil|fun(n:string):boolean
function ExtraPlayers.GetWidgetId(containerwid, name, extra)
	local c = ExtraPlayers.WidgetIdCache[containerwid]
	if not c then
		c = {}
		ExtraPlayers.WidgetIdCache[containerwid] = c
	end
	if c[name] then
		return c[name]
	end
	if not extra then
		extra = function(_)
			return true
		end
	end
	for _, id in CppLogic.UI.ContainerWidgetGetAllChildren(containerwid) do
		local n = CppLogic.UI.GetWidgetName(id)
		if string.find(n, name, nil, true) and extra(n) then
			c[name] = id
			return id
		end
	end
end

---@type table<number, number>
ExtraPlayers.PlayerToResDonationType = {}

function ExtraPlayers.GUIUpdate_DiplomacyPlayerName()
	---@type number?
	local sel = ExtraPlayers.DiplomacyPlayerScroll:GetElementOf(XGUIEng.GetCurrentWidgetID(), 1)
	if not sel then
		return
	end
	local name = CppLogic.Logic.GetPlayerName(sel)
	if name == "" and sel == GUI.GetPlayerID() then
		name = UserTool_GetPlayerName(sel)
	end
	XGUIEng.SetText(XGUIEng.GetCurrentWidgetID(), name)

	local container = XGUIEng.GetWidgetsMotherID(XGUIEng.GetCurrentWidgetID())

	local diplowid = ExtraPlayers.GetWidgetId(container, "OpponentState")
	local colorwid = ExtraPlayers.GetWidgetId(container, "Color", function(n)
		return not string.find(n, "ColorFrame", nil, true)
	end)
	local resconfwid = ExtraPlayers.GetWidgetId(container, "ResourceSend")
	local resamountwid = ExtraPlayers.GetWidgetId(container, "ResourceAmount")
	local restypewid = ExtraPlayers.GetWidgetId(container, "ResourceName")

	local diplo = Logic.GetDiplomacyState(sel, GUI.GetPlayerID())
	if sel == GUI.GetPlayerID() then
		XGUIEng.ShowWidget(diplowid, 0)
	else
		XGUIEng.ShowWidget(diplowid, 1)
		local source = "data\\graphics\\textures\\gui\\window_status_neutral.png"
		if diplo == Diplomacy.Friendly then
			source = "data\\graphics\\textures\\gui\\window_status_friend.png"
		elseif diplo == Diplomacy.Hostile then
			source = "data\\graphics\\textures\\gui\\window_status_hostile.png"
		end
		XGUIEng.SetMaterialTexture(diplowid, 0, source)
	end

	local r, g, b = GUI.GetPlayerColor(sel)
	XGUIEng.SetMaterialColor(colorwid, 0, r, g, b, 255)

	if not ExtraPlayers.ShowResDonation(sel) then
		XGUIEng.ShowWidget(restypewid, 0)
		XGUIEng.ShowWidget(resamountwid, 0)
		XGUIEng.ShowWidget(resconfwid, 0)
	else
		XGUIEng.ShowWidget(restypewid, 1)
		XGUIEng.ShowWidget(resamountwid, 1)
		XGUIEng.ShowWidget(resconfwid, 1)
	end
end

---@diagnostic disable-next-line: duplicate-set-field
function ExtraPlayers.ShowResDonation(p)
	return true
end

function ExtraPlayers.GUIAction_DiplomacyResType()
	---@type number?
	local sel = ExtraPlayers.DiplomacyPlayerScroll:GetElementOf(XGUIEng.GetCurrentWidgetID(), 1)
	if not sel then
		return
	end
	local rt = ExtraPlayers.PlayerToResDonationType[sel] or ResourceType.Gold
	if rt <= ResourceType.Gold then
		rt = ResourceType.Stone
	elseif rt <= ResourceType.Stone then
		rt = ResourceType.Iron
	elseif rt <= ResourceType.Iron then
		rt = ResourceType.Sulfur
	elseif rt <= ResourceType.Sulfur then
		rt = ResourceType.Clay
	elseif rt <= ResourceType.Clay then
		rt = ResourceType.Wood
	else
		rt = ResourceType.Gold
	end
	ExtraPlayers.PlayerToResDonationType[sel] = rt
end

function ExtraPlayers.GUIUpdate_DiplomacyResType()
	---@type number?
	local sel = ExtraPlayers.DiplomacyPlayerScroll:GetElementOf(XGUIEng.GetCurrentWidgetID(), 1)
	if not sel then
		return
	end
	local rt = ExtraPlayers.PlayerToResDonationType[sel] or ResourceType.Gold
	local name = ""
	if rt == ResourceType.Gold then
		name = "InGameMessages/GUI_NameMoney"
	elseif rt == ResourceType.Stone then
		name = "InGameMessages/GUI_NameStone"
	elseif rt == ResourceType.Iron then
		name = "InGameMessages/GUI_NameIron"
	elseif rt == ResourceType.Sulfur then
		name = "InGameMessages/GUI_NameSulfur"
	elseif rt == ResourceType.Clay then
		name = "InGameMessages/GUI_NameClay"
	else
		name = "InGameMessages/GUI_NameWood"
	end
	name = XGUIEng.GetStringTableText(name)
	XGUIEng.SetText(XGUIEng.GetCurrentWidgetID(), name)
end

function ExtraPlayers.GUIAction_DiploSendRes()
	---@type number?
	local sel = ExtraPlayers.DiplomacyPlayerScroll:GetElementOf(XGUIEng.GetCurrentWidgetID(), 1)
	if not sel then
		return
	end
	local wid = ExtraPlayers.GetWidgetId(XGUIEng.GetWidgetsMotherID(XGUIEng.GetCurrentWidgetID()), "ResourceAmount")
	local text = CppLogic.UI.TextInputCustomWidgetGetText(wid)
	local num = tonumber(text)
	if not num then
		return
	end
	local rt = ExtraPlayers.PlayerToResDonationType[sel] or ResourceType.Gold
	CppLogic.UI.Commands.Player_DonateResources(sel, rt, num)
end

function ExtraPlayers_InitUI()
	ExtraPlayers.WidgetIdCache = {}

	local function rem(w)
		if CppLogic.UI.IsContainerWidget(w) then
			for _, c in ipairs(CppLogic.UI.ContainerWidgetGetAllChildren(w)) do
				rem(c)
			end
		end
		CppLogic.UI.RemoveWidget(w)
	end


	for i = 1, 8 do
		rem("DiplomacyWindowPlayer0" .. i)
	end
	rem("DiplomacyWindowController")

	assert(XGUIEng.GetWidgetID("DiplomacyWindowPlayers")==0, "DiplomacyWindowPlayers already exists")
	assert(XGUIEng.GetWidgetID("DiplomacyWindowSelectors")==0, "DiplomacyWindowSelectors already exists")
	assert(XGUIEng.GetWidgetID("DiplomacyWindowPlayer")==0, "DiplomacyWindowPlayer already exists")
	assert(XGUIEng.GetWidgetID("DiplomacyWindowPlayerColorFrame")==0, "DiplomacyWindowPlayerColorFrame already exists")
	assert(XGUIEng.GetWidgetID("DiplomacyWindowPlayerName")==0, "DiplomacyWindowPlayerName already exists")
	assert(XGUIEng.GetWidgetID("DiplomacyWindowPlayerSetAlly")==0, "DiplomacyWindowPlayerSetAlly already exists")
	assert(XGUIEng.GetWidgetID("DiplomacyWindowPlayerSetNeutral")==0, "DiplomacyWindowPlayerSetNeutral already exists")
	assert(XGUIEng.GetWidgetID("DiplomacyWindowPlayerSetHostile")==0, "DiplomacyWindowPlayerSetHostile already exists")
	assert(XGUIEng.GetWidgetID("DiplomacyWindowPlayerMPResourceAmount")==0, "DiplomacyWindowPlayerMPResourceAmount already exists")
	assert(XGUIEng.GetWidgetID("DiplomacyWindowPlayerMPResourceName")==0, "DiplomacyWindowPlayerMPResourceName already exists")
	assert(XGUIEng.GetWidgetID("DiplomacyWindowPlayerMPResourceSend")==0, "DiplomacyWindowPlayerMPResourceSend already exists")
	assert(XGUIEng.GetWidgetID("DiplomacyWindowPlayerColor")==0, "DiplomacyWindowPlayerColor already exists")
	assert(XGUIEng.GetWidgetID("DiplomacyWindowPlayerOpponentState")==0, "DiplomacyWindowPlayerOpponentState already exists")
	assert(XGUIEng.GetWidgetID("DiplomacyWindowScroll")==0, "DiplomacyWindowScroll already exists")
	CppLogic.UI.ContainerWidgetCreateContainerWidgetChild("DiplomacyWindow", "DiplomacyWindowPlayers", "DiplomacyStatusFriendly")
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowPlayers", 0, 0, 900, 414)
	XGUIEng.ShowWidget("DiplomacyWindowPlayers", 1)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowPlayers", 0, false, false)
	CppLogic.UI.ContainerWidgetCreateContainerWidgetChild("DiplomacyWindowPlayers", "DiplomacyWindowSelectors", nil)
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowSelectors", 56.5, 16, 785, 340)
	XGUIEng.ShowWidget("DiplomacyWindowSelectors", 1)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowSelectors", 0, false, false)
	CppLogic.UI.ContainerWidgetCreateContainerWidgetChild("DiplomacyWindowSelectors", "DiplomacyWindowPlayer", nil)
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowPlayer", 0, 0, 785, 42)
	XGUIEng.ShowWidget("DiplomacyWindowPlayer", 1)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowPlayer", 0, false, false)
	CppLogic.UI.ContainerWidgetCreateStaticWidgetChild("DiplomacyWindowPlayer", "DiplomacyWindowPlayerColorFrame", nil)
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowPlayerColorFrame", 304, 5, 32, 32)
	XGUIEng.ShowWidget("DiplomacyWindowPlayerColorFrame", 1)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowPlayerColorFrame", 10, false, false)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerColorFrame", 0, 0, 0, 1, 1)
	XGUIEng.SetMaterialTexture("DiplomacyWindowPlayerColorFrame", 0, "data\\graphics\\textures\\gui\\window_bg32x32.png")
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerColorFrame", 0, 255, 255, 255, 255)
	CppLogic.UI.ContainerWidgetCreateStaticTextWidgetChild("DiplomacyWindowPlayer", "DiplomacyWindowPlayerName", nil)
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowPlayerName", 3, 5, 258.5, 32)
	XGUIEng.ShowWidget("DiplomacyWindowPlayerName", 1)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowPlayerName", 0, false, false)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerName", 0, 0, 0, 1, 1)
	XGUIEng.SetMaterialTexture("DiplomacyWindowPlayerName", 0, "data\\graphics\\textures\\gui\\window_bg258x32.png")
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerName", 0, 0, 0, 0, 128)
	CppLogic.UI.WidgetSetFont("DiplomacyWindowPlayerName", "data\\menu\\fonts\\medium11bold.met")
	CppLogic.UI.WidgetSetStringFrameDistance("DiplomacyWindowPlayerName", 8)
	XGUIEng.SetText("DiplomacyWindowPlayerName", "", 1)
	XGUIEng.SetTextColor("DiplomacyWindowPlayerName", 255, 255, 255, 255)
	CppLogic.UI.WidgetSetUpdateManualFlag("DiplomacyWindowPlayerName", false)
	CppLogic.UI.WidgetOverrideUpdateFunc("DiplomacyWindowPlayerName", function() ExtraPlayers.GUIUpdate_DiplomacyPlayerName() end)
	XGUIEng.SetLinesToPrint("DiplomacyWindowPlayerName", 0, 0)
	CppLogic.UI.StaticTextWidgetSetLineDistanceFactor("DiplomacyWindowPlayerName", 0)
	CppLogic.UI.ContainerWidgetCreateGFXButtonWidgetChild("DiplomacyWindowPlayer", "DiplomacyWindowPlayerSetAlly", nil)
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowPlayerSetAlly", 395.25, 4, 35, 35)
	XGUIEng.ShowWidget("DiplomacyWindowPlayerSetAlly", 0)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowPlayerSetAlly", 0, false, false)
	XGUIEng.DisableButton("DiplomacyWindowPlayerSetAlly", 0)
	XGUIEng.HighLightButton("DiplomacyWindowPlayerSetAlly", 0)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetAlly", 0, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetAlly", 0, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetAlly", 1, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetAlly", 1, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetAlly", 2, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetAlly", 2, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetAlly", 3, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetAlly", 3, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetAlly", 4, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetAlly", 4, 255, 255, 255, 255)
	CppLogic.UI.WidgetSetTooltipData("DiplomacyWindowPlayerSetAlly", nil, false, true)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetAlly", 10, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetAlly", 10, 255, 255, 255, 255)
	CppLogic.UI.WidgetSetUpdateManualFlag("DiplomacyWindowPlayerSetAlly", true)
	CppLogic.UI.ContainerWidgetCreateGFXButtonWidgetChild("DiplomacyWindowPlayer", "DiplomacyWindowPlayerSetNeutral", nil)
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowPlayerSetNeutral", 356.25, 4, 35, 35)
	XGUIEng.ShowWidget("DiplomacyWindowPlayerSetNeutral", 0)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowPlayerSetNeutral", 0, false, false)
	XGUIEng.DisableButton("DiplomacyWindowPlayerSetNeutral", 0)
	XGUIEng.HighLightButton("DiplomacyWindowPlayerSetNeutral", 0)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetNeutral", 0, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetNeutral", 0, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetNeutral", 1, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetNeutral", 1, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetNeutral", 2, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetNeutral", 2, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetNeutral", 3, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetNeutral", 3, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetNeutral", 4, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetNeutral", 4, 255, 255, 255, 255)
	CppLogic.UI.WidgetSetTooltipData("DiplomacyWindowPlayerSetNeutral", nil, false, true)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetNeutral", 10, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetNeutral", 10, 255, 255, 255, 255)
	CppLogic.UI.WidgetSetUpdateManualFlag("DiplomacyWindowPlayerSetNeutral", true)
	CppLogic.UI.ContainerWidgetCreateGFXButtonWidgetChild("DiplomacyWindowPlayer", "DiplomacyWindowPlayerSetHostile", nil)
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowPlayerSetHostile", 316.25, 4, 35, 35)
	XGUIEng.ShowWidget("DiplomacyWindowPlayerSetHostile", 0)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowPlayerSetHostile", 0, false, false)
	XGUIEng.DisableButton("DiplomacyWindowPlayerSetHostile", 0)
	XGUIEng.HighLightButton("DiplomacyWindowPlayerSetHostile", 0)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetHostile", 0, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetHostile", 0, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetHostile", 1, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetHostile", 1, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetHostile", 2, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetHostile", 2, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetHostile", 3, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetHostile", 3, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetHostile", 4, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetHostile", 4, 255, 255, 255, 255)
	CppLogic.UI.WidgetSetTooltipData("DiplomacyWindowPlayerSetHostile", nil, false, true)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerSetHostile", 10, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerSetHostile", 10, 255, 255, 255, 255)
	CppLogic.UI.WidgetSetUpdateManualFlag("DiplomacyWindowPlayerSetHostile", true)
	CppLogic.UI.ContainerWidgetCreateCustomWidgetChild("DiplomacyWindowPlayer", "DiplomacyWindowPlayerMPResourceAmount", "CppLogic::Mod::UI::TextInputCustomWidget", nil, 4, 0, 0, 0, -1073741824, 100, "", "")
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowPlayerMPResourceAmount", 610, 5, 120, 31)
	XGUIEng.ShowWidget("DiplomacyWindowPlayerMPResourceAmount", 1)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowPlayerMPResourceAmount", 0, false, false)
	CppLogic.UI.ContainerWidgetCreateTextButtonWidgetChild("DiplomacyWindowPlayer", "DiplomacyWindowPlayerMPResourceName", nil)
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowPlayerMPResourceName", 440, 5, 160, 31)
	XGUIEng.ShowWidget("DiplomacyWindowPlayerMPResourceName", 1)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowPlayerMPResourceName", 0, false, false)
	XGUIEng.DisableButton("DiplomacyWindowPlayerMPResourceName", 0)
	XGUIEng.HighLightButton("DiplomacyWindowPlayerMPResourceName", 0)
	CppLogic.UI.ButtonOverrideActionFunc("DiplomacyWindowPlayerMPResourceName", function() ExtraPlayers.GUIAction_DiplomacyResType() end)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerMPResourceName", 0, 0, 0, 1, 1)
	XGUIEng.SetMaterialTexture("DiplomacyWindowPlayerMPResourceName", 0, "data\\graphics\\textures\\gui\\window_bg187x32.png")
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerMPResourceName", 0, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerMPResourceName", 1, 0, 0, 1, 1)
	XGUIEng.SetMaterialTexture("DiplomacyWindowPlayerMPResourceName", 1, "data\\graphics\\textures\\gui\\window_bg187x32.png")
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerMPResourceName", 1, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerMPResourceName", 2, 0, 0, 1, 1)
	XGUIEng.SetMaterialTexture("DiplomacyWindowPlayerMPResourceName", 2, "data\\graphics\\textures\\gui\\window_bg187x32.png")
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerMPResourceName", 2, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerMPResourceName", 3, 0, 0, 1, 1)
	XGUIEng.SetMaterialTexture("DiplomacyWindowPlayerMPResourceName", 3, "data\\graphics\\textures\\gui\\window_bg187x32.png")
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerMPResourceName", 3, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerMPResourceName", 4, 0, 0, 1, 1)
	XGUIEng.SetMaterialTexture("DiplomacyWindowPlayerMPResourceName", 4, "data\\graphics\\textures\\gui\\window_bg187x32.png")
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerMPResourceName", 4, 255, 255, 255, 255)
	CppLogic.UI.WidgetSetTooltipData("DiplomacyWindowPlayerMPResourceName", nil, false, true)
	CppLogic.UI.WidgetSetFont("DiplomacyWindowPlayerMPResourceName", "data\\menu\\fonts\\medium11bold.met")
	CppLogic.UI.WidgetSetStringFrameDistance("DiplomacyWindowPlayerMPResourceName", 6)
	XGUIEng.SetText("DiplomacyWindowPlayerMPResourceName", "Type", 1)
	XGUIEng.SetTextColor("DiplomacyWindowPlayerMPResourceName", 255, 255, 255, 255)
	CppLogic.UI.TextButtonSetCenterText("DiplomacyWindowPlayerMPResourceName", true)
	CppLogic.UI.WidgetSetUpdateManualFlag("DiplomacyWindowPlayerMPResourceName", false)
	CppLogic.UI.WidgetOverrideUpdateFunc("DiplomacyWindowPlayerMPResourceName", function() ExtraPlayers.GUIUpdate_DiplomacyResType() end)
	CppLogic.UI.ContainerWidgetCreateTextButtonWidgetChild("DiplomacyWindowPlayer", "DiplomacyWindowPlayerMPResourceSend", nil)
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowPlayerMPResourceSend", 753, 5, 32, 32)
	XGUIEng.ShowWidget("DiplomacyWindowPlayerMPResourceSend", 1)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowPlayerMPResourceSend", 0, false, false)
	XGUIEng.DisableButton("DiplomacyWindowPlayerMPResourceSend", 0)
	XGUIEng.HighLightButton("DiplomacyWindowPlayerMPResourceSend", 0)
	CppLogic.UI.ButtonOverrideActionFunc("DiplomacyWindowPlayerMPResourceSend", function() ExtraPlayers.GUIAction_DiploSendRes() end)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerMPResourceSend", 0, 0, 0, 1, 1)
	XGUIEng.SetMaterialTexture("DiplomacyWindowPlayerMPResourceSend", 0, "data\\graphics\\textures\\gui\\trade_ok.png")
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerMPResourceSend", 0, 200, 200, 200, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerMPResourceSend", 1, 0, 0, 1, 1)
	XGUIEng.SetMaterialTexture("DiplomacyWindowPlayerMPResourceSend", 1, "data\\graphics\\textures\\gui\\trade_ok.png")
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerMPResourceSend", 1, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerMPResourceSend", 2, 0, 0, 1, 1)
	XGUIEng.SetMaterialTexture("DiplomacyWindowPlayerMPResourceSend", 2, "data\\graphics\\textures\\gui\\trade_ok.png")
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerMPResourceSend", 2, 255, 255, 255, 255)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerMPResourceSend", 3, 0, 0, 1, 1)
	XGUIEng.SetMaterialTexture("DiplomacyWindowPlayerMPResourceSend", 3, "data\\graphics\\textures\\gui\\trade_ok.png")
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerMPResourceSend", 3, 128, 128, 128, 128)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerMPResourceSend", 4, 0.6100000143051147, 0, 0.20000000298023224, 0.10000000149011612)
	XGUIEng.SetMaterialTexture("DiplomacyWindowPlayerMPResourceSend", 4, "data\\graphics\\textures\\gui\\trade_ok.png")
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerMPResourceSend", 4, 255, 255, 255, 255)
	CppLogic.UI.WidgetSetTooltipData("DiplomacyWindowPlayerMPResourceSend", nil, false, true)
	CppLogic.UI.WidgetSetFont("DiplomacyWindowPlayerMPResourceSend", "data\\menu\\fonts\\standard10.met")
	CppLogic.UI.WidgetSetStringFrameDistance("DiplomacyWindowPlayerMPResourceSend", 0)
	XGUIEng.SetText("DiplomacyWindowPlayerMPResourceSend", "", 1)
	XGUIEng.SetTextColor("DiplomacyWindowPlayerMPResourceSend", 0, 0, 0, 255)
	CppLogic.UI.TextButtonSetCenterText("DiplomacyWindowPlayerMPResourceSend", true)
	CppLogic.UI.WidgetSetUpdateManualFlag("DiplomacyWindowPlayerMPResourceSend", true)
	CppLogic.UI.ContainerWidgetCreateStaticWidgetChild("DiplomacyWindowPlayer", "DiplomacyWindowPlayerColor", nil)
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowPlayerColor", 304, 5, 32, 32)
	XGUIEng.ShowWidget("DiplomacyWindowPlayerColor", 1)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowPlayerColor", 0, false, false)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerColor", 0, 0, 0, 1, 1)
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerColor", 0, 81, 85, 255, 255)
	CppLogic.UI.ContainerWidgetCreateStaticWidgetChild("DiplomacyWindowPlayer", "DiplomacyWindowPlayerOpponentState", nil)
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowPlayerOpponentState", 267, 5, 32, 32)
	XGUIEng.ShowWidget("DiplomacyWindowPlayerOpponentState", 1)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowPlayerOpponentState", 0, false, false)
	CppLogic.UI.WidgetMaterialSetTextureCoordinates("DiplomacyWindowPlayerOpponentState", 0, 0, 0, 1, 1)
	XGUIEng.SetMaterialTexture("DiplomacyWindowPlayerOpponentState", 0, "graphics\\textures\\gui\\window_status_hostile.png")
	XGUIEng.SetMaterialColor("DiplomacyWindowPlayerOpponentState", 0, 255, 255, 255, 255)
	CppLogic.UI.ContainerWidgetCreateCustomWidgetChild("DiplomacyWindowPlayers", "DiplomacyWindowScroll", "CppLogic::Mod::UI::AutoScrollCustomWidget", nil, 0, 0, 0, 0, 0, 0, "", "DiplomacyWindowPlayer")
	CppLogic.UI.WidgetSetPositionAndSize("DiplomacyWindowScroll", 0, 0, 900, 414)
	XGUIEng.ShowWidget("DiplomacyWindowScroll", 1)
	CppLogic.UI.WidgetSetBaseData("DiplomacyWindowScroll", 0, false, false)

	ExtraPlayers.DiplomacyPlayerScroll:Setup()
end

Trigger.RequestTrigger(Events.CPPLOGIC_EVENT_ON_MAP_STARTED, nil, "InitExtraPlayers", 1)
Trigger.RequestTrigger(Events.CPPLOGIC_EVENT_ON_SAVEGAME_LOADED, nil, "ExtraPlayers_InitUI", 1)
