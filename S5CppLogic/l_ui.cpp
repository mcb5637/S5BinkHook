#include "pch.h"
#include "l_ui.h"
#include <filesystem>
#include <WinUser.h>
#include "s5_widget.h"
#include "s5_filesystem.h"
#include "s5_classfactory.h"
#include "s5_framework.h"
#include "s5_idmanager.h"
#include "s5_mapdisplay.h"
#include "s5_entity.h"
#include "s5_events.h"
#include "s5_defines.h"
#include "s5_RWE_2d.h"
#include "s5_ui.h"
#include "s5_netevents.h"
#include "s5_cutscene.h"
#include "hooks.h"
#include "luaserializer.h"
#include "ModUI.h"
#include "LuaEventInterface.h"
#include "WinAPIUtil.h"
#include "ModBehavior.h"

namespace CppLogic::UI {
	void StringHandlerSetString(luaext::EState L, EGUIX::CSingleStringHandler& h, int i) {
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
		if ((str.rfind("data\\maps\\externalmap\\", 0) == std::string::npos) && (str.rfind("data\\menu\\fonts\\", 0) == std::string::npos))
			throw lua::LuaException("incorrect folder");
	}

	int WidgetGetAddress(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* w = L.CheckWidget(1);
		L.Push(reinterpret_cast<int>(w));
		EGUIX::CLuaFunctionHelper* bh = w->GetUpdateFunc();
		if (bh) {
			L.Push((int)bh);
			return 2;
		}
		return 1;
	}

	int WidgetGetPositionAndSize(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* w = L.CheckWidget(1);
		L.Push(w->PosAndSize.X);
		L.Push(w->PosAndSize.Y);
		L.Push(w->PosAndSize.W);
		L.Push(w->PosAndSize.H);
		return 4;
	}

	int WidgetSetPositionAndSize(lua::State ls) {
		luaext::EState L{ ls };
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

	int WidgetGetUpdateManualFlag(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		bool* f = wid->GetUpdateManualFlag();
		if (f) {
			L.Push(*f);
			return 1;
		}
		throw lua::LuaException("widget has no known updatemanual flag");
	}
	int WidgetSetUpdateManualFlag(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		bool flg = L.ToBoolean(2);
		bool* f = wid->GetUpdateManualFlag();
		if (f) {
			*f = flg;
			return 0;
		}
		throw lua::LuaException("widget has no known updatemanual flag");
	}

	int WidgetGetUpdateFunc(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CLuaFunctionHelper* fh = wid->GetUpdateFunc();
		if (!fh)
			throw lua::LuaException("widget has no known update func");
		L.Push(fh->LuaCommand.c_str());
		if (fh->FuncRefCommand.L.GetState() == L.GetState())
			L.Push(lua::Reference{ fh->FuncRefCommand.Ref }, L.REGISTRYINDEX);
		else
			L.Push("no compiled func found");
		return 2;
	}
	int WidgetCallUpdateFunc(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CLuaFunctionHelper* fh = wid->GetUpdateFunc();
		if (!fh)
			throw lua::LuaException("widget has no known update func");
		fh->Call(wid->WidgetID);
		return 0;
	}
	int WidgetOverrideUpdateFunc(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CLuaFunctionHelper* fh = wid->GetUpdateFunc();
		if (!fh)
			throw lua::LuaException("widget has no known update func");
		fh->FuncRefCommand.ReplaceFunc(ls, 2);
		return 0;
	}

	int ContainerWidgetGetAllChildren(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CContainerWidget* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
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

	int IsContainerWidget(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		if (dynamic_cast<EGUIX::CContainerWidget*>(wid))
			L.Push(true);
		else
			L.Push(false);
		return 1;
	}

	int WidgetMaterialGetTextureCoordinates(lua::State ls) {
		luaext::EState L{ ls };
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
	int WidgetMaterialSetTextureCoordinates(lua::State ls) {
		luaext::EState L{ ls };
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

	int WidgetGetTooltipData(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		L.Push(tt->TargetWidget);
		L.Push(tt->ControlTargetWidgetDisplayState);
		L.Push(tt->ToolTipEnabledFlag);
		return 3;
	}
	int WidgetSetTooltipData(lua::State ls) {
		luaext::EState L{ ls };
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

	int WidgetGetTooltipString(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		L.Push(tt->ToolTipString.RawString.c_str());
		L.Push(tt->ToolTipString.StringTableKey.c_str());
		return 2;
	}
	int WidgetSetTooltipString(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		StringHandlerSetString(L, tt->ToolTipString, 2);
		return 0;
	}

	int WidgetGetTooltipFunc(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		L.Push(tt->UpdateFunction.LuaCommand.c_str());
		if (tt->UpdateFunction.FuncRefCommand.L.GetState() == L.GetState())
			L.Push(lua::Reference{ tt->UpdateFunction.FuncRefCommand.Ref }, L.REGISTRYINDEX);
		else
			L.Push("no compiled func found");
		return 2;
	}
	int WidgetCallTooltipFunc(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		tt->UpdateFunction.Call(wid->WidgetID);
		return 0;
	}
	int WidgetOverrideTooltipFunc(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		tt->UpdateFunction.FuncRefCommand.ReplaceFunc(L, 2);
		return 0;
	}

	int ButtonGetActionFunc(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CButtonHelper* fh = wid->GetButtonHelper();
		if (!fh)
			throw lua::LuaException("widget has no known action func");
		L.Push(fh->ActionFunction.LuaCommand.c_str());
		if (fh->ActionFunction.FuncRefCommand.L.GetState() == L.GetState())
			L.Push(lua::Reference{ fh->ActionFunction.FuncRefCommand.Ref }, L.REGISTRYINDEX);
		else
			L.Push("no compiled func found");
		return 2;
	}
	int ButtonCallActionFunc(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CButtonHelper* fh = wid->GetButtonHelper();
		if (!fh)
			throw lua::LuaException("widget has no known action func");
		fh->ActionFunction.Call(wid->WidgetID);
		return 0;
	}
	int ButtonOverrideActionFunc(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CButtonHelper* fh = wid->GetButtonHelper();
		if (!fh)
			throw lua::LuaException("widget has no known action func");
		fh->ActionFunction.FuncRefCommand.ReplaceFunc(L, 2);
		return 0;
	}

	int WidgetIsTooltipShown(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CToolTipHelper* tt = wid->GetTooltipHelper();
		if (!tt)
			throw lua::LuaException("no known tooltip");
		L.Push(tt->IsToolTipShown);
		return 1;
	}

	int WidgetSetFont(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CWidgetStringHelper* s = wid->GetStringHelper();
		if (!s)
			throw lua::LuaException("no known stringhelper");
		const char* font = L.CheckString(2);
		CheckFontString(font);
		s->FontHandler.LoadFont(font);
		return 0;
	}

	int WidgetGetBaseData(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		L.Push(wid->ZPriority);
		L.Push(wid->ForceToHandleMouseEventsFlag);
		L.Push(wid->ForceToNeverBeFoundFlag);
		return 3;
	}
	int WidgetSetBaseData(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		if (L.IsNumber(2))
			wid->ZPriority = L.CheckFloat(2);
		if (L.IsBoolean(3))
			wid->ForceToHandleMouseEventsFlag = L.ToBoolean(3);
		if (L.IsBoolean(4))
			wid->ForceToNeverBeFoundFlag = L.ToBoolean(4);
		return 0;
	}

	int WidgetGetStringFrameDistance(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CWidgetStringHelper* s = wid->GetStringHelper();
		if (!s)
			throw lua::LuaException("no string helper");
		L.Push(s->StringFrameDistance);
		return 1;
	}
	int WidgetSetStringFrameDistance(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CWidgetStringHelper* s = wid->GetStringHelper();
		if (!s)
			throw lua::LuaException("no string helper");
		s->StringFrameDistance = L.CheckFloat(2);
		return 0;
	}

	int StaticTextWidgetGetLineDistanceFactor(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CStaticTextWidget* tw = dynamic_cast<EGUIX::CStaticTextWidget*>(wid);
		if (!tw)
			throw lua::LuaException("no static text widget");
		L.Push(tw->LineDistanceFactor);
		return 1;
	}
	int StaticTextWidgetSetLineDistanceFactor(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CStaticTextWidget* tw = dynamic_cast<EGUIX::CStaticTextWidget*>(wid);
		if (!tw)
			throw lua::LuaException("no static text widget");
		tw->LineDistanceFactor = L.CheckFloat(2);
		return 0;
	}

	int ButtonGetShortcutString(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CButtonHelper* b = wid->GetButtonHelper();
		if (!b)
			throw lua::LuaException("no button");
		L.Push(b->ShortCutString.RawString.c_str());
		L.Push(b->ShortCutString.StringTableKey.c_str());
		return 2;
	}
	int ButtonSetShortcutString(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CButtonHelper* b = wid->GetButtonHelper();
		if (!b)
			throw lua::LuaException("no button");
		StringHandlerSetString(L, b->ShortCutString, 2);
		return 0;
	}

	int WidgetSetGroup(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		const char* s = L.CheckString(2);
		EGUIX::CWidgetGroupManager* wgm = EGUIX::CWidgetGroupManager::GlobalObj();
		auto g = wgm->GetGroupId(s);
		if (g == shok::WidgetGroupId::Invalid)
			g = wgm->CreateGroup(s);
		wid->Group = g;
		return 0;
	}

	int TextButtonSetCenterText(lua::State ls) {
		luaext::EState L{ ls };
		auto* b = BB::IdentifierCast<EGUIX::CTextButtonWidget>(L.CheckWidget(1));
		if (b == nullptr)
			throw lua::LuaException{ "no textbutton" };
		b->CppLogic_CenterText = L.CheckBool(2);
		return 0;
	}

	int ContainerWidgetCreateStaticWidgetChild(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CContainerWidget* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
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
	int ContainerWidgetCreateStaticTextWidgetChild(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CContainerWidget* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
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
	int ContainerWidgetCreatePureTooltipWidgetChild(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CContainerWidget* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
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
	int ContainerWidgetCreateGFXButtonWidgetChild(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CContainerWidget* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
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
	int ContainerWidgetCreateTextButtonWidgetChild(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CContainerWidget* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
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
	int ContainerWidgetCreateProgressBarWidgetChild(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CContainerWidget* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
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
	int ContainerWidgetCreateContainerWidgetChild(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CContainerWidget* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
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
	int ContainerWidgetCreateCustomWidgetChild(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CContainerWidget* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
		if (!c)
			throw lua::LuaException("no container widget");
		const char* name = L.CheckString(2);
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != shok::WidgetId::Invalid)
			throw lua::LuaException("name already in use");
		EGUIX::CBaseWidget* bef = nullptr;
		const char* customname = L.CheckString(3);
		if (!L.IsNoneOrNil(4))
			bef = L.CheckWidget(4);
		EGUIX::CCustomWidget* ne = (*BB::CClassFactory::GlobalObj)->CreateObject<EGUIX::CCustomWidget>();
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

	int FontGetConfig(lua::State ls) {
		luaext::EState L{ ls };
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
	int FontSetConfig(lua::State ls) {
		luaext::EState L{ ls };
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

	int PreLoadGUITexture(lua::State ls) {
		luaext::EState L{ ls };
		const char* n = L.CheckString(1);
		if (!BB::CFileSystemMgr::DoesFileExist(n))
			throw lua::LuaException{ "files does not exist" };
		auto* m = EGUIX::TextureManager::GlobalObj();
		auto id = m->GetTextureID(n);
		m->GetTextureByID(id);
		L.Push(id);
		return 1;
	}

	int RemoveWidget(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CContainerWidget* cw = dynamic_cast<EGUIX::CContainerWidget*>(wid);
		if (cw)
			if (cw->WidgetListHandler.SubWidgets.size() != 0)
				throw lua::LuaException("container widget has to be empty");
		delete wid;
		return 0;
	}

	int ReloadGUI(lua::State L) {
		const char* str = L.OptString(1, "Data\\Menu\\Projects\\Ingame.xml");
		if (!BB::CFileSystemMgr::DoesFileExist(str))
			throw lua::LuaException{ "files does not exist" };
		EGUIX::WidgetLoader::GlobalObj()->LoadGUI(str);
		return 0;
	}

	int SetShowWoodInUI(lua::State L) {
		bool s = L.CheckBool(1);
		GGUI::C3DOnScreenInformationCustomWidget::HookResourceFloatieShowWood(s);
		GGUI::C3DOnScreenInformationCustomWidget::HookResourceElementWood(s);
		return 0;
	}

	bool SetCharTrigger_CB(int c) {
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
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
	int SetCharTrigger(lua::State ls) {
		luaext::EState L{ ls };
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
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
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
	int SetKeyTrigger(lua::State ls) {
		luaext::EState L{ ls };
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

		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
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
	int SetMouseTrigger(lua::State ls) {
		luaext::EState L{ ls };
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
	int SetMouseTriggerMainMenu(lua::State ls) {
		luaext::EState L{ ls };
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

				lua::State L{ shok::LuaStateMainmenu };
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

	int ShowResourceFloatieOnEntity(lua::State ls) {
		luaext::EState L{ ls };
		(*GGUI::C3DOnScreenInformationCustomWidget::GlobalObj)->ShowResourceFloatieOnEntity(L.CheckEntity(1)->EntityId, L.CheckInt(2));
		return 0;
	}

	int ShowAdvancedFloatie(lua::State ls) {
		luaext::EState L{ ls };
		GGUI::C3DOnScreenInformationCustomWidget::HookAdditionalFloaties();
		GGUI::AdvancedFloatieManager::GlobalObj.AddFloatie(L.CheckPos(1), L.CheckStringView(2));
		return 0;
	}

	int GetClientSize(lua::State L) {
		RECT r;
		if (GetClientRect(*shok::MainWindowHandle, &r)) {
			L.Push(static_cast<double>(r.right));
			L.Push(static_cast<double>(r.bottom));
			return 2;
		}
		return 0;
	}

	int GetWidgetName(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* w = L.CheckWidget(1);
		L.Push(EGUIX::WidgetManager::GlobalObj()->WidgetNameManager->GetNameByID(static_cast<int>(w->WidgetID)));
		return 1;
	}

	int GetLandscapePosAtScreenPos(lua::State ls) {
		luaext::EState L{ ls };
		int x = L.CheckInt(1);
		int y = L.CheckInt(2);
		shok::PositionRot p;
		if ((*ED::CGlobalsLogicEx::GlobalObj)->Landscape->GetTerrainPosAtScreenCoords(p, x, y)) {
			L.PushPos(p);
			L.Push(p.r);
			return 2;
		}
		return 0;
	}

	int ShowCommandAcknowledgementAtPosition(lua::State ls) {
		luaext::EState L{ ls };
		shok::Position p = L.CheckPos(1);
		(*ED::CGlobalsBaseEx::GlobalObj)->CommandAcks->ShowAck(p);
		return 0;
	}

	int CreateMiniMapMarker(lua::State ls) {
		luaext::EState L{ ls };
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

	int CreateMiniMapScriptSignal(lua::State ls) {
		luaext::EState L{ ls };
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

	int GetCutsceneFarClipPlaneMinAndMax(lua::State L) {
		L.Push(*ERwTools::CRwCameraHandler::CutsceneFarClipPlaneMax);
		L.Push(*ERwTools::CRwCameraHandler::CutsceneFarClipPlaneMin);
		return 2;
	}
	int SetCutsceneFarClipPlaneMinAndMax(lua::State L) {
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

	int SetGUIStateLuaSelection(lua::State ls) {
		luaext::EState L{ ls };
		L.CheckType(1, lua::LType::Function);
		auto* vh = GGUI::CManager::GlobalObj()->C3DViewHandler;
		vh->StateIdManager->GetIDByNameOrCreate(GUIState_LuaSelection::Name, GUIState_LuaSelection::Id); // make sure the state id exists
		vh->SetGUIState<CppLogic::UI::GUIState_LuaSelection>();
		CppLogic::UI::GUIState_LuaSelection* s = dynamic_cast<CppLogic::UI::GUIState_LuaSelection*>(vh->CurrentState);
		L.PushValue(1);
		s->RefOnKlick = L.Ref(L.REGISTRYINDEX);
		if (!L.IsNoneOrNil(2)) {
			L.CheckType(2, lua::LType::Function);
			L.PushValue(2);
			s->RefOnCancel = L.Ref(L.REGISTRYINDEX);
		}
		return 0;
	}

	int SetGUIStatePlaceBuildingEx(lua::State ls) {
		luaext::EState L{ ls };
		auto* vh = GGUI::CManager::GlobalObj()->C3DViewHandler;
		vh->StateIdManager->GetIDByNameOrCreate(GUIState_PlaceBuildingEx::Name, GUIState_PlaceBuildingEx::Id); // make sure the state id exists
		GGUI::SPlaceBuildingStateParameters p{ L.CheckEnum<shok::UpgradeCategoryId>(1) };
		vh->SetGUIState<CppLogic::UI::GUIState_PlaceBuildingEx>(&p);
		return 0;
	}

	int SetPlaceBuildingRotation(lua::State L) {
		//GGUI::CPlaceBuildingState::HookPlacementRotation();
		auto* s = dynamic_cast<CppLogic::UI::GUIState_PlaceBuildingEx*>(GGUI::CManager::GlobalObj()->C3DViewHandler->CurrentState);
		if (s) {
			s->SetRotation(L.CheckFloat(1));
			s->OnRotationChanged();
		}
		return 0;
	}
	int GetPlaceBuildingRotation(lua::State L) {
		auto* s = dynamic_cast<CppLogic::UI::GUIState_PlaceBuildingEx*>(GGUI::CManager::GlobalObj()->C3DViewHandler->CurrentState);
		if (!s)
			return 0;
		L.Push(s->GetRotation());
		return 1;
	}

	int GetPlaceBuildingUCat(lua::State ls) {
		luaext::EState L{ ls };
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

	int GetWidgetAtPosition(lua::State ls) {
		luaext::EState L{ ls };
		float p[2]{ L.CheckFloat(2), L.CheckFloat(3) };
		auto* rel = L.CheckWidget(1);
		L.Push(rel->GetMouseOverWidget(p, true, false));
		return 1;
	}

	int StringInputWidgetGetIgnoreNextChar(lua::State ls) {
		luaext::EState L{ ls };
		auto* w = dynamic_cast<EGUIX::CCustomWidget*>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "no customwidget" };
		auto* cw = dynamic_cast<EGUIX::CStringInputCustomWidget*>(w->CustomWidget);
		if (cw == nullptr)
			throw lua::LuaException{ "no string input wiget" };
		L.Push(cw->IgnoreNextChar);
		return 1;
	}
	int StringInputWidgetSetIgnoreNextChar(lua::State ls) {
		luaext::EState L{ ls };
		auto* w = dynamic_cast<EGUIX::CCustomWidget*>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "no customwidget" };
		auto* cw = dynamic_cast<EGUIX::CStringInputCustomWidget*>(w->CustomWidget);
		if (cw == nullptr)
			throw lua::LuaException{ "no string input wiget" };
		cw->IgnoreNextChar = L.CheckBool(2);
		return 0;
	}

	int StringInputWidgetSetBufferSize(lua::State ls) {
		luaext::EState L{ ls };
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

	int GetShortMessages(lua::State ls) {
		luaext::EState L{ ls };
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
			L.PushPos(m.Pos);
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

	int ReInitShortMessagesSize(lua::State L) {
		auto* mes = *GGUI::CShortMessagesWindowControllerCustomWidget::GlobalObj;
		mes->Initialize();
		return 0;
	}

	int CreateShortMessage(lua::State ls) {
		luaext::EState L{ ls };
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

	int RemoveShortMessage(lua::State L) {
		auto* mes = *GGUI::CShortMessagesWindowControllerCustomWidget::GlobalObj;
		mes->StandardMessage.RemoveMessage(L.CheckInt(1));
		return 0;
	}

	int InitAutoScrollCustomWidget(lua::State l) {
		luaext::EState L{ l };
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
	int GetAutoScrollCustomWidgetOffset(lua::State l) {
		luaext::EState L{ l };
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
	int AutoScrollCustomWidgetModOffset(lua::State l) {
		luaext::EState L{ l };
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
	int AutoScrollCustomWidgetSetOffset(lua::State l) {
		luaext::EState L{ l };
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
	int AutoScrollCustomWidgetSetMaterial(lua::State l) {
		luaext::EState L{ l };
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
	int TextInputCustomWidgetGetText(lua::State l) {
		luaext::EState L{ l };
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* t = dynamic_cast<CppLogic::Mod::UI::TextInputCustomWidget*>(w->CustomWidget);
		if (t == nullptr)
			throw lua::LuaException{ "not a TextInputCustomWidget" };
		L.Push(t->ClearTextOutput());
		return 1;
	}
	int TextInputCustomWidgetSetText(lua::State l) {
		luaext::EState L{ l };
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
	int TextInputCustomWidgetSetIgnoreNextChar(lua::State l) {
		luaext::EState L{ l };
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* t = dynamic_cast<CppLogic::Mod::UI::TextInputCustomWidget*>(w->CustomWidget);
		if (t == nullptr)
			throw lua::LuaException{ "not a TextInputCustomWidget" };
		t->IgnoreNextChar = L.CheckBool(2);
		return 0;
	}
	int InputCustomWidgetHasFocus(lua::State l) {
		luaext::EState L{ l };
		auto* w = BB::IdentifierCast<EGUIX::CCustomWidget>(L.CheckWidget(1));
		if (w == nullptr)
			throw lua::LuaException{ "not a customwidget" };
		auto* t = dynamic_cast<CppLogic::Mod::UI::InputFocusWidget*>(w->CustomWidget);
		if (t == nullptr)
			throw lua::LuaException{ "not a InputFocusWidget" };
		L.Push(t->HasFocus());
		return 1;
	}
	int InputCustomWidgetSetFocus(lua::State l) {
		luaext::EState L{ l };
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

	int DumpVideoModes(lua::State L) {
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

	int GetCameraData(lua::State L) {
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

	int SetCameraData(lua::State L) {
		auto* cam = dynamic_cast<ERwTools::CRwCameraHandler*>(*ERwTools::CRwCameraHandler::GlobalObj);
		cam->CameraInfo.LookAtX = L.CheckFloat(1);
		cam->CameraInfo.LookAtY = L.CheckFloat(2);
		cam->CameraInfo.LookAtZ = L.CheckFloat(3);
		cam->CameraInfo.Distance = L.CheckFloat(4);
		cam->HorizontalAngle = L.CheckFloat(5);
		cam->VerticalAngle = L.CheckFloat(6);
		return 0;
	}

	int ListCutscenes(lua::State L) {
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

	int GetCutscene(lua::State L) {
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

	int SetCutscene(lua::State L) {
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

	int ExportCutscenes(lua::State L) {
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

	int VideoCustomWidgetGetVideoSize(lua::State l) {
		luaext::EState L{ l };
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

	int VideoCustomWidgetSetVideoSize(lua::State l) {
		luaext::EState L{ l };
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

	int CreateSelectionDecal(lua::State l) {
		luaext::EState L{ l };

		shok::SelectionTextureId t = L.CheckEnum<shok::SelectionTextureId>(1);
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

	int InitNetHandlers(lua::State L) {
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
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		if (RefOnKlick != lua::State::NoRef)
			L.UnRef(RefOnKlick, L.REGISTRYINDEX);
		if (RefOnCancel != lua::State::NoRef)
			L.UnRef(RefOnCancel, L.REGISTRYINDEX);
	}

	shok::ClassId __stdcall GUIState_LuaSelection::GetClassIdentifier() const
	{
		return GUIState_LuaSelection::Identifier;
	}

	bool GUIState_LuaSelection::OnMouseEvent(BB::CEvent* ev) {
		BB::CMouseEvent* mev = BB::IdentifierCast<BB::CMouseEvent>(ev);
		if (mev && mev->IsEvent(shok::InputEventIds::MouseButtonDown)) {
			if (mev->IsKey(shok::Keys::MouseLButton)) {
				bool r = true;
				if (RefOnKlick != lua::State::NoRef) {
					lua::State L{ *EScr::CScriptTriggerSystem::GameState };
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
		if (calllua && RefOnCancel != lua::State::NoRef) {
			lua::State L{ *EScr::CScriptTriggerSystem::GameState };
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
				lua::State L{ m->GameState };
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
			if (static_cast<int>(d->TargetPos.X) == PosToBuild.X && static_cast<int>(d->TargetPos.Y) == PosToBuild.X) {
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

	float CppLogic::UI::GUIState_PlaceBuildingEx::GetRotation()
	{
		return StepToDegrees * CurrentStep;
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

	void Cleanup(lua::State L) {
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

	void CppLogic::UI::OnSaveLoaded(lua::State L)
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

	int CppLogic::UI::TerrainDecalAccess::Destroy(lua::State L)
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

	int CppLogic::UI::TerrainDecalAccess::SetPos(lua::State L)
	{
		auto* th = L.CheckUserClass<TerrainDecalAccess>(1);
		if (th->Decal == nullptr)
			throw lua::LuaException{ "already destroyed" };
		auto p = luaext::EState{ L }.CheckPos(2);
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
		lua::FuncReference::GetRef<WidgetGetPositionAndSize>("WidgetGetPositionAndSize"),
		lua::FuncReference::GetRef<WidgetSetPositionAndSize>("WidgetSetPositionAndSize"),
		lua::FuncReference::GetRef<WidgetGetUpdateManualFlag>("WidgetGetUpdateManualFlag"),
		lua::FuncReference::GetRef<WidgetSetUpdateManualFlag>("WidgetSetUpdateManualFlag"),
		lua::FuncReference::GetRef<WidgetGetUpdateFunc>("WidgetGetUpdateFunc"),
		lua::FuncReference::GetRef<WidgetCallUpdateFunc>("WidgetCallUpdateFunc"),
		lua::FuncReference::GetRef<WidgetOverrideUpdateFunc>("WidgetOverrideUpdateFunc"),
		lua::FuncReference::GetRef<ContainerWidgetGetAllChildren>("ContainerWidgetGetAllChildren"),
		lua::FuncReference::GetRef<WidgetMaterialGetTextureCoordinates>("WidgetMaterialGetTextureCoordinates"),
		lua::FuncReference::GetRef<WidgetMaterialSetTextureCoordinates>("WidgetMaterialSetTextureCoordinates"),
		lua::FuncReference::GetRef<ButtonGetActionFunc>("ButtonGetActionFunc"),
		lua::FuncReference::GetRef<ButtonCallActionFunc>("ButtonCallActionFunc"),
		lua::FuncReference::GetRef<ButtonOverrideActionFunc>("ButtonOverrideActionFunc"),
		lua::FuncReference::GetRef<WidgetGetTooltipData>("WidgetGetTooltipData"),
		lua::FuncReference::GetRef<WidgetSetTooltipData>("WidgetSetTooltipData"),
		lua::FuncReference::GetRef<WidgetGetTooltipString>("WidgetGetTooltipString"),
		lua::FuncReference::GetRef<WidgetSetTooltipString>("WidgetSetTooltipString"),
		lua::FuncReference::GetRef<WidgetGetTooltipFunc>("WidgetGetTooltipFunc"),
		lua::FuncReference::GetRef<WidgetCallTooltipFunc>("WidgetCallTooltipFunc"),
		lua::FuncReference::GetRef<WidgetOverrideTooltipFunc>("WidgetOverrideTooltipFunc"),
		lua::FuncReference::GetRef<WidgetIsTooltipShown>("WidgetIsTooltipShown"),
		lua::FuncReference::GetRef<WidgetSetFont>("WidgetSetFont"),
		lua::FuncReference::GetRef<WidgetGetBaseData>("WidgetGetBaseData"),
		lua::FuncReference::GetRef<WidgetSetBaseData>("WidgetSetBaseData"),
		lua::FuncReference::GetRef<WidgetGetStringFrameDistance>("WidgetGetStringFrameDistance"),
		lua::FuncReference::GetRef<WidgetSetStringFrameDistance>("WidgetSetStringFrameDistance"),
		lua::FuncReference::GetRef<StaticTextWidgetGetLineDistanceFactor>("StaticTextWidgetGetLineDistanceFactor"),
		lua::FuncReference::GetRef<StaticTextWidgetSetLineDistanceFactor>("StaticTextWidgetSetLineDistanceFactor"),
		lua::FuncReference::GetRef<ButtonGetShortcutString>("ButtonGetShortcutString"),
		lua::FuncReference::GetRef<ButtonSetShortcutString>("ButtonSetShortcutString"),
		lua::FuncReference::GetRef<WidgetSetGroup>("WidgetSetGroup"),
		lua::FuncReference::GetRef<TextButtonSetCenterText>("TextButtonSetCenterText"),
		lua::FuncReference::GetRef<FontGetConfig>("FontGetConfig"),
		lua::FuncReference::GetRef<FontSetConfig>("FontSetConfig"),
		lua::FuncReference::GetRef<PreLoadGUITexture>("PreLoadGUITexture"),
		lua::FuncReference::GetRef<ContainerWidgetCreateStaticWidgetChild>("ContainerWidgetCreateStaticWidgetChild"),
		lua::FuncReference::GetRef<ContainerWidgetCreateStaticTextWidgetChild>("ContainerWidgetCreateStaticTextWidgetChild"),
		lua::FuncReference::GetRef<ContainerWidgetCreatePureTooltipWidgetChild>("ContainerWidgetCreatePureTooltipWidgetChild"),
		lua::FuncReference::GetRef<ContainerWidgetCreateGFXButtonWidgetChild>("ContainerWidgetCreateGFXButtonWidgetChild"),
		lua::FuncReference::GetRef<ContainerWidgetCreateTextButtonWidgetChild>("ContainerWidgetCreateTextButtonWidgetChild"),
		lua::FuncReference::GetRef<ContainerWidgetCreateProgressBarWidgetChild>("ContainerWidgetCreateProgressBarWidgetChild"),
		lua::FuncReference::GetRef<ContainerWidgetCreateContainerWidgetChild>("ContainerWidgetCreateContainerWidgetChild"),
		lua::FuncReference::GetRef<ContainerWidgetCreateCustomWidgetChild>("ContainerWidgetCreateCustomWidgetChild"),
		lua::FuncReference::GetRef<SetCharTrigger>("SetCharTrigger"),
		lua::FuncReference::GetRef<SetKeyTrigger>("SetKeyTrigger"),
		lua::FuncReference::GetRef<SetMouseTrigger>("SetMouseTrigger"),
		lua::FuncReference::GetRef<ShowResourceFloatieOnEntity>("ShowResourceFloatieOnEntity"),
		lua::FuncReference::GetRef<ShowAdvancedFloatie>("ShowAdvancedFloatie"),
		lua::FuncReference::GetRef<GetClientSize>("GetClientSize"),
		lua::FuncReference::GetRef<IsContainerWidget>("IsContainerWidget"),
		lua::FuncReference::GetRef<GetWidgetName>("GetWidgetName"),
		lua::FuncReference::GetRef<SetGUIStateLuaSelection>("SetGUIStateLuaSelection"),
		lua::FuncReference::GetRef<GetLandscapePosAtScreenPos>("GetLandscapePosAtScreenPos"),
		lua::FuncReference::GetRef<ShowCommandAcknowledgementAtPosition>("ShowCommandAcknowledgementAtPosition"),
		lua::FuncReference::GetRef<CreateMiniMapMarker>("CreateMiniMapMarker"),
		lua::FuncReference::GetRef<CreateMiniMapScriptSignal>("CreateMiniMapScriptSignal"),
		lua::FuncReference::GetRef<GetCutsceneFarClipPlaneMinAndMax>("GetCutsceneFarClipPlaneMinAndMax"),
		lua::FuncReference::GetRef<SetCutsceneFarClipPlaneMinAndMax>("SetCutsceneFarClipPlaneMinAndMax"),
		lua::FuncReference::GetRef<ReloadGUI>("ReloadGUI"),
		lua::FuncReference::GetRef<SetShowWoodInUI>("SetShowWoodInUI"),
		lua::FuncReference::GetRef<SetGUIStatePlaceBuildingEx>("SetGUIStatePlaceBuildingEx"),
		lua::FuncReference::GetRef<SetPlaceBuildingRotation>("SetPlaceBuildingRotation"),
		lua::FuncReference::GetRef<GetPlaceBuildingRotation>("GetPlaceBuildingRotation"),
		lua::FuncReference::GetRef<GetPlaceBuildingUCat>("GetPlaceBuildingUCat"),
		lua::FuncReference::GetRef<GetWidgetAtPosition>("GetWidgetAtPosition"),
		lua::FuncReference::GetRef<StringInputWidgetGetIgnoreNextChar>("StringInputWidgetGetIgnoreNextChar"),
		lua::FuncReference::GetRef<StringInputWidgetSetIgnoreNextChar>("StringInputWidgetSetIgnoreNextChar"),
		lua::FuncReference::GetRef<StringInputWidgetSetBufferSize>("StringInputWidgetSetBufferSize"),
		lua::FuncReference::GetRef<GetShortMessages>("GetShortMessages"),
		lua::FuncReference::GetRef<ReInitShortMessagesSize>("ReInitShortMessagesSize"),
		lua::FuncReference::GetRef<CreateShortMessage>("CreateShortMessage"),
		lua::FuncReference::GetRef<RemoveShortMessage>("RemoveShortMessage"),
		lua::FuncReference::GetRef<InitAutoScrollCustomWidget>("InitAutoScrollCustomWidget"),
		lua::FuncReference::GetRef<GetAutoScrollCustomWidgetOffset>("GetAutoScrollCustomWidgetOffset"),
		lua::FuncReference::GetRef<AutoScrollCustomWidgetModOffset>("AutoScrollCustomWidgetModOffset"),
		lua::FuncReference::GetRef<AutoScrollCustomWidgetSetOffset>("AutoScrollCustomWidgetSetOffset"),
		lua::FuncReference::GetRef<AutoScrollCustomWidgetSetMaterial>("AutoScrollCustomWidgetSetMaterial"),
		lua::FuncReference::GetRef<TextInputCustomWidgetGetText>("TextInputCustomWidgetGetText"),
		lua::FuncReference::GetRef<TextInputCustomWidgetSetText>("TextInputCustomWidgetSetText"),
		lua::FuncReference::GetRef<TextInputCustomWidgetSetIgnoreNextChar>("TextInputCustomWidgetSetIgnoreNextChar"),
		lua::FuncReference::GetRef<InputCustomWidgetHasFocus>("InputCustomWidgetHasFocus"),
		lua::FuncReference::GetRef<InputCustomWidgetSetFocus>("InputCustomWidgetSetFocus"),
		lua::FuncReference::GetRef<DumpVideoModes>("DumpVideoModes"),
		lua::FuncReference::GetRef<GetCameraData>("GetCameraData"),
		lua::FuncReference::GetRef<SetCameraData>("SetCameraData"),
		lua::FuncReference::GetRef<ListCutscenes>("ListCutscenes"),
		lua::FuncReference::GetRef<GetCutscene>("GetCutscene"),
		lua::FuncReference::GetRef<SetCutscene>("SetCutscene"),
		lua::FuncReference::GetRef<ExportCutscenes>("ExportCutscenes"),
		lua::FuncReference::GetRef<VideoCustomWidgetGetVideoSize>("VideoCustomWidgetGetVideoSize"),
		lua::FuncReference::GetRef<VideoCustomWidgetSetVideoSize>("VideoCustomWidgetSetVideoSize"),
		lua::FuncReference::GetRef<CreateSelectionDecal>("CreateSelectionDecal"),
		lua::FuncReference::GetRef<InitNetHandlers>("InitNetHandlers"),
		lua::FuncReference::GetRef<RemoveWidget>("RemoveWidget"),
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
		GGlue::CGlueEntityProps* ety = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(bottom);
		if (!ety)
			throw lua::LuaException("no bottom entitytype");
		if (!ety->IsBuildingType())
			throw lua::LuaException("bottom not a building");
		auto top = ev.CannonType;
		if (!(*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(top))
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
		GGL::CSniperAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CSniperAbilityProps>();
		if (!e->Position.IsInRange(oth->Position, bp->Range))
			throw lua::LuaException("target not in range");
	}
	void CheckShurikenEvent(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		GGL::CShurikenAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CShurikenAbilityProps>();
		if (!e->Position.IsInRange(oth->Position, bp->Range))
			throw lua::LuaException("target not in range");
	}
	void CheckLightingStrike(EGL::CNetEventEntityAndPos& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		auto* b = e->GetBehavior<CppLogic::Mod::LightningStrikeAbility>();
		auto* bp = e->GetEntityType()->GetBehaviorProps<CppLogic::Mod::LightningStrikeAbilityProps>();
		if (!e->Position.IsInRange(shok::Position{ ev.X, ev.Y }, bp->Range))
			throw lua::LuaException("not in range");
		if ((*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId)->CurrentResources.WeatherEnergy < bp->WeatherEnergyCost)
			throw lua::LuaException("not enough weather energy");
	}
	void CheckResourceRefill(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		auto* t = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		auto* b = e->GetBehavior<CppLogic::Mod::ResDoodadRefillBehavior>();
		auto* bp = e->GetEntityType()->GetBehaviorProps<CppLogic::Mod::ResDoodadRefillBehaviorProps>();
		auto* res = EGL::CGLEEntity::GetEntityByID(t->GetFirstAttachedEntity(shok::AttachmentType::MINE_RESOURCE));
		if (res == nullptr)
			throw lua::LuaException("no mine");
		if (!res->IsEntityInCategory(bp->AffectedTypes))
			throw lua::LuaException("not affectable");
	}
	void CheckBombardment(EGL::CNetEventEntityAndPos& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		auto* b = e->GetBehavior<CppLogic::Mod::BombardmentAbility>();
		auto* bp = e->GetEntityType()->GetBehaviorProps<CppLogic::Mod::BombardmentAbilityProps>();
		if (!e->Position.IsInRange(shok::Position{ ev.X, ev.Y }, bp->AttackRange))
			throw lua::LuaException("not in range");
	}

	int CommandMove(lua::State l) {
		luaext::EState L{ l };
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
	int CommandPatrol(lua::State l) {
		luaext::EState L{ l };
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
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Entity_AttackEntity,
			LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>>>("Entity_AttackEntity"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Serf_ConstructBuilding,
			CheckConstruct>>("Serf_ConstructBuilding"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Serf_RepairBuilding,
			CheckRepair>>("Serf_RepairBuilding"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<GGL::CNetEventExtractResource, shok::NetEventIds::Serf_ExtractResource,
			CheckExtract>>("Serf_ExtractResource"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::Entity_AttackPos,
			LuaEventInterface::CheckEntityOfLocalPlayer>>("Entity_AttackPos"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Entity_GuardEntity,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityDiploState<shok::DiploState::Friendly>>>("Entity_GuardEntity"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::BombPlacer_PlaceBombAt,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityPlaceBomb>>>("BombPlacer_PlaceBombAt"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<GGL::CNetEventCannonCreator, shok::NetEventIds::CannonPlacer_HeroPlaceCannonAt,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityBuildCannon>,
			CheckPlaceCannonEvent>>("CannonPlacer_HeroPlaceCannonAt"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Hero_NPCInteraction,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityActorCategory<shok::EntityCategory::Hero>>>("Hero_NPCInteraction"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::ConvertSettler_Convert,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckActorAbility<shok::AbilityId::AbilityConvertSettlers>,
			LuaEventInterface::CheckConvertible, LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>,
			LuaEventInterface::CheckSettler>>("ConvertSettler_Convert"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Thief_StealFrom,
			LuaEventInterface::CheckEntityOfLocalPlayer, CheckIsThief,
			LuaEventInterface::CheckBuilding, LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>>>("Thief_StealFrom"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Thief_CarryStolenStuffToHQ,
			LuaEventInterface::CheckEntityOfLocalPlayer, CheckIsThief,
			LuaEventInterface::CheckBuilding, LuaEventInterface::CheckTargetCategory<shok::EntityCategory::Headquarters>,
			LuaEventInterface::CheckEntityDiploState<shok::DiploState::Friendly>>>("Thief_CarryStolenStuffToHQ"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Sabotage_Sabotage,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckActorAbility<shok::AbilityId::AbilityPlaceKeg>,
			LuaEventInterface::CheckBuilding, CheckThiefNotCarryingEvent,
			CheckSabotageEvent>>("Sabotage_Sabotage"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Sabotage_Defuse,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckActorAbility<shok::AbilityId::AbilityPlaceKeg>,
			CheckThiefNotCarryingEvent, CheckSabotageEvent>>("Sabotage_Defuse"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::Binoculars_Observe,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityScoutBinoculars>>>("Binoculars_Observe"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Sniper_Snipe,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckActorAbility<shok::AbilityId::AbilitySniper>, CheckSnipeEvent,
			LuaEventInterface::CheckSettler, LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>>>("Sniper_Snipe"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::TorchPlacer_Place,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityScoutTorches>>>("TorchPlacer_Place"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::Shuriken_Activate,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckActorAbility<shok::AbilityId::AbilityShuriken>, CheckShurikenEvent,
			LuaEventInterface::CheckSettler, LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>>>("Shuriken_Activate"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::CppL_LightningStrike_Activate, CheckLightingStrike,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityLightningStrike>>>("LightningStrike_Activate"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::CppL_ResDoodadRefill_Activate,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckBuilding, CheckResourceRefill,
			LuaEventInterface::CheckActorAbility<shok::AbilityId::AbiltyResourceDoodadRefill>>>("ResDoodadRefill_Activate"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityID, shok::NetEventIds::CppL_ShieldCover_Activate,
			LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityShieldCover>>>("ShieldCover_Activate"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEvent2Entities, shok::NetEventIds::CppL_Resurrect_Activate,
			LuaEventInterface::CheckEntityOfLocalPlayer, CheckResurrectEvent,
			LuaEventInterface::CheckActorAbility<shok::AbilityId::AbilityResurrect>>>("Resurrect_Activate"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::CppL_BombardmentActivate,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityBombardment>>>("Bombardment_Activate"),
		lua::FuncReference::GetRef<LuaEventInterface::NetEvent<EGL::CNetEventEntityAndPos, shok::NetEventIds::CppL_BombComboCannonActivate,
			LuaEventInterface::CheckEntityOfLocalPlayer, LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityPlaceBomb>,
			LuaEventInterface::CheckEntityBehavior<CppLogic::Mod::ReloadableCannonBuilderAbility>>>("BombCannonCombo_Activate"),
		lua::FuncReference::GetRef<CommandMove>("Entity_Move"),
		lua::FuncReference::GetRef<CommandPatrol>("Entity_Patrol"),
	};

	void Init(lua::State L)
	{
		L.RegisterFuncs(UI, -3);
#ifdef _DEBUG
		L.RegisterFunc<WidgetGetAddress>("WidgetGetAddress", -3);
#endif
		L.Push("Commands");
		L.NewTable();
		L.RegisterFuncs(Commands, -3);
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
