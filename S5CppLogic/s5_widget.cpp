#include "pch.h"
#include "s5_widget.h"
#include "s5_defines.h"
#include "s5_events.h"
#include "s5_mem.h"
#include "s5_RWEngine.h"
#include "s5_idmanager.h"
#include "s5_mapdisplay.h"
#include "s5_classfactory.h"
#include "hooks.h"
#include "mod.h"
#include "EntityAddonData.h"
#include "s5_RWE_2d.h"

shok::Color EGUIX::Color::ToShokColor() const
{
    return shok::Color{ Red, Green, Blue, Alpha };
}
EGUIX::Color::Color(shok::Color c) : Red(c.R), Green(c.G), Blue(c.B), Alpha(c.A)
{
}
EGUIX::Color::Color(int r, int g, int b, int a) : Red(r), Green(g), Blue(b), Alpha(a)
{
}

static inline shok::GUITextureId(__thiscall* const texman_getid)(EGUIX::TextureManager* th, const char* n) = reinterpret_cast<shok::GUITextureId(__thiscall*)(EGUIX::TextureManager*, const char*)>(0x55664A);
shok::GUITextureId EGUIX::TextureManager::GetTextureID(const char* name)
{
    return texman_getid(this, name);
}
static inline shok::GUITextureId(__thiscall* const texman_getidnoadd)(EGUIX::TextureManager* th, const char* n) = reinterpret_cast<shok::GUITextureId(__thiscall*)(EGUIX::TextureManager*, const char*)>(0x5566A5);
shok::GUITextureId EGUIX::TextureManager::GetTextureIDNoAdd(const char* name)
{
    return texman_getidnoadd(this, name);
}
static inline RWE::RwTexture* (__thiscall* const texman_gettex)(EGUIX::TextureManager* th, shok::GUITextureId id) = reinterpret_cast<RWE::RwTexture* (__thiscall*)(EGUIX::TextureManager*, shok::GUITextureId)>(0x556B1C);
RWE::RwTexture* EGUIX::TextureManager::GetTextureByID(shok::GUITextureId id)
{
    return texman_gettex(this, id);
}
static inline RWE::RwTexture* (__thiscall* const texman_reloadtext)(EGUIX::TextureManager* th, shok::GUITextureId id) = reinterpret_cast<RWE::RwTexture * (__thiscall*)(EGUIX::TextureManager*, shok::GUITextureId)>(0x556D03);
RWE::RwTexture* EGUIX::TextureManager::ReloadTexture(shok::GUITextureId id)
{
    if (static_cast<int>(id) >= static_cast<int>(Textures.size()))
        return GetTextureByID(id);
    return texman_reloadtext(this, id); // this thing actually throws if the texture vector is too small
}
void EGUIX::TextureManager::FreeTexture(shok::GUITextureId i)
{
    int id = static_cast<int>(i);
    Textures[id]->Destroy();
    Textures[id] = nullptr;
}

shok::ClassId __stdcall EGUIX::CFontIDHandler::GetClassIdentifier() const
{
    return Identifier;
}

void EGUIX::CFontIDHandler::LoadFont(const char* name)
{
    EGUIX::FontManager::LoadFont(&FontID, name);
}

shok::ClassId __stdcall EGUIX::CSingleStringHandler::GetClassIdentifier() const
{
    return Identifier;
}

static inline const char* (__thiscall* const sinstringhand_getstr)(EGUIX::CSingleStringHandler* th) = reinterpret_cast<const char* (__thiscall*)(EGUIX::CSingleStringHandler*)>(0x55BFBC);
const char* EGUIX::CSingleStringHandler::GetString()
{
    return sinstringhand_getstr(this);
}

shok::ClassId __stdcall EGUIX::CWidgetStringHelper::GetClassIdentifier() const
{
    return Identifier;
}

shok::ClassId __stdcall EGUIX::CMaterial::GetClassIdentifier() const
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


shok::ClassId __stdcall EGUIX::CLuaFunctionHelper::GetClassIdentifier() const
{
    return Identifier;
}

static inline void(__thiscall* const funchelper_call)(EGUIX::CLuaFunctionHelper* th, shok::WidgetId widgetID) = reinterpret_cast<void(__thiscall*)(EGUIX::CLuaFunctionHelper*, shok::WidgetId)>(0x55BDDE);
void EGUIX::CLuaFunctionHelper::Call(shok::WidgetId widgetID)
{
    funchelper_call(this, widgetID);
}

shok::ClassId __stdcall EGUIX::CButtonHelper::GetClassIdentifier() const
{
    return Identifier;
}

static inline void(__thiscall* const butthelp_callaction)(EGUIX::CButtonHelper* th, shok::WidgetId wid) = reinterpret_cast<void(__thiscall*)(EGUIX::CButtonHelper*, shok::WidgetId)>(0x55A3F1);
void EGUIX::CButtonHelper::PressButton(shok::WidgetId widgetID)
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

shok::ClassId __stdcall EGUIX::CToolTipHelper::GetClassIdentifier() const
{
    return Identifier;
}

void EGUIX::CToolTipHelper::OnRegistrationChanged(shok::WidgetId id, int mode)
{
    throw 0;
}


bool* EGUIX::CBaseWidget::GetUpdateManualFlag()
{
    if (auto* w = BB::IdentifierCast<EGUIX::CStaticTextWidget>(this))
        return &w->UpdateManualFlag;
    if (auto* w = BB::IdentifierCast<EGUIX::CGfxButtonWidget>(this))
        return &w->UpdateManualFlag;
    if (auto* w = BB::IdentifierCast<EGUIX::CTextButtonWidget>(this))
        return &w->UpdateManualFlag;
    if (auto* w = BB::IdentifierCast<EGUIX::CProgressBarWidget>(this))
        return &w->UpdateManualFlag;
    return nullptr;
}
EGUIX::CLuaFunctionHelper* EGUIX::CBaseWidget::GetUpdateFunc()
{
    if (auto* w = BB::IdentifierCast<EGUIX::CStaticTextWidget>(this))
        return &w->UpdateFunction;
    if (auto* w = BB::IdentifierCast<EGUIX::CGfxButtonWidget>(this))
        return &w->UpdateFunction;
    if (auto* w = BB::IdentifierCast<EGUIX::CTextButtonWidget>(this))
        return &w->UpdateFunction;
    if (auto* w = BB::IdentifierCast<EGUIX::CProgressBarWidget>(this))
        return &w->UpdateFunction;
    return nullptr;
}
EGUIX::CMaterial* EGUIX::CBaseWidget::GetMaterial(int m)
{
    auto* im = CastToIdentifier<EGUIX::IMaterialAccess>();
    if (im == nullptr)
        return nullptr;
    return im->GetMaterial(m);
}
EGUIX::CButtonHelper* EGUIX::CBaseWidget::GetButtonHelper()
{
    if (auto* w = BB::IdentifierCast<EGUIX::CButtonWidget, EGUIX::CBaseWidget, EGUIX::CGfxButtonWidget, EGUIX::CTextButtonWidget>(this))
        return &w->ButtonHelper;
    return nullptr;
}
EGUIX::CToolTipHelper* EGUIX::CBaseWidget::GetTooltipHelper()
{
    if (auto* w = BB::IdentifierCast<EGUIX::CButtonWidget, EGUIX::CBaseWidget, EGUIX::CGfxButtonWidget, EGUIX::CTextButtonWidget>(this))
        return &w->ToolTipHelper;
    if (auto* w = BB::IdentifierCast<EGUIX::CPureTooltipWidget>(this))
        return &w->ToolTipHelper;
    return nullptr;
}
EGUIX::CWidgetStringHelper* EGUIX::CBaseWidget::GetStringHelper()
{
    if (auto* w = BB::IdentifierCast<EGUIX::CStaticTextWidget>(this))
        return &w->StringHelper;
    if (auto* w = BB::IdentifierCast<EGUIX::CTextButtonWidget>(this))
        return &w->StringHelper;
    return nullptr;
}

EGUIX::Rect EGUIX::CBaseWidget::CalcGlobalPosAndSize()
{
    // 559BC9(rect* out) does the same
    EGUIX::Rect r = PosAndSize;
    auto* root = WidgetLoader::GlobalObj()->RootWid;
    auto* m = WidgetManager::GlobalObj();
    auto* w = this;
    while (w->WidgetID != root->WidgetID) {
        w = m->GetWidgetByID(w->MotherWidgetID);
        r.X += w->PosAndSize.X;
        r.Y += w->PosAndSize.Y;
    }
    return r;
}


static inline void(__thiscall* const minimappulse_ctor)(GGUI::CMiniMapSignalPulse* th, float x, float y, int colorcode, int mode, float time1, float speed2, float time2) = reinterpret_cast<void(__thiscall*)(GGUI::CMiniMapSignalPulse*, float, float, int, int, float, float, float)>(0x53D595);
GGUI::CMiniMapSignalPulse::CMiniMapSignalPulse(float x, float y, bool pulsing, int r, int g, int b, float timeFactor, float scaleFactor)
{
    minimappulse_ctor(this, x, y, 0, pulsing ? 1 : 2, 2.0f * timeFactor, 2.5f, (pulsing ? 0.8f : 0.128f) * scaleFactor);
    R = r;
    G = g;
    B = b;
}
bool GGUI::CMiniMapSignalPulse::Update()
{
    return false;
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
bool GGUI::CMiniMapSignalDefault::Update()
{
    return false;
}

static inline void(__thiscall* const minimapattleshort_ctor)(GGUI::CMiniMapSignalBattleShort* th, float x, float y) = reinterpret_cast<void(__thiscall*)(GGUI::CMiniMapSignalBattleShort*, float, float)>(0x53D63B);
GGUI::CMiniMapSignalBattleShort::CMiniMapSignalBattleShort(float x, float y)
{
    minimapattleshort_ctor(this, x, y);
}
bool GGUI::CMiniMapSignalBattleShort::Update()
{
    return false;
}

static inline void(__thiscall* const minimapattledet_ctor)(GGUI::CMiniMapSignalBattleDetail* th, float x, float y, int c) = reinterpret_cast<void(__thiscall*)(GGUI::CMiniMapSignalBattleDetail*, float, float, int)>(0x53D5E0);
GGUI::CMiniMapSignalBattleDetail::CMiniMapSignalBattleDetail(float x, float y, int colorCode)
{
    minimapattledet_ctor(this, x, y, colorCode);
}
bool GGUI::CMiniMapSignalBattleDetail::Update()
{
    return false;
}

static inline void(__thiscall* const minimapattlerou_ctor)(GGUI::CMiniMapSignalBattleRough* th, float x, float y, float c) = reinterpret_cast<void(__thiscall*)(GGUI::CMiniMapSignalBattleRough*, float, float, float)>(0x53D55C);
GGUI::CMiniMapSignalBattleRough::CMiniMapSignalBattleRough(float x, float y, float u)
{
    minimapattlerou_ctor(this, x, y, u);
}
bool GGUI::CMiniMapSignalBattleRough::Update()
{
    return false;
}

static inline void(__thiscall* const minimapmarkerhandler_addpulse)(shok::Vector<GGUI::CMiniMapSignalPulse>* th, GGUI::CMiniMapSignalPulse* p) = reinterpret_cast<void(__thiscall*)(shok::Vector<GGUI::CMiniMapSignalPulse>*, GGUI::CMiniMapSignalPulse*)>(0x52AFA5);
void GGUI::MiniMapHandler::CreateMarker(const shok::Position& p, bool pulsing, int r, int g, int b, float timeFactor, float scaleFactor)
{
    static constexpr float scale = 1.0f / 100.0f;
    EGL::CGLETerrainHiRes* hire = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->HiRes;
    float x = p.X * scale / hire->MaxSizeX;
    float y = p.Y * scale / hire->MaxSizeY;
    GGUI::CMiniMapSignalPulse pul{ x, y, pulsing, r, g, b, timeFactor, scaleFactor };
    minimapmarkerhandler_addpulse(&Markers.Pulses, &pul);
}

static inline void(__thiscall* const minimapmarkerhandler_adddefault)(shok::Vector<GGUI::CMiniMapSignalDefault>* th, GGUI::CMiniMapSignalDefault* p) = reinterpret_cast<void(__thiscall*)(shok::Vector<GGUI::CMiniMapSignalDefault>*, GGUI::CMiniMapSignalDefault*)>(0x52AEA5);
void GGUI::MiniMapHandler::CreateSignalDefault(const shok::Position& p, int r, int g, int b, float scaleFactor)
{
    static constexpr float scale = 1.0f / 100.0f;
    EGL::CGLETerrainHiRes* hire = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->HiRes;
    float x = p.X * scale / hire->MaxSizeX;
    float y = p.Y * scale / hire->MaxSizeY;
    GGUI::CMiniMapSignalDefault def{ x, y, r, g, b, scaleFactor };
    minimapmarkerhandler_adddefault(&Markers.Defaults, &def);
}

static inline void(__thiscall* const minimaptextu_settorend)(GGUI::MiniMapHandler::MiniMapTexture*) = reinterpret_cast<void(__thiscall*)(GGUI::MiniMapHandler::MiniMapTexture*)>(0x53D6F3);
void GGUI::MiniMapHandler::MiniMapTexture::SetToRender()
{
    minimaptextu_settorend(this);
}

inline void(__thiscall* const scrollbar_update)(EGUIX::CScrollBarButtonCustomWidget* th, int val, bool cb, EGUIX::CBaseWidget* wid) = reinterpret_cast<void(__thiscall*)(EGUIX::CScrollBarButtonCustomWidget*, int, bool, EGUIX::CBaseWidget*)>(0x55CB11);
void EGUIX::CScrollBarButtonCustomWidget::UpdateSlider(int value, bool callback)
{
    scrollbar_update(this, value, callback, callback ? EGUIX::WidgetManager::GlobalObj()->GetWidgetByID(WidgetId) : nullptr);
}

inline bool(__thiscall* const videoplaybaccw_init)(EGUIX::CVideoPlaybackCustomWidget* cw, EGUIX::CBaseWidget* w) = reinterpret_cast<bool(__thiscall*)(EGUIX::CVideoPlaybackCustomWidget*, EGUIX::CBaseWidget*)>(0x55C4BF);
bool EGUIX::CVideoPlaybackCustomWidget::Init(CBaseWidget* wid)
{
    if (wid == nullptr)
        wid = EGUIX::WidgetManager::GlobalObj()->GetWidgetByID(WidgetId);
    return videoplaybaccw_init(this, wid);
}

static inline shok::WidgetId(__thiscall* const widman_getidbyname)(EGUIX::WidgetManager* th, const char* n) = reinterpret_cast<shok::WidgetId(__thiscall*)(EGUIX::WidgetManager*, const char*)>(0x5588A0);
shok::WidgetId EGUIX::WidgetManager::GetIdByName(const char* name)
{
    return widman_getidbyname(this, name);
}

static inline EGUIX::CBaseWidget* (__thiscall* const widman_getwid)(EGUIX::WidgetManager* th, shok::WidgetId id) = reinterpret_cast<EGUIX::CBaseWidget * (__thiscall*)(EGUIX::WidgetManager*, shok::WidgetId)>(0x558966);
EGUIX::CBaseWidget* EGUIX::WidgetManager::GetWidgetByID(shok::WidgetId id)
{
    return widman_getwid(this, id);
}
static inline void(__thiscall* const widman_removewid)(EGUIX::WidgetManager* th, EGUIX::CBaseWidget* w, shok::WidgetId id) = reinterpret_cast<void(__thiscall*)(EGUIX::WidgetManager*, EGUIX::CBaseWidget * w, shok::WidgetId)>(0x558915);
void EGUIX::WidgetManager::RemoveWidget(EGUIX::CBaseWidget* w)
{
    widman_removewid(this, w, w->WidgetID);
}
static inline void(__thiscall* const widman_addWidget)(EGUIX::WidgetManager* th, EGUIX::CBaseWidget* a, shok::WidgetId id) = reinterpret_cast<void(__thiscall*)(EGUIX::WidgetManager*, EGUIX::CBaseWidget*, shok::WidgetId)>(0x558AA2);
void EGUIX::WidgetManager::AddWidget(EGUIX::CBaseWidget* w, shok::WidgetId id)
{
    widman_addWidget(this, w, id);
}
static inline shok::WidgetId(__thiscall* const widman_registerName)(EGUIX::WidgetManager* th, const char* name) = reinterpret_cast<shok::WidgetId(__thiscall*)(EGUIX::WidgetManager*, const char*)>(0x55884F);
shok::WidgetId EGUIX::WidgetManager::RegisterName(const char* name)
{
    return widman_registerName(this, name);
}

static inline shok::WidgetGroupId(__thiscall* const widgroupman_getgroupid)(EGUIX::CWidgetGroupManager* th, const char* s) = reinterpret_cast<shok::WidgetGroupId(__thiscall*)(EGUIX::CWidgetGroupManager*, const char*)>(0x583214);
shok::WidgetGroupId EGUIX::CWidgetGroupManager::GetGroupId(const char* s)
{
    return widgroupman_getgroupid(this, s);
}
static inline shok::WidgetGroupId(__thiscall* const widgroupman_creategroup)(int man, const char* s, int z) = reinterpret_cast<shok::WidgetGroupId(__thiscall*)(int, const char*, int)>(0x54F656);
shok::WidgetGroupId EGUIX::CWidgetGroupManager::CreateGroup(const char* s)
{
    return widgroupman_creategroup(*reinterpret_cast<int*>(0x894EA4), s, 0);
}

void EGUIX::CContainerWidget::AddWidget(EGUIX::CBaseWidget* toAdd, const char* name, const EGUIX::CBaseWidget* before)
{
    EGUIX::WidgetManager* m = EGUIX::WidgetManager::GlobalObj();
    shok::WidgetId newId = m->RegisterName(name);
    if (newId != static_cast<shok::WidgetId>(0)) {
        for (auto* w : WidgetListHandler.SubWidgets)
            w->ZPriority = 0;
        toAdd->ZPriority = 0;
        toAdd->WidgetID = newId;
        m->AddWidget(toAdd, newId);
        AddChild(toAdd);
        if (before) {
            auto l = WidgetListHandler.SubWidgets.SaveList();
            auto newit = std::find(l.List.begin(), l.List.end(), toAdd);
            if (newit == l.List.end()) // wtf, just added it
                return;
            auto it = std::find(l.List.begin(), l.List.end(), before);
            if (it != l.List.end()) {
                l.List.splice(it, l.List, newit);
            }
        }
    }
}

EGUIX::CBaseWidget* EGUIX::CContainerWidget::CloneAsChild(EGUIX::CBaseWidget* toClone, std::function<std::string(const char* n, EGUIX::CBaseWidget* oldWid)> nameGen, EGUIX::CBaseWidget* before)
{
    auto* mng = EGUIX::WidgetManager::GlobalObj();
    EGUIX::CBaseWidget* w;
    auto* cont = dynamic_cast<EGUIX::CContainerWidget*>(toClone);
    EGUIX::CContainerWidget* contcl = nullptr;
    std::string n = nameGen(mng->WidgetNameManager->GetNameByID(static_cast<int>(toClone->WidgetID)), toClone);
    if (cont) {
        contcl = EGUIX::CContainerWidget::Create();
        w = contcl;
        contcl->PosAndSize = cont->PosAndSize;
        contcl->IsShown = cont->IsShown;
        contcl->ZPriority = cont->ZPriority;
        contcl->Group = cont->Group;
        contcl->ForceToHandleMouseEventsFlag = cont->ForceToHandleMouseEventsFlag;
        contcl->ForceToNeverBeFoundFlag = cont->ForceToNeverBeFoundFlag;
    }
    else {
        w = toClone->Clone();
    }
    if (auto* stxtw = BB::IdentifierCast<EGUIX::CStaticTextWidget>(toClone)) {
        auto* cotxt = static_cast<EGUIX::CStaticTextWidget*>(w);
        cotxt->FirstLineToPrint = stxtw->FirstLineToPrint;
        cotxt->NumberOfLinesToPrint = stxtw->NumberOfLinesToPrint;
    }
    this->AddWidget(w, n.c_str(), before);
    if (cont) {
        for (auto* chi : cont->WidgetListHandler.SubWidgets) {
            contcl->CloneAsChild(chi, nameGen, nullptr);
        }
    }
    return w;
}

void(__thiscall* const shok_EGUIX_CLuaFunctionHelper_ctor)(EGUIX::CLuaFunctionHelper* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CLuaFunctionHelper * th)>(0x55BEA2);
static inline void(__thiscall* const widlisthandler_ctor)(EGUIX::CWidgetListHandler* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CWidgetListHandler*)>(0x55BB26);

static inline void(__thiscall* const statwid_ctor)(EGUIX::CStaticWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CStaticWidget*)>(0x5603F1);
EGUIX::CStaticWidget* EGUIX::CStaticWidget::Create()
{
    EGUIX::CStaticWidget* r = (*BB::CClassFactory::GlobalObj)->CreateObject<CStaticWidget>();
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const stattextwid_ctor)(EGUIX::CStaticTextWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CStaticTextWidget*)>(0x55FBB1);
EGUIX::CStaticTextWidget* EGUIX::CStaticTextWidget::Create()
{
    EGUIX::CStaticTextWidget* r = (*BB::CClassFactory::GlobalObj)->CreateObject<CStaticTextWidget>();
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const purettwid_ctor)(EGUIX::CPureTooltipWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CPureTooltipWidget*)>(0x55DF3E);
EGUIX::CPureTooltipWidget* EGUIX::CPureTooltipWidget::Create()
{
    EGUIX::CPureTooltipWidget* r = (*BB::CClassFactory::GlobalObj)->CreateObject<CPureTooltipWidget>();
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const gfxbutwid_ctor)(EGUIX::CGfxButtonWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CGfxButtonWidget*)>(0x55E917);
EGUIX::CGfxButtonWidget* EGUIX::CGfxButtonWidget::Create()
{
    EGUIX::CGfxButtonWidget* r = (*BB::CClassFactory::GlobalObj)->CreateObject<CGfxButtonWidget>();
    r->IsShown = false;
    r->IconMaterial.Color.Alpha = 0;
    return r;
}
static inline void(__thiscall* const txtbutwid_ctor)(EGUIX::CTextButtonWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CTextButtonWidget*)>(0x55EFA3);
EGUIX::CTextButtonWidget* EGUIX::CTextButtonWidget::Create()
{
    EGUIX::CTextButtonWidget* r = (*BB::CClassFactory::GlobalObj)->CreateObject<CTextButtonWidget>();
    r->IsShown = false;
    return r;
}
BB::SerializationData textbuttonwid_seridataaddon[] = {
    BB::SerializationData::EmbeddedData(nullptr, 0, sizeof(EGUIX::CTextButtonWidget), nullptr, nullptr),
    AutoMemberSerializationName(EGUIX::CTextButtonWidget, CppLogic_CenterText, "CenterText"),
    BB::SerializationData::GuardData(),
};
void __declspec(naked) textbuttonwid_fixtextrenderasm() {
    __asm {
        lea eax, [ebp - 0x20];
        push eax;
        lea ecx, [ebx - 14 * 4];
        call EGUIX::CTextButtonWidget::CenterRenderArea;

        push 0x55F15D;
        ret;
    };
}
void __declspec(naked) textbuttonwid_ctorasm() {
    __asm {
        mov dword ptr[esi], 0x780DB0;
        mov byte ptr[esi + 190 * 4], 0;
        mov byte ptr[esi + 190 * 4 + 1], 1;

        push 0x55EFD9;
        ret;
    };
}
void __declspec(naked) textbuttonwid_cloneasm() {
    __asm {
        mov ax, [edi + 190 * 4];
        mov[esi + 190 * 4], ax;

        push 0x55F5D4;
        ret;
    };
}
void EGUIX::CTextButtonWidget::HookFixTextRender()
{
    CppLogic::Hooks::SaveVirtualProtect vp{ 0x100, {
        reinterpret_cast<void*>(0x55F14F),
        reinterpret_cast<void*>(0x55F16A),
        reinterpret_cast<void*>(0x55EFD3),
        reinterpret_cast<void*>(0x55F5C8),
    } };
    CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x55F14F), &textbuttonwid_fixtextrenderasm, reinterpret_cast<void*>(0x55F15D));
    CppLogic::Hooks::WriteNops(reinterpret_cast<void*>(0x55F16A), reinterpret_cast<void*>(0x55F17C));
    CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x55EFD3), &textbuttonwid_ctorasm, reinterpret_cast<void*>(0x55EFD9));
    CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x55F5C8), &textbuttonwid_cloneasm, reinterpret_cast<void*>(0x55F5D4));
    BB::CClassFactory* f = *BB::CClassFactory::GlobalObj;
    auto& info = f->Info.at(Identifier);
    textbuttonwid_seridataaddon[0].SubElementData = info.SData;
    info.SData = textbuttonwid_seridataaddon;
}
void EGUIX::CTextButtonWidget::CenterRenderArea(Rect* render) const
{
    if (CppLogic_CenterText) {
        float y = shok::UIRenderer::GlobalObj()->RenderSizeY / shok::UIRenderer::ScaledScreenSize.Y;
        render->Y += (y - 1.0f) * 8.0f;
    }
}

static inline void(__thiscall* const progbarwid_ctor)(EGUIX::CProgressBarWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CProgressBarWidget*)>(0x55E300);
EGUIX::CProgressBarWidget* EGUIX::CProgressBarWidget::Create()
{
    EGUIX::CProgressBarWidget* r = (*BB::CClassFactory::GlobalObj)->CreateObject<CProgressBarWidget>();
    r->IsShown = false;
    return r;
}
static inline void(__thiscall* const contwid_ctor)(EGUIX::CContainerWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CContainerWidget*)>(0x560DAD);
EGUIX::CContainerWidget* EGUIX::CContainerWidget::Create()
{
    EGUIX::CContainerWidget* r = (*BB::CClassFactory::GlobalObj)->CreateObject<CContainerWidget>();
    r->IsShown = false;
    return r;
}

int EGUIX::ICustomWidget::uk3()
{
    return 0;
}

inline void(__thiscall* const icustomwid_renderline)(EGUIX::ICustomWidget* th, const EGUIX::Rect* screenCoords, const EGUIX::Color* c, float x1, float y1, float x2, float y2) = reinterpret_cast<void(__thiscall*)(EGUIX::ICustomWidget*, const EGUIX::Rect*, const EGUIX::Color*, float, float, float, float)>(0x530726);
void EGUIX::ICustomWidget::RenderLine(const Rect* screenCoords, const EGUIX::Color* c, float x1, float y1, float x2, float y2)
{
    icustomwid_renderline(this, screenCoords, c, x1, y1, x2, y2);
}

static inline void(__thiscall* const customwid_initwid)(EGUIX::CCustomWidget* th) = reinterpret_cast<void(__thiscall*)(EGUIX::CCustomWidget*)>(0x560BA3);
void EGUIX::CCustomWidget::InitializeCustomWidget()
{
    customwid_initwid(this);
}

inline const char* (__thiscall* const stringinputcw_buffer_get)(EGUIX::CStringInputCustomWidget::Buffer* th, bool pwm) = reinterpret_cast<const char* (__thiscall*)(EGUIX::CStringInputCustomWidget::Buffer*, bool)>(0x55D283);
const char* EGUIX::CStringInputCustomWidget::GetBuffer(bool passwordMask)
{
    return stringinputcw_buffer_get(&Buff, passwordMask);
}
inline void(__thiscall* const stringinputcw_buffer_reserve)(EGUIX::CStringInputCustomWidget::Buffer* th, size_t s) = reinterpret_cast<void(__thiscall*)(EGUIX::CStringInputCustomWidget::Buffer*, size_t)>(0x55D35D);
void EGUIX::CStringInputCustomWidget::Reserve(size_t s)
{
    stringinputcw_buffer_reserve(&Buff, s);
}
inline void(__thiscall* const stringinputcw_conf)(EGUIX::CStringInputCustomWidget* th, const char* buff, EGUIX::CCustomWidget* cw) = reinterpret_cast<void(__thiscall*)(EGUIX::CStringInputCustomWidget*, const char*, EGUIX::CCustomWidget*)>(0x55D599);
void EGUIX::CStringInputCustomWidget::CallOnConfirm(CCustomWidget* wid, const char* content)
{
    if (content == nullptr)
        content = GetBuffer();
    stringinputcw_conf(this, content, wid);
}
inline void(__thiscall* const stringinputcw_assing)(EGUIX::CStringInputCustomWidget* th, const char* buff) = reinterpret_cast<void(__thiscall*)(EGUIX::CStringInputCustomWidget*, const char*)>(0x55D1CE);
void EGUIX::CStringInputCustomWidget::Assign(const char* buff)
{
    stringinputcw_assing(this, buff);
}
inline void(__thiscall* const stringinputcw_close)(EGUIX::CStringInputCustomWidget* th, EGUIX::CCustomWidget* cw) = reinterpret_cast<void(__thiscall*)(EGUIX::CStringInputCustomWidget*, EGUIX::CCustomWidget*)>(0x55D2F5);
void EGUIX::CStringInputCustomWidget::Close(CCustomWidget* wid)
{
    stringinputcw_close(this, wid);
}
inline bool(__thiscall* const stringinputcw_input)(EGUIX::CStringInputCustomWidget* th, EGUIX::CCustomWidget* cw, shok::Keys keyCode, char keyChar) = reinterpret_cast<bool(__thiscall*)(EGUIX::CStringInputCustomWidget*, EGUIX::CCustomWidget*, shok::Keys, char)>(0x55D60D);
bool EGUIX::CStringInputCustomWidget::HandleInput(CCustomWidget* wid, shok::Keys keyCode, char keyChar)
{
    return stringinputcw_input(this, wid, keyCode, keyChar);
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
        xor ebx, ebx;
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

static inline void(__thiscall* const showresfloatie)(GGUI::C3DOnScreenInformationCustomWidget* th, shok::EntityId eid, int am) = reinterpret_cast<void(__thiscall*)(GGUI::C3DOnScreenInformationCustomWidget*, shok::EntityId, int)>(0x536361);
void GGUI::C3DOnScreenInformationCustomWidget::ShowResourceFloatieOnEntity(shok::EntityId eid, int amount)
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
            if (auto* rb = b->CastToIdentifier<CppLogic::Mod::OnScreenInfoDisplayBehavior>()) {
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
                , false, x, y, 0, &c, 0);
        }
        ++f;
    }
}
void GGUI::AdvancedFloatieManager::AddFloatie(const shok::Position& pos, float height, std::string_view txt)
{
    AddFloatie(pos, height, std::string(txt));
}
void GGUI::AdvancedFloatieManager::AddFloatie(const shok::Position& pos, std::string_view txt)
{
    float h = (*ED::CGlobalsLogicEx::GlobalObj)->Landscape->GetTerrainHeightAtPos(pos);
    AddFloatie(pos, h, txt);
}
void GGUI::AdvancedFloatieManager::AddFloatie(const shok::Position& pos, float height, std::string&& txt)
{
    Floaties.emplace_back(pos, height, shok::GetCurrentTimeFloat(), std::move(txt));
}
void GGUI::AdvancedFloatieManager::AddFloatie(const shok::Position& pos, std::string&& txt)
{
    float h = (*ED::CGlobalsLogicEx::GlobalObj)->Landscape->GetTerrainHeightAtPos(pos);
    AddFloatie(pos, h, std::move(txt));
}

inline void(__thiscall* const shortmsg_entr_add)(GGUI::CShortMessagesWindowControllerCustomWidget::Entries* th, GGUI::CShortMessagesWindowControllerCustomWidget::MessageType type, BB::CEvent* ev, float duration, const shok::Position* pos, const char* tooltip)
= reinterpret_cast<void(__thiscall*)(GGUI::CShortMessagesWindowControllerCustomWidget::Entries*, GGUI::CShortMessagesWindowControllerCustomWidget::MessageType, BB::CEvent*, float, const shok::Position*, const char*)>(0x532054);
void GGUI::CShortMessagesWindowControllerCustomWidget::Entries::Add(MessageType type, BB::CEvent* ev, float duration, const shok::Position* pos, const char* tooltip)
{
    shortmsg_entr_add(this, type, ev, duration, pos, tooltip);
}
inline void(__thiscall* const shortmsg_entr_clearold)(GGUI::CShortMessagesWindowControllerCustomWidget::Entries* th) = reinterpret_cast<void(__thiscall* const)(GGUI::CShortMessagesWindowControllerCustomWidget::Entries*)>(0x531B72);
void GGUI::CShortMessagesWindowControllerCustomWidget::Entries::ClearOldMessages()
{
    shortmsg_entr_clearold(this);
}

inline GGUI::CShortMessagesWindowControllerCustomWidget::Message* (__thiscall* const shortmsg_entr_get)(GGUI::CShortMessagesWindowControllerCustomWidget::Entries* th, int id) = reinterpret_cast<GGUI::CShortMessagesWindowControllerCustomWidget::Message * (__thiscall* const)(GGUI::CShortMessagesWindowControllerCustomWidget::Entries*, int)>(0x53162B);
GGUI::CShortMessagesWindowControllerCustomWidget::Message* GGUI::CShortMessagesWindowControllerCustomWidget::Entries::GetMessageWithId(int id)
{
    return shortmsg_entr_get(this, id);
}
inline void (__thiscall* const shortmsg_entr_rem)(GGUI::CShortMessagesWindowControllerCustomWidget::Entries* th, int id) = reinterpret_cast<void (__thiscall* const)(GGUI::CShortMessagesWindowControllerCustomWidget::Entries*, int)>(0x531B36);
void GGUI::CShortMessagesWindowControllerCustomWidget::Entries::RemoveMessage(int id)
{
    shortmsg_entr_rem(this, id);
}

inline void(__thiscall* const statrender_stat_toggletime)(GGUI::CStatisticsRendererCustomWidget::StatToRender* th) = reinterpret_cast<void(__thiscall*)(GGUI::CStatisticsRendererCustomWidget::StatToRender*)>(0x53069A);
void GGUI::CStatisticsRendererCustomWidget::StatToRender::ToggleTimeScale()
{
    statrender_stat_toggletime(this);
}
inline int(__thiscall* const statrender_stat_getmin)(GGUI::CStatisticsRendererCustomWidget::StatToRender* th, shok::PlayerId pl, GGUI::CStatisticsRendererCustomWidget* cw) = reinterpret_cast<int(__thiscall*)(GGUI::CStatisticsRendererCustomWidget::StatToRender*, shok::PlayerId, GGUI::CStatisticsRendererCustomWidget*)>(0x530B2C);
int GGUI::CStatisticsRendererCustomWidget::StatToRender::GetMinutes(shok::PlayerId pl, CStatisticsRendererCustomWidget* cw)
{
    return statrender_stat_getmin(this, pl, cw);
}

inline shok::PlayerId(__thiscall* const statrender_getplayer)(const GGUI::CStatisticsRendererCustomWidget* th) = reinterpret_cast<shok::PlayerId(__thiscall*)(const GGUI::CStatisticsRendererCustomWidget*)>(0x5309F8);
shok::PlayerId GGUI::CStatisticsRendererCustomWidget::GetPlayer() const
{
    return statrender_getplayer(this);
}
inline bool(__thiscall* const statrender_ishuman)(const GGUI::CStatisticsRendererCustomWidget* th, shok::PlayerId pl) = reinterpret_cast<bool(__thiscall*)(const GGUI::CStatisticsRendererCustomWidget*, shok::PlayerId)>(0x530840);
bool GGUI::CStatisticsRendererCustomWidget::IsHumanPlayer(shok::PlayerId pl) const
{
    return statrender_ishuman(this, pl);
}

inline GGL::CGameStatistics* (__thiscall* const statrender_getstat)(GGUI::CStatisticsRendererCustomWidget* th, shok::PlayerId pl) = reinterpret_cast<GGL::CGameStatistics * (__thiscall*)(GGUI::CStatisticsRendererCustomWidget*, shok::PlayerId)>(0x53080A);
GGL::CGameStatistics* GGUI::CStatisticsRendererCustomWidget::GetStatistics(shok::PlayerId player)
{
    return statrender_getstat(this, player);
}

static inline void(__stdcall* const loadfont)(shok::FontId* out, const char* name) = reinterpret_cast<void(__stdcall*)(shok::FontId*, const char*)>(0x55D99E);
void EGUIX::FontManager::LoadFont(shok::FontId* outFontID, const char* fontName)
{
    loadfont(outFontID, fontName);
}
static inline RWE::P2D::Rt2dFont* (__thiscall* const fontmng_getfont)(EGUIX::FontManager* th, shok::FontId id) = reinterpret_cast<RWE::P2D::Rt2dFont * (__thiscall*)(EGUIX::FontManager*, shok::FontId)>(0x5597C7);
RWE::P2D::Rt2dFont* EGUIX::FontManager::GetFontObj(shok::FontId id)
{
    return fontmng_getfont(this, id);
}

void EGUIX::FontManager::ClearFont(shok::FontId id)
{
    if (static_cast<int>(id) >= static_cast<int>(Fonts.size()))
        return;
    auto* f = Fonts.at(static_cast<int>(id));
    f->Destroy();
    Fonts[static_cast<int>(id)] = nullptr;
}

void EGUIX::FontManager::PopFont(shok::FontId id)
{
    ClearFont(id);
    Manager->RemoveID(static_cast<int>(id));
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
        push 0x407451;
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
    CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x40744B), &uiinput_asm, reinterpret_cast<void*>(0x407451));
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

static inline bool(__thiscall* const eventmng_ispressed)(shok::Keys* ob7, shok::Keys m) = reinterpret_cast<bool(__thiscall*)(shok::Keys*, shok::Keys)>(0x558C1C);
bool EGUIX::CEventManager::IsModifierPressed(shok::Keys m)
{
    return eventmng_ispressed(&CurrentModifiers, m);
}
