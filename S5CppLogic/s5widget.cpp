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


static inline int(__thiscall* const widman_getidbyname)(shok_widgetManager* th, const char* n) = (int(__thiscall*)(shok_widgetManager*, const char*))0x5588A0;
int shok_widgetManager::GetIdByName(const char* name)
{
    return widman_getidbyname(this, name);
}

static inline shok_EGUIX_CBaseWidget* (__thiscall* const widman_getwid)(shok_widgetManager* th, int id) = (shok_EGUIX_CBaseWidget * (__thiscall*)(shok_widgetManager*, int))0x558966;
shok_EGUIX_CBaseWidget* shok_widgetManager::GetWidgetByID(int id)
{
    return widman_getwid(this, id);
}

void shok_EGUIX_CFontIDHandler::LoadFont(const char* name)
{
    shok_fontManager::LoadFont(&FontID, name);
}

int shok_EGUIX_CBaseWidget::SetPosAndSize(float x, float y, float w, float h)
{
    return ((shok_vtable_EGUIX_CBaseWidget*)vtable)->SetSizeAndPos(this, x, y, w, h);
}

static inline void(__thiscall* const funchelper_call)(shok_EGUIX_CLuaFunctionHelper* th, int widgetID) = (void(__thiscall*)(shok_EGUIX_CLuaFunctionHelper*, int))0x55BDDE;
void shok_EGUIX_CLuaFunctionHelper::Call(int widgetID)
{
    funchelper_call(this, widgetID);
}


byte* shok_EGUIX_CBaseWidget::GetUpdateManualFlag()
{
    if (vtable == shok_vtp_EGUIX_CStaticTextWidget)
        return &((shok_EGUIX_CStaticTextWidget*)this)->UpdateManualFlag;
    if (vtable == shok_vtp_EGUIX_CGfxButtonWidget)
        return &((shok_EGUIX_CGfxButtonWidget*)this)->UpdateManualFlag;
    if (vtable == shok_vtp_EGUIX_CTextButtonWidget)
        return &((shok_EGUIX_CTextButtonWidget*)this)->UpdateManualFlag;
    if (vtable == shok_vtp_EGUIX_CProgressBarWidget)
        return &((shok_EGUIX_CProgressBarWidget*)this)->UpdateManualFlag;
    return nullptr;
}
shok_EGUIX_CLuaFunctionHelper* shok_EGUIX_CBaseWidget::GetUpdateFunc()
{
    if (vtable == shok_vtp_EGUIX_CStaticTextWidget)
        return &((shok_EGUIX_CStaticTextWidget*)this)->UpdateFunction;
    if (vtable == shok_vtp_EGUIX_CGfxButtonWidget)
        return &((shok_EGUIX_CGfxButtonWidget*)this)->UpdateFunction;
    if (vtable == shok_vtp_EGUIX_CTextButtonWidget)
        return &((shok_EGUIX_CTextButtonWidget*)this)->UpdateFunction;
    if (vtable == shok_vtp_EGUIX_CProgressBarWidget)
        return &((shok_EGUIX_CProgressBarWidget*)this)->UpdateFunction;
    return nullptr;
}
bool shok_EGUIX_CBaseWidget::IsContainerWidget()
{
    return vtable == shok_vtp_EGUIX_CContainerWidget;
}
shok_EGUIX_CMaterial* shok_EGUIX_CBaseWidget::GetMaterials(int* count)
{
    if (vtable == shok_vtp_EGUIX_CStaticWidget || vtable == shok_vtp_EGUIX_CStaticTextWidget || vtable == shok_vtp_EGUIX_CProgressBarWidget) {
        *count = 1;
        return &((shok_EGUIX_CStaticWidget*)this)->BackgroundMaterial;
    }
    if (vtable == shok_vtp_EGUIX_CGfxButtonWidget || vtable == shok_vtp_EGUIX_CTextButtonWidget) {
        *count = 5;
        return ((shok_EGUIX_CButtonWidget*)this)->Materials;
    }
    return nullptr;
}
shok_EGUIX_CButtonHelper* shok_EGUIX_CBaseWidget::GetButtonHelper()
{
    if (vtable == shok_vtp_EGUIX_CGfxButtonWidget || vtable == shok_vtp_EGUIX_CTextButtonWidget || vtable == shok_vtp_EGUIX_CButtonWidget)
        return &((shok_EGUIX_CButtonWidget*)this)->ButtonHelper;
    return nullptr;
}
shok_EGUIX_CToolTipHelper* shok_EGUIX_CBaseWidget::GetTooltipHelper()
{
    if (vtable == shok_vtp_EGUIX_CGfxButtonWidget || vtable == shok_vtp_EGUIX_CTextButtonWidget || vtable == shok_vtp_EGUIX_CButtonWidget)
        return &((shok_EGUIX_CButtonWidget*)this)->ToolTipHelper;
    if (vtable == shok_vtp_EGUIX_CPureTooltipWidget)
        return &((shok_EGUIX_CPureTooltipWidget*)this)->ToolTipHelper;
    return nullptr;
}
shok_EGUIX_CWidgetStringHelper* shok_EGUIX_CBaseWidget::GetStringHelper()
{
    if (vtable == shok_vtp_EGUIX_CStaticTextWidget)
        return &((shok_EGUIX_CStaticTextWidget*)this)->StringHelper;
    if (vtable == shok_vtp_EGUIX_CTextButtonWidget)
        return &((shok_EGUIX_CTextButtonWidget*)this)->StringHelper;
    return nullptr;
}

static inline int(__thiscall* const widman_registerName)(shok_widgetManager* th, const char* name) = (int(__thiscall*)(shok_widgetManager*, const char*))0x55884F;
static inline void(__thiscall* const widman_addWidget)(shok_widgetManager* th, shok_EGUIX_CBaseWidget* a, int id) = (void(__thiscall*)(shok_widgetManager*, shok_EGUIX_CBaseWidget*, int))0x558AA2;
void shok_EGUIX_CContainerWidget::AddWidget(shok_EGUIX_CBaseWidget* toAdd, const char* name, const shok_EGUIX_CBaseWidget* before)
{
    shok_widgetManager* m = shok_getWidgetManagerObj();
    int newId = widman_registerName(m, name);
    if (newId) {
        toAdd->WidgetID = newId;
        widman_addWidget(m, toAdd, newId);
        ((shok_vtable_EGUIX_CContainerWidget*)vtable)->AddChild(this, toAdd);
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
    m.vtable = shok_vtp_EGUIX_CMaterial;
    m.TextureCoordinates = { 0, 0, 1, 1 };
    m.Color = { 0xFF, 0xFF, 0xFF, 0xFF };
}
void initSingleSting(shok_EGUIX_CSingleStringHandler& h) {
    h.RawString = shok_string("");
    h.StringTableKey = shok_string("");
}
void initStringHelper(shok_EGUIX_CWidgetStringHelper& h) {
    h.vtable = shok_vtp_EGUIX_CWidgetStringHelper;
    h.FontHandler.vtable = shok_vtp_EGUIX_CFontIDHandler;
    h.SingleStringHandler.vtable = shok_vtp_EGUIX_CSingleStringHandler;
    h.FontHandler.FontID = 1;
    h.Color = { 0, 0, 0, 0xFF };
    initSingleSting(h.SingleStringHandler);
}
void initLuaFunc(shok_EGUIX_CLuaFunctionHelper& h) {
    h.vtable = shok_vtp_EGUIX_CLuaFunctionHelper;
    h.FuncRefCommand.vtable = shok_vtp_EScr_CLuaFuncRefCommand;
    h.LuaCommand = shok_string("");
}
void initTooltipHelper(shok_EGUIX_CToolTipHelper& h) {
    h.vtable = shok_vtp_EGUIX_CToolTipHelper;
    h.vtable_EGUIX_IWidgetRegistrationCallback = (void*)0x780B28;
    initSingleSting(h.ToolTipString);
    initLuaFunc(h.UpdateFunction);
}
void initButtonHelper(shok_EGUIX_CButtonHelper& h) {
    h.vtable = shok_vtp_EGUIX_CButtonHelper;
    initSingleSting(h.ShortCutString);
    initLuaFunc(h.ActionFunction);
}
static inline void(__thiscall* const widlisthandler_ctor)(shok_EGUIX_CWidgetListHandler* th) = (void(__thiscall*)(shok_EGUIX_CWidgetListHandler*))0x55BB26;
void initWidgetListHandler(shok_EGUIX_CWidgetListHandler& h) {
    widlisthandler_ctor(&h);
}

shok_EGUIX_CStaticWidget* shok_EGUIX_CStaticWidget::Create()
{
    shok_EGUIX_CStaticWidget* r = (shok_EGUIX_CStaticWidget*)shok_malloc(sizeof(shok_EGUIX_CStaticWidget));
    memset(r, 0, sizeof(shok_EGUIX_CStaticWidget));
    r->vtable = shok_vtp_EGUIX_CStaticWidget;
    r->vtable_EGUIX_IOnEvent = (void*)0x780F78;
    r->vtable_EGUIX_IRender = (void*)0x780F6C;
    r->vtable_EGUIX_IMaterialAccess = (void*)0x780F58;
    initMaterial(r->BackgroundMaterial);
    return r;
}
shok_EGUIX_CStaticTextWidget* shok_EGUIX_CStaticTextWidget::Create()
{
    shok_EGUIX_CStaticTextWidget* r = (shok_EGUIX_CStaticTextWidget*)shok_malloc(sizeof(shok_EGUIX_CStaticTextWidget));
    memset(r, 0, sizeof(shok_EGUIX_CStaticTextWidget));
    r->vtable = shok_vtp_EGUIX_CStaticTextWidget;
    r->vtable_EGUIX_IOnEvent = (void*)0x780ED8;
    r->vtable_EGUIX_IRender = (void*)0x780ECC;
    r->vtable_EGUIX_IMaterialAccess = (void*)0x780EB8;
    r->vtable_EGUIX_ITextAccess = (void*)0x780E90;
    initMaterial(r->BackgroundMaterial);
    initStringHelper(r->StringHelper);
    initLuaFunc(r->UpdateFunction);
    r->UpdateManualFlag = true;
    return r;
}
shok_EGUIX_CPureTooltipWidget* shok_EGUIX_CPureTooltipWidget::Create()
{
    shok_EGUIX_CPureTooltipWidget* r = (shok_EGUIX_CPureTooltipWidget*)shok_malloc(sizeof(shok_EGUIX_CPureTooltipWidget));
    memset(r, 0, sizeof(shok_EGUIX_CPureTooltipWidget));
    r->vtable = shok_vtp_EGUIX_CPureTooltipWidget;
    r->vtable_EGUIX_IOnEvent = (void*)0x780BA4;
    initTooltipHelper(r->ToolTipHelper);
    return r;
}
shok_EGUIX_CGfxButtonWidget* shok_EGUIX_CGfxButtonWidget::Create()
{
    shok_EGUIX_CGfxButtonWidget* r = (shok_EGUIX_CGfxButtonWidget*)shok_malloc(sizeof(shok_EGUIX_CGfxButtonWidget));
    memset(r, 0, sizeof(shok_EGUIX_CGfxButtonWidget));
    r->vtable = shok_vtp_EGUIX_CGfxButtonWidget;
    r->vtable_EGUIX_IOnEvent = (void*)0x780CC4;
    r->vtable_EGUIX_IRender = (void*)0x780CB8;
    r->vtable_EGUIX_IMaterialAccess = (void*)0x780CA4;
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
    shok_EGUIX_CTextButtonWidget* r = (shok_EGUIX_CTextButtonWidget*)shok_malloc(sizeof(shok_EGUIX_CTextButtonWidget));
    memset(r, 0, sizeof(shok_EGUIX_CTextButtonWidget));
    r->vtable = shok_vtp_EGUIX_CTextButtonWidget;
    r->vtable_EGUIX_IOnEvent = (void*)0x780DA4;
    r->vtable_EGUIX_IRender = (void*)0x780D98;
    r->vtable_EGUIX_IMaterialAccess = (void*)0x780D84;
    r->vtable_EGUIX_ITextAccess = (void*)0x780D5C;
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
    shok_EGUIX_CProgressBarWidget* r = (shok_EGUIX_CProgressBarWidget*)shok_malloc(sizeof(shok_EGUIX_CProgressBarWidget));
    memset(r, 0, sizeof(shok_EGUIX_CProgressBarWidget));
    r->vtable = shok_vtp_EGUIX_CProgressBarWidget;
    r->vtable_EGUIX_IOnEvent = (void*)0x780C14;
    r->vtable_EGUIX_IRender = (void*)0x780C08;
    r->vtable_EGUIX_IMaterialAccess = (void*)0x780BF4;
    initMaterial(r->BackgroundMaterial);
    initLuaFunc(r->UpdateFunction);
    r->ProgressBarLimit = 1.0f;
    r->ProgressBarValue = 1.0f;
    return r;
}
shok_EGUIX_CContainerWidget* shok_EGUIX_CContainerWidget::Create()
{
    shok_EGUIX_CContainerWidget* r = (shok_EGUIX_CContainerWidget*)shok_malloc(sizeof(shok_EGUIX_CContainerWidget));
    memset(r, 0, sizeof(shok_EGUIX_CContainerWidget));
    r->vtable = shok_vtp_EGUIX_CContainerWidget;
    r->vtable_EGUIX_IOnEvent = (void*)0x781140;
    r->vtable_EGUIX_IRender = (void*)0x781134;
    r->vtable_EGUIX_IWidgetRegistrationCallback = (void*)0x78112C;
    initWidgetListHandler(r->WidgetListHandler);
    return r;
}

static inline void(__stdcall* const loadfont)(int* out, const char* name) = (void(__stdcall*)(int*, const char*))0x55D99E;
void shok_fontManager::LoadFont(int* outFontID, const char* fontName)
{
    loadfont(outFontID, fontName);
}
static inline shok_font* (__thiscall* const fontmng_getfont)(shok_fontManager* th, int id) = (shok_font * (__thiscall*)(shok_fontManager*, int)) 0x5597C7;
shok_font* shok_fontManager::GetFontObj(int id)
{
    return fontmng_getfont(this, id);
}
