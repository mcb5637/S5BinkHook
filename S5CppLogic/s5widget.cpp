#include "pch.h"
#include "s5data.h"

struct shok_vtable_EGUIX_CBaseWidget {
    PADDINGI(7);
    int(__thiscall* SetSizeAndPos)(shok_EGUIX_CBaseWidget* th, float x, float y, float w, float h); // 7
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


byte* shok_EGUIX_CBaseWidget::GetUpdateManualFlag()
{
    if (vtable == shok_vtp_EGUIX_CStaticTextWidget)
        return &((shok_EGUIX_CStaticTextWidget*)this)->UpdateManualFlag;
    if (vtable == shok_vtp_EGUIX_CGfxButtonWidget)
        return &((shok_EGUIX_CGfxButtonWidget*)this)->UpdateManualFlag;
    if (vtable == shok_vtp_EGUIX_CTextButtonWidget)
        return &((shok_EGUIX_CTextButtonWidget*)this)->UpdateManualFlag;
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
    return nullptr;
}
