#include "pch.h"
#include "l_ui.h"
#include <filesystem>
#include <WinUser.h>
#include <shok/ui/s5_widget.h>
#include <shok/globals/s5_filesystem.h>
#include <shok/globals/s5_classfactory.h>
#include <shok/globals/s5_framework.h>
#include <shok/globals/s5_idmanager.h>
#include <shok/globals/s5_mapdisplay.h>
#include <shok/entity/s5_entity.h>
#include <shok/events/s5_events.h>
#include <shok/s5_defines.h>
#include <shok/engine/s5_RWE_2d.h>
#include <shok/ui/s5_ui.h>
#include <shok/events/s5_netevents.h>
#include <shok/globals/s5_cutscene.h>
#include <utility/hooks.h>
#include <utility/luaserializer.h>
#include <utility/ModUI.h>
#include <utility/LuaEventInterface.h>
#include <utility/WinAPIUtil.h>
#include <utility/ModBehavior.h>

namespace CppLogic::UI {
	void StringHandlerSetString(luaext::State L, EGUIX::CSingleStringHandler& h, int i) {
		const char* s = L.CheckString(i);
		if (L.ToBoolean(i + 1)) {
			h.StringTableKey.assign(s);
			h.RawString.assign("");
		}
		else {
			h.RawString.assign(s);
			h.StringTableKey.assign("");
		}
	}

	void CheckFontString(const char* font) {
		if (!BB::CFileSystemMgr::DoesFileExist(font))
			throw lua::LuaException("file doesnt exist");
		if (!str_ends_with(font, ".met"))
			throw lua::LuaException("wrong file extension");
		std::string_view str{ font };
		if ((str.rfind(R"(data\maps\externalmap\)", 0) == std::string::npos) && (str.rfind(R"(data\menu\fonts\)", 0) == std::string::npos))
			throw lua::LuaException("incorrect folder");
	}

	int WidgetGetAddress(luaext::State L) {
		EGUIX::CBaseWidget* w = L.CheckWidget(1);
		L.Push(reinterpret_cast<int>(w));
		return 1;
	}

	int WidgetGetPositionAndSize(luaext::State L) {
		EGUIX::CBaseWidget* w = L.CheckWidget(1);
		L.Push(w->PosAndSize.X);
		L.Push(w->PosAndSize.Y);
		L.Push(w->PosAndSize.W);
		L.Push(w->PosAndSize.H);
		return 4;
	}

	int WidgetSetPositionAndSize(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		float x, y, w, h;
		if (L.IsNumber(2))
			x = L.CheckFloat(2);
		else
			x = wid->PosAndSize.X;
		if (L.IsNumber(3))
			y = L.CheckFloat(3);
		else
			y = wid->PosAndSize.Y;
		if (L.IsNumber(4))
			w = L.CheckFloat(4);
		else
			w = wid->PosAndSize.W;
		if (L.IsNumber(5))
			h = L.CheckFloat(5);
		else
			h = wid->PosAndSize.H;
		wid->SetPosAndSize(x, y, w, h);
		return 0;
	}

	int WidgetGetUpdateManualFlag(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		bool* f = wid->GetUpdateManualFlag();
		if (f) {
			L.Push(*f);
			return 1;
		}
		throw lua::LuaException("widget has no known updatemanual flag");
	}
	int WidgetSetUpdateManualFlag(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		bool flg = L.ToBoolean(2);
		bool* f = wid->GetUpdateManualFlag();
		if (f) {
			*f = flg;
			return 0;
		}
		throw lua::LuaException("widget has no known updatemanual flag");
	}

	int WidgetGetUpdateFunc(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CLuaFunctionHelper* fh = wid->GetUpdateFunc();
		if (!fh)
			throw lua::LuaException("widget has no known update func");
		L.Push(fh->LuaCommand.c_str());
		if (fh->FuncRefCommand.L.GetState() == L.GetState())
			L.Push(luaext::Reference{ fh->FuncRefCommand.Ref }, L.REGISTRYINDEX);
		else
			L.Push("no compiled func found");
		return 2;
	}
	int WidgetCallUpdateFunc(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CLuaFunctionHelper* fh = wid->GetUpdateFunc();
		if (!fh)
			throw lua::LuaException("widget has no known update func");
		fh->Call(wid->WidgetID);
		return 0;
	}
	int WidgetOverrideUpdateFunc(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CLuaFunctionHelper* fh = wid->GetUpdateFunc();
		if (!fh)
			throw lua::LuaException("widget has no known update func");
		fh->FuncRefCommand.ReplaceFunc(L, 2);
		return 0;
	}

	int ContainerWidgetGetAllChildren(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		auto* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
		if (!c)
			throw lua::LuaException("no container widget");
		L.NewTable();
		int i = 1;
		for (EGUIX::CBaseWidget* p : c->WidgetListHandler.SubWidgets) {
			L.Push(p->WidgetID);
			L.SetTableRaw(-2, i);
			i++;
		}
		return 1;
	}

	int IsContainerWidget(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		if (dynamic_cast<EGUIX::CContainerWidget*>(wid))
			L.Push(true);
		else
			L.Push(false);
		return 1;
	}

	int WidgetMaterialGetTextureCoordinates(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CMaterial* m = wid->GetMaterial(L.CheckInt(2));
		if (!m)
			throw lua::LuaException("no material");
		L.Push(m->TextureCoordinates.X);
		L.Push(m->TextureCoordinates.Y);
		L.Push(m->TextureCoordinates.W);
		L.Push(m->TextureCoordinates.H);
		return 4;
	}
	int WidgetMaterialSetTextureCoordinates(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CMaterial* m = wid->GetMaterial(L.CheckInt(2));
		if (!m)
			throw lua::LuaException("no material");
		if (L.IsNumber(3))
			m->TextureCoordinates.X = L.CheckFloat(3);
		if (L.IsNumber(4))
			m->TextureCoordinates.Y = L.CheckFloat(4);
		if (L.IsNumber(5))
			m->TextureCoordinates.W = L.CheckFloat(5);
		if (L.IsNumber(6))
			m->TextureCoordinates.H = L.CheckFloat(6);
		return 0;
	}

	int WidgetGetTooltipData(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		L.Push(tt->TargetWidget);
		L.Push(tt->ControlTargetWidgetDisplayState);
		L.Push(tt->ToolTipEnabledFlag);
		return 3;
	}
	int WidgetSetTooltipData(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		if (!L.IsNoneOrNil(2)) {
			tt->TargetWidget = L.CheckWidget(2)->WidgetID;
		}
		if (!L.IsNoneOrNil(3))
			tt->ControlTargetWidgetDisplayState = L.ToBoolean(3);
		if (!L.IsNoneOrNil(4))
			tt->ToolTipEnabledFlag = L.ToBoolean(4);
		return 0;
	}

	int WidgetGetTooltipString(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		L.Push(tt->ToolTipString.RawString.c_str());
		L.Push(tt->ToolTipString.StringTableKey.c_str());
		return 2;
	}
	int WidgetSetTooltipString(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		StringHandlerSetString(L, tt->ToolTipString, 2);
		return 0;
	}

	int WidgetGetTooltipFunc(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		L.Push(tt->UpdateFunction.LuaCommand.c_str());
		if (tt->UpdateFunction.FuncRefCommand.L.GetState() == L.GetState())
			L.Push(luaext::Reference{ tt->UpdateFunction.FuncRefCommand.Ref }, L.REGISTRYINDEX);
		else
			L.Push("no compiled func found");
		return 2;
	}
	int WidgetCallTooltipFunc(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		tt->UpdateFunction.Call(wid->WidgetID);
		return 0;
	}
	int WidgetOverrideTooltipFunc(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		tt->UpdateFunction.FuncRefCommand.ReplaceFunc(L, 2);
		return 0;
	}

	int ButtonGetActionFunc(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CButtonHelper* fh = wid->GetButtonHelper();
		if (!fh)
			throw lua::LuaException("widget has no known action func");
		L.Push(fh->ActionFunction.LuaCommand.c_str());
		if (fh->ActionFunction.FuncRefCommand.L.GetState() == L.GetState())
			L.Push(luaext::Reference{ fh->ActionFunction.FuncRefCommand.Ref }, L.REGISTRYINDEX);
		else
			L.Push("no compiled func found");
		return 2;
	}
	int ButtonCallActionFunc(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CButtonHelper* fh = wid->GetButtonHelper();
		if (!fh)
			throw lua::LuaException("widget has no known action func");
		fh->ActionFunction.Call(wid->WidgetID);
		return 0;
	}
	int ButtonOverrideActionFunc(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CButtonHelper* fh = wid->GetButtonHelper();
		if (!fh)
			throw lua::LuaException("widget has no known action func");
		fh->ActionFunction.FuncRefCommand.ReplaceFunc(L, 2);
		return 0;
	}

	int WidgetIsTooltipShown(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		L.Push(tt->IsToolTipShown);
		return 1;
	}

	int WidgetSetFont(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CWidgetStringHelper* s = wid->GetStringHelper();
		if (!s)
			throw lua::LuaException("no known stringhelper");
		const char* font = L.CheckString(2);
		CheckFontString(font);
		s->FontHandler.LoadFont(font);
		return 0;
	}

	int WidgetGetBaseData(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		L.Push(wid->ZPriority);
		L.Push(wid->ForceToHandleMouseEventsFlag);
		L.Push(wid->ForceToNeverBeFoundFlag);
		return 3;
	}
	int WidgetSetBaseData(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		if (L.IsNumber(2))
			wid->ZPriority = L.CheckFloat(2);
		if (L.IsBoolean(3))
			wid->ForceToHandleMouseEventsFlag = L.ToBoolean(3);
		if (L.IsBoolean(4))
			wid->ForceToNeverBeFoundFlag = L.ToBoolean(4);
		return 0;
	}

	int WidgetGetStringFrameDistance(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CWidgetStringHelper* s = wid->GetStringHelper();
		if (!s)
			throw lua::LuaException("no string helper");
		L.Push(s->StringFrameDistance);
		return 1;
	}
	int WidgetSetStringFrameDistance(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CWidgetStringHelper* s = wid->GetStringHelper();
		if (!s)
			throw lua::LuaException("no string helper");
		s->StringFrameDistance = L.CheckFloat(2);
		return 0;
	}

	int StaticTextWidgetGetLineDistanceFactor(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		auto* tw = dynamic_cast<EGUIX::CStaticTextWidget*>(wid);
		if (!tw)
			throw lua::LuaException("no static text widget");
		L.Push(tw->LineDistanceFactor);
		return 1;
	}
	int StaticTextWidgetSetLineDistanceFactor(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		auto* tw = dynamic_cast<EGUIX::CStaticTextWidget*>(wid);
		if (!tw)
			throw lua::LuaException("no static text widget");
		tw->LineDistanceFactor = L.CheckFloat(2);
		return 0;
	}

	int ButtonGetShortcutString(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CButtonHelper* b = wid->GetButtonHelper();
		if (!b)
			throw lua::LuaException("no button");
		L.Push(b->ShortCutString.RawString.c_str());
		L.Push(b->ShortCutString.StringTableKey.c_str());
		return 2;
	}
	int ButtonSetShortcutString(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CButtonHelper* b = wid->GetButtonHelper();
		if (!b)
			throw lua::LuaException("no button");
		StringHandlerSetString(L, b->ShortCutString, 2);
		return 0;
	}

	int WidgetSetGroup(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		const char* s = L.CheckString(2);
		EGUIX::CWidgetGroupManager* wgm = EGUIX::CWidgetGroupManager::GlobalObj();
		auto g = wgm->GetGroupId(s);
		if (g == shok::WidgetGroupId::Invalid)
			g = wgm->CreateGroup(s);
		wid->Group = g;
		return 0;
	}

	int TextButtonSetCenterText(luaext::State L) {
		auto* b = BB::IdentifierCast<EGUIX::CTextButtonWidget>(L.CheckWidget(1));
		if (b == nullptr)
			throw lua::LuaException{ "no textbutton" };
		b->CppLogic_CenterText = L.CheckBool(2);
		return 0;
	}

	int ContainerWidgetCreateStaticWidgetChild(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		auto* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
		if (!c)
			throw lua::LuaException("no container widget");
		const char* name = L.CheckString(2);
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != shok::WidgetId::Invalid)
			throw lua::LuaException("name already in use");
		EGUIX::CBaseWidget* bef = nullptr;
		if (!L.IsNoneOrNil(3))
			bef = L.CheckWidget(3);
		EGUIX::CStaticWidget* ne = EGUIX::CStaticWidget::Create();
		c->AddWidget(ne, name, bef);
		L.Push(c->WidgetID);
		return 1;
	}
	int ContainerWidgetCreateStaticTextWidgetChild(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		auto* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
		if (!c)
			throw lua::LuaException("no container widget");
		const char* name = L.CheckString(2);
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != shok::WidgetId::Invalid)
			throw lua::LuaException("name already in use");
		EGUIX::CBaseWidget* bef = nullptr;
		if (!L.IsNoneOrNil(3))
			bef = L.CheckWidget(3);
		EGUIX::CStaticTextWidget* ne = EGUIX::CStaticTextWidget::Create();
		c->AddWidget(ne, name, bef);
		L.Push(c->WidgetID);
		return 1;
	}
	int ContainerWidgetCreatePureTooltipWidgetChild(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		auto* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
		if (!c)
			throw lua::LuaException("no container widget");
		const char* name = L.CheckString(2);
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != shok::WidgetId::Invalid)
			throw lua::LuaException("name already in use");
		EGUIX::CBaseWidget* bef = nullptr;
		if (!L.IsNoneOrNil(3))
			bef = L.CheckWidget(3);
		EGUIX::CPureTooltipWidget* ne = EGUIX::CPureTooltipWidget::Create();
		c->AddWidget(ne, name, bef);
		L.Push(c->WidgetID);
		return 1;
	}
	int ContainerWidgetCreateGFXButtonWidgetChild(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		auto* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
		if (!c)
			throw lua::LuaException("no container widget");
		const char* name = L.CheckString(2);
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != shok::WidgetId::Invalid)
			throw lua::LuaException("name already in use");
		EGUIX::CBaseWidget* bef = nullptr;
		if (!L.IsNoneOrNil(3))
			bef = L.CheckWidget(3);
		EGUIX::CGfxButtonWidget* ne = EGUIX::CGfxButtonWidget::Create();
		c->AddWidget(ne, name, bef);
		L.Push(c->WidgetID);
		return 1;
	}
	int ContainerWidgetCreateTextButtonWidgetChild(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		auto* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
		if (!c)
			throw lua::LuaException("no container widget");
		const char* name = L.CheckString(2);
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != shok::WidgetId::Invalid)
			throw lua::LuaException("name already in use");
		EGUIX::CBaseWidget* bef = nullptr;
		if (!L.IsNoneOrNil(3))
			bef = L.CheckWidget(3);
		EGUIX::CTextButtonWidget* ne = EGUIX::CTextButtonWidget::Create();
		c->AddWidget(ne, name, bef);
		L.Push(c->WidgetID);
		return 1;
	}
	int ContainerWidgetCreateProgressBarWidgetChild(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		auto* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
		if (!c)
			throw lua::LuaException("no container widget");
		const char* name = L.CheckString(2);
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != shok::WidgetId::Invalid)
			throw lua::LuaException("name already in use");
		EGUIX::CBaseWidget* bef = nullptr;
		if (!L.IsNoneOrNil(3))
			bef = L.CheckWidget(3);
		EGUIX::CProgressBarWidget* ne = EGUIX::CProgressBarWidget::Create();
		c->AddWidget(ne, name, bef);
		L.Push(c->WidgetID);
		return 1;
	}
	int ContainerWidgetCreateContainerWidgetChild(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		auto* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
		if (!c)
			throw lua::LuaException("no container widget");
		const char* name = L.CheckString(2);
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != shok::WidgetId::Invalid)
			throw lua::LuaException("name already in use");
		EGUIX::CBaseWidget* bef = nullptr;
		if (!L.IsNoneOrNil(3))
			bef = L.CheckWidget(3);
		EGUIX::CContainerWidget* ne = EGUIX::CContainerWidget::Create();
		c->AddWidget(ne, name, bef);
		L.Push(c->WidgetID);
		return 1;
	}
	int ContainerWidgetCreateCustomWidgetChild(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		auto* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
		if (!c)
			throw lua::LuaException("no container widget");
		const char* name = L.CheckString(2);
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != shok::WidgetId::Invalid)
			throw lua::LuaException("name already in use");
		EGUIX::CBaseWidget* bef = nullptr;
		const char* customname = L.CheckString(3);
		if (!L.IsNoneOrNil(4))
			bef = L.CheckWidget(4);
		auto* ne = (*BB::CClassFactory::GlobalObj)->CreateObject<EGUIX::CCustomWidget>();
		c->AddWidget(ne, name, bef);
		ne->CustomClassName.assign(customname);
		ne->IntegerUserVariable0DefaultValue = L.OptInt(5, 0);
		ne->IntegerUserVariable1DefaultValue = L.OptInt(6, 0);
		ne->IntegerUserVariable2DefaultValue = L.OptInt(7, 0);
		ne->IntegerUserVariable3DefaultValue = L.OptInt(8, 0);
		ne->IntegerUserVariable4DefaultValue = L.OptInt(9, 0);
		ne->IntegerUserVariable5DefaultValue = L.OptInt(10, 0);
		ne->StringUserVariable0DefaultValue.assign(L.OptString(11, ""));
		ne->StringUserVariable1DefaultValue.assign(L.OptString(12, ""));
		ne->InitializeCustomWidget();
		L.Push(c->WidgetID);
		return 1;
	}

	int FontGetConfig(luaext::State L) {
		const char* font = L.CheckString(1);
		CheckFontString(font);
		shok::FontId id = shok::FontId::Invalid;
		EGUIX::FontManager::LoadFont(&id, font);
		auto* f = EGUIX::FontManager::GlobalObj()->GetFontObj(id);
		L.Push(f->Size);
		L.Push(f->Offset);
		L.Push(f->Spacing);
		return 3;
	}
	int FontSetConfig(luaext::State L) {
		const char* font = L.CheckString(1);
		CheckFontString(font);
		shok::FontId id = shok::FontId::Invalid;
		EGUIX::FontManager::LoadFont(&id, font);
		auto* f = EGUIX::FontManager::GlobalObj()->GetFontObj(id);
		if (L.IsNumber(2))
			f->Size = L.CheckFloat(2);
		if (L.IsNumber(3))
			f->Offset = L.CheckFloat(3);
		if (L.IsNumber(4))
			f->Spacing = L.CheckFloat(4);
		return 0;
	}

	int PreLoadGUITexture(luaext::State L) {
		const char* n = L.CheckString(1);
		if (!BB::CFileSystemMgr::DoesFileExist(n))
			throw lua::LuaException{ "files does not exist" };
		auto* m = EGUIX::TextureManager::GlobalObj();
		auto id = m->GetTextureID(n);
		m->GetTextureByID(id);
		L.Push(id);
		return 1;
	}

	int RemoveWidget(luaext::State L) {
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		auto* cw = dynamic_cast<EGUIX::CContainerWidget*>(wid);
		if (cw)
			if (cw->WidgetListHandler.SubWidgets.size() != 0)
				throw lua::LuaException("container widget has to be empty");
		delete wid;
		return 0;
	}

	int ReorderWidgets(luaext::State L) {
		EGUIX::CBaseWidget* tomove = L.CheckWidget(1);
		EGUIX::CBaseWidget* before = L.IsNoneOrNil(2) ? nullptr : L.CheckWidget(2);
		if (before != nullptr && tomove->MotherWidgetID != before->MotherWidgetID)
			throw lua::LuaException{ "not same mother widget" };
		auto* mother = dynamic_cast<EGUIX::CContainerWidget*>(EGUIX::WidgetManager::GlobalObj()->GetWidgetByID(tomove->MotherWidgetID));
		if (mother == nullptr)
			throw lua::LuaException{ "no mother widget???" };
		auto sl = mother->WidgetListHandler.SubWidgets.SaveList();
		auto m = std::find(sl.List.begin(), sl.List.end(), tomove);
		auto b = std::find(sl.List.begin(), sl.List.end(), before);
		sl.List.splice(b, sl.List, m);
		return 0;
	}

	int ReloadGUI(luaext::State L) {
		const char* str = L.OptString(1, R"(Data\Menu\Projects\Ingame.xml)");
		if (!BB::CFileSystemMgr::DoesFileExist(str))
			throw lua::LuaException{ "files does not exist" };
		EGUIX::WidgetLoader::GlobalObj()->LoadGUI(str);
		return 0;
	}

	int SetShowWoodInUI(luaext::State L) {
		bool s = L.CheckBool(1);
		GGUI::C3DOnScreenInformationCustomWidget::HookResourceFloatieShowWood(s);
		GGUI::C3DOnScreenInformationCustomWidget::HookResourceElementWood(s);
		return 0;
	}

	bool SetCharTrigger_CB(int c) {
		luaext::State L{ *EScr::CScriptTriggerSystem::GameState };
		int t = L.GetTop();
		bool r = false;

		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(CharTriggerRegKey);
		L.GetTableRaw(-2);
		L.Push(c);
		if (L.PCall(1, 1, 0) == lua::ErrorCode::Success) {
			r = L.OptBool(-1, false);
		}

		L.SetTop(t);
		return r;
	}
	int SetCharTrigger(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		if (L.IsNil(1)) {
			EGUIX::UIInput_Char_Callback = nullptr;
			CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
			L.Push(CharTriggerRegKey);
			L.Push();
			L.SetTableRaw(-3);
			return 0;
		}

		L.CheckType(1, lua::LType::Function);
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(CharTriggerRegKey);
		L.PushValue(1);
		L.SetTableRaw(-3);

		if (!EGUIX::UIInput_Char_Callback) {
			EGUIX::HookUIInput();
			EGUIX::UIInput_Char_Callback = &SetCharTrigger_CB;
		}
		return 0;
	}
	bool SetKeyTrigger_CB(int c, win_mouseEvents id) {
		luaext::State L{ *EScr::CScriptTriggerSystem::GameState };
		int t = L.GetTop();
		bool r = false;

		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(KeyTriggerRegKey);
		L.GetTableRaw(-2);
		L.Push(c);
		L.Push(id == win_mouseEvents::KeyUp || id == win_mouseEvents::SysKeyUp);
		if (L.PCall(2, 1, 0) == lua::ErrorCode::Success) {
			r = L.OptBool(-1, false);
		}

		L.SetTop(t);
		return r;
	}
	int SetKeyTrigger(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		if (L.IsNil(1)) {
			EGUIX::UIInput_Key_Callback = nullptr;
			CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
			L.Push(KeyTriggerRegKey);
			L.Push();
			L.SetTableRaw(-3);
			return 0;
		}

		L.CheckType(1, lua::LType::Function);
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(KeyTriggerRegKey);
		L.PushValue(1);
		L.SetTableRaw(-3);

		if (!EGUIX::UIInput_Key_Callback) {
			EGUIX::HookUIInput();
			EGUIX::UIInput_Key_Callback = &SetKeyTrigger_CB;
		}
		return 0;
	}
	bool SetMouseTrigger_CB(win_mouseEvents id, int w, int l) {
		if (id == win_mouseEvents::MouseMove)
			return false;

		luaext::State L{ *EScr::CScriptTriggerSystem::GameState };
		int t = L.GetTop();
		bool r = false;

		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(MouseTriggerRegKey);
		L.GetTableRaw(-2);
		L.Push(static_cast<int>(id));
		L.Push(l & 0xFFFF);
		L.Push((l >> 16) & 0xFFFF);
		lua::ErrorCode res = lua::ErrorCode::Syntax;

		if (id >= win_mouseEvents::LButtonDown && id <= win_mouseEvents::MButtonDBl) {
			res = L.PCall(3, 1, 0);
		}
		else if (id == win_mouseEvents::MouseWheel) {
			L.Push(w > 0);
			res = L.PCall(4, 1, 0);
		}
		else if (id >= win_mouseEvents::XButtonDown && id <= win_mouseEvents::XButtonDBl) {
			L.Push(((w >> 16) & 0xFFFF) - 1);
			res = L.PCall(4, 1, 0);
		}
		if (res == lua::ErrorCode::Success) {
			r = L.OptBool(-1, false);
		}

		L.SetTop(t);
		return r;
	}
	int SetMouseTrigger(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		if (L.IsNil(1)) {
			EGUIX::UIInput_Mouse_Callback = nullptr;
			CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
			L.Push(MouseTriggerRegKey);
			L.Push();
			L.SetTableRaw(-3);
			return 0;
		}

		L.CheckType(1, lua::LType::Function);
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(MouseTriggerRegKey);
		L.PushValue(1);
		L.SetTableRaw(-3);

		if (!EGUIX::UIInput_Mouse_Callback) {
			EGUIX::HookUIInput();
			EGUIX::UIInput_Mouse_Callback = &SetMouseTrigger_CB;
		}
		return 0;
	}
	int SetMouseTriggerMainMenu(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		if (L.IsNil(1)) {
			EGUIX::UIInput_Mouse_CallbackMainMenu = nullptr;
			L.PushLightUserdata(&SetMouseTriggerMainMenu);
			L.Push();
			L.SetTableRaw(L.REGISTRYINDEX);
			return 0;
		}

		L.CheckType(1, lua::LType::Function);
		L.PushLightUserdata(&SetMouseTriggerMainMenu);
		L.PushValue(1);
		L.SetTableRaw(L.REGISTRYINDEX);

		if (!EGUIX::UIInput_Mouse_CallbackMainMenu) {
			EGUIX::HookUIInput();
			EGUIX::UIInput_Mouse_CallbackMainMenu = [](win_mouseEvents id, int w, int l) {
				if (id == win_mouseEvents::MouseMove)
					return false;

				if ((*Framework::CMain::GlobalObj)->CurrentMode != Framework::CMain::Mode::MainMenu)
					return false;

				luaext::State L{ shok::LuaStateMainmenu };
				int t = L.GetTop();
				bool r = false;

				L.PushLightUserdata(&SetMouseTriggerMainMenu);
				L.GetTableRaw(L.REGISTRYINDEX);
				L.Push(static_cast<int>(id));
				L.Push(l & 0xFFFF);
				L.Push((l >> 16) & 0xFFFF);
				lua::ErrorCode res = lua::ErrorCode::Syntax;

				if (id >= win_mouseEvents::LButtonDown && id <= win_mouseEvents::MButtonDBl) {
					res = L.PCall(3, 1, 0);
				}
				else if (id == win_mouseEvents::MouseWheel) {
					L.Push(w > 0);
					res = L.PCall(4, 1, 0);
				}
				else if (id >= win_mouseEvents::XButtonDown && id <= win_mouseEvents::XButtonDBl) {
					L.Push(((w >> 16) & 0xFFFF) - 1);
					res = L.PCall(4, 1, 0);
				}
				if (res == lua::ErrorCode::Success) {
					r = L.OptBool(-1, false);
				}

				L.SetTop(t);
				return r;
				};
		}
		return 0;
	}

	int ShowResourceFloatieOnEntity(luaext::State L) {
		(*GGUI::C3DOnScreenInformationCustomWidget::GlobalObj)->ShowResourceFloatieOnEntity(L.CheckEntity(1)->EntityId, L.CheckInt(2));
		return 0;
	}

	int ShowAdvancedFloatie(luaext::State L) {
		GGUI::C3DOnScreenInformationCustomWidget::HookAdditionalFloaties();
		GGUI::AdvancedFloatieManager::GlobalObj.AddFloatie(L.CheckPos(1), L.CheckStringView(2));
		return 0;
	}

	int GetClientSize(luaext::State L) {
		RECT r;
		if (GetClientRect(*shok::MainWindowHandle, &r)) {
			L.Push(static_cast<double>(r.right));
			L.Push(static_cast<double>(r.bottom));
			return 2;
		}
		return 0;
	}

	int GetMousePosDirect(luaext::State L) {
		RECT r;
		if (!GetClientRect(*shok::MainWindowHandle, &r))
			return 0;
		POINT p;
		if (GetCursorPos(&p)) {
			if (ScreenToClient(*shok::MainWindowHandle, &p)) {
				if (p.x < 0 || p.y < 0 || p.x > r.right || p.y > r.bottom)
					return 0;
				L.Push(static_cast<double>(p.x));
				L.Push(static_cast<double>(p.y));
				return 2;
			}
		}
		return 0;
	}

	int GetWidgetName(luaext::State L) {
		EGUIX::CBaseWidget* w = L.CheckWidget(1);
		L.Push(EGUIX::WidgetManager::GlobalObj()->WidgetNameManager->GetNameByID(static_cast<int>(w->WidgetID)));
		return 1;
	}

	int GetLandscapePosAtScreenPos(luaext::State L) {
		int x = L.CheckInt(1);
		int y = L.CheckInt(2);
		shok::PositionRot p;
		if ((*ED::CGlobalsLogicEx::GlobalObj)->Landscape->GetTerrainPosAtScreenCoords(p, x, y)) {
			L.Push(p);
			L.Push(p.r);
			return 2;
		}
		return 0;
	}

	int ShowCommandAcknowledgementAtPosition(luaext::State L) {
		shok::Position p = L.CheckPos(1);
		(*ED::CGlobalsBaseEx::GlobalObj)->CommandAcks->ShowAck(p);
		return 0;
	}

	int CreateMiniMapMarker(luaext::State L) {
		shok::Position p = L.CheckPos(1);
		bool pulsing = L.CheckBool(2);
		int r = L.CheckInt(3);
		if (r < 0 || r > 255)
			throw lua::LuaException("r out of range");
		int g = L.CheckInt(4);
		if (g < 0 || g > 255)
			throw lua::LuaException("g out of range");
		int b = L.CheckInt(5);
		if (b < 0 || b > 255)
			throw lua::LuaException("b out of range");
		float timefactor = L.OptFloat(6, 1);
		float scalefactor = L.OptFloat(7, 1);
		GGUI::MiniMapHandler::GlobalObj()->CreateMarker(p, pulsing, r, g, b, timefactor, scalefactor);
		return 0;
	}

	int CreateMiniMapScriptSignal(luaext::State L) {
		shok::Position p = L.CheckPos(1);
		int r = L.CheckInt(2);
		if (r < 0 || r > 255)
			throw lua::LuaException("r out of range");
		int g = L.CheckInt(3);
		if (g < 0 || g > 255)
			throw lua::LuaException("g out of range");
		int b = L.CheckInt(4);
		if (b < 0 || b > 255)
			throw lua::LuaException("b out of range");
		float scalefactor = L.OptFloat(5, 1);
		GGUI::MiniMapHandler::GlobalObj()->CreateSignalDefault(p, r, g, b, scalefactor);
		return 0;
	}

	int GetCutsceneFarClipPlaneMinAndMax(luaext::State L) {
		L.Push(*ERwTools::CRwCameraHandler::CutsceneFarClipPlaneMax);
		L.Push(*ERwTools::CRwCameraHandler::CutsceneFarClipPlaneMin);
		return 2;
	}
	int SetCutsceneFarClipPlaneMinAndMax(luaext::State L) {
		float max = L.CheckFloat(1);
		float min = L.CheckFloat(2);
		if (max < min)
			throw lua::LuaException("max < min");
		CppLogic::Hooks::SaveVirtualProtect vp{ ERwTools::CRwCameraHandler::CutsceneFarClipPlaneMax ,4 };
		*ERwTools::CRwCameraHandler::CutsceneFarClipPlaneMax = max;
		CppLogic::Hooks::SaveVirtualProtect vp2{ ERwTools::CRwCameraHandler::CutsceneFarClipPlaneMin ,4 };
		*ERwTools::CRwCameraHandler::CutsceneFarClipPlaneMin = min;
		return 0;
	}

	int SetGUIStateLuaSelection(luaext::State L) {
		L.CheckType(1, lua::LType::Function);
		auto* vh = GGUI::CManager::GlobalObj()->C3DViewHandler;
		vh->StateIdManager->GetIDByNameOrCreate(GUIState_LuaSelection::Name, GUIState_LuaSelection::Id); // make sure the state id exists
		vh->SetGUIState<CppLogic::UI::GUIState_LuaSelection>();
		auto* s = dynamic_cast<CppLogic::UI::GUIState_LuaSelection*>(vh->CurrentState);
		L.PushValue(1);
		s->RefOnKlick = L.Ref(L.REGISTRYINDEX);
		if (!L.IsNoneOrNil(2)) {
			L.CheckType(2, lua::LType::Function);
			L.PushValue(2);
			s->RefOnCancel = L.Ref(L.REGISTRYINDEX);
		}
		return 0;
	}

	int SetGUIStatePlaceBuildingEx(luaext::State L) {
		auto* vh = GGUI::CManager::GlobalObj()->C3DViewHandler;
		vh->StateIdManager->GetIDByNameOrCreate(GUIState_PlaceBuildingEx::Name, GUIState_PlaceBuildingEx::Id); // make sure the state id exists
		GGUI::SPlaceBuildingStateParameters p{ L.CheckEnum<shok::UpgradeCategoryId>(1) };
		vh->SetGUIState<CppLogic::UI::GUIState_PlaceBuildingEx>(&p);
		return 0;
	}

	int SetPlaceBuildingRotation(luaext::State L) {
		//GGUI::CPlaceBuildingState::HookPlacementRotation();
		auto* s = dynamic_cast<CppLogic::UI::GUIState_PlaceBuildingEx*>(GGUI::CManager::GlobalObj()->C3DViewHandler->CurrentState);
		if (s) {
			s->SetRotation(L.CheckFloat(1));
			s->OnRotationChanged();
		}
		return 0;
	}
	int GetPlaceBuildingRotation(luaext::State L) {
		auto* s = dynamic_cast<CppLogic::UI::GUIState_PlaceBuildingEx*>(GGUI::CManager::GlobalObj()->C3DViewHandler->CurrentState);
		if (!s)
			return 0;
		L.Push(s->GetRotation());
		return 1;
	}

	int GetPlaceBuildingUCat(luaext::State L) {
		auto s = GGUI::CManager::GlobalObj()->C3DViewHandler->CurrentState;
		if (auto* pb = dynamic_cast<GGUI::CPlaceBuildingState*>(s)) {
			L.Push(pb->UpgradeCategory);
			return 1;
		}
		else if (auto* pc = dynamic_cast<GGUI::CPlaceCannonState*>(s)) {
			L.Push(pc->FoundationType);
			L.Push(pc->TopType);
			return 2;
		}
		throw lua::LuaException{ "invalid gui state" };
	}

	int GetWidgetAtPosition(luaext::State L) {
		float p[2]{ L.CheckFloat(2), L.CheckFloat(3) };
		auto* rel = L.CheckWidget(1);
		L.Push(rel->GetMouseOverWidget(p, true, false));
		return 1;
	}

	int StringInputWidgetGetIgnoreNextChar(luaext::State L) {
		auto* w = dynamic_cast<EGUIX::CCustomWidget*>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "no customwidget" };
		auto* cw = dynamic_cast<EGUIX::CStringInputCustomWidget*>(w->CustomWidget);
		if (cw == nullptr)
			throw lua::LuaException{ "no string input wiget" };
		L.Push(cw->IgnoreNextChar);
		return 1;
	}
	int StringInputWidgetSetIgnoreNextChar(luaext::State L) {
		auto* w = dynamic_cast<EGUIX::CCustomWidget*>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "no customwidget" };
		auto* cw = dynamic_cast<EGUIX::CStringInputCustomWidget*>(w->CustomWidget);
		if (cw == nullptr)
			throw lua::LuaException{ "no string input wiget" };
		cw->IgnoreNextChar = L.CheckBool(2);
		return 0;
	}

	int StringInputWidgetSetBufferSize(luaext::State L) {
		auto* w = dynamic_cast<EGUIX::CCustomWidget*>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "no customwidget" };
		auto* cw = dynamic_cast<EGUIX::CStringInputCustomWidget*>(w->CustomWidget);
		if (cw == nullptr)
			throw lua::LuaException{ "no string input wiget" };
		int s = L.CheckInt(2);
		if (s <= 0)
			throw lua::LuaException{ "size <= 0" };
		cw->BufferSize = s;
		cw->Reserve(cw->BufferSize);
		return 0;
	}

	int GetShortMessages(luaext::State L) {
		auto* mes = *GGUI::CShortMessagesWindowControllerCustomWidget::GlobalObj;
		int i = 1;
		auto dump = [&L, &i](const GGUI::CShortMessagesWindowControllerCustomWidget::Message& m) {
			L.NewTable();
			L.Push("Id");
			L.Push(m.MessageId);
			L.SetTableRaw(-3);
			L.Push("Type");
			L.Push(static_cast<int>(m.Type));
			L.SetTableRaw(-3);
			L.Push("StartTime");
			L.Push(m.StartTime);
			L.SetTableRaw(-3);
			L.Push("Duration");
			L.Push(m.Duration);
			L.SetTableRaw(-3);
			L.Push("Tooltip");
			L.Push(m.Tooltip.c_str());
			L.SetTableRaw(-3);
			L.Push("Pos");
			L.Push(m.Pos);
			L.SetTableRaw(-3);
			L.SetTableRaw(-2, i);
			++i;
			};

		L.NewTable();
		for (const auto* m : mes->StandardMessage.Messages) {
			dump(*m);
		}
		i = 1;
		L.NewTable();
		for (const auto* m : mes->History.Messages) {
			dump(*m);
		}
		return 2;
	}

	int ReInitShortMessagesSize(luaext::State L) {
		auto* mes = *GGUI::CShortMessagesWindowControllerCustomWidget::GlobalObj;
		mes->Initialize();
		return 0;
	}

	int CreateShortMessage(luaext::State L) {
		auto* mes = *GGUI::CShortMessagesWindowControllerCustomWidget::GlobalObj;
		shok::Position po{};
		shok::Position* p = nullptr;
		if (!L.IsNoneOrNil(4)) {
			po = L.CheckPos(4);
			p = &po;
		}
		mes->StandardMessage.Add(static_cast<GGUI::CShortMessagesWindowControllerCustomWidget::MessageType>(L.CheckInt(1)),
			nullptr, L.CheckFloat(2), p, L.CheckString(3)
		);
		return 0;
	}

	int RemoveShortMessage(luaext::State L) {
		auto* mes = *GGUI::CShortMessagesWindowControllerCustomWidget::GlobalObj;
		mes->StandardMessage.RemoveMessage(L.CheckInt(1));
		return 0;
	}

	int InitAutoScrollCustomWidget(luaext::State L) {
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* sc = dynamic_cast<CppLogic::Mod::UI::AutoScrollCustomWidget*>(w->CustomWidget);
		if (sc == nullptr)
			throw lua::LuaException{ "not a AutoScrollCustomWidget" };
		sc->ElementCount = L.CheckInt(2);
		sc->ReInit();
		L.Push(sc->ElementCount);
		L.Push(sc->WidgetCount);
		return 2;
	}
	int GetAutoScrollCustomWidgetOffset(luaext::State L) {
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* sc = dynamic_cast<CppLogic::Mod::UI::AutoScrollCustomWidget*>(w->CustomWidget);
		if (sc == nullptr)
			throw lua::LuaException{ "not a AutoScrollCustomWidget" };
		L.Push(sc->Offset);
		L.Push(sc->WidgetCount);
		L.Push(sc->ElementCount);
		return 3;
	}
	int AutoScrollCustomWidgetModOffset(luaext::State L) {
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* sc = dynamic_cast<CppLogic::Mod::UI::AutoScrollCustomWidget*>(w->CustomWidget);
		if (sc == nullptr)
			throw lua::LuaException{ "not a AutoScrollCustomWidget" };
		sc->Offset += L.CheckFloat(2);
		sc->Clamp();
		sc->Update();
		return 0;
	}
	int AutoScrollCustomWidgetSetOffset(luaext::State L) {
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* sc = dynamic_cast<CppLogic::Mod::UI::AutoScrollCustomWidget*>(w->CustomWidget);
		if (sc == nullptr)
			throw lua::LuaException{ "not a AutoScrollCustomWidget" };
		sc->Offset = L.CheckFloat(2);
		sc->Clamp();
		sc->Update();
		return 0;
	}
	int AutoScrollCustomWidgetSetMaterial(luaext::State L) {
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* sc = dynamic_cast<CppLogic::Mod::UI::AutoScrollCustomWidget*>(w->CustomWidget);
		if (sc == nullptr)
			throw lua::LuaException{ "not a AutoScrollCustomWidget" };
		sc->PartialWidget.SetTexture(L.CheckString(2));
		sc->PartialWidget.TextureCoordinates.X = L.CheckFloat(3);
		sc->PartialWidget.TextureCoordinates.Y = L.CheckFloat(4);
		sc->PartialWidget.TextureCoordinates.W = L.CheckFloat(5);
		sc->PartialWidget.TextureCoordinates.H = L.CheckFloat(6);
		sc->PartialWidget.Color.Red = L.CheckInt(7);
		sc->PartialWidget.Color.Green = L.CheckInt(8);
		sc->PartialWidget.Color.Blue = L.CheckInt(9);
		sc->PartialWidget.Color.Alpha = L.CheckInt(10);
		return 0;
	}
	int TextInputCustomWidgetGetText(luaext::State L) {
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* t = dynamic_cast<CppLogic::Mod::UI::TextInputCustomWidget*>(w->CustomWidget);
		if (t == nullptr)
			throw lua::LuaException{ "not a TextInputCustomWidget" };
		const auto& [str, _] = t->ClearTextOutput();
		L.Push(str);
		return 1;
	}
	int TextInputCustomWidgetSetText(luaext::State L) {
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* t = dynamic_cast<CppLogic::Mod::UI::TextInputCustomWidget*>(w->CustomWidget);
		if (t == nullptr)
			throw lua::LuaException{ "not a TextInputCustomWidget" };
		t->CurrentTextRaw = L.CheckStringView(2);
		t->CurrentPos = t->CurrentTextRaw.size();
		t->RefreshDisplayText();
		return 0;
	}
	int TextInputCustomWidgetSetIgnoreNextChar(luaext::State L) {
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* t = dynamic_cast<CppLogic::Mod::UI::TextInputCustomWidget*>(w->CustomWidget);
		if (t == nullptr)
			throw lua::LuaException{ "not a TextInputCustomWidget" };
		t->IgnoreNextChar = L.CheckBool(2);
		return 0;
	}
	int InputCustomWidgetHasFocus(luaext::State L) {
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* t = dynamic_cast<CppLogic::Mod::UI::InputFocusWidget*>(w->CustomWidget);
		if (t == nullptr)
			throw lua::LuaException{ "not a InputFocusWidget" };
		L.Push(t->HasFocus());
		return 1;
	}
	int InputCustomWidgetSetFocus(luaext::State L) {
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* t = dynamic_cast<CppLogic::Mod::UI::InputFocusWidget*>(w->CustomWidget);
		if (t == nullptr)
			throw lua::LuaException{ "not a InputFocusWidget" };
		if (L.CheckBool(2))
			t->GetFocus();
		else
			t->ClearFocus();
		return 0;
	}

	int DumpVideoModes(luaext::State L) {
		L.NewTable();
		RWE::RwVideoMode m{};
		int ma = RWE::RwVideoMode::GetNumVideoModes();
		for (int i = 0; i < ma; ++i) {
			L.NewTable();
			m.GetInfo(i);

			L.Push("width");
			L.Push(m.width);
			L.SetTableRaw(-3);

			L.Push("height");
			L.Push(m.height);
			L.SetTableRaw(-3);

			L.Push("depth");
			L.Push(m.depth);
			L.SetTableRaw(-3);

			L.Push("flags");
			L.Push((int)m.flags);
			L.SetTableRaw(-3);

			L.Push("refRate");
			L.Push(m.refRate);
			L.SetTableRaw(-3);

			L.Push("format");
			L.Push(m.format);
			L.SetTableRaw(-3);

			L.SetTableRaw(-2, i);
		}
		L.Push(RWE::RwVideoMode::GetVideoMode());
		return 2;
	}

	int GetCameraData(luaext::State L) {
		auto* cam = dynamic_cast<ERwTools::CRwCameraHandler*>(*ERwTools::CRwCameraHandler::GlobalObj);
		L.Push(cam->CameraInfo.LookAtX);
		L.Push(cam->CameraInfo.LookAtY);
		L.Push(cam->CameraInfo.LookAtZ);
		L.Push(cam->CameraInfo.Distance);
		L.Push(cam->HorizontalAngle);
		L.Push(cam->VerticalAngle);
		if (cam->VerticalAngle > 90.0f) {
			cam->VerticalAngle = 90.0f;
		}
		if (cam->VerticalAngle < -90.0f) {
			cam->VerticalAngle = -90.0f;
		}
		return 6;
	}

	int SetCameraData(luaext::State L) {
		auto* cam = dynamic_cast<ERwTools::CRwCameraHandler*>(*ERwTools::CRwCameraHandler::GlobalObj);
		cam->CameraInfo.LookAtX = L.CheckFloat(1);
		cam->CameraInfo.LookAtY = L.CheckFloat(2);
		cam->CameraInfo.LookAtZ = L.CheckFloat(3);
		cam->CameraInfo.Distance = L.CheckFloat(4);
		cam->HorizontalAngle = L.CheckFloat(5);
		cam->VerticalAngle = L.CheckFloat(6);
		return 0;
	}

	int ListCutscenes(luaext::State L) {
		ECS::CManager* m = **ECS::CManager::GlobalObj;
		L.NewTable();
		int i = 1;
		for (auto& c : m->Cutscenes) {
			L.Push(c.first);
			L.SetTableRaw(-2, i);
			++i;
		}
		return 1;
	}

	int GetCutscene(luaext::State L) {
		ECS::CManager* m = **ECS::CManager::GlobalObj;
		auto n = L.CheckStringView(1);
		for (auto& c : m->Cutscenes) {
			if (c.first == n) {
				CppLogic::Serializer::ObjectToLuaSerializer::Serialize(L, &c.second, c.second.SerializationData());
				return 1;
			}
		}
		return 0;
	}

	int SetCutscene(luaext::State L) {
		ECS::CManager* m = **ECS::CManager::GlobalObj;
		auto n = L.CheckStringView(1);
		ECS::Cutscene* cs = nullptr;
		for (auto& c : m->Cutscenes) {
			if (c.first == n) {
				cs = &c.second;
				cs->CameraLookAt.Element.clear();
				cs->CameraPos.Element.clear();
				cs->LuaFunctions.Element.clear();
			}
		}
		if (cs == nullptr) {
			auto it = m->Cutscenes.emplace(std::piecewise_construct, std::forward_as_tuple(n), std::forward_as_tuple());
			cs = &it->second;
			cs->Name = n;
		}
		if (cs == nullptr)
			throw lua::LuaException{ "something went wrong with the map" };
		L.PushValue(2);
		CppLogic::Serializer::ObjectToLuaSerializer::Deserialize(L, cs, cs->SerializationData());
		return 0;
	}

	int ExportCutscenes(luaext::State L) {
		CppLogic::WinAPI::FileDialog dlg{};
		dlg.Filter = "CutsceneNames.xml\0CutsceneNames.xml\0";
		dlg.Title = "Export Cutscenes to";
		dlg.SelectedPath = L.OptStringView(1, "");
		if (dlg.Show()) {
			std::filesystem::path p(dlg.SelectedPath);
			p.remove_filename();
			auto ps = p.string();
			(**ECS::CManager::GlobalObj)->SaveCutscenesTo(ps.c_str());
		}
		return 0;
	}

	int VideoCustomWidgetGetVideoSize(luaext::State L) {
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* t = dynamic_cast<EGUIX::CVideoPlaybackCustomWidget*>(w->CustomWidget);
		if (t == nullptr)
			throw lua::LuaException{ "not a CVideoPlaybackCustomWidget" };
		if (t->Collection == nullptr)
			return 0;
		int x, y;
		t->Collection->GetSize(&x, &y);
		L.Push(x);
		L.Push(y);
		return 2;
	}

	int VideoCustomWidgetSetVideoSize(luaext::State L) {
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* t = dynamic_cast<EGUIX::CVideoPlaybackCustomWidget*>(w->CustomWidget);
		if (t == nullptr)
			throw lua::LuaException{ "not a CVideoPlaybackCustomWidget" };
		if (t->Collection == nullptr)
			t->Init(w);
		t->Collection->Init(L.CheckInt(2), L.CheckInt(3));
		return 0;
	}

	int MiniMapOverlaySetCallbackFuncName(luaext::State L) {
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto f = L.CheckString(2);
		if (auto* ovl = dynamic_cast<GGUI::CMiniMapOverlayCustomWidget*>(w->CustomWidget)) {
			ovl->Destroy();
			w->CustomWidget = nullptr;
			w->CustomClassName = "CppLogic::Mod::UI::MiniMapOverlayWithCallbackCustomWidget";
			w->InitializeCustomWidget();
		}
		auto* ovl = dynamic_cast<CppLogic::Mod::UI::MiniMapOverlayWithCallbackCustomWidget*>(w->CustomWidget);
		if (ovl == nullptr)
			throw lua::LuaException{ "not a MiniMapOverlayWithCallbackCustomWidget" };
		ovl->FuncName() = f;
		return 0;
	}

	int CreateSelectionDecal(luaext::State L) {

		auto t = L.CheckEnum<shok::SelectionTextureId>(1);
		shok::Position p = L.CheckPos(2);

		ED::CTerrainDecals* dec = (*ED::CGlobalsLogicEx::GlobalObj)->TerrainDecalsManager;
		BB::TResourceProxyResMgr<RWE::RwTexture*>* texpr = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->SelectionTextures->Get(t);

		if (texpr == nullptr)
			throw lua::LuaException{ "no textureproxy" };
		auto* tex = texpr->Get();
		if (tex == nullptr)
			throw lua::LuaException{ "failed to load texture" };

		L.NewUserClass<TerrainDecalAccess>(dec->CreateTerrainDecal(2, tex->raster, p.X, p.Y, L.CheckFloat(3), L.CheckFloat(4)));
		return 1;
	}

	int InitNetHandlers(luaext::State L) {
		auto* gl = *GGL::CGLGameLogic::GlobalObj;
		gl->CreateNetEventHandler<shok::NetEventIds::CppL_LightningStrike_Activate>(Mod::LightningStrikeAbility::NetEventLightningStrike);
		gl->CreateNetEventHandler<shok::NetEventIds::CppL_ResDoodadRefill_Activate>(Mod::ResDoodadRefillBehavior::NetEventRefillResDoodad);
		gl->CreateNetEventHandler<shok::NetEventIds::CppL_ShieldCover_Activate>(Mod::ShieldCoverAbility::NetEventShieldCover);
		gl->CreateNetEventHandler<shok::NetEventIds::CppL_Resurrect_Activate>(Mod::ResurrectAbility::NetEventResurrect);
		gl->CreateNetEventHandler<shok::NetEventIds::CppL_BombardmentActivate>(Mod::BombardmentAbility::NetEventBombard);
		gl->CreateNetEventHandler<shok::NetEventIds::CppL_BombComboCannonActivate>(Mod::ReloadableCannonBuilderAbility::NetEventBombComboCannon);
		return 0;
	}

	void* GUIState_LuaSelection::operator new(size_t s)
	{
		return shok::Malloc(s);
	}
	void GUIState_LuaSelection::operator delete(void* p) {
		shok::Free(p);
	}

	GUIState_LuaSelection::~GUIState_LuaSelection() {
		luaext::State L{ *EScr::CScriptTriggerSystem::GameState };
		if (RefOnKlick != luaext::State::NoRef)
			L.UnRef(RefOnKlick, L.REGISTRYINDEX);
		if (RefOnCancel != luaext::State::NoRef)
			L.UnRef(RefOnCancel, L.REGISTRYINDEX);
	}

	shok::ClassId __stdcall GUIState_LuaSelection::GetClassIdentifier() const
	{
		return GUIState_LuaSelection::Identifier;
	}

	bool GUIState_LuaSelection::OnMouseEvent(BB::CEvent* ev) {
		auto* mev = BB::IdentifierCast<BB::CMouseEvent>(ev);
		if (mev && mev->IsEvent(shok::InputEventIds::MouseButtonDown)) {
			if (mev->IsKey(shok::Keys::MouseLButton)) {
				bool r = true;
				if (RefOnKlick != luaext::State::NoRef) {
					luaext::State L{ *EScr::CScriptTriggerSystem::GameState };
					int i = L.GetTop();
					L.Push(RefOnKlick, L.REGISTRYINDEX);
					L.Push(mev->X);
					L.Push(mev->Y);
					L.PCall(2, 1, 0);
					if (L.IsBoolean(-1))
						r = L.ToBoolean(-1);
					L.SetTop(i);
				}
				if (r)
					Cancel(false);
				return r;
			}
			else if (mev->IsKey(shok::Keys::MouseRButton)) { // rmb
				Cancel(true);
				return true;
			}
		}
		return false;
	}

	bool GUIState_LuaSelection::Cancel()
	{
		Cancel(true);
		return true;
	}

	const char* GUIState_LuaSelection::GetName()
	{
		return Name;
	}


	void GUIState_LuaSelection::Initialize()
	{
		(*BB::CClassFactory::GlobalObj)->AddClassToFactory<GUIState_LuaSelection>();
	}

	void GUIState_LuaSelection::Cancel(bool calllua)
	{
		if (calllua && RefOnCancel != luaext::State::NoRef) {
			luaext::State L{ *EScr::CScriptTriggerSystem::GameState };
			int i = L.GetTop();
			L.Push(RefOnCancel, L.REGISTRYINDEX);
			L.PCall(0, 0, 0);
			L.SetTop(i);
		}
		C3DViewHandler->SetGUIStateByIdentfierOnNextUpdate<GGUI::CSelectionState>();
	}

	void CppLogic::UI::GUIState_PlaceBuildingEx::Initialize()
	{
		(*BB::CClassFactory::GlobalObj)->AddClassToFactory<GUIState_PlaceBuildingEx>();
	}

	void* CppLogic::UI::GUIState_PlaceBuildingEx::operator new(size_t s)
	{
		return shok::Malloc(s);
	}
	void CppLogic::UI::GUIState_PlaceBuildingEx::operator delete(void* p)
	{
		shok::Free(p);
	}

	shok::ClassId __stdcall CppLogic::UI::GUIState_PlaceBuildingEx::GetClassIdentifier() const
	{
		return Identifier;
	}

	const char* CppLogic::UI::GUIState_PlaceBuildingEx::GetName()
	{
		return Name;
	}

	void CppLogic::UI::GUIState_PlaceBuildingEx::SetStateParameters(GGUI::SStateParameters* p)
	{
		GGUI::CPlaceBuildingState::SetStateParameters(p);
		ERwTools::CDefCameraBehaviour::HookEnableZoom(false);
	}

	bool CppLogic::UI::GUIState_PlaceBuildingEx::OnMouseEvent(BB::CEvent* ev)
	{
		if (auto* e = BB::IdentifierCast<BB::CMouseEvent>(ev)) {
			if (e->IsEvent(shok::InputEventIds::MouseWheel)) {
				if (e->IsModifier(shok::Keys::ModifierControl)) {
					(*ERwTools::CRwCameraHandler::GlobalObj)->ScrollWheelZoom(e->Delta);
					e->EventHandeled = true;
					return true;
				}
				else {
					if (e->Delta < 0) {
						CurrentStep = (CurrentStep + 1) % NumSteps;
					}
					else {
						CurrentStep--;
						while (CurrentStep < 0)
							CurrentStep += NumSteps;
					}
					PosToBuild = {};
					UpdateModel(e->X, e->Y);
					e->EventHandeled = true;
					return true;
				}
			}
		}

		return GGUI::CPlaceBuildingState::OnMouseEvent(ev);
	}

	bool CppLogic::UI::GUIState_PlaceBuildingEx::CheckCommandValid(TargetData* d, int z)
	{
		auto m = GGUI::CManager::GlobalObj();
		auto i = m->GUIInterface;
		auto ety = i->GetSettlerTypeByUCat(m->ControlledPlayer, UpgradeCategory);
		if (!i->CheckBuildingPlacementAndCost(m->ControlledPlayer, ety, d->TargetPos.X, d->TargetPos.Y, CppLogic::DegreesToRadians(GetRotation())))
			return false;
		bool hasserf = false;
		bool hassector = false;
		auto sector = i->GetSector(&d->TargetPos);
		for (const auto& e : m->SelectedEntities) {
			if (i->IsSerf(e.Id)) {
				hasserf = true;
				if (i->GetSector(e.Id) == sector) {
					hassector = true;
					break;
				}
			}
		}
		if (!hasserf)
			return false;
		if (!hassector)
			return false;
		if (!i->IsPositionExploredByPlayer(m->ControlledPlayer, &d->TargetPos))
			return false;
		return true;
	}

	void CppLogic::UI::GUIState_PlaceBuildingEx::ExecuteCommand(TargetData* d, ExecuteData* selectedID)
	{
		if (selectedID->CurrentID == selectedID->FirstID) {
			if (CheckCommandValid(d, 0)) {
				auto m = GGUI::CManager::GlobalObj();
				GGL::CNetEventBuildingCreator ev{ shok::NetEventIds::Player_BuyBuilding, m->ControlledPlayer, UpgradeCategory, shok::PositionRot{d->TargetPos.X, d->TargetPos.Y, CppLogic::DegreesToRadians(GetRotation())} };
				{
					auto v = ev.Serf.SaveVector();
					for (const auto& se : m->SelectedEntities) {
						if (m->GUIInterface->IsSerf(se.Id))
							v.Vector.push_back(se.Id);
					}
				}
				GGUI::CManager::PostEventFromUI(&ev);
				luaext::State L{ m->GameState };
				int top = L.GetTop();
				L.GetGlobal("GameCallback_GUI_AfterBuildingPlacement");
				L.PCall(0, 0);
				L.SetTop(top);
				SetModelToRender(nullptr, 0);
			}
		}
	}

	GGUI::CBasicState::TargetData* CppLogic::UI::GUIState_PlaceBuildingEx::GetTargetData(TargetData* d, int x, int y)
	{
		if (UpgradeCategory != shok::UpgradeCategoryId::Invalid) {
			auto m = GGUI::CManager::GlobalObj();
			auto ety = m->GUIInterface->GetSettlerTypeByUCat(m->ControlledPlayer, UpgradeCategory);
			FillPosData(d, x, y);
			d->TargetPos.FloorToBuildingPlacement();
			if (static_cast<int>(d->TargetPos.X) == static_cast<int>(PosToBuild.X) && static_cast<int>(d->TargetPos.Y) == static_cast<int>(PosToBuild.X)) {
				d->TargetPos = PosToBuild;
			}
			else {
				shok::PositionRot p = GetNearestPlacementPos(ety, shok::PositionRot{ d->TargetPos.X, d->TargetPos.Y, CppLogic::DegreesToRadians(GetRotation()) }, (*GGL::CLogicProperties::GlobalObj)->BuildingPlacementSnapDistance);
				if (p.X >= 0) {
					d->TargetPos.X = p.X;
					d->TargetPos.Y = p.Y;
					SetRotation(CppLogic::RadiansToDegrees(p.r));
				}
				PosToBuild = d->TargetPos;
			}
			d->FillPosWithZFromPos();
		}
		return d;
	}

	void CppLogic::UI::GUIState_PlaceBuildingEx::OnMouseMove(int x, int y)
	{
		MouseX = x;
		MouseY = y;
		UpdateModel(x, y);
	}

	bool CppLogic::UI::GUIState_PlaceBuildingEx::OnCancel()
	{
		ERwTools::CDefCameraBehaviour::HookEnableZoom(true);
		return GGUI::CPlaceBuildingState::OnCancel();
	}

	void CppLogic::UI::GUIState_PlaceBuildingEx::UpdateModel(int x, int y)
	{
		if (UpgradeCategory == shok::UpgradeCategoryId::Invalid)
			return;
		SetModelToRender();
		TargetData d{};
		GetTargetData(&d, x, y);
		C3DViewHandler->ClumpRenerable->Model->GetFrame()->Rotate(GetRotation(), RWE::RwOpCombineType::Replace);
		C3DViewHandler->ClumpRenerable->SetBuildingRedColor(!CheckCommandValid(&d, 0));
		C3DViewHandler->ClumpRenerable->SetPosition(d.TargetPos, d.TargetPosWithZ.r);
	}

	float CppLogic::UI::GUIState_PlaceBuildingEx::GetRotation() const
	{
		return StepToDegrees * static_cast<float>(CurrentStep);
	}

	void CppLogic::UI::GUIState_PlaceBuildingEx::SetRotation(float deg)
	{
		CurrentStep = static_cast<int>(std::roundf(deg / StepToDegrees));
	}

	void CppLogic::UI::GUIState_PlaceBuildingEx::OnRotationChanged()
	{
		PosToBuild = {};
		UpdateModel(MouseX, MouseY);
	}

	void Cleanup(luaext::State L) {
		EGUIX::UIInput_Char_Callback = nullptr;
		EGUIX::UIInput_Key_Callback = nullptr;
		EGUIX::UIInput_Mouse_Callback = nullptr;
		GGUI::C3DOnScreenInformationCustomWidget::HookResourceFloatieShowWood(false);
		GGUI::C3DOnScreenInformationCustomWidget::HookResourceElementWood(false);
		if (EGUIX::WidgetManager* wm = EGUIX::WidgetManager::GlobalObj()) {
			for (EGUIX::CBaseWidget* wid : wm->Widgets) {
				if (wid == nullptr)
					continue;
				if (EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper()) {
					tt->UpdateFunction.FuncRefCommand.Clear();
				}
				if (EGUIX::CLuaFunctionHelper* fh = wid->GetUpdateFunc()) {
					fh->FuncRefCommand.Clear();
				}
				if (EGUIX::CButtonHelper* bh = wid->GetButtonHelper()) {
					bh->ActionFunction.FuncRefCommand.Clear();
				}
			}
		}
		TerrainDecalAccess::Cleanup();
	}

	void OnSaveLoaded(luaext::State L)
	{
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);

		L.Push(CharTriggerRegKey);
		L.GetTableRaw(-2);
		if (!L.IsNil(-1)) {
			EGUIX::HookUIInput();
			EGUIX::UIInput_Char_Callback = &SetCharTrigger_CB;
		}
		L.Pop(1);

		L.Push(KeyTriggerRegKey);
		L.GetTableRaw(-2);
		if (!L.IsNil(-1)) {
			EGUIX::HookUIInput();
			EGUIX::UIInput_Key_Callback = &SetKeyTrigger_CB;
		}
		L.Pop(1);

		L.Push(MouseTriggerRegKey);
		L.GetTableRaw(-2);
		if (!L.IsNil(-1)) {
			EGUIX::HookUIInput();
			EGUIX::UIInput_Mouse_Callback = &SetMouseTrigger_CB;
		}
		L.Pop(1);

		L.Pop(1);
	}

	CppLogic::UI::TerrainDecalAccess::TerrainDecalAccess(ED::CTerrainDecalBase* d)
		: Decal(d)
	{
		Actives.insert(this);
	}

	CppLogic::UI::TerrainDecalAccess::~TerrainDecalAccess()
	{
		Destroy();
	}

	int CppLogic::UI::TerrainDecalAccess::Destroy(luaext::State L)
	{
		L.CheckUserClass<TerrainDecalAccess>(1)->Destroy();
		return 0;
	}
	void CppLogic::UI::TerrainDecalAccess::Destroy()
	{
		Decal = nullptr;
		auto i = Actives.find(this);
		if (i != Actives.end())
			Actives.erase(i);
	}

	int CppLogic::UI::TerrainDecalAccess::SetPos(luaext::State L)
	{
		auto* th = L.CheckUserClass<TerrainDecalAccess>(1);
		if (th->Decal == nullptr)
			throw lua::LuaException{ "already destroyed" };
		auto p = L.CheckPos(2);
		th->Decal->SetPos(p.X, p.Y);
		return 0;
	}

	std::set<TerrainDecalAccess*> TerrainDecalAccess::Actives{};
	void TerrainDecalAccess::Cleanup()
	{
		for (TerrainDecalAccess* a : Actives)
			a->Decal = nullptr;
		Actives.clear();
	}

	constexpr std::array UI{
		luaext::FuncReference::GetRef<WidgetGetPositionAndSize>("WidgetGetPositionAndSize"),
		luaext::FuncReference::GetRef<WidgetSetPositionAndSize>("WidgetSetPositionAndSize"),
		luaext::FuncReference::GetRef<WidgetGetUpdateManualFlag>("WidgetGetUpdateManualFlag"),
		luaext::FuncReference::GetRef<WidgetSetUpdateManualFlag>("WidgetSetUpdateManualFlag"),
		luaext::FuncReference::GetRef<WidgetGetUpdateFunc>("WidgetGetUpdateFunc"),
		luaext::FuncReference::GetRef<WidgetCallUpdateFunc>("WidgetCallUpdateFunc"),
		luaext::FuncReference::GetRef<WidgetOverrideUpdateFunc>("WidgetOverrideUpdateFunc"),
		luaext::FuncReference::GetRef<ContainerWidgetGetAllChildren>("ContainerWidgetGetAllChildren"),
		luaext::FuncReference::GetRef<WidgetMaterialGetTextureCoordinates>("WidgetMaterialGetTextureCoordinates"),
		luaext::FuncReference::GetRef<WidgetMaterialSetTextureCoordinates>("WidgetMaterialSetTextureCoordinates"),
		luaext::FuncReference::GetRef<ButtonGetActionFunc>("ButtonGetActionFunc"),
		luaext::FuncReference::GetRef<ButtonCallActionFunc>("ButtonCallActionFunc"),
		luaext::FuncReference::GetRef<ButtonOverrideActionFunc>("ButtonOverrideActionFunc"),
		luaext::FuncReference::GetRef<WidgetGetTooltipData>("WidgetGetTooltipData"),
		luaext::FuncReference::GetRef<WidgetSetTooltipData>("WidgetSetTooltipData"),
		luaext::FuncReference::GetRef<WidgetGetTooltipString>("WidgetGetTooltipString"),
		luaext::FuncReference::GetRef<WidgetSetTooltipString>("WidgetSetTooltipString"),
		luaext::FuncReference::GetRef<WidgetGetTooltipFunc>("WidgetGetTooltipFunc"),
		luaext::FuncReference::GetRef<WidgetCallTooltipFunc>("WidgetCallTooltipFunc"),
		luaext::FuncReference::GetRef<WidgetOverrideTooltipFunc>("WidgetOverrideTooltipFunc"),
		luaext::FuncReference::GetRef<WidgetIsTooltipShown>("WidgetIsTooltipShown"),
		luaext::FuncReference::GetRef<WidgetSetFont>("WidgetSetFont"),
		luaext::FuncReference::GetRef<WidgetGetBaseData>("WidgetGetBaseData"),
		luaext::FuncReference::GetRef<WidgetSetBaseData>("WidgetSetBaseData"),
		luaext::FuncReference::GetRef<WidgetGetStringFrameDistance>("WidgetGetStringFrameDistance"),
		luaext::FuncReference::GetRef<WidgetSetStringFrameDistance>("WidgetSetStringFrameDistance"),
		luaext::FuncReference::GetRef<StaticTextWidgetGetLineDistanceFactor>("StaticTextWidgetGetLineDistanceFactor"),
		luaext::FuncReference::GetRef<StaticTextWidgetSetLineDistanceFactor>("StaticTextWidgetSetLineDistanceFactor"),
		luaext::FuncReference::GetRef<ButtonGetShortcutString>("ButtonGetShortcutString"),
		luaext::FuncReference::GetRef<ButtonSetShortcutString>("ButtonSetShortcutString"),
		luaext::FuncReference::GetRef<WidgetSetGroup>("WidgetSetGroup"),
		luaext::FuncReference::GetRef<TextButtonSetCenterText>("TextButtonSetCenterText"),
		luaext::FuncReference::GetRef<FontGetConfig>("FontGetConfig"),
		luaext::FuncReference::GetRef<FontSetConfig>("FontSetConfig"),
		luaext::FuncReference::GetRef<PreLoadGUITexture>("PreLoadGUITexture"),
		luaext::FuncReference::GetRef<ContainerWidgetCreateStaticWidgetChild>("ContainerWidgetCreateStaticWidgetChild"),
		luaext::FuncReference::GetRef<ContainerWidgetCreateStaticTextWidgetChild>("ContainerWidgetCreateStaticTextWidgetChild"),
		luaext::FuncReference::GetRef<ContainerWidgetCreatePureTooltipWidgetChild>("ContainerWidgetCreatePureTooltipWidgetChild"),
		luaext::FuncReference::GetRef<ContainerWidgetCreateGFXButtonWidgetChild>("ContainerWidgetCreateGFXButtonWidgetChild"),
		luaext::FuncReference::GetRef<ContainerWidgetCreateTextButtonWidgetChild>("ContainerWidgetCreateTextButtonWidgetChild"),
		luaext::FuncReference::GetRef<ContainerWidgetCreateProgressBarWidgetChild>("ContainerWidgetCreateProgressBarWidgetChild"),
		luaext::FuncReference::GetRef<ContainerWidgetCreateContainerWidgetChild>("ContainerWidgetCreateContainerWidgetChild"),
		luaext::FuncReference::GetRef<ContainerWidgetCreateCustomWidgetChild>("ContainerWidgetCreateCustomWidgetChild"),
		luaext::FuncReference::GetRef<SetCharTrigger>("SetCharTrigger"),
		luaext::FuncReference::GetRef<SetKeyTrigger>("SetKeyTrigger"),
		luaext::FuncReference::GetRef<SetMouseTrigger>("SetMouseTrigger"),
		luaext::FuncReference::GetRef<ShowResourceFloatieOnEntity>("ShowResourceFloatieOnEntity"),
		luaext::FuncReference::GetRef<ShowAdvancedFloatie>("ShowAdvancedFloatie"),
		luaext::FuncReference::GetRef<GetClientSize>("GetClientSize"),
		luaext::FuncReference::GetRef<GetMousePosDirect>("GetMousePosDirect"),
		luaext::FuncReference::GetRef<IsContainerWidget>("IsContainerWidget"),
		luaext::FuncReference::GetRef<GetWidgetName>("GetWidgetName"),
		luaext::FuncReference::GetRef<SetGUIStateLuaSelection>("SetGUIStateLuaSelection"),
		luaext::FuncReference::GetRef<GetLandscapePosAtScreenPos>("GetLandscapePosAtScreenPos"),
		luaext::FuncReference::GetRef<ShowCommandAcknowledgementAtPosition>("ShowCommandAcknowledgementAtPosition"),
		luaext::FuncReference::GetRef<CreateMiniMapMarker>("CreateMiniMapMarker"),
		luaext::FuncReference::GetRef<CreateMiniMapScriptSignal>("CreateMiniMapScriptSignal"),
		luaext::FuncReference::GetRef<GetCutsceneFarClipPlaneMinAndMax>("GetCutsceneFarClipPlaneMinAndMax"),
		luaext::FuncReference::GetRef<SetCutsceneFarClipPlaneMinAndMax>("SetCutsceneFarClipPlaneMinAndMax"),
		luaext::FuncReference::GetRef<ReloadGUI>("ReloadGUI"),
		luaext::FuncReference::GetRef<SetShowWoodInUI>("SetShowWoodInUI"),
		luaext::FuncReference::GetRef<SetGUIStatePlaceBuildingEx>("SetGUIStatePlaceBuildingEx"),
		luaext::FuncReference::GetRef<SetPlaceBuildingRotation>("SetPlaceBuildingRotation"),
		luaext::FuncReference::GetRef<GetPlaceBuildingRotation>("GetPlaceBuildingRotation"),
		luaext::FuncReference::GetRef<GetPlaceBuildingUCat>("GetPlaceBuildingUCat"),
		luaext::FuncReference::GetRef<GetWidgetAtPosition>("GetWidgetAtPosition"),
		luaext::FuncReference::GetRef<StringInputWidgetGetIgnoreNextChar>("StringInputWidgetGetIgnoreNextChar"),
		luaext::FuncReference::GetRef<StringInputWidgetSetIgnoreNextChar>("StringInputWidgetSetIgnoreNextChar"),
		luaext::FuncReference::GetRef<StringInputWidgetSetBufferSize>("StringInputWidgetSetBufferSize"),
		luaext::FuncReference::GetRef<GetShortMessages>("GetShortMessages"),
		luaext::FuncReference::GetRef<ReInitShortMessagesSize>("ReInitShortMessagesSize"),
		luaext::FuncReference::GetRef<CreateShortMessage>("CreateShortMessage"),
		luaext::FuncReference::GetRef<RemoveShortMessage>("RemoveShortMessage"),
		luaext::FuncReference::GetRef<InitAutoScrollCustomWidget>("InitAutoScrollCustomWidget"),
		luaext::FuncReference::GetRef<GetAutoScrollCustomWidgetOffset>("GetAutoScrollCustomWidgetOffset"),
		luaext::FuncReference::GetRef<AutoScrollCustomWidgetModOffset>("AutoScrollCustomWidgetModOffset"),
		luaext::FuncReference::GetRef<AutoScrollCustomWidgetSetOffset>("AutoScrollCustomWidgetSetOffset"),
		luaext::FuncReference::GetRef<AutoScrollCustomWidgetSetMaterial>("AutoScrollCustomWidgetSetMaterial"),
		luaext::FuncReference::GetRef<TextInputCustomWidgetGetText>("TextInputCustomWidgetGetText"),
		luaext::FuncReference::GetRef<TextInputCustomWidgetSetText>("TextInputCustomWidgetSetText"),
		luaext::FuncReference::GetRef<TextInputCustomWidgetSetIgnoreNextChar>("TextInputCustomWidgetSetIgnoreNextChar"),
		luaext::FuncReference::GetRef<InputCustomWidgetHasFocus>("InputCustomWidgetHasFocus"),
		luaext::FuncReference::GetRef<InputCustomWidgetSetFocus>("InputCustomWidgetSetFocus"),
		luaext::FuncReference::GetRef<DumpVideoModes>("DumpVideoModes"),
		luaext::FuncReference::GetRef<GetCameraData>("GetCameraData"),
		luaext::FuncReference::GetRef<SetCameraData>("SetCameraData"),
		luaext::FuncReference::GetRef<ListCutscenes>("ListCutscenes"),
		luaext::FuncReference::GetRef<GetCutscene>("GetCutscene"),
		luaext::FuncReference::GetRef<SetCutscene>("SetCutscene"),
		luaext::FuncReference::GetRef<ExportCutscenes>("ExportCutscenes"),
		luaext::FuncReference::GetRef<VideoCustomWidgetGetVideoSize>("VideoCustomWidgetGetVideoSize"),
		luaext::FuncReference::GetRef<VideoCustomWidgetSetVideoSize>("VideoCustomWidgetSetVideoSize"),
		luaext::FuncReference::GetRef<CreateSelectionDecal>("CreateSelectionDecal"),
		luaext::FuncReference::GetRef<InitNetHandlers>("InitNetHandlers"),
		luaext::FuncReference::GetRef<RemoveWidget>("RemoveWidget"),
		luaext::FuncReference::GetRef<ReorderWidgets>("ReorderWidgets"),
		luaext::FuncReference::GetRef<MiniMapOverlaySetCallbackFuncName>("MiniMapOverlaySetCallbackFuncName"),
	};

	void CheckConstruct(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		if (e->PlayerId != GGUI::CManager::GlobalObj()->ControlledPlayer)
			throw lua::LuaException{ "player not controlled" };
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		if (!e->IsEntityInCategory(shok::EntityCategory::Serf))
			throw lua::LuaException{ "not a serf" };
		if (dynamic_cast<GGL::CBuilding*>(oth) == nullptr)
			throw lua::LuaException{ "not a building" };
		if (oth->IsEntityInCategory(shok::EntityCategory::Bridge))
			return;
		if (e->PlayerId != oth->PlayerId)
			throw lua::LuaException{ "not same player" };
	}

	void CheckRepair(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		if (e->PlayerId != GGUI::CManager::GlobalObj()->ControlledPlayer)
			throw lua::LuaException{ "player not controlled" };
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		if (!e->IsEntityInCategory(shok::EntityCategory::Serf))
			throw lua::LuaException{ "not a serf" };
		if (dynamic_cast<GGL::CBuilding*>(oth) == nullptr)
			throw lua::LuaException{ "not a building" };
		if (e->PlayerId != oth->PlayerId)
			throw lua::LuaException{ "not same player" };
	}

	void CheckExtract(GGL::CNetEventExtractResource& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.SerfID);
		if (e->PlayerId != GGUI::CManager::GlobalObj()->ControlledPlayer)
			throw lua::LuaException{ "player not controlled" };
		if (!e->IsEntityInCategory(shok::EntityCategory::Serf))
			throw lua::LuaException{ "not a serf" };
	}

	void CheckPlaceCannonEvent(GGL::CNetEventCannonCreator& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		ev.Position.FloorToBuildingPlacement();
		auto bottom = ev.FoundationType;
		GGlue::CGlueEntityProps* ety = CppLogic::GetEntityType(bottom);
		if (!ety)
			throw lua::LuaException("no bottom entitytype");
		if (!ety->IsBuildingType())
			throw lua::LuaException("bottom not a building");
		auto top = ev.CannonType;
		if (!CppLogic::GetEntityType(top))
			throw lua::LuaException("no top entitytype");
		if (!GGL::CPlayerStatus::CanPlaceBuilding(bottom, e->PlayerId, &ev.Position, ev.Orientation, shok::EntityId::Invalid))
			throw lua::LuaException("cannot place foundation at that position");
	}

	void CheckIsThief(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		if (e->GetBehavior<GGL::CThiefBehavior>() == nullptr)
			throw lua::LuaException("not a thief");
	}
	void CheckSabotageEvent(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		if (!(oth->IsEntityInCategory(shok::EntityCategory::Bridge) || GGL::CPlayerStatus::ArePlayersHostile(e->PlayerId, oth->PlayerId)))
			throw lua::LuaException("target is not hostile or bridge");
	}
	void CheckThiefNotCarryingEvent(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		if (!(e->GetBehavior<GGL::CThiefBehavior>()->ResourceType == shok::ResourceType::None))
			throw lua::LuaException("is carrying resources");
	}
	void CheckDefuseEvent(EGL::CNetEvent2Entities& ev) {
		auto* t = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		if (!t->GetBehavior<GGL::CKegBehavior>())
			throw lua::LuaException("target is no keg");
	}
	void CheckResurrectEvent(EGL::CNetEvent2Entities& ev) {
		auto* t = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		if (t->GetBehavior<GGL::CHeroBehavior>() == nullptr)
			throw lua::LuaException("not a hero");
		if (!t->IsDead())
			throw lua::LuaException("is alive");
	}
	void CheckSnipeEvent(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		auto* bp = e->GetEntityType()->GetBehaviorProps<GGL::CSniperAbilityProps>();
		if (!e->Position.IsInRange(oth->Position, bp->Range))
			throw lua::LuaException("target not in range");
	}
	void CheckShurikenEvent(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		auto* bp = e->GetEntityType()->GetBehaviorProps<GGL::CShurikenAbilityProps>();
		if (!e->Position.IsInRange(oth->Position, bp->Range))
			throw lua::LuaException("target not in range");
	}
	void CheckLightingStrike(EGL::CNetEventEntityAndPos& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		auto* bp = e->GetEntityType()->GetBehaviorProps<CppLogic::Mod::LightningStrikeAbilityProps>();
		if (!e->Position.IsInRange(shok::Position{ ev.X, ev.Y }, bp->Range))
			throw lua::LuaException("not in range");
		if ((*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId)->CurrentResources.WeatherEnergy < bp->WeatherEnergyCost)
			throw lua::LuaException("not enough weather energy");
	}
	void CheckResourceRefill(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		auto* t = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		auto* bp = e->GetEntityType()->GetBehaviorProps<CppLogic::Mod::ResDoodadRefillBehaviorProps>();
		auto* res = EGL::CGLEEntity::GetEntityByID(t->GetFirstAttachedEntity(shok::AttachmentType::MINE_RESOURCE));
		if (res == nullptr)
			throw lua::LuaException("no mine");
		if (!res->IsEntityInCategory(bp->AffectedTypes))
			throw lua::LuaException("not affectable");
	}
	void CheckBombardment(EGL::CNetEventEntityAndPos& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		auto* bp = e->GetEntityType()->GetBehaviorProps<CppLogic::Mod::BombardmentAbilityProps>();
		if (!e->Position.IsInRange(shok::Position{ ev.X, ev.Y }, bp->AttackRange))
			throw lua::LuaException("not in range");
	}

	int CommandMove(luaext::State L) {
		auto* s = L.CheckSettler(1);
		auto p = L.CheckPos(2);
		float r = L.OptFloat(3, -1);
		EGL::CNetEventEntityAndPosArray ev{ shok::NetEventIds::Entity_Move, s->EntityId, r };
		LuaEventInterface::CheckEntityOfLocalPlayer(ev);
		{
			auto v = ev.Position.SaveVector();
			v.Vector.push_back(p);
		}
		GGUI::CManager::PostEventFromUI(&ev);
		return 0;
	}
	int CommandPatrol(luaext::State L) {
		auto* s = L.CheckSettler(1);
		EGL::CNetEventEntityAndPosArray ev{ shok::NetEventIds::Entity_Patrol, s->EntityId, -1 };
		LuaEventInterface::CheckEntityOfLocalPlayer(ev);
		{
			auto v = ev.Position.SaveVector();
			for (int i = 2; i <= L.GetTop(); ++i) {
				auto p = L.CheckPos(i);
				v.Vector.push_back(p);
			}
		}
		if (ev.Position.size() == 0)
			throw lua::LuaException("needs more than one pos");
		GGUI::CManager::PostEventFromUI(&ev);
		return 0;
	}

	constexpr std::array Commands{ // move, patrol
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Entity_AttackEntity,
			LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>>>("Entity_AttackEntity"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Serf_ConstructBuilding,
			CheckConstruct>>("Serf_ConstructBuilding"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Serf_RepairBuilding,
			CheckRepair>>("Serf_RepairBuilding"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<GGL::CNetEventExtractResource, shok::NetEventIds::Serf_ExtractResource,
			CheckExtract>>("Serf_ExtractResource"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::Entity_AttackPos,
			LuaEventInterface::CheckEntityOfLocalPlayer>>("Entity_AttackPos"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Entity_GuardEntity,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityDiploState<shok::DiploState::Friendly>>>("Entity_GuardEntity"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::BombPlacer_PlaceBombAt,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityPlaceBomb>>>("BombPlacer_PlaceBombAt"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<GGL::CNetEventCannonCreator, shok::NetEventIds::CannonPlacer_HeroPlaceCannonAt,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityBuildCannon>,
			CheckPlaceCannonEvent>>("CannonPlacer_HeroPlaceCannonAt"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Hero_NPCInteraction,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityActorCategory<shok::EntityCategory::Hero>>>("Hero_NPCInteraction"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::ConvertSettler_Convert,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckActorAbility<shok::AbilityId::AbilityConvertSettlers>,
			LuaEventInterface::CheckConvertible, LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>,
			LuaEventInterface::CheckSettler>>("ConvertSettler_Convert"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Thief_StealFrom,
			LuaEventInterface::CheckEntityOfLocalPlayer, CheckIsThief,
			LuaEventInterface::CheckBuilding, LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>>>("Thief_StealFrom"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Thief_CarryStolenStuffToHQ,
			LuaEventInterface::CheckEntityOfLocalPlayer, CheckIsThief,
			LuaEventInterface::CheckBuilding, LuaEventInterface::CheckTargetCategory<shok::EntityCategory::Headquarters>,
			LuaEventInterface::CheckEntityDiploState<shok::DiploState::Friendly>>>("Thief_CarryStolenStuffToHQ"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Sabotage_Sabotage,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckActorAbility<shok::AbilityId::AbilityPlaceKeg>,
			LuaEventInterface::CheckBuilding, CheckThiefNotCarryingEvent,
			CheckSabotageEvent>>("Sabotage_Sabotage"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Sabotage_Defuse,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckActorAbility<shok::AbilityId::AbilityPlaceKeg>,
			CheckThiefNotCarryingEvent, CheckSabotageEvent>>("Sabotage_Defuse"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::Binoculars_Observe,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityScoutBinoculars>>>("Binoculars_Observe"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Sniper_Snipe,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckActorAbility<shok::AbilityId::AbilitySniper>, CheckSnipeEvent,
			LuaEventInterface::CheckSettler, LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>>>("Sniper_Snipe"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::TorchPlacer_Place,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityScoutTorches>>>("TorchPlacer_Place"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Shuriken_Activate,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckActorAbility<shok::AbilityId::AbilityShuriken>, CheckShurikenEvent,
			LuaEventInterface::CheckSettler, LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>>>("Shuriken_Activate"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::CppL_LightningStrike_Activate, CheckLightingStrike,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityLightningStrike>>>("LightningStrike_Activate"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::CppL_ResDoodadRefill_Activate,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckBuilding, CheckResourceRefill,
			LuaEventInterface::CheckActorAbility<shok::AbilityId::AbiltyResourceDoodadRefill>>>("ResDoodadRefill_Activate"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityID, shok::NetEventIds::CppL_ShieldCover_Activate,
			LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityShieldCover>>>("ShieldCover_Activate"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::CppL_Resurrect_Activate,
			LuaEventInterface::CheckEntityOfLocalPlayer, CheckResurrectEvent,
			LuaEventInterface::CheckActorAbility<shok::AbilityId::AbilityResurrect>>>("Resurrect_Activate"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::CppL_BombardmentActivate,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityBombardment>>>("Bombardment_Activate"),
		luaext::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::CppL_BombComboCannonActivate,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityPlaceBomb>,
			LuaEventInterface::CheckEntityBehavior<CppLogic::Mod::ReloadableCannonBuilderAbility>>>("BombCannonCombo_Activate"),
		luaext::FuncReference::GetRef<CommandMove>("Entity_Move"),
		luaext::FuncReference::GetRef<CommandPatrol>("Entity_Patrol"),
	};

	void Init(luaext::State L)
	{
		L.RegisterFuncs(UI, -3);
#ifdef DEBUG_FUNCS
		L.RegisterFunc<WidgetGetAddress>("WidgetGetAddress", -3);
#endif
		L.Push("Commands");
		L.NewTable();
		L.RegisterFuncs(Commands, -3);
		L.SetTableRaw(-3);

		L.Push("TextInputCustomWidgetEvent");
		GetIdManager<Mod::UI::TextInputCustomWidget::Event>().PushToState(L);
		L.SetTableRaw(-3);

		if (L.GetState() == shok::LuaStateMainmenu) {
			L.RegisterFunc<SetMouseTriggerMainMenu>("SetMouseTriggerMainMenu", -3);
			CppLogic::UI::GUIState_LuaSelection::Initialize();
			GUIState_PlaceBuildingEx::Initialize();
		}
	}
}

/*
 CppLogic.UI.WidgetGetAddress("StartMenu00_EndGame")
 CppLogic.UI.WidgetGetAddress("AutoAddSerfJobs")
 StartMenu00_VersionNumber
 StartMenu00_EndGame
 StartMenu00
 GoldTooltipController
 CppLogic.UI.ButtonOverrideActionFunc("StartMenu00_EndGame", function() LuaDebugger.Log(XGUIEng.GetCurrentWidgetID()) end)
 CppLogic.UI.ContainerWidgetCreateStaticWidgetChild("StartMenu00", "test", "StartMenu00_EndGame"); XGUIEng.SetMaterialTexture("test", 0, "data\\graphics\\textures\\gui\\hero_sel_dario.png"); CppLogic.UI.WidgetSetPositionAndSize("test", 783, 500, 32, 32); XGUIEng.ShowWidget("test", 1);
 CppLogic.UI.ContainerWidgetCreateStaticTextWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.SetMaterialColor("test", 0, 0,0,0,0); XGUIEng.SetTextColor("test",255,255,255,255); CppLogic.UI.WidgetSetFont("test", "data\\menu\\fonts\\standardinvert10.met"); XGUIEng.SetText("test","tst"); XGUIEng.ShowWidget("test", 1);
 CppLogic.UI.ContainerWidgetCreatePureTooltipWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); CppLogic.UI.WidgetSetTooltipData("test", "StartMenu_TooltipText", true, true); CppLogic.UI.WidgetSetTooltipString("test", "tst"); CppLogic.UI.WidgetOverrideTooltipFunc("test", function() LuaDebugger.Log(1) end); XGUIEng.ShowWidget("test", 1);
 CppLogic.UI.ContainerWidgetCreateGFXButtonWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.ShowWidget("test", 1);
 CppLogic.UI.ContainerWidgetCreateTextButtonWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.SetText("test","tst"); CppLogic.UI.WidgetSetFont("test", "data\\menu\\fonts\\standardinvert10.met"); XGUIEng.ShowWidget("test", 1);
 CppLogic.UI.ContainerWidgetCreateProgressBarWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.ShowWidget("test", 1);
 CppLogic.UI.ContainerWidgetCreateContainerWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 100, 100); XGUIEng.ShowWidget("test", 1);
 CppLogic.UI.WidgetSetFont("StartMenu00_EndGame", "data\\menu\\fonts\\medium11bold.met") --"data\\menu\\fonts\\mainmenularge.met"
 CppLogic.UI.FontGetConfig("data\\menu\\fonts\\standard10.met")
 XGUIEng.SetText("StartMenu00_EndGame", "@center @color:0,255,0 test")
 XGUIEng.SetText("StartMenu00_EndGame", "test@icon:graphics\\textures\\gui\\i_refine_iron|test")
 XGUIEng.SetText("StartMenu00_EndGame", "test@icon:data\\graphics\\textures\\gui\\b_units_heroes.png,2.500000e-001,0.000000e+000,2.500000e-001,6.250000e-002|test") XGUIEng.SetText("StartMenu00_StartSinglePlayer", "test@icon:data\\graphics\\textures\\gui\\b_units_heroes.png,2.500000e-001,0.000000e+000,2.500000e-001,6.250000e-002|test")
 CppLogic.UI.RemoveWidget("StartMenu00_VersionNumber")
 CppLogic.UI.SetGUIStateLuaSelection(function(x, y) LuaDebugger.Log(x.."/"..y) end)
 CppLogic.UI.ContainerWidgetCreateCustomWidgetChild("StartMenu00", "test", "EGUIX::CVideoPlaybackCustomWidget"); XGUIEng.ShowWidget("test", 1); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 250, 250); XGUIEng.StartVideoPlayback("test", "data\\graphics\\videos\\pu_farmer.bik", 1);
 CppLogic.UI.ContainerWidgetCreateCustomWidgetChild("StartMenu00", "test1", "CppLogic::Mod::UI::TextInputCustomWidget", nil, 0, 0, 0, 0, 0, 0, "LuaDebugger.Log", "LuaDebugger.Log"); XGUIEng.ShowWidget("test1", 1); CppLogic.UI.WidgetSetPositionAndSize("test1", 0, 0, 250, 250);
 CppLogic.UI.ContainerWidgetCreateCustomWidgetChild("StartMenu00", "test2", "CppLogic::Mod::UI::TextInputCustomWidget", nil, 1, 0, 0, 0, 0, 0, "LuaDebugger.Log", "LuaDebugger.Log"); XGUIEng.ShowWidget("test2", 1); CppLogic.UI.WidgetSetPositionAndSize("test2", 250, 0, 250, 250);
 CppLogic.UI.ContainerWidgetCreateCustomWidgetChild("StartMenu00", "test3", "CppLogic::Mod::UI::TextInputCustomWidget", nil, 2, 0, 0, 0, 0, 0, "LuaDebugger.Log", "LuaDebugger.Log"); XGUIEng.ShowWidget("test3", 1); CppLogic.UI.WidgetSetPositionAndSize("test3", 500, 0, 250, 250);
 */
