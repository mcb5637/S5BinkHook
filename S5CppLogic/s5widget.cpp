#include "pch.h"
#include "s5data.h"

struct shok_vtable_EGUIX_CBaseWidget : shok_vtable_BB_IObject {
    PADDINGI(4);
    int(__thiscall* SetSizeAndPos)(shok_EGUIX_CBaseWidget* th, float x, float y, float w, float h); // 7
    PADDINGI(4);
};
struct shok_vtable_EGUIX_CContainerWidget : shok_vtable_EGUIX_CBaseWidget {
    void(__thiscall* AddChild)(shok_EGUIX_CContainerWidget* th, shok_EGUIX_CBaseWidget* ch); // 12
};
//constexpr int i = offsetof(shok_vtable_EGUIX_CContainerWidget, AddChild) / 4;


static inline int(__thiscall* const widman_getidbyname)(shok_widgetManager* th, const char* n) = reinterpret_cast<int(__thiscall*)(shok_widgetManager*, const char*)>(0x5588A0);
int shok_widgetManager::GetIdByName(const char* name)
{
    return widman_getidbyname(this, name);
}

static inline shok_EGUIX_CBaseWidget* (__thiscall* const widman_getwid)(shok_widgetManager* th, int id) = reinterpret_cast<shok_EGUIX_CBaseWidget * (__thiscall*)(shok_widgetManager*, int)>(0x558966);
shok_EGUIX_CBaseWidget* shok_widgetManager::GetWidgetByID(int id)
{
    return widman_getwid(this, id);
}
static inline void(__thiscall* const widman_removewid)(shok_widgetManager* th, shok_EGUIX_CBaseWidget* w, int id) = reinterpret_cast<void(__thiscall*)(shok_widgetManager*, shok_EGUIX_CBaseWidget * w, int)>(0x558915);
void shok_widgetManager::RemoveWidget(shok_EGUIX_CBaseWidget* w)
{
    widman_removewid(this, w, w->WidgetID);
}

static inline int(__thiscall* const widgroupman_getgroupid)(shok_EGUIX_CWidgetGroupManager* th, const char* s) = reinterpret_cast<int(__thiscall*)(shok_EGUIX_CWidgetGroupManager*, const char*)>(0x583214);
int shok_EGUIX_CWidgetGroupManager::GetGroupId(const char* s)
{
    return widgroupman_getgroupid(this, s);
}
static inline int(__thiscall* const widgroupman_creategroup)(int man, const char* s, int z) = reinterpret_cast<int(__thiscall*)(int, const char*, int)>(0x54F656);
int shok_EGUIX_CWidgetGroupManager::CreateGroup(const char* s)
{
    return widgroupman_creategroup(*reinterpret_cast<int*>(0x894EA4), s, 0);
}

void shok_EGUIX_CFontIDHandler::LoadFont(const char* name)
{
    shok_fontManager::LoadFont(&FontID, name);
}

int shok_EGUIX_CBaseWidget::SetPosAndSize(float x, float y, float w, float h)
{
    return reinterpret_cast<shok_vtable_EGUIX_CBaseWidget*>(vtable)->SetSizeAndPos(this, x, y, w, h);
}

static inline void(__thiscall* const funchelper_call)(shok_EGUIX_CLuaFunctionHelper* th, int widgetID) = reinterpret_cast<void(__thiscall*)(shok_EGUIX_CLuaFunctionHelper*, int)>(0x55BDDE);
void shok_EGUIX_CLuaFunctionHelper::Call(int widgetID)
{
    funchelper_call(this, widgetID);
}


byte* shok_EGUIX_CBaseWidget::GetUpdateManualFlag()
{
    if (shok_EGUIX_CStaticTextWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CStaticTextWidget>(this))
        return &w->UpdateManualFlag;
    if (shok_EGUIX_CGfxButtonWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CGfxButtonWidget>(this))
        return &w->UpdateManualFlag;
    if (shok_EGUIX_CTextButtonWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CTextButtonWidget>(this))
        return &w->UpdateManualFlag;
    if (shok_EGUIX_CProgressBarWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CProgressBarWidget>(this))
        return &w->UpdateManualFlag;
    return nullptr;
}
shok_EGUIX_CLuaFunctionHelper* shok_EGUIX_CBaseWidget::GetUpdateFunc()
{
    if (shok_EGUIX_CStaticTextWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CStaticTextWidget>(this))
        return &w->UpdateFunction;
    if (shok_EGUIX_CGfxButtonWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CGfxButtonWidget>(this))
        return &w->UpdateFunction;
    if (shok_EGUIX_CTextButtonWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CTextButtonWidget>(this))
        return &w->UpdateFunction;
    if (shok_EGUIX_CProgressBarWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CProgressBarWidget>(this))
        return &w->UpdateFunction;
    return nullptr;
}
shok_EGUIX_CMaterial* shok_EGUIX_CBaseWidget::GetMaterials(int* count)
{
    if (shok_EGUIX_CStaticWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CStaticWidget>(this)) {
        *count = 1;
        return &w->BackgroundMaterial;
    }
    if (shok_EGUIX_CButtonWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CButtonWidget>(this)) {
        *count = 5;
        return w->Materials;
    }
    return nullptr;
}
shok_EGUIX_CButtonHelper* shok_EGUIX_CBaseWidget::GetButtonHelper()
{
    if (shok_EGUIX_CButtonWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CButtonWidget>(this))
        return &w->ButtonHelper;
    return nullptr;
}
shok_EGUIX_CToolTipHelper* shok_EGUIX_CBaseWidget::GetTooltipHelper()
{
    if (shok_EGUIX_CButtonWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CButtonWidget>(this))
        return &w->ToolTipHelper;
    if (shok_EGUIX_CPureTooltipWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CPureTooltipWidget>(this))
        return &w->ToolTipHelper;
    return nullptr;
}
shok_EGUIX_CWidgetStringHelper* shok_EGUIX_CBaseWidget::GetStringHelper()
{
    if (shok_EGUIX_CStaticTextWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CStaticTextWidget>(this))
        return &w->StringHelper;
    if (shok_EGUIX_CTextButtonWidget* w = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CTextButtonWidget>(this))
        return &w->StringHelper;
    return nullptr;
}

static inline int(__thiscall* const widman_registerName)(shok_widgetManager* th, const char* name) = reinterpret_cast<int(__thiscall*)(shok_widgetManager*, const char*)>(0x55884F);
static inline void(__thiscall* const widman_addWidget)(shok_widgetManager* th, shok_EGUIX_CBaseWidget* a, int id) = reinterpret_cast<void(__thiscall*)(shok_widgetManager*, shok_EGUIX_CBaseWidget*, int)>(0x558AA2);
void shok_EGUIX_CContainerWidget::AddWidget(shok_EGUIX_CBaseWidget* toAdd, const char* name, const shok_EGUIX_CBaseWidget* before)
{
    shok_widgetManager* m = shok_widgetManager::GlobalObj();
    int newId = widman_registerName(m, name);
    if (newId) {
        toAdd->WidgetID = newId;
        widman_addWidget(m, toAdd, newId);
        reinterpret_cast<shok_vtable_EGUIX_CContainerWidget*>(vtable)->AddChild(this, toAdd);
        if (before) {
            shok_saveList<shok_EGUIX_CBaseWidget*>(&WidgetListHandler.SubWidgets, [before](std::list<shok_EGUIX_CBaseWidget*, shok_allocator<shok_EGUIX_CBaseWidget*>>& l) {
                std::list<shok_EGUIX_CBaseWidget*, shok_allocator<shok_EGUIX_CBaseWidget*>>::iterator it = l.begin();
                while (it != l.end()) {
                    if (*it == before) {
                        l.splice(it, l, --l.end());
                        break;
                    }

                    it++;
                }
                });
        }
    }
}

void(__thiscall* const shok_EGUIX_CLuaFunctionHelper_ctor)(shok_EGUIX_CLuaFunctionHelper* th) = reinterpret_cast<void(__thiscall*)(shok_EGUIX_CLuaFunctionHelper * th)>(0x55BEA2);
static inline void(__thiscall* const widlisthandler_ctor)(shok_EGUIX_CWidgetListHandler* th) = reinterpret_cast<void(__thiscall*)(shok_EGUIX_CWidgetListHandler*)>(0x55BB26);

static inline void(__thiscall* const statwid_ctor)(shok_EGUIX_CStaticWidget* th) = reinterpret_cast<void(__thiscall*)(shok_EGUIX_CStaticWidget*)>(0x5603F1);
shok_EGUIX_CStaticWidget* shok_EGUIX_CStaticWidget::Create()
{
    shok_EGUIX_CStaticWidget* r = reinterpret_cast<shok_EGUIX_CStaticWidget*>(shok_malloc(sizeof(shok_EGUIX_CStaticWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CStaticWidget));
    statwid_ctor(r);
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const stattextwid_ctor)(shok_EGUIX_CStaticTextWidget* th) = reinterpret_cast<void(__thiscall*)(shok_EGUIX_CStaticTextWidget*)>(0x55FBB1);
shok_EGUIX_CStaticTextWidget* shok_EGUIX_CStaticTextWidget::Create()
{
    shok_EGUIX_CStaticTextWidget* r = reinterpret_cast<shok_EGUIX_CStaticTextWidget*>(shok_malloc(sizeof(shok_EGUIX_CStaticTextWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CStaticTextWidget));
    stattextwid_ctor(r);
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const purettwid_ctor)(shok_EGUIX_CPureTooltipWidget* th) = reinterpret_cast<void(__thiscall*)(shok_EGUIX_CPureTooltipWidget*)>(0x55DF3E);
shok_EGUIX_CPureTooltipWidget* shok_EGUIX_CPureTooltipWidget::Create()
{
    shok_EGUIX_CPureTooltipWidget* r = reinterpret_cast<shok_EGUIX_CPureTooltipWidget*>(shok_malloc(sizeof(shok_EGUIX_CPureTooltipWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CPureTooltipWidget));
    purettwid_ctor(r);
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const gfxbutwid_ctor)(shok_EGUIX_CGfxButtonWidget* th) = reinterpret_cast<void(__thiscall*)(shok_EGUIX_CGfxButtonWidget*)>(0x55E917);
shok_EGUIX_CGfxButtonWidget* shok_EGUIX_CGfxButtonWidget::Create()
{
    shok_EGUIX_CGfxButtonWidget* r = reinterpret_cast<shok_EGUIX_CGfxButtonWidget*>(shok_malloc(sizeof(shok_EGUIX_CGfxButtonWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CGfxButtonWidget));
    gfxbutwid_ctor(r);
    r->IsShown = false;
    r->IconMaterial.Color.Alpha = 0;
    return r;
}
static inline void(__thiscall* const txtbutwid_ctor)(shok_EGUIX_CTextButtonWidget* th) = reinterpret_cast<void(__thiscall*)(shok_EGUIX_CTextButtonWidget*)>(0x55EFA3);
shok_EGUIX_CTextButtonWidget* shok_EGUIX_CTextButtonWidget::Create()
{
    shok_EGUIX_CTextButtonWidget* r = reinterpret_cast<shok_EGUIX_CTextButtonWidget*>(shok_malloc(sizeof(shok_EGUIX_CTextButtonWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CTextButtonWidget));
    txtbutwid_ctor(r);
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const progbarwid_ctor)(shok_EGUIX_CProgressBarWidget* th) = reinterpret_cast<void(__thiscall*)(shok_EGUIX_CProgressBarWidget*)>(0x55E300);
shok_EGUIX_CProgressBarWidget* shok_EGUIX_CProgressBarWidget::Create()
{
    shok_EGUIX_CProgressBarWidget* r = reinterpret_cast<shok_EGUIX_CProgressBarWidget*>(shok_malloc(sizeof(shok_EGUIX_CProgressBarWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CProgressBarWidget));
    progbarwid_ctor(r);
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const contwid_ctor)(shok_EGUIX_CContainerWidget* th) = reinterpret_cast<void(__thiscall*)(shok_EGUIX_CContainerWidget*)>(0x560DAD);
shok_EGUIX_CContainerWidget* shok_EGUIX_CContainerWidget::Create()
{
    shok_EGUIX_CContainerWidget* r = reinterpret_cast<shok_EGUIX_CContainerWidget*>(shok_malloc(sizeof(shok_EGUIX_CContainerWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CContainerWidget));
    contwid_ctor(r);
    r->IsShown = false;
    return r;
}

static inline void(__thiscall* const customwid_initwid)(shok_EGUIX_CCustomWidget* th) = reinterpret_cast<void(__thiscall*)(shok_EGUIX_CCustomWidget*)>(0x560BA3);
void shok_EGUIX_CCustomWidget::InitializeCustomWidget()
{
    customwid_initwid(this);
}

static inline void(__thiscall* const showresfloatie)(shok_GGUI_C3DOnScreenInformationCustomWidget* th, int eid, int am) = reinterpret_cast<void(__thiscall*)(shok_GGUI_C3DOnScreenInformationCustomWidget*, int, int)>(0x536361);
void shok_GGUI_C3DOnScreenInformationCustomWidget::ShowResourceFloatieOnEntity(int eid, int amount)
{
    showresfloatie(this, eid, amount);
}

void shok_GGUI_C3DOnScreenInformationCustomWidget::HookResourceFloatieShowWood(bool showwood)
{
    shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x529067), 10 };
    *reinterpret_cast<byte*>(0x529067) = showwood ? 0xFF : static_cast<int>(shok_ResourceType::WoodRaw);
}

static inline void(__stdcall* const loadfont)(int* out, const char* name) = reinterpret_cast<void(__stdcall*)(int*, const char*)>(0x55D99E);
void shok_fontManager::LoadFont(int* outFontID, const char* fontName)
{
    loadfont(outFontID, fontName);
}
static inline shok_font* (__thiscall* const fontmng_getfont)(shok_fontManager* th, int id) = reinterpret_cast<shok_font * (__thiscall*)(shok_fontManager*, int)>(0x5597C7);
shok_font* shok_fontManager::GetFontObj(int id)
{
    return fontmng_getfont(this, id);
}

bool (*UIInput_Char_Callback)(int c) = nullptr;
bool (*UIInput_Key_Callback)(int c, win_mouseEvents ev) = nullptr;
bool (*UIInput_Mouse_Callback)(win_mouseEvents id, int w, int l) = nullptr;
bool (*UIInput_Mouse_CallbackMainMenu)(win_mouseEvents id, int w, int l) = nullptr;
int __stdcall uiinput_firecbs(win_mouseEvents ev, int w, int l) {
    int r = 0;
    if (ev == win_mouseEvents::Char) {
        if (UIInput_Char_Callback)
            if (UIInput_Char_Callback(w))
                r = 1;
    }
    else if (ev == win_mouseEvents::KeyDown || ev == win_mouseEvents::KeyUp || ev == win_mouseEvents::SysKeyDown || ev == win_mouseEvents::SysKeyUp) {
        if (UIInput_Key_Callback)
            if (UIInput_Key_Callback(w, ev))
                r = 1;
    }
    else if (ev > win_mouseEvents::MouseMove && ev <= win_mouseEvents::XButtonDBl) {
        if (UIInput_Mouse_Callback)
            if (UIInput_Mouse_Callback(ev, w, l))
                r = 1;
        if (UIInput_Mouse_CallbackMainMenu)
            if (UIInput_Mouse_CallbackMainMenu(ev, w, l))
                r = 1;
    }
    return r;
}
void __declspec(naked) uiinput_asm() {
    __asm {
        sub esp, 0x24;
        push ebx;
        push esi;
        push edi;

        push[ebp + 0x14];
        push[ebp + 0x10];
        push[ebp + 0xC];
        call uiinput_firecbs;
        cmp eax, 0;
        je cont;

        push 0x40751B;
        ret;

    cont:
        push - 21;
        push[ebp + 8];

        push 0x407456;
        ret;
    };
}
bool HookUIInput_Hooked = false;
void HookUIInput()
{
    if (HookUIInput_Hooked)
        return;
    HookUIInput_Hooked = true;
    shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x40744B), 10 };
    WriteJump(reinterpret_cast<void*>(0x40744B), &uiinput_asm);
}

void(__thiscall* const shok_feedbackEventHandler_fireevent)(shok_feedbackEventHandler* th, shok_BB_CEvent* e) = reinterpret_cast<void(__thiscall*)(shok_feedbackEventHandler*, shok_BB_CEvent*)>(0x582EB2);
void shok_feedbackEventHandler::FireEvent(shok_BB_CEvent* ev)
{
    shok_feedbackEventHandler_fireevent(this, ev);
}
