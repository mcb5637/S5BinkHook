
---@generic T
---@class CPPLAutoScroll
---@field Over T[]
---@field Selected T?
---@field private CustomWidget string
---@field private UpButton string?
---@field private DownButton string?
---@field private SliderBackground string?
---@field OnSelectedChanged fun(old:T?, new:T?)?
---@field StringExtract nil|fun(t:T):string
AutoScroll = {}

---@generic T
---@param cw string
---@param down string?
---@param up string?
---@param bg string?
---@param noSetup boolean?
---@return CPPLAutoScroll<T>
function AutoScroll.Init(cw, up, down, bg, noSetup)
	---@type CPPLAutoScroll
	local r = {
		Over = {},
		CustomWidget = cw,
		UpButton = up,
		DownButton = down,
		SliderBackground = bg
	}
	for k,v in pairs(AutoScroll) do
		r[k] = v
	end

	if not noSetup then
		r:Setup()
	end

	return r
end

---@generic T
---@param self CPPLAutoScroll<T>
---@param over T[]
---@param reselect boolean|nil|fun(a:T, a:T):boolean
---@param scrollTo boolean?
function AutoScroll:SetDataToScrollOver(over, reselect, scrollTo)
	self.Over = over
	local n, w = CppLogic.UI.InitAutoScrollCustomWidget(self.CustomWidget, table.getn(self.Over))
	local sliderUsed = n > w and 1 or 0
	if self.UpButton then
		XGUIEng.ShowWidget(self.UpButton, sliderUsed)
	end
	if self.DownButton then
		XGUIEng.ShowWidget(self.DownButton, sliderUsed)
	end
	if self.SliderBackground then
		XGUIEng.ShowWidget(self.SliderBackground, sliderUsed)
	end
	if reselect and self.Selected then
		if type(reselect)~="function" then
			reselect = function(a, b)
				return a == b
			end
		end
		for i, o in ipairs(self.Over) do
			if reselect(o, self.Selected) then
				self:SetSelected(o)
				if scrollTo then
					self:ScrollToCenterIndex(i)
				end
				return
			end
		end
		self:SetSelected(nil)
	end
end

---@generic T
---@param self CPPLAutoScroll<T>
---@param up number?
---@param deselect boolean?
function AutoScroll:GUIAction_DefaultSelect(up, deselect)
	local wid = XGUIEng.GetCurrentWidgetID()
	local o = self:GetElementOf(wid, up)
	self:SetSelected(o, false, deselect)
end

---@generic T
---@param self CPPLAutoScroll<T>
---@param highlightBySelect boolean?
---@param up number?
function AutoScroll:GUIUpdate_DefaultSelect(highlightBySelect, up)
	local wid = XGUIEng.GetCurrentWidgetID()
	local o = self:GetElementOf(wid, up)
	if self.StringExtract then
		local s = ""
		if o then
			s = self.StringExtract(o)
		end
		XGUIEng.SetText(wid, s)
	end
	if highlightBySelect then
		XGUIEng.HighLightButton(wid, o == self.Selected and 1 or 0)
	end
end

---@generic T
---@param self CPPLAutoScroll<T>
---@param widget string|number
---@param up number?
---@return T?
function AutoScroll:GetElementOf(widget, up)
	up = up or 0
	for _=1,up do
		widget = XGUIEng.GetWidgetsMotherID(widget)
	end
	local i = XGUIEng.GetBaseWidgetUserVariable(widget, 0) + 1
	return self.Over[i]
end

---@generic T
---@param self CPPLAutoScroll<T>
---@return T?
function AutoScroll:GetSelected()
	return self.Selected
end

---@generic T
---@param self CPPLAutoScroll<T>
---@param new T?
---@param scrollTo boolean?
---@param deselect boolean?
function AutoScroll:SetSelected(new, scrollTo, deselect)
	local old = self.Selected
	if deselect and new == self.Selected then
		new = nil
	end
	self.Selected = new
	if self.OnSelectedChanged then
		self.OnSelectedChanged(old, new)
	end
	if scrollTo and new then
		self:ScrollToCenter(new)
	end
end

---@generic T
---@param self CPPLAutoScroll<T>
---@param o T
function AutoScroll:ScrollToCenter(o)
	local i = 0
	for j,e in ipairs(self.Over) do
		if e == o then
			i = j
			break
		end
	end
	self:ScrollToCenterIndex(i)
end

---@generic T
---@param self CPPLAutoScroll<T>
---@param i number
function AutoScroll:ScrollToCenterIndex(i)
	if i > 0 then
		local _, widc = CppLogic.UI.GetAutoScrollCustomWidgetOffset(self.CustomWidget)
		CppLogic.UI.AutoScrollCustomWidgetSetOffset(self.CustomWidget, i - 1 - (widc / 2))
	end
end

---@param d number
function AutoScroll:GUIAction_ModScroll(d)
	CppLogic.UI.AutoScrollCustomWidgetModOffset(self.CustomWidget, d)
end

function AutoScroll:GUIUpdate_ScrollUp()
	local off = CppLogic.UI.GetAutoScrollCustomWidgetOffset(self.CustomWidget)
	local disableState = 0
	if off <= 0 then
		disableState = 1
	end
	XGUIEng.DisableButton(XGUIEng.GetCurrentWidgetID(), disableState)
end

function AutoScroll:GUIUpdate_ScrollDown()
	local off, widc, elc = CppLogic.UI.GetAutoScrollCustomWidgetOffset(self.CustomWidget)
	local disableState = 0
	if off >= elc - widc then
		disableState = 1
	end
	XGUIEng.DisableButton(XGUIEng.GetCurrentWidgetID(), disableState)
end

function AutoScroll:Setup()
	if self.UpButton and XGUIEng.GetWidgetID(self.UpButton)~=0 then
		CppLogic.UI.ButtonOverrideActionFunc(self.UpButton, function() self:GUIAction_ModScroll(-1) end)
		CppLogic.UI.WidgetOverrideUpdateFunc(self.UpButton, function() self:GUIUpdate_ScrollUp() end)
	end
	if self.DownButton and XGUIEng.GetWidgetID(self.DownButton)~=0 then
		CppLogic.UI.ButtonOverrideActionFunc(self.DownButton, function() self:GUIAction_ModScroll(1) end)
		CppLogic.UI.WidgetOverrideUpdateFunc(self.DownButton, function() self:GUIUpdate_ScrollDown() end)
	end
end
