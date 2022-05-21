#include "pch.h"
#include "l_ui.h"
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
#include "hooks.h"

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
			L.Push(fh->FuncRefCommand.Ref, L.REGISTRYINDEX);
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
		int c = 0;
		EGUIX::CMaterial* m = wid->GetMaterials(&c);
		if (!m)
			throw lua::LuaException("no known materials");
		int min = L.CheckInt(2);
		if (!(min >= 0 && min < c))
			throw lua::LuaException("invalid index");
		L.Push(m[min].TextureCoordinates.X);
		L.Push(m[min].TextureCoordinates.Y);
		L.Push(m[min].TextureCoordinates.W);
		L.Push(m[min].TextureCoordinates.H);
		return 4;
	}
	int WidgetMaterialSetTextureCoordinates(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		int c = 0;
		EGUIX::CMaterial* m = wid->GetMaterials(&c);
		if (!m)
			throw lua::LuaException("no known materials");
		int min = L.CheckInt(2);
		if (!(min >= 0 && min < c))
			throw lua::LuaException("invalid index");
		if (L.IsNumber(3))
			m[min].TextureCoordinates.X = L.CheckFloat(3);
		if (L.IsNumber(4))
			m[min].TextureCoordinates.Y = L.CheckFloat(4);
		if (L.IsNumber(5))
			m[min].TextureCoordinates.W = L.CheckFloat(5);
		if (L.IsNumber(6))
			m[min].TextureCoordinates.H = L.CheckFloat(6);
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
			L.Push(tt->UpdateFunction.FuncRefCommand.Ref, L.REGISTRYINDEX);
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
			L.Push(fh->ActionFunction.FuncRefCommand.Ref, L.REGISTRYINDEX);
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
		int g = wgm->GetGroupId(s);
		if (!g)
			g = wgm->CreateGroup(s);
		wid->Group = g;
		return 0;
	}

	int ContainerWidgetCreateStaticWidgetChild(lua::State ls) {
		luaext::EState L{ ls };
		EGUIX::CBaseWidget* wid = L.CheckWidget(1);
		EGUIX::CContainerWidget* c = dynamic_cast<EGUIX::CContainerWidget*>(wid);
		if (!c)
			throw lua::LuaException("no container widget");
		const char* name = L.CheckString(2);
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != 0)
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
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != 0)
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
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != 0)
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
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != 0)
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
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != 0)
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
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != 0)
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
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != 0)
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
		if (EGUIX::WidgetManager::GlobalObj()->GetIdByName(name) != 0)
			throw lua::LuaException("name already in use");
		EGUIX::CBaseWidget* bef = nullptr;
		const char* customname = L.CheckString(3);
		if (!L.IsNoneOrNil(4))
			bef = L.CheckWidget(4);
		EGUIX::CCustomWidget* ne = (*BB::CClassFactory::GlobalObj)->CreateObject<EGUIX::CCustomWidget>();
		c->AddWidget(ne, name, bef);
		ne->CustomClassName.assign(customname);
		ne->IntegerUserVariable0DefaultValue = L.OptInteger(5, 0);
		ne->IntegerUserVariable1DefaultValue = L.OptInteger(6, 0);
		ne->IntegerUserVariable2DefaultValue = L.OptInteger(7, 0);
		ne->IntegerUserVariable3DefaultValue = L.OptInteger(8, 0);
		ne->IntegerUserVariable4DefaultValue = L.OptInteger(9, 0);
		ne->IntegerUserVariable5DefaultValue = L.OptInteger(10, 0);
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
		int id = 0;
		EGUIX::FontManager::LoadFont(&id, font);
		EGUIX::Font* f = EGUIX::FontManager::GlobalObj()->GetFontObj(id);
		L.Push(f->Size);
		L.Push(f->Offset);
		L.Push(f->Spacing);
		return 3;
	}
	int FontSetConfig(lua::State ls) {
		luaext::EState L{ ls };
		const char* font = L.CheckString(1);
		CheckFontString(font);
		int id = 0;
		EGUIX::FontManager::LoadFont(&id, font);
		EGUIX::Font* f = EGUIX::FontManager::GlobalObj()->GetFontObj(id);
		if (L.IsNumber(2))
			f->Size = L.CheckFloat(2);
		if (L.IsNumber(3))
			f->Offset = L.CheckFloat(3);
		if (L.IsNumber(4))
			f->Spacing = L.CheckFloat(4);
		return 0;
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

	int SetCharTrigger(lua::State ls) {
		luaext::EState L{ ls };
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		if (L.IsNil(1)) {
			EGUIX::UIInput_Char_Callback = nullptr;
			L.PushLightUserdata(&SetCharTrigger);
			L.Push();
			L.SetTableRaw(L.REGISTRYINDEX);
			return 0;
		}

		L.CheckType(1, lua::LType::Function);
		L.PushLightUserdata(&SetCharTrigger);
		L.PushValue(1);
		L.SetTableRaw(L.REGISTRYINDEX);

		if (!EGUIX::UIInput_Char_Callback) {
			EGUIX::HookUIInput();
			EGUIX::UIInput_Char_Callback = [](int c) {
				lua::State L{ *EScr::CScriptTriggerSystem::GameState };
				int t = L.GetTop();
				bool r = false;

				L.PushLightUserdata(&SetCharTrigger);
				L.GetTableRaw(L.REGISTRYINDEX);
				L.Push(c);
				if (L.PCall(1, 1, 0) == lua::ErrorCode::Success) {
					r = L.OptBool(-1, false);
				}

				L.SetTop(t);
				return r;
			};
		}
		return 0;
	}
	int SetKeyTrigger(lua::State ls) {
		luaext::EState L{ ls };
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		if (L.IsNil(1)) {
			EGUIX::UIInput_Key_Callback = nullptr;
			L.PushLightUserdata(&SetKeyTrigger);
			L.Push();
			L.SetTableRaw(L.REGISTRYINDEX);
			return 0;
		}

		L.CheckType(1, lua::LType::Function);
		L.PushLightUserdata(&SetKeyTrigger);
		L.PushValue(1);
		L.SetTableRaw(L.REGISTRYINDEX);

		if (!EGUIX::UIInput_Key_Callback) {
			EGUIX::HookUIInput();
			EGUIX::UIInput_Key_Callback = [](int c, win_mouseEvents id) {
				lua::State L{ *EScr::CScriptTriggerSystem::GameState };
				int t = L.GetTop();
				bool r = false;

				L.PushLightUserdata(&SetKeyTrigger);
				L.GetTableRaw(L.REGISTRYINDEX);
				L.Push(c);
				L.Push(id == win_mouseEvents::KeyUp || id == win_mouseEvents::SysKeyUp);
				if (L.PCall(2, 1, 0) == lua::ErrorCode::Success) {
					r = L.OptBool(-1, false);
				}

				L.SetTop(t);
				return r;
			};
		}
		return 0;
	}
	int SetMouseTrigger(lua::State ls) {
		luaext::EState L{ ls };
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		if (L.IsNil(1)) {
			EGUIX::UIInput_Mouse_Callback = nullptr;
			L.PushLightUserdata(&SetMouseTrigger);
			L.Push();
			L.SetTableRaw(L.REGISTRYINDEX);
			return 0;
		}

		L.CheckType(1, lua::LType::Function);
		L.PushLightUserdata(&SetMouseTrigger);
		L.PushValue(1);
		L.SetTableRaw(L.REGISTRYINDEX);

		if (!EGUIX::UIInput_Mouse_Callback) {
			EGUIX::HookUIInput();
			EGUIX::UIInput_Mouse_Callback = [](win_mouseEvents id, int w, int l) {
				if (id == win_mouseEvents::MouseMove)
					return false;

				lua::State L{ *EScr::CScriptTriggerSystem::GameState };
				int t = L.GetTop();
				bool r = false;

				L.PushLightUserdata(&SetMouseTrigger);
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
		L.Push(EGUIX::WidgetManager::GlobalObj()->WidgetNameManager->GetNameByID(w->WidgetID));
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
		GGUI::MiniMapMarkerHandler::GlobalObj()->CreateMarker(p, pulsing, r, g, b, timefactor, scalefactor);
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
		GGUI::MiniMapMarkerHandler::GlobalObj()->CreateSignalDefault(p, r, g, b, scalefactor);
		return 0;
	}

	int SetGUIStateLuaSelection(lua::State ls) {
		luaext::EState L{ ls };
		L.CheckType(1, lua::LType::Function);
		auto* vh = GGUI::CManager::GlobalObj()->C3DViewHandler;
		vh->StateIdManager->GetIDByNameOrCreate("LuaSelectionState", 27); // make sure the state id exists
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

	unsigned int __stdcall GUIState_LuaSelection::GetClassIdentifier() const
	{
		return GUIState_LuaSelection::Identifier;
	}

	bool GUIState_LuaSelection::OnMouseEvent(BB::CEvent* ev) {
		BB::CMouseEvent* mev = dynamic_cast<BB::CMouseEvent*>(ev);
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

	void GUIState_LuaSelection::SetStateParameters(GGUI::SStateParameters* p)
	{

	}

	bool GUIState_LuaSelection::Cancel()
	{
		Cancel(true);
		return true;
	}

	const char* GUIState_LuaSelection::GetName()
	{
		return "LuaSelectionState";
	}

	int GUIState_LuaSelection::OnSelectionChanged(int z)
	{
		if (!z)
			Cancel();
		return 1;
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


	void Cleanup(lua::State L) {
		EGUIX::UIInput_Char_Callback = nullptr;
		EGUIX::UIInput_Key_Callback = nullptr;
		EGUIX::UIInput_Mouse_Callback = nullptr;
		if (EGUIX::WidgetManager* wm = EGUIX::WidgetManager::GlobalObj()) {
			for (EGUIX::CBaseWidget* wid : wm->Widgets) {
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
	}

	constexpr std::array<lua::FuncReference, 53> UI{ {
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
		lua::FuncReference::GetRef<FontGetConfig>("FontGetConfig"),
		lua::FuncReference::GetRef<FontSetConfig>("FontSetConfig"),
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
		lua::FuncReference::GetRef<GetClientSize>("GetClientSize"),
		lua::FuncReference::GetRef<IsContainerWidget>("IsContainerWidget"),
		lua::FuncReference::GetRef<GetWidgetName>("GetWidgetName"),
		lua::FuncReference::GetRef<SetGUIStateLuaSelection>("SetGUIStateLuaSelection"),
		lua::FuncReference::GetRef<GetLandscapePosAtScreenPos>("GetLandscapePosAtScreenPos"),
		lua::FuncReference::GetRef<ShowCommandAcknowledgementAtPosition>("ShowCommandAcknowledgementAtPosition"),
		lua::FuncReference::GetRef<CreateMiniMapMarker>("CreateMiniMapMarker"),
		lua::FuncReference::GetRef<CreateMiniMapScriptSignal>("CreateMiniMapScriptSignal"),
	} };

	void Init(lua::State L)
	{
		L.RegisterFuncs(UI, -3);
#ifdef _DEBUG
		L.RegisterFunc<WidgetGetAddress>("WidgetGetAddress" -3);
#endif

		if (L.GetState() == shok::LuaStateMainmenu) {
			L.RegisterFunc<SetMouseTriggerMainMenu>("SetMouseTriggerMainMenu", -3);
			CppLogic::UI::GUIState_LuaSelection::Initialize();
		}
	}
}

// CppLogic.UI.WidgetGetAddress("StartMenu00_EndGame")
// CppLogic.UI.WidgetGetAddress("AutoAddSerfJobs")
// StartMenu00_VersionNumber
// StartMenu00_EndGame
// StartMenu00
// GoldTooltipController
// CppLogic.UI.ButtonOverrideActionFunc("StartMenu00_EndGame", function() LuaDebugger.Log(XGUIEng.GetCurrentWidgetID()) end)
// CppLogic.UI.ContainerWidgetCreateStaticWidgetChild("StartMenu00", "test", "StartMenu00_EndGame"); XGUIEng.SetMaterialTexture("test", 0, "data\\graphics\\textures\\gui\\hero_sel_dario.png"); CppLogic.UI.WidgetSetPositionAndSize("test", 783, 500, 32, 32); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateStaticTextWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.SetMaterialColor("test", 0, 0,0,0,0); XGUIEng.SetTextColor("test",255,255,255,255); CppLogic.UI.WidgetSetFont("test", "data\\menu\\fonts\\standardinvert10.met"); XGUIEng.SetText("test","tst"); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreatePureTooltipWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); CppLogic.UI.WidgetSetTooltipData("test", "StartMenu_TooltipText", true, true); CppLogic.UI.WidgetSetTooltipString("test", "tst"); CppLogic.UI.WidgetOverrideTooltipFunc("test", function() LuaDebugger.Log(1) end); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateGFXButtonWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateTextButtonWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.SetText("test","tst"); CppLogic.UI.WidgetSetFont("test", "data\\menu\\fonts\\standardinvert10.met"); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateProgressBarWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateContainerWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 100, 100); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.WidgetSetFont("StartMenu00_EndGame", "data\\menu\\fonts\\medium11bold.met") --"data\\menu\\fonts\\mainmenularge.met"
// CppLogic.UI.FontGetConfig("data\\menu\\fonts\\standard10.met")
// XGUIEng.SetText("StartMenu00_EndGame", "@center @color:0,255,0 test")
// CppLogic.UI.RemoveWidget("StartMenu00_VersionNumber")
// CppLogic.UI.SetGUIStateLuaSelection(function(x, y) LuaDebugger.Log(x.."/"..y) end)
// CppLogic.UI.ContainerWidgetCreateCustomWidgetChild("StartMenu00", "test", "EGUIX::CVideoPlaybackCustomWidget"); XGUIEng.ShowWidget("test", 1); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 250, 250); XGUIEng.StartVideoPlayback("test", "data\\graphics\\videos\\pu_farmer.bik", 1);
