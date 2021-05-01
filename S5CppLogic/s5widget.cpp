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


static inline int(__thiscall* widman_getidbyname)(shok_widgetManager* th, const char* n) = (int(__thiscall*)(shok_widgetManager*, const char*))0x5588A0;
int shok_widgetManager::GetIdByName(const char* name)
{
    return widman_getidbyname(this, name);
}

static inline shok_EGUIX_CBaseWidget* (__thiscall* widman_getwid)(shok_widgetManager* th, int id) = (shok_EGUIX_CBaseWidget * (__thiscall*)(shok_widgetManager*, int))0x558966;
shok_EGUIX_CBaseWidget* shok_widgetManager::GetWidgetByID(int id)
{
    return widman_getwid(this, id);
}

int shok_EGUIX_CBaseWidget::SetPosAndSize(float x, float y, float w, float h)
{
    return ((shok_vtable_EGUIX_CBaseWidget*)vtable)->SetSizeAndPos(this, x, y, w, h);
}

static inline void(__thiscall* funchelper_call)(shok_EGUIX_CLuaFunctionHelper* th, int widgetID) = (void(__thiscall*)(shok_EGUIX_CLuaFunctionHelper*, int))0x55BDDE;
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

static inline int(__thiscall* widman_registerName)(shok_widgetManager* th, const char* name) = (int(__thiscall*)(shok_widgetManager*, const char*))0x55884F;
static inline void(__thiscall* widman_addWidget)(shok_widgetManager* th, shok_EGUIX_CBaseWidget* a, int id) = (void(__thiscall*)(shok_widgetManager*, shok_EGUIX_CBaseWidget*, int))0x558AA2;
void shok_EGUIX_CContainerWidget::AddWidget(shok_EGUIX_CBaseWidget* toAdd, const char* name)
{
    shok_widgetManager* m = shok_getWidgetManagerObj();
    int newId = widman_registerName(m, name);
    if (newId) {
        toAdd->WidgetID = newId;
        widman_addWidget(m, toAdd, newId);
        ((shok_vtable_EGUIX_CContainerWidget*)vtable)->AddChild(this, toAdd);
    }
}

void initMaterial(shok_EGUIX_CMaterial& m) {
    m.vtable = shok_vtp_EGUIX_CMaterial;
}
void initStringHelper(shok_EGUIX_CWidgetStringHelper& h) {
    h.vtable = shok_vtp_EGUIX_CWidgetStringHelper;
    h.FontHandler.vtable = shok_vtp_EGUIX_CFontIDHandler;
    h.SingleStringHandler.vtable = shok_vtp_EGUIX_CSingleStringHandler;
    h.FontHandler.FontID = 1;
    h.SingleStringHandler.RawString = shok_string("");
    h.SingleStringHandler.StringTableKey = shok_string("");
}
void initLuaFunc(shok_EGUIX_CLuaFunctionHelper& h) {
    h.vtable = shok_vtp_EGUIX_CLuaFunctionHelper;
    h.FuncRefCommand.vtable = shok_vtp_EScr_CLuaFuncRefCommand;
    h.LuaCommand = shok_string("");
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
