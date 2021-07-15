#include "pch.h"
#include "s5data.h"

struct shok_vtable_EGUIX_CBaseWidget {
    PADDINGI(7);
    int(__thiscall* SetSizeAndPos)(shok_EGUIX_CBaseWidget* th, float x, float y, float w, float h); // 7
};
struct shok_vtable_EGUIX_CContainerWidget {
    PADDINGI(7);
    int(__thiscall* SetSizeAndPos)(shok_EGUIX_CBaseWidget* th, float x, float y, float w, float h); // 7
    PADDINGI(4);
    void(__thiscall* AddChild)(shok_EGUIX_CContainerWidget* th, shok_EGUIX_CBaseWidget* ch); // 12
};


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

void initMaterial(shok_EGUIX_CMaterial& m) {
    m.vtable = shok_EGUIX_CMaterial::vtp;
    m.TextureCoordinates = { 0, 0, 1, 1 };
    m.Color = { 0xFF, 0xFF, 0xFF, 0xFF };
}
void initSingleSting(shok_EGUIX_CSingleStringHandler& h) {
    h.RawString = shok_string("");
    h.StringTableKey = shok_string("");
}
void initStringHelper(shok_EGUIX_CWidgetStringHelper& h) {
    h.vtable = shok_EGUIX_CWidgetStringHelper::vtp;
    h.FontHandler.vtable = shok_EGUIX_CFontIDHandler::vtp;
    h.SingleStringHandler.vtable = shok_EGUIX_CSingleStringHandler::vtp;
    h.FontHandler.FontID = 1;
    h.Color = { 0, 0, 0, 0xFF };
    initSingleSting(h.SingleStringHandler);
}
void(__thiscall* const shok_EGUIX_CLuaFunctionHelper_ctor)(shok_EGUIX_CLuaFunctionHelper* th) = reinterpret_cast<void(__thiscall*)(shok_EGUIX_CLuaFunctionHelper * th)>(0x55BEA2);
void initLuaFunc(shok_EGUIX_CLuaFunctionHelper& h) {
    /*h.vtable = shok_EGUIX_CLuaFunctionHelper::vtp;
    h.FuncRefCommand.vtable = shoc_EScr_CLuaFuncRefCommand::vtp;
    h.LuaCommand = shok_string("");
    h.FuncRefCommand.LuaCommand = shok_string("");
    h.FuncRefCommand.NeedsCompile = 1;*/
    shok_EGUIX_CLuaFunctionHelper_ctor(&h);
}
void initTooltipHelper(shok_EGUIX_CToolTipHelper& h) {
    h.vtable = shok_EGUIX_CToolTipHelper::vtp;
    h.vtable_EGUIX_IWidgetRegistrationCallback = 0x780B28;
    initSingleSting(h.ToolTipString);
    initLuaFunc(h.UpdateFunction);
}
void initButtonHelper(shok_EGUIX_CButtonHelper& h) {
    h.vtable = shok_EGUIX_CButtonHelper::vtp;
    initSingleSting(h.ShortCutString);
    initLuaFunc(h.ActionFunction);
}
static inline void(__thiscall* const widlisthandler_ctor)(shok_EGUIX_CWidgetListHandler* th) = reinterpret_cast<void(__thiscall*)(shok_EGUIX_CWidgetListHandler*)>(0x55BB26);
void initWidgetListHandler(shok_EGUIX_CWidgetListHandler& h) {
    widlisthandler_ctor(&h);
}

shok_EGUIX_CStaticWidget* shok_EGUIX_CStaticWidget::Create()
{
    shok_EGUIX_CStaticWidget* r = reinterpret_cast<shok_EGUIX_CStaticWidget*>(shok_malloc(sizeof(shok_EGUIX_CStaticWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CStaticWidget));
    r->vtable = shok_EGUIX_CStaticWidget::vtp;
    r->vtable_EGUIX_IOnEvent = 0x780F78;
    r->vtable_EGUIX_IRender = 0x780F6C;
    r->vtable_EGUIX_IMaterialAccess = 0x780F58;
    initMaterial(r->BackgroundMaterial);
    return r;
}
shok_EGUIX_CStaticTextWidget* shok_EGUIX_CStaticTextWidget::Create()
{
    shok_EGUIX_CStaticTextWidget* r = reinterpret_cast<shok_EGUIX_CStaticTextWidget*>(shok_malloc(sizeof(shok_EGUIX_CStaticTextWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CStaticTextWidget));
    r->vtable = shok_EGUIX_CStaticTextWidget::vtp;
    r->vtable_EGUIX_IOnEvent = 0x780ED8;
    r->vtable_EGUIX_IRender = 0x780ECC;
    r->vtable_EGUIX_IMaterialAccess = 0x780EB8;
    r->vtable_EGUIX_ITextAccess = 0x780E90;
    initMaterial(r->BackgroundMaterial);
    initStringHelper(r->StringHelper);
    initLuaFunc(r->UpdateFunction);
    r->UpdateManualFlag = true;
    return r;
}
shok_EGUIX_CPureTooltipWidget* shok_EGUIX_CPureTooltipWidget::Create()
{
    shok_EGUIX_CPureTooltipWidget* r = reinterpret_cast<shok_EGUIX_CPureTooltipWidget*>(shok_malloc(sizeof(shok_EGUIX_CPureTooltipWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CPureTooltipWidget));
    r->vtable = shok_EGUIX_CPureTooltipWidget::vtp;
    r->vtable_EGUIX_IOnEvent = 0x780BA4;
    initTooltipHelper(r->ToolTipHelper);
    return r;
}
shok_EGUIX_CGfxButtonWidget* shok_EGUIX_CGfxButtonWidget::Create()
{
    shok_EGUIX_CGfxButtonWidget* r = reinterpret_cast<shok_EGUIX_CGfxButtonWidget*>(shok_malloc(sizeof(shok_EGUIX_CGfxButtonWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CGfxButtonWidget));
    r->vtable = shok_EGUIX_CGfxButtonWidget::vtp;
    r->vtable_EGUIX_IOnEvent = 0x780CC4;
    r->vtable_EGUIX_IRender = 0x780CB8;
    r->vtable_EGUIX_IMaterialAccess = 0x780CA4;
    initButtonHelper(r->ButtonHelper);
    for (int i = 0; i < 5; i++)
        initMaterial(r->Materials[i]);
    r->NumMaterials = 5;
    initTooltipHelper(r->ToolTipHelper);
    initMaterial(r->IconMaterial);
    r->IconMaterial.Color.Alpha = 0;
    initLuaFunc(r->UpdateFunction);
    return r;
}
shok_EGUIX_CTextButtonWidget* shok_EGUIX_CTextButtonWidget::Create()
{
    shok_EGUIX_CTextButtonWidget* r = reinterpret_cast<shok_EGUIX_CTextButtonWidget*>(shok_malloc(sizeof(shok_EGUIX_CTextButtonWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CTextButtonWidget));
    r->vtable = shok_EGUIX_CTextButtonWidget::vtp;
    r->vtable_EGUIX_IOnEvent = 0x780DA4;
    r->vtable_EGUIX_IRender = 0x780D98;
    r->vtable_EGUIX_IMaterialAccess = 0x780D84;
    r->vtable_EGUIX_ITextAccess = 0x780D5C;
    initButtonHelper(r->ButtonHelper);
    for (int i = 0; i < 5; i++)
        initMaterial(r->Materials[i]);
    r->NumMaterials = 5;
    initTooltipHelper(r->ToolTipHelper);
    initStringHelper(r->StringHelper);
    initLuaFunc(r->UpdateFunction);
    return r;
}
shok_EGUIX_CProgressBarWidget* shok_EGUIX_CProgressBarWidget::Create()
{
    shok_EGUIX_CProgressBarWidget* r = reinterpret_cast<shok_EGUIX_CProgressBarWidget*>(shok_malloc(sizeof(shok_EGUIX_CProgressBarWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CProgressBarWidget));
    r->vtable = shok_EGUIX_CProgressBarWidget::vtp;
    r->vtable_EGUIX_IOnEvent = 0x780C14;
    r->vtable_EGUIX_IRender = 0x780C08;
    r->vtable_EGUIX_IMaterialAccess = 0x780BF4;
    initMaterial(r->BackgroundMaterial);
    initLuaFunc(r->UpdateFunction);
    r->ProgressBarLimit = 1.0f;
    r->ProgressBarValue = 1.0f;
    return r;
}
shok_EGUIX_CContainerWidget* shok_EGUIX_CContainerWidget::Create()
{
    shok_EGUIX_CContainerWidget* r = reinterpret_cast<shok_EGUIX_CContainerWidget*>(shok_malloc(sizeof(shok_EGUIX_CContainerWidget)));
    memset(r, 0, sizeof(shok_EGUIX_CContainerWidget));
    r->vtable = shok_EGUIX_CContainerWidget::vtp;
    r->vtable_EGUIX_IOnEvent = 0x781140;
    r->vtable_EGUIX_IRender = 0x781134;
    r->vtable_EGUIX_IWidgetRegistrationCallback = 0x78112C;
    initWidgetListHandler(r->WidgetListHandler);
    return r;
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

void (*UIInput_Char_Callback)(int c) = nullptr;
int(__stdcall* const uiinput_char_orig)(void* ev, int id, int w, int h) = reinterpret_cast<int(__stdcall* const)(void*, int, int, int)>(0x54E649);
int __stdcall uiinput_char(void* ev, int id, int w, int l) {
    if (UIInput_Char_Callback)
        UIInput_Char_Callback(w);
    return uiinput_char_orig(ev, id, w, l);
}
void (*UIInput_Key_Callback)(int c, int ev) = nullptr;
int(__stdcall* const uiinput_key_orig)(void* ev, int id, int w, int h) = reinterpret_cast<int(__stdcall* const)(void*, int, int, int)>(0x54E82F);
int __stdcall uiinput_key(void* ev, int id, int w, int l) {
    if (UIInput_Key_Callback)
        UIInput_Key_Callback(w, id);
    return uiinput_key_orig(ev, id, w, l);
}
void (*UIInput_Mouse_Callback)(win_mouseEvents id, int w, int l) = nullptr;
int(__stdcall* const uiinput_mouse_orig)(void* ev, int hwind, win_mouseEvents id, int w, int h) = reinterpret_cast<int(__stdcall* const)(void*, int, win_mouseEvents, int, int)>(0x54E6BD);
int __stdcall uiinput_mouse(void* ev, int hwind, win_mouseEvents id, int w, int l) {
    if (id >= win_mouseEvents::MouseMove && id <= win_mouseEvents::XButtonDBl && UIInput_Mouse_Callback)
        UIInput_Mouse_Callback(id, w, l);
    return uiinput_mouse_orig(ev, hwind, id, w, l);
}
bool HookUIInput_Hooked = false;
void HookUIInput()
{
    if (HookUIInput_Hooked)
        return;
    HookUIInput_Hooked = true;
    shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x40747E), 0x40754C - 0x40747E + 10 };
    RedirectCall(reinterpret_cast<void*>(0x40754C), &uiinput_char);
    RedirectCall(reinterpret_cast<void*>(0x40757D), &uiinput_key);
    RedirectCall(reinterpret_cast<void*>(0x40747E), &uiinput_mouse);
}
