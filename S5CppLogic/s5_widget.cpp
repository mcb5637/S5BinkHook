#include "pch.h"
#include "s5_widget.h"
#include "s5_defines.h"
#include "s5_events.h"
#include "s5_mem.h"
#include "s5_RWEngine.h"
#include "s5_idmanager.h"
#include "s5_mapdisplay.h"
#include "hooks.h"
#include "mod.h"
#include "EntityAddonData.h"

struct shok_vtable_EGUIX_CBaseWidget : BB::IObject::_vtableS {
    PADDINGI(4);
    int(__thiscall* SetSizeAndPos)(EGUIX::CBaseWidget* th, float x, float y, float w, float h); // 7
    PADDINGI(4);
};
struct shok_vtable_EGUIX_CContainerWidget : shok_vtable_EGUIX_CBaseWidget {
    void(__thiscall* AddChild)(EGUIX::CContainerWidget* th, EGUIX::CBaseWidget* ch); // 12
};
//constexpr int i = offsetof(shok_vtable_EGUIX_CContainerWidget, AddChild) / 4;

shok::Color EGUIX::Color::ToShokColor() const
{
    return shok::Color{ Red, Green, Blue, Alpha };
}

static inline int(__thiscall* const texman_getid)(EGUIX::TextureManager* th, const char* n) = reinterpret_cast<int(__thiscall*)(EGUIX::TextureManager*, const char*)>(0x55664A);
int EGUIX::TextureManager::GetTextureID(const char* name)
{
    return texman_getid(this, name);
}
static inline int(__thiscall* const texman_getidnoadd)(EGUIX::TextureManager* th, const char* n) = reinterpret_cast<int(__thiscall*)(EGUIX::TextureManager*, const char*)>(0x5566A5);
int EGUIX::TextureManager::GetTextureIDNoAdd(const char* name)
{
    return texman_getidnoadd(this, name);
}
static inline RWE::RwTexture* (__thiscall* const texman_gettex)(EGUIX::TextureManager* th, int id) = reinterpret_cast<RWE::RwTexture* (__thiscall*)(EGUIX::TextureManager*, int)>(0x556B1C);
RWE::RwTexture* EGUIX::TextureManager::GetTextureByID(int id)
{
    return texman_gettex(this, id);
}
static inline RWE::RwTexture* (__thiscall* const texman_reloadtext)(EGUIX::TextureManager* th, int id) = reinterpret_cast<RWE::RwTexture * (__thiscall*)(EGUIX::TextureManager*, int)>(0x556D03);
RWE::RwTexture* EGUIX::TextureManager::ReloadTexture(int id)
{
    if (id >= static_cast<int>(Textures.size()))
        return GetTextureByID(id);
    return texman_reloadtext(this, id); // this thing actually throws if the texture vector is too small
}
void EGUIX::TextureManager::FreeTexture(int id)
{
    Textures[id]->Destroy();
    Textures[id] = nullptr;
}

unsigned int __stdcall EGUIX::CFontIDHandler::GetClassIdentifier() const
{
    return Identifier;
}

void EGUIX::CFontIDHandler::LoadFont(const char* name)
{
    EGUIX::FontManager::LoadFont(&FontID, name);
}

unsigned int __stdcall EGUIX::CSingleStringHandler::GetClassIdentifier() const
{
    return Identifier;
}

static inline const char* (__thiscall* const sinstringhand_getstr)(EGUIX::CSingleStringHandler* th) = reinterpret_cast<const char* (__thiscall*)(EGUIX::CSingleStringHandler*)>(0x55BFBC);
const char* EGUIX::CSingleStringHandler::GetString()
{
    return sinstringhand_getstr(this);
}

unsigned int __stdcall EGUIX::CWidgetStringHelper::GetClassIdentifier() const
{
    return Identifier;
}

unsigned int __stdcall EGUIX::CMaterial::GetClassIdentifier() const
{
    return Identifier;
}

EGUIX::CMaterial::CMaterial()
{
    *reinterpret_cast<int*>(this) = vtp;
}

static inline int(__thiscall* const materi_settex)(EGUIX::CMaterial* th, const char* n) = reinterpret_cast<int(__thiscall*)(EGUIX::CMaterial*, const char*)>(0x55AA38);
void EGUIX::CMaterial::SetTexture(const char* name)
{
    materi_settex(this, name);
}

static inline void(__thiscall* const materi_getsize)(EGUIX::CMaterial* th, float* x, float* y) = reinterpret_cast<void(__thiscall*)(EGUIX::CMaterial*, float*, float*)>(0x55AA52);
shok::Position EGUIX::CMaterial::GetSize()
{
    shok::Position r{};
    materi_getsize(this, &r.X, &r.Y);
    return r;
}

RWE::RwTexture* EGUIX::CMaterial::GetTexture()
{
    return TextureManager::GlobalObj()->GetTextureByID(Texture);
}


unsigned int __stdcall EGUIX::CLuaFunctionHelper::GetClassIdentifier() const
{
    return Identifier;
}

static inline void(__thiscall* const funchelper_call)(EGUIX::CLuaFunctionHelper* th, int widgetID) = reinterpret_cast<void(__thiscall*)(EGUIX::CLuaFunctionHelper*, int)>(0x55BDDE);
void EGUIX::CLuaFunctionHelper::Call(int widgetID)
{
    funchelper_call(this, widgetID);
}

unsigned int __stdcall EGUIX::CButtonHelper::GetClassIdentifier() const
{
    return Identifier;
}

static inline void(__thiscall* const butthelp_callaction)(EGUIX::CButtonHelper* th, int wid) = reinterpret_cast<void(__thiscall*)(EGUIX::CButtonHelper*, int)>(0x55A3F1);
void EGUIX::CButtonHelper::PressButton(int widgetID)
{
    butthelp_callaction(this, widgetID);
}

bool shortcutsignextend_hooked = false;
void EGUIX::CButtonHelper::HookShortcutSignExtend()
{
    if (shortcutsignextend_hooked)
        return;
    shortcutsignextend_hooked = true;
    CppLogic::Hooks::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x55A62B), 10 };
    *reinterpret_cast<byte*>(0x55A62B) = 0xB6; // movsx to movzx (sign-extend to zero-extend) (middle part of 3 byte opcode)
}

shok::Keys extractKey(char c) {
    return static_cast<shok::Keys>(static_cast<unsigned int>(static_cast<unsigned char>(c)));
}
shok::Keys extractModifiers(char c) {
    return static_cast<shok::Keys>(static_cast<unsigned int>(static_cast<unsigned char>(c)) << 16) & (shok::Keys::ModifierAlt | shok::Keys::ModifierControl | shok::Keys::ModifierShift);
}
bool __stdcall buttohelp_shortcutcomparison(EGUIX::CButtonHelper* th, BB::CEvent* ev, EGUIX::CBaseWidget* wid) {
    if (th->CurrentState == 3 || th->CurrentState == 2)
        return false;
    const char* keystr = th->ShortCutString.GetString();
    if (!keystr)
        return false;

    shok::Keys key = shok::Keys::None;
    shok::Keys modif = shok::Keys::None;
    shok::Keys ignoremodif = shok::Keys::None;
    bool mayscin4 = false;

    size_t len = strnlen(keystr, 10);
    if (len == 1) {
        key = extractKey(keystr[0]);
    }
    else if (len == 4) {
        key = extractKey(keystr[0]);
        modif = extractModifiers(keystr[1]);
        ignoremodif = extractModifiers(keystr[2]);
        mayscin4 = keystr[3] > 2;
    }
    else {
        return false;
    }
    if (th->CurrentState == 4 && !mayscin4)
        return false;

    BB::CKeyEvent* kev = dynamic_cast<BB::CKeyEvent*>(ev);
    if (!kev)
        return false;

    if ((kev->KeyData & (~ignoremodif)) == (key | modif)) {
        th->PressButton(wid->WidgetID);
        return true;
    }

    return false;
}
void __declspec(naked) buttohelp_shortcutcomparisonasm() {
    __asm {

        push[ebp + 0x10];
        push[ebp + 0xC]; // for some reason, the same event is in 2 arguments...
        push esi;
        call buttohelp_shortcutcomparison;


        push 0x55A64A;
        ret;
    };
}

bool shortcutcmo_hooked = false;
void EGUIX::CButtonHelper::HookShortcutComparison()
{
    if (shortcutcmo_hooked)
        return;
    shortcutcmo_hooked = true;
    CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x55A597), 0x30 };
    CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x55A597), &buttohelp_shortcutcomparisonasm, reinterpret_cast<void*>(0x55A5A5));
}

unsigned int __stdcall EGUIX::CToolTipHelper::GetClassIdentifier() const
{
    return Identifier;
}

void EGUIX::CToolTipHelper::OnRegistrationChanged(int id, int mode)
{
    throw 0;
}


bool* EGUIX::CBaseWidget::GetUpdateManualFlag()
{
    if (EGUIX::CStaticTextWidget* w = dynamic_cast<EGUIX::CStaticTextWidget*>(this))
        return &w->UpdateManualFlag;
    if (EGUIX::CGfxButtonWidget* w = dynamic_cast<EGUIX::CGfxButtonWidget*>(this))
        return &w->UpdateManualFlag;
    if (EGUIX::CTextButtonWidget* w = dynamic_cast<EGUIX::CTextButtonWidget*>(this))
        return &w->UpdateManualFlag;
    if (EGUIX::CProgressBarWidget* w = dynamic_cast<EGUIX::CProgressBarWidget*>(this))
        return &w->UpdateManualFlag;
    return nullptr;
}
EGUIX::CLuaFunctionHelper* EGUIX::CBaseWidget::GetUpdateFunc()
{
    if (EGUIX::CStaticTextWidget* w = dynamic_cast<EGUIX::CStaticTextWidget*>(this))
        return &w->UpdateFunction;
    if (EGUIX::CGfxButtonWidget* w = dynamic_cast<EGUIX::CGfxButtonWidget*>(this))
        return &w->UpdateFunction;
    if (EGUIX::CTextButtonWidget* w = dynamic_cast<EGUIX::CTextButtonWidget*>(this))
        return &w->UpdateFunction;
    if (EGUIX::CProgressBarWidget* w = dynamic_cast<EGUIX::CProgressBarWidget*>(this))
        return &w->UpdateFunction;
    return nullptr;
}
EGUIX::CMaterial* EGUIX::CBaseWidget::GetMaterials(int* count)
{
    if (EGUIX::CStaticWidget* w = dynamic_cast<EGUIX::CStaticWidget*>(this)) {
        *count = 1;
        return &w->BackgroundMaterial;
    }
    if (EGUIX::CButtonWidget* w = dynamic_cast<EGUIX::CButtonWidget*>(this)) {
        *count = 5;
        return w->Materials;
    }
    return nullptr;
}
EGUIX::CButtonHelper* EGUIX::CBaseWidget::GetButtonHelper()
{
    if (EGUIX::CButtonWidget* w = dynamic_cast<EGUIX::CButtonWidget*>(this))
        return &w->ButtonHelper;
    return nullptr;
}
EGUIX::CToolTipHelper* EGUIX::CBaseWidget::GetTooltipHelper()
{
    if (EGUIX::CButtonWidget* w = dynamic_cast<EGUIX::CButtonWidget*>(this))
        return &w->ToolTipHelper;
    if (EGUIX::CPureTooltipWidget* w = dynamic_cast<EGUIX::CPureTooltipWidget*>(this))
        return &w->ToolTipHelper;
    return nullptr;
}
EGUIX::CWidgetStringHelper* EGUIX::CBaseWidget::GetStringHelper()
{
    if (EGUIX::CStaticTextWidget* w = dynamic_cast<EGUIX::CStaticTextWidget*>(this))
        return &w->StringHelper;
    if (EGUIX::CTextButtonWidget* w = dynamic_cast<EGUIX::CTextButtonWidget*>(this))
        return &w->StringHelper;
    return nullptr;
}


static inline void(__thiscall* const minimappulse_ctor)(GGUI::CMiniMapSignalPulse* th, float x, float y, int colorcode, int mode, float time1, float speed2, float time2) = reinterpret_cast<void(__thiscall*)(GGUI::CMiniMapSignalPulse*, float, float, int, int, float, float, float)>(0x53D595);
GGUI::CMiniMapSignalPulse::CMiniMapSignalPulse(float x, float y, bool pulsing, int r, int g, int b, float timeFactor, float scaleFactor)
{
    minimappulse_ctor(this, x, y, 0, pulsing ? 1 : 2, 2.0f * timeFactor, 2.5f, (pulsing ? 0.8f : 0.128f) * scaleFactor);
    R = r;
    G = g;
    B = b;
}

static inline void(__thiscall* const minimapsignaldefault_ctor)(GGUI::CMiniMapSignalDefault* th, float x, float y, int colorcode) = reinterpret_cast<void(__thiscall*)(GGUI::CMiniMapSignalDefault*, float, float, int)>(0x53D688);
GGUI::CMiniMapSignalDefault::CMiniMapSignalDefault(float x, float y, int r, int g, int b, float scaleFactor)
{
    minimapsignaldefault_ctor(this, x, y, 0);
    R = r;
    G = g;
    B = b;
    Scale *= scaleFactor;
}

static inline void(__thiscall* const minimapmarkerhandler_addpulse)(shok::Vector<GGUI::CMiniMapSignalPulse>* th, GGUI::CMiniMapSignalPulse* p) = reinterpret_cast<void(__thiscall*)(shok::Vector<GGUI::CMiniMapSignalPulse>*, GGUI::CMiniMapSignalPulse*)>(0x52AFA5);
void GGUI::MiniMapMarkerHandler::CreateMarker(const shok::Position& p, bool pulsing, int r, int g, int b, float timeFactor, float scaleFactor)
{
    static constexpr float scale = 1.0f / 100.0f;
    EGL::CGLETerrainHiRes* hire = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->HiRes;
    float x = p.X * scale / hire->MaxSizeX;
    float y = p.Y * scale / hire->MaxSizeY;
    GGUI::CMiniMapSignalPulse pul{ x, y, pulsing, r, g, b, timeFactor, scaleFactor };
    minimapmarkerhandler_addpulse(&Pulses, &pul);
}

static inline void(__thiscall* const minimapmarkerhandler_adddefault)(shok::Vector<GGUI::CMiniMapSignalDefault>* th, GGUI::CMiniMapSignalDefault* p) = reinterpret_cast<void(__thiscall*)(shok::Vector<GGUI::CMiniMapSignalDefault>*, GGUI::CMiniMapSignalDefault*)>(0x52AEA5);
void GGUI::MiniMapMarkerHandler::CreateSignalDefault(const shok::Position& p, int r, int g, int b, float scaleFactor)
{
    static constexpr float scale = 1.0f / 100.0f;
    EGL::CGLETerrainHiRes* hire = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->HiRes;
    float x = p.X * scale / hire->MaxSizeX;
    float y = p.Y * scale / hire->MaxSizeY;
    GGUI::CMiniMapSignalDefault def{ x, y, r, g, b, scaleFactor };
    minimapmarkerhandler_adddefault(&Defaults, &def);
}

static inline int(__thiscall* const widman_getidbyname)(EGUIX::WidgetManager* th, const char* n) = reinterpret_cast<int(__thiscall*)(EGUIX::WidgetManager*, const char*)>(0x5588A0);
int EGUIX::WidgetManager::GetIdByName(const char* name)
{
    return widman_getidbyname(this, name);
}

static inline EGUIX::CBaseWidget* (__thiscall* const widman_getwid)(EGUIX::WidgetManager* th, int id) = reinterpret_cast<EGUIX::CBaseWidget * (__thiscall*)(EGUIX::WidgetManager*, int)>(0x558966);
EGUIX::CBaseWidget* EGUIX::WidgetManager::GetWidgetByID(int id)
{
    return widman_getwid(this, id);
}
static inline void(__thiscall* const widman_removewid)(EGUIX::WidgetManager* th, EGUIX::CBaseWidget* w, int id) = reinterpret_cast<void(__thiscall*)(EGUIX::WidgetManager*, EGUIX::CBaseWidget * w, int)>(0x558915);
void EGUIX::WidgetManager::RemoveWidget(EGUIX::CBaseWidget* w)
{
    widman_removewid(this, w, w->WidgetID);
}

static inline int(__thiscall* const widgroupman_getgroupid)(EGUIX::CWidgetGroupManager* th, const char* s) = reinterpret_cast<int(__thiscall*)(EGUIX::CWidgetGroupManager*, const char*)>(0x583214);
int EGUIX::CWidgetGroupManager::GetGroupId(const char* s)
{
    return widgroupman_getgroupid(this, s);
}
static inline int(__thiscall* const widgroupman_creategroup)(int man, const char* s, int z) = reinterpret_cast<int(__thiscall*)(int, const char*, int)>(0x54F656);
int EGUIX::CWidgetGroupManager::CreateGroup(const char* s)
{
    return widgroupman_creategroup(*reinterpret_cast<int*>(0x894EA4), s, 0);
}

static inline int(__thiscall* const widman_registerName)(EGUIX::WidgetManager* th, const char* name) = reinterpret_cast<int(__thiscall*)(EGUIX::WidgetManager*, const char*)>(0x55884F);
static inline void(__thiscall* const widman_addWidget)(EGUIX::WidgetManager* th, EGUIX::CBaseWidget* a, int id) = reinterpret_cast<void(__thiscall*)(EGUIX::WidgetManager*, EGUIX::CBaseWidget*, int)>(0x558AA2);
void EGUIX::CContainerWidget::AddWidget(EGUIX::CBaseWidget* toAdd, const char* name, const EGUIX::CBaseWidget* before)
{
    EGUIX::WidgetManager* m = EGUIX::WidgetManager::GlobalObj();
    int newId = widman_registerName(m, name);
    if (newId) {
        toAdd->WidgetID = newId;
        widman_addWidget(m, toAdd, newId);
        AddChild(toAdd);
        if (before) {
            auto l = WidgetListHandler.SubWidgets.SaveList();
            auto it = l.List.begin();
            while (it != l.List.end()) {
                if (*it == before) {
                    l.List.splice(it, l.List, --l.List.end());
                    break;
                }

                it++;
            }
        }
    }
}

void(__thiscall* const shok_EGUIX_CLuaFunctionHelper_ctor)(EGUIX::CLuaFunctionHelper* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CLuaFunctionHelper * th)>(0x55BEA2);
static inline void(__thiscall* const widlisthandler_ctor)(EGUIX::CWidgetListHandler* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CWidgetListHandler*)>(0x55BB26);

static inline void(__thiscall* const statwid_ctor)(EGUIX::CStaticWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CStaticWidget*)>(0x5603F1);
EGUIX::CStaticWidget* EGUIX::CStaticWidget::Create()
{
    EGUIX::CStaticWidget* r = reinterpret_cast<EGUIX::CStaticWidget*>(shok::Malloc(sizeof(EGUIX::CStaticWidget)));
    memset(r, 0, sizeof(EGUIX::CStaticWidget));
    statwid_ctor(r);
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const stattextwid_ctor)(EGUIX::CStaticTextWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CStaticTextWidget*)>(0x55FBB1);
EGUIX::CStaticTextWidget* EGUIX::CStaticTextWidget::Create()
{
    EGUIX::CStaticTextWidget* r = reinterpret_cast<EGUIX::CStaticTextWidget*>(shok::Malloc(sizeof(EGUIX::CStaticTextWidget)));
    memset(r, 0, sizeof(EGUIX::CStaticTextWidget));
    stattextwid_ctor(r);
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const purettwid_ctor)(EGUIX::CPureTooltipWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CPureTooltipWidget*)>(0x55DF3E);
EGUIX::CPureTooltipWidget* EGUIX::CPureTooltipWidget::Create()
{
    EGUIX::CPureTooltipWidget* r = reinterpret_cast<EGUIX::CPureTooltipWidget*>(shok::Malloc(sizeof(EGUIX::CPureTooltipWidget)));
    memset(r, 0, sizeof(EGUIX::CPureTooltipWidget));
    purettwid_ctor(r);
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const gfxbutwid_ctor)(EGUIX::CGfxButtonWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CGfxButtonWidget*)>(0x55E917);
EGUIX::CGfxButtonWidget* EGUIX::CGfxButtonWidget::Create()
{
    EGUIX::CGfxButtonWidget* r = reinterpret_cast<EGUIX::CGfxButtonWidget*>(shok::Malloc(sizeof(EGUIX::CGfxButtonWidget)));
    memset(r, 0, sizeof(EGUIX::CGfxButtonWidget));
    gfxbutwid_ctor(r);
    r->IsShown = false;
    r->IconMaterial.Color.Alpha = 0;
    return r;
}
static inline void(__thiscall* const txtbutwid_ctor)(EGUIX::CTextButtonWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CTextButtonWidget*)>(0x55EFA3);
EGUIX::CTextButtonWidget* EGUIX::CTextButtonWidget::Create()
{
    EGUIX::CTextButtonWidget* r = reinterpret_cast<EGUIX::CTextButtonWidget*>(shok::Malloc(sizeof(EGUIX::CTextButtonWidget)));
    memset(r, 0, sizeof(EGUIX::CTextButtonWidget));
    txtbutwid_ctor(r);
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const progbarwid_ctor)(EGUIX::CProgressBarWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CProgressBarWidget*)>(0x55E300);
EGUIX::CProgressBarWidget* EGUIX::CProgressBarWidget::Create()
{
    EGUIX::CProgressBarWidget* r = reinterpret_cast<EGUIX::CProgressBarWidget*>(shok::Malloc(sizeof(EGUIX::CProgressBarWidget)));
    memset(r, 0, sizeof(EGUIX::CProgressBarWidget));
    progbarwid_ctor(r);
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const contwid_ctor)(EGUIX::CContainerWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CContainerWidget*)>(0x560DAD);
EGUIX::CContainerWidget* EGUIX::CContainerWidget::Create()
{
    EGUIX::CContainerWidget* r = reinterpret_cast<EGUIX::CContainerWidget*>(shok::Malloc(sizeof(EGUIX::CContainerWidget)));
    memset(r, 0, sizeof(EGUIX::CContainerWidget));
    contwid_ctor(r);
    r->IsShown = false;
    return r;
}

static inline void(__thiscall* const customwid_initwid)(EGUIX::CCustomWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CCustomWidget*)>(0x560BA3);
void EGUIX::CCustomWidget::InitializeCustomWidget()
{
    customwid_initwid(this);
}


static inline void(__thiscall* const onscreenmoti_render)(GGUI::COnScreenElementMotivation* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementMotivation*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x53E93C);
void GGUI::COnScreenElementMotivation::Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenmoti_render(this, screenPos, data);
}

static inline void(__thiscall* const onscreenbuilding_inactive)(GGUI::COnScreenElementBuilding* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementBuilding*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x540922);
void GGUI::COnScreenElementBuilding::RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenbuilding_inactive(this, screenPos, data);
}

static inline void(__thiscall* const onscreentask_render)(GGUI::COnScreenElementTask* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementTask*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x53F762);
void GGUI::COnScreenElementTask::Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreentask_render(this, screenPos, data);
}

static inline void(__thiscall* const onscreenexp_render)(GGUI::COnScreenElementExperience* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementExperience*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x53F392);
void GGUI::COnScreenElementExperience::Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenexp_render(this, screenPos, data);
}

static inline void(__thiscall* const onscreenhealthbar_render)(GGUI::COnScreenElementHealthbar* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementHealthbar*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x540109);
void GGUI::COnScreenElementHealthbar::Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenhealthbar_render(this, screenPos, data);
}

static inline void(__thiscall* const onscreenworkers_render)(GGUI::COnScreenElementWorkers* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementWorkers*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x540E9C);
void GGUI::COnScreenElementWorkers::Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenworkers_render(this, screenPos, data);
}

static inline void(__thiscall* const onscreenres_render)(GGUI::COnScreenElementResources* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementResources*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x53F5E8);
void GGUI::COnScreenElementResources::Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenres_render(this, screenPos, data);
}

static inline void(__thiscall* const onscreensleep_render)(GGUI::COnScreenElementSleepers* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementSleepers*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x540C74);
void GGUI::COnScreenElementSleepers::Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreensleep_render(this, screenPos, data);
}

static inline void(__thiscall* const onscreenchom_render)(GGUI::COnScreenElementChomping* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementChomping*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x540D88);
void GGUI::COnScreenElementChomping::Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenchom_render(this, screenPos, data);
}

static inline void(__thiscall* const onscreentype_render)(GGUI::COnScreenElementType* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data, int c) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementType*, const shok::Position*, const GGL::IGLGUIInterface::UIData*, int)>(0x53F8E6);
void GGUI::COnScreenElementType::Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data, bool centered)
{
    onscreentype_render(this, screenPos, data, centered);
}

const char* __stdcall GGUI::COnScreenElementType::GetDisplayName(const GGL::IGLGUIInterface::UIData* data)
{
    EGL::CGLEEntity* e = data->BuildingToDisplay;
    if (!e) {
        e = static_cast<EGL::CGLEEntity*>(data->Entity->Entity);
    }
    if (!e)
        return "";
    auto* d = e->GetAdditionalData(false);
    if (d && d->NameOverride.size() > 0) {
        return d->NameOverride.c_str();
    }
    return EGL::CGLEEntitiesProps::GetEntityTypeDisplayName(e->EntityType);
}
void __declspec(naked) displaynamehook_asm() {
    __asm {
        push edi;
        mov edi, ecx;

        push[ebp + 0xC];
        call GGUI::COnScreenElementType::GetDisplayName;

        push 0x53F91D;
        ret;
    }
}
bool HookDisplayName_Hooked = false;
void GGUI::COnScreenElementType::HookDisplayName()
{
    if (HookDisplayName_Hooked)
        return;
    HookDisplayName_Hooked = true;
    CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x53F8F1), 0x53F8F9 - 0x53F8F1 };
    CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x53F8F1), &displaynamehook_asm, reinterpret_cast<void*>(0x53F8F9));
}

static inline void(__thiscall* const onscreenrefiner_render)(GGUI::COnScreenElementRefiner* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementRefiner*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x53EBBB);
void GGUI::COnScreenElementRefiner::Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenrefiner_render(this, screenPos, data);
}

static inline void(__thiscall* const onscreensol_render)(GGUI::COnScreenElementSoldiers* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementSoldiers*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x540552);
void GGUI::COnScreenElementSoldiers::Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreensol_render(this, screenPos, data);
}

static inline void(__thiscall* const onscreennpc_inactive)(GGUI::COnScreenElementNpc* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementNpc*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x53EAB5);
void GGUI::COnScreenElementNpc::RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreennpc_inactive(this, screenPos, data);
}

static inline void(__thiscall* const onscreenhero_inactive)(GGUI::COnScreenElementHero* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementHero*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x53FBBA);
void GGUI::COnScreenElementHero::RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenhero_inactive(this, screenPos, data);
}

static inline void(__thiscall* const onscreenmerch_render)(GGUI::COnScreenElementMerchantBuilding* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementMerchantBuilding*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x54002D);
void GGUI::COnScreenElementMerchantBuilding::Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenmerch_render(this, screenPos, data);
}

static inline void(__thiscall* const onscreenbridge_inactive)(GGUI::COnScreenElementBridge* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementBridge*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x53ED0E);
void GGUI::COnScreenElementBridge::RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenbridge_inactive(this, screenPos, data);
}

static inline void(__thiscall* const onscreenthief_render)(GGUI::COnScreenElementThief* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementThief*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x53EEFE);
void GGUI::COnScreenElementThief::Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenthief_render(this, screenPos, data);
}

static inline void(__thiscall* const onscreenbombtick_inactive)(GGUI::COnScreenElementBombTick* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementBombTick*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x53F2F8);
void GGUI::COnScreenElementBombTick::RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenbombtick_inactive(this, screenPos, data);
}

static inline void(__thiscall* const onscreenplunder_inactive)(GGUI::COnScreenElementPlunder* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::COnScreenElementPlunder*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x53F0FB);
void GGUI::COnScreenElementPlunder::RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreenplunder_inactive(this, screenPos, data);
}

static inline void(__thiscall* const onscreeninfo_render_inactive)(GGUI::OnScreenInfoRenderer* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::OnScreenInfoRenderer*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x54167B);
void GGUI::OnScreenInfoRenderer::RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreeninfo_render_inactive(this, screenPos, data);
}

static inline void(__thiscall* const onscreeninfo_render_active)(GGUI::OnScreenInfoRenderer* th, const shok::Position* ScreenPos, const GGL::IGLGUIInterface::UIData* data) = reinterpret_cast<void(__thiscall*)(GGUI::OnScreenInfoRenderer*, const shok::Position*, const GGL::IGLGUIInterface::UIData*)>(0x54170B);
void GGUI::OnScreenInfoRenderer::RenderActive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data)
{
    onscreeninfo_render_active(this, screenPos, data);
}

inline void(__thiscall* const onscreeninfo_render_floaties)(GGUI::OnScreenInfoRenderer* th) = reinterpret_cast<void(__thiscall*)(GGUI::OnScreenInfoRenderer*)>(0x540FD7);
void GGUI::OnScreenInfoRenderer::RenderFloaties()
{
    onscreeninfo_render_floaties(this);
}

static inline EGUIX::CMaterial* (__thiscall* const onscreeninforender_getresicon)(GGUI::OnScreenInfoRenderer* th, shok::ResourceType rt) = reinterpret_cast<EGUIX::CMaterial * (__thiscall*)(GGUI::OnScreenInfoRenderer*, shok::ResourceType)>(0x541B43);
EGUIX::CMaterial* GGUI::OnScreenInfoRenderer::GetResourceIcon(shok::ResourceType rt)
{
    return onscreeninforender_getresicon(this, rt);
}

static inline EGUIX::CMaterial* (__thiscall* const onscreeninforender_getrefresicon)(GGUI::OnScreenInfoRenderer* th, shok::ResourceType rt) = reinterpret_cast<EGUIX::CMaterial * (__thiscall*)(GGUI::OnScreenInfoRenderer*, shok::ResourceType)>(0x541AD6);
EGUIX::CMaterial* GGUI::OnScreenInfoRenderer::GetRefinerResourceIcon(shok::ResourceType rt)
{
    return onscreeninforender_getrefresicon(this, rt);
}

static inline void(__thiscall* const showresfloatie)(GGUI::C3DOnScreenInformationCustomWidget* th, int eid, int am) = reinterpret_cast<void(__thiscall*)(GGUI::C3DOnScreenInformationCustomWidget*, int, int)>(0x536361);
void GGUI::C3DOnScreenInformationCustomWidget::ShowResourceFloatieOnEntity(int eid, int amount)
{
    showresfloatie(this, eid, amount);
}

void GGUI::C3DOnScreenInformationCustomWidget::HookResourceFloatieShowWood(bool showwood)
{
    CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x529067), 10 };
    *reinterpret_cast<byte*>(0x529067) = showwood ? 0xFF : static_cast<int>(shok::ResourceType::WoodRaw);
    CppLogic::Hooks::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x536362), 30 };
    CppLogic::Hooks::WriteNops(reinterpret_cast<void*>(0x536362), 4); // push arg
    CppLogic::Hooks::WriteNops(reinterpret_cast<void*>(0x536368), 9); // call, test, jz
}


EGUIX::CMaterial* __thiscall GGUI::OnScreenInfoRenderer::GetResourceIconOverride(shok::ResourceType rt)
{
    switch (rt) {
    case shok::ResourceType::Gold:
    case shok::ResourceType::GoldRaw:
        return &TextureData.Texture_ResGold;
    case shok::ResourceType::Stone:
    case shok::ResourceType::StoneRaw:
        return &TextureData.Texture_ResStone;
    case shok::ResourceType::Iron:
    case shok::ResourceType::IronRaw:
        return &TextureData.Texture_ResIron;
    case shok::ResourceType::Sulfur:
    case shok::ResourceType::SulfurRaw:
        return &TextureData.Texture_ResSulfur;
    case shok::ResourceType::Clay:
    case shok::ResourceType::ClayRaw:
        return &TextureData.Texture_ResMud;
    case shok::ResourceType::Wood:
    case shok::ResourceType::WoodRaw:
        return &TextureData.Texture_ResWood;
    default:
        return &TextureData.Texture_ResSulfur;
    }
}

void GGUI::C3DOnScreenInformationCustomWidget::HookResourceElementWood(bool showwood)
{
    CppLogic::Hooks::SaveVirtualProtect vp{ 10, {reinterpret_cast<void*>(0x53F62E),
        onscreeninforender_getresicon
    } };
    *reinterpret_cast<byte*>(0x53F62E) = showwood ? 0xFF : static_cast<int>(shok::ResourceType::WoodRaw);
    CppLogic::Hooks::WriteJump(onscreeninforender_getresicon, CppLogic::Hooks::MemberFuncPointerToVoid(&OnScreenInfoRenderer::GetResourceIconOverride, 0), reinterpret_cast<void*>(0x541B48));
}

void __stdcall GGUI::C3DOnScreenInformationCustomWidget::RenderHooked(shok::Position* screenPos, GGL::IGLGUIInterface::UIData* data, bool* active)
{
    bool skip = false;
    if (ShowAllInformationFlag && data->Settler)
        *active = true;
    if (data->Entity) {
        for (auto* b : data->Entity->DisplayBehaviors) {
            if (auto* rb = dynamic_cast<CppLogic::Mod::OnScreenInfoDisplayBehavior*>(b)) {
                skip |= rb->RenderUI(&Renderer, screenPos, data, active);
            }
        }
    }
    if (!skip) {
        Renderer.RenderInactive(screenPos, data);
        if (*active)
            Renderer.RenderActive(screenPos, data);
    }
}
void __declspec(naked) OSIRenderer_renderhookedasm() {
    __asm {
        lea eax, [ebp - 0xD];
        push eax;
        lea eax, [ebp - 0x10C];
        push eax;
        lea eax, [ebp - 0x1C];
        push eax;
        lea eax, [ebx - 4];
        push eax;
        call GGUI::C3DOnScreenInformationCustomWidget::RenderHooked;

        push 0x53680F;
        ret;
    };
}
bool HookOnScreenInfoDisplayBehavior_Hooked = false;
void GGUI::C3DOnScreenInformationCustomWidget::HookOnScreenInfoDisplayBehavior()
{
    if (HookOnScreenInfoDisplayBehavior_Hooked)
        return;
    HookOnScreenInfoDisplayBehavior_Hooked = true;
    CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x5367D0), 10 };
    CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5367D0), &OSIRenderer_renderhookedasm, reinterpret_cast<void*>(0x5367D7));
}

void __thiscall GGUI::OnScreenInfoRenderer::RenderFloatiesHooked()
{
    RenderFloaties();
    GGUI::AdvancedFloatieManager::GlobalObj.RenderFloaties();
}

bool HookAdditionalFloaties_Hooked = false;
void GGUI::C3DOnScreenInformationCustomWidget::HookAdditionalFloaties()
{
    if (HookAdditionalFloaties_Hooked)
        return;
    HookAdditionalFloaties_Hooked = true;
    CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x536842), 0x10 };
    CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x536842), CppLogic::Hooks::MemberFuncPointerToVoid(&OnScreenInfoRenderer::RenderFloatiesHooked, 0));
}

GGUI::AdvancedFloatieManager GGUI::AdvancedFloatieManager::GlobalObj{};
void GGUI::AdvancedFloatieManager::RenderFloaties()
{
    float ct = shok::GetCurrentTimeFloat();
    auto f = Floaties.begin();
    while (f != Floaties.end()) {
        float t = ct - f->StartTime;
        if (t > 3.0) {
            f = Floaties.erase(f);
            continue;
        }
        float x, y;
        if ((*ERwTools::CRwCameraHandler::GlobalObj)->GetScreenCoord(f->Pos.X, f->Pos.Y, f->Height, &x, &y)) {
            y -= t * 40;
            EGUIX::Color c{ 255, 255, 0, t > 1.5f ? static_cast<int>(255.0f - (t - 1.5f) * 170.0f) : 255};
            shok::UIRenderer::GlobalObj()->RenderText(f->Text.c_str(), (*GGUI::C3DOnScreenInformationCustomWidget::GlobalObj)->Renderer.TextureData.FontID_OnscreenNumbersSmall
                , x, y, 0, &c, 0);
        }
        ++f;
    }
}
void GGUI::AdvancedFloatieManager::AddFloatie(const shok::Position& pos, float height, const char* txt)
{
    Floaties.emplace_back(pos, height, shok::GetCurrentTimeFloat(), txt);
}
void GGUI::AdvancedFloatieManager::AddFloatie(const shok::Position& pos, const char* txt)
{
    float h = (*ED::CGlobalsLogicEx::GlobalObj)->Landscape->GetTerrainHeightAtPos(pos);
    AddFloatie(pos, h, txt);
}

static inline void(__stdcall* const loadfont)(int* out, const char* name) = reinterpret_cast<void(__stdcall*)(int*, const char*)>(0x55D99E);
void EGUIX::FontManager::LoadFont(int* outFontID, const char* fontName)
{
    loadfont(outFontID, fontName);
}
static inline RWE::P2D::Rt2dFont* (__thiscall* const fontmng_getfont)(EGUIX::FontManager* th, int id) = reinterpret_cast<RWE::P2D::Rt2dFont * (__thiscall*)(EGUIX::FontManager*, int)>(0x5597C7);
RWE::P2D::Rt2dFont* EGUIX::FontManager::GetFontObj(int id)
{
    return fontmng_getfont(this, id);
}

bool (*EGUIX::UIInput_Char_Callback)(int c) = nullptr;
bool (*EGUIX::UIInput_Key_Callback)(int c, win_mouseEvents ev) = nullptr;
bool (*EGUIX::UIInput_Mouse_Callback)(win_mouseEvents id, int w, int l) = nullptr;
bool (*EGUIX::UIInput_Mouse_CallbackMainMenu)(win_mouseEvents id, int w, int l) = nullptr;
int __stdcall uiinput_firecbs(win_mouseEvents ev, int w, int l) {
    int r = 0;
    if (ev == win_mouseEvents::Char) {
        if (EGUIX::UIInput_Char_Callback)
            if (EGUIX::UIInput_Char_Callback(w))
                r = 1;
    }
    else if (ev == win_mouseEvents::KeyDown || ev == win_mouseEvents::KeyUp || ev == win_mouseEvents::SysKeyDown || ev == win_mouseEvents::SysKeyUp) {
        if (EGUIX::UIInput_Key_Callback)
            if (EGUIX::UIInput_Key_Callback(w, ev))
                r = 1;
    }
    else if (ev > win_mouseEvents::MouseMove && ev <= win_mouseEvents::XButtonDBl) {
        if (EGUIX::UIInput_Mouse_Callback)
            if (EGUIX::UIInput_Mouse_Callback(ev, w, l))
                r = 1;
        if (EGUIX::UIInput_Mouse_CallbackMainMenu)
            if (EGUIX::UIInput_Mouse_CallbackMainMenu(ev, w, l))
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
void EGUIX::HookUIInput()
{
    if (HookUIInput_Hooked)
        return;
    HookUIInput_Hooked = true;
    CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x40744B), 10 };
    CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x40744B), &uiinput_asm, reinterpret_cast<void*>(0x407456));
}

void(__thiscall* const shok_feedbackEventHandler_fireevent)(EGUIX::FeedbackEventHandler* th, BB::CEvent* e) = reinterpret_cast<void(__thiscall*)(EGUIX::FeedbackEventHandler*, BB::CEvent*)>(0x582EB2);
void EGUIX::FeedbackEventHandler::FireEvent(BB::CEvent* ev)
{
    shok_feedbackEventHandler_fireevent(this, ev);
}

void(__thiscall* const widloader_load)(EGUIX::WidgetLoader* th, const char* f, bool fa) = reinterpret_cast<void(__thiscall*)(EGUIX::WidgetLoader*, const char*, bool)>(0x556464);
void EGUIX::WidgetLoader::LoadGUI(const char* file)
{
    widloader_load(this, file, false);
}
