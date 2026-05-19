#include "s5_ui.h"
#include <shok/globals/s5_mapdisplay.h>
#include <shok/ui/s5_guistates.h>
#include <shok/s5_defines.h>
#include <shok/ui/s5_widget.h>
#include <shok/engine/s5_RWE_2d.h>
#include <utility/entityiterator.h>
#include <utility/hooks.h>
#include <cstdlib>
#include <shok/entity/s5_tasklist.h>
#include <shok/globals/s5_classfactory.h>
#include <shok/events/s5_netevents.h>
#include <shok/globals/s5_entityiterators.h>
#include <utility/TextRenderer.h>

static inline void(__thiscall* const textset_load)(BB::CTextSet* th) = reinterpret_cast<void(__thiscall*)(BB::CTextSet*)>(0x723647);
void BB::CTextSet::Load()
{
	textset_load(this);
}
static inline void(__thiscall* const textset_merge)(BB::CTextSet* th, BB::CTextSet* o) = reinterpret_cast<void(__thiscall*)(BB::CTextSet*, BB::CTextSet*)>(0x723647);
void BB::CTextSet::Merge(CTextSet* other)
{
	textset_merge(this, other);
}

const char* (*BB::StringTableText::GetStringTableTextOverride)(const char* s) = nullptr;
const char* __stdcall BB::StringTableText::GetSTTOverride(const char* s)
{
	if (!BB::StringTableText::GetStringTableTextOverride)
		return nullptr;
	return BB::StringTableText::GetStringTableTextOverride(s);
}

void NAKED_DEF BB::StringTableText::GetSTTOverrideASM() {
	__asm {
		//int 3
		push[esp + 4];
		call BB::StringTableText::GetSTTOverride;
		test eax, eax;
		jz notfound;
		ret;

	notfound:
		mov eax, 0x894508;
		mov eax, [eax];
		push 0x556D33;
		ret;
	}
}
bool HookGetStringTableText_Hooked = false;
void BB::StringTableText::HookGetStringTableText()
{
	if (HookGetStringTableText_Hooked)
		return;
	if (CppLogic::HasSCELoader())
		throw std::logic_error("HasSCELoader");
	HookGetStringTableText_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ BB::StringTableText::GetStringTableText , 0x20 };
	CppLogic::Hooks::WriteJump(BB::StringTableText::GetStringTableText, &GetSTTOverrideASM, reinterpret_cast<void*>(0x556D33));
}

static inline void(__thiscall* const uirender_rtext)(shok::UIRenderer* r, const char* txt, shok::FontId font, bool scale, float x, float y, float xend, const EGUIX::Color* color, shok::UIRenderCustomColorContext* ccc, float ldf) =
reinterpret_cast<void(__thiscall*)(shok::UIRenderer*, const char*, shok::FontId, bool, float, float, float, const EGUIX::Color*, shok::UIRenderCustomColorContext*, float)>(0x5577E1);
void shok::UIRenderer::RenderText(const char* txt, shok::FontId fontid, bool scale, float x, float y, float xend, const EGUIX::Color* color, float linedistancefactor)
{
	uirender_rtext(this, txt, fontid, scale, x, y, xend, color, nullptr, linedistancefactor);
}
void shok::UIRenderer::SetTextRenderColor(shok::Color c) const
{
	RWE::RwRGBA co{ c.R, c.G, c.B, c.A };
	Brush->SetRGBA(&co, &co, &co, &co);
}
static inline void(__thiscall* const uirender_rmaterial)(shok::UIRenderer* r, const EGUIX::CMaterial* m, bool s, const EGUIX::Rect* p) = reinterpret_cast<void(__thiscall*)(shok::UIRenderer*, const EGUIX::CMaterial*, bool, const EGUIX::Rect*)>(0x557167);
void shok::UIRenderer::RenderMaterial(const EGUIX::CMaterial* mat, bool scale, const EGUIX::Rect* pos)
{
	uirender_rmaterial(this, mat, scale, pos);
}
inline void(__thiscall* const uirender_rline)(shok::UIRenderer* r, const EGUIX::Color* c, bool scale, float x1, float y1, float x2, float y2) = reinterpret_cast<void(__thiscall*)(shok::UIRenderer*, const EGUIX::Color*, bool, float, float, float, float)>(0x557576);
void shok::UIRenderer::RenderLine(const EGUIX::Color* c, bool scale, float x1, float y1, float x2, float y2)
{
	uirender_rline(this, c, scale, x1, y1, x2, y2);
}
inline float(__thiscall* const uirender_textwidth)(shok::UIRenderer* r, const char* t, shok::FontId f) = reinterpret_cast<float(__thiscall*)(shok::UIRenderer*, const char*, shok::FontId)>(0x556FFF);
float shok::UIRenderer::GetTextWidth(const char* text, shok::FontId font)
{
	return uirender_textwidth(this, text, font);
}
float shok::UIRenderer::GetTextHeight(shok::FontId font)
{
	return EGUIX::FontManager::GlobalObj()->GetFontObj(font)->GetHeight();
}

static inline const EGUIX::Color* (__thiscall* const uircustomcolorcont_getcol)(shok::UIRenderCustomColorContext* th, int i) = reinterpret_cast<const EGUIX::Color * (__thiscall*)(shok::UIRenderCustomColorContext*, int)>(0x5577B6);
const EGUIX::Color* shok::UIRenderCustomColorContext::GetColorByInt(int i)
{
	return uircustomcolorcont_getcol(this, i);
}

float __cdecl printstr_getlength_override(RWE::P2D::Rt2dFont* f, const char* str, float multiply) {
	int charlen;
	unsigned char mask;
	int off;
	float* ld = f->GetLengthData();
	bool addlen = true;
	if (f->Flags & 2) {
		charlen = 2;
		mask = 0xFF;
		off = 1;
	}
	else {
		charlen = 1;
		mask = 0;
		off = 0;
	}
	float len = 0;
	bool lastat = false;
	unsigned int c = static_cast<unsigned int>(static_cast<unsigned char>(str[0])) | (static_cast<unsigned int>(static_cast<unsigned char>(mask & static_cast<unsigned char>(str[off]))) << 8);
	while (c > 0) {
		bool skipthis = false;
		if (addlen && c == '@') {
			addlen = false;
			lastat = true;

			str += charlen;
			c = static_cast<unsigned int>(static_cast<unsigned char>(str[0])) | (static_cast<unsigned int>(static_cast<unsigned char>(mask & static_cast<unsigned char>(str[off]))) << 8);
			continue;
		}
		else if (!addlen && c == '@' && lastat) { // NOLINT(*-branch-clone)
			addlen = true;
		}
		else if (!addlen && c == ' ') {
			addlen = true;
		}
		else if (!addlen && c == '|') {
			addlen = true;
			skipthis = true;
		}
		if (addlen && !skipthis) {
			int d;
			if (c < 128) {
				d = f->CharData[c];
			}
			else {
				c = c - f->ExtCharOffset;
				if (c < 0 || c > f->ExtCharLength) {
					d = -1;
				}
				else {
					d = f->ExtCharData[c];
				}
			}
			if (d >= 0) {
				len += ld[8 * d] + f->Spacing;
			}
		}

		lastat = false;
		str += charlen;
		c = static_cast<unsigned int>(static_cast<unsigned char>(str[0])) | (static_cast<unsigned int>(static_cast<unsigned char>(mask & static_cast<unsigned char>(str[off]))) << 8);
	}
	return len * multiply;
}

int __fastcall printstr_override(shok::UIRenderer* r, int _, const char* txt, shok::FontId font, bool uk, float x, float y, float xend, const EGUIX::Color* color, shok::UIRenderCustomColorContext* customcolordata, float ldf) {
	if (!r->ShouldRenderText)
		return 0;
	if (!txt || *txt == 0)
		return 1;
	auto* f = EGUIX::FontManager::GlobalObj()->GetFontObj(font);
	if (!f)
		return 0;
	// no idea what these funcs do
	RWE::P2D::CTM::Push();
	RWE::P2D::CTM::SetIdentity();
	r->Brush->SetTexture(nullptr);

	const shok::Color c = color ? color->ToShokColor() : shok::Color{};
	r->SetTextRenderColor(c);
	RWE::RwV2d anchor = { x,y };
	RWE::RwV2d posTransform = { 1.0f, 1.0f };
	float end = xend;
	if (!uk) {
		anchor.x = anchor.x * shok::UIRenderer::ScaledScreenSize.X / r->RenderSizeX;
		anchor.y = anchor.y * shok::UIRenderer::ScaledScreenSize.Y / r->RenderSizeY;
		end = end * shok::UIRenderer::ScaledScreenSize.X / r->RenderSizeX;
	}

	const float fontsize = f->GetHeight(); //0.036
	//const float linedistance = fontsize * (ldf == 0.0f ? 1.0f : ldf);

	RWE::RwV2d stepx;
	RWE::RwV2d stepy;
	RWE::RwV2d stepori;
	RWE::P2D::Device::GetStep(&stepx, &stepy, &stepori);
	anchor.x = anchor.x * stepx.x * r->RenderSizeX / shok::UIRenderer::ScaledScreenSize.X;
	posTransform.x = posTransform.x * stepx.x * r->RenderSizeX / shok::UIRenderer::ScaledScreenSize.X;
	end = end * stepx.x * r->RenderSizeX / shok::UIRenderer::ScaledScreenSize.X;
	if (!r->SomeTextBool) {
		RWE::P2D::CTM::Scale(shok::UIRenderer::ScaledScreenSize.X / r->RenderSizeX, shok::UIRenderer::ScaledScreenSize.Y / r->RenderSizeY);
		anchor.y = r->RenderSizeY / shok::UIRenderer::ScaledScreenSize.Y - (fontsize * shok::UIRenderer::ScaledScreenSize.Y + anchor.y) / shok::UIRenderer::ScaledScreenSize.Y;
		posTransform.y = posTransform.y * r->RenderSizeY / shok::UIRenderer::ScaledScreenSize.Y / shok::UIRenderer::ScaledScreenSize.Y;
	}
	else {
		anchor.y = 1.0f - (fontsize * shok::UIRenderer::ScaledScreenSize.Y + anchor.y) / shok::UIRenderer::ScaledScreenSize.Y;
		posTransform.y = posTransform.y * r->RenderSizeY / shok::UIRenderer::ScaledScreenSize.Y / shok::UIRenderer::ScaledScreenSize.Y;
	}

	(*RWE::RwGlobals::GlobalObj)->dOpenDevice.SetTextureRaster(nullptr);
	// ReSharper disable once CppExpressionWithoutSideEffects
	(*RWE::RwGlobals::GlobalObj)->dOpenDevice.SetTextureFilterMode(RWE::RwTextureFilterMode::rwFILTERLINEAR);

	// i dont think this was a template when BB did this, but this is way nicer as one
	if (f->IsWchar()) {
		static wchar_t buff[5001]{}; // same size that is used in the original func
		shok::UIRenderer::MultibyteToWString(txt, buff, 5000);
		CppLogic::TextRenderer<wchar_t> rend{ r, buff, f, anchor, end, ldf, posTransform, customcolordata, c };
		rend.MainRender();
	}
	else {
		CppLogic::TextRenderer<char> rend{ r, txt, f, anchor, end, ldf, posTransform, customcolordata, c };
		rend.MainRender();
	}

	RWE::P2D::CTM::Pop();

	return 1;
}
bool HookTextPrinting_Hooked = false; // 1103
void shok::HookTextPrinting()
{
	if (HookTextPrinting_Hooked)
		return;
	HookTextPrinting_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {reinterpret_cast<void*>(0x5577E1),
		reinterpret_cast<void*>(0x708F00),
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5577E1), &printstr_override, reinterpret_cast<void*>(0x5577EB));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x708F00), &printstr_getlength_override, reinterpret_cast<void*>(0x708F0E));
}


void ERwTools::CRpClumpRenderable::SetModelData(const ED::ModelData* modeldata, float rotation)
{
	SetModelData(modeldata ? modeldata->Clump : nullptr, rotation);
}
void ERwTools::CRpClumpRenderable::SetPosition(const shok::Position& p, float z)
{
	SetPosition(p.X, p.Y, z);
}

void ERwTools::CRpClumpRenderable::SetBuildingRedColor(bool r)
{
	SetBuildingRedColorI(r ? 2 : 1);
}

static inline void(__thiscall* const c3dviewhandler_setguistate)(GGUI::C3DViewHandler* th, shok::ClassId id, const GGUI::SStateParameters* p) = reinterpret_cast<void(__thiscall*)(GGUI::C3DViewHandler*, shok::ClassId, const GGUI::SStateParameters*)>(0x52820C);
void GGUI::C3DViewHandler::SetGUIStateByIdentifier(shok::ClassId identifier, const GGUI::SStateParameters* p)
{
	c3dviewhandler_setguistate(this, identifier, p);
}
static inline void(__thiscall* const c3dviewhandler_setguistateonupdate)(GGUI::C3DViewHandler* th, shok::ClassId id) = reinterpret_cast<void(__thiscall*)(GGUI::C3DViewHandler*, shok::ClassId)>(0x5280DE);
void GGUI::C3DViewHandler::SetGUIStateByIdentfierOnNextUpdate(shok::ClassId identifier)
{
	c3dviewhandler_setguistateonupdate(this, identifier);
}

bool HookClickOnMapTrigger_Hooked = false;
void GGUI::C3DViewHandler::HookClickOnMapTrigger() {
	if (HookClickOnMapTrigger_Hooked)
		return;
	HookClickOnMapTrigger_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x10, {reinterpret_cast<void*>(0x528507)}};
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x528507), &OnClickOnMapAsm, reinterpret_cast<void*>(0x52850c));
}

bool __stdcall GGUI::C3DViewHandler::OnClickOnMap(C3DViewHandler *th, BB::CEvent *ev) {
	if (ev->IsEvent(shok::InputEventIds::MouseButtonDown)) {
		if (auto* me = BB::IdentifierCast<BB::CMouseEvent>(ev)) {
			shok::PositionRot p;
			if ((*ED::CGlobalsLogicEx::GlobalObj)->Landscape->GetTerrainPosAtScreenCoords(p, me->X, me->Y)) {
				CppLogic::Events::ClickOnMapEvent x{shok::EventIDs::CppLogicEvent_OnClickMap, p, me->KeyData};
				(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&x);
				if (x.Done)
					return true;
			}
		}
	}
	if (th->CurrentState != nullptr)
		return th->CurrentState->OnMouseEvent(ev);
	return false;
}

void NAKED_DEF GGUI::C3DViewHandler::OnClickOnMapAsm() {
	__asm {
		push [ebp+8];
		push esi;
		call GGUI::C3DViewHandler::OnClickOnMap;

		mov edi, [ebp+8];
		push 0x528517;
		ret;
	};
}

void ERwTools::CDefCameraBehaviour::HookEnableZoom(bool ena)
{
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x52212B), 10 };
	*reinterpret_cast<byte*>(0x52212B) = ena ? 0x75 : 0xEB; // jnz : jmp
}

bool GGL::CGLGUIInterface::GetNearestFreePosForBuildingPlacement(shok::EntityTypeId ety, const shok::Position& inp, shok::Position& outp)
{
	return GetNearestFreePosForBuildingPlacement(ety, inp.X, inp.Y, &outp.X, &outp.Y, -1);
}


void NAKED_DEF GGL::CGLGUIInterface::GetMaxHPOverrideASM() {
	__asm {
		mov ecx, [ebp + 0xC];
		call EGL::CGLEEntity::GetMaxHPOverrideStatic;
		push eax;
		fild dword ptr [esp];
		pop eax;
		push 0x4BDEE0;
		ret;
	}
}
void GGL::CGLGUIInterface::HookFillDataHealth()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x16, {
		reinterpret_cast<void*>(0x4BDED8)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4BDED8), &GetMaxHPOverrideASM, reinterpret_cast<void*>(0x4BDEE0));
}

static inline void(__thiscall* const guimng_setpl)(GGUI::CManager* th, shok::PlayerId p) = reinterpret_cast<void(__thiscall*)(GGUI::CManager*, shok::PlayerId)>(0x52371C);
void GGUI::CManager::SetControlledPlayer(shok::PlayerId pl)
{
	guimng_setpl(this, pl);
}

inline bool(__thiscall* const guimng_sel_isselected)(const shok::Vector<GGUI::CManager::SelectionData>* th, shok::EntityId id) = reinterpret_cast<bool(__thiscall*)(const shok::Vector<GGUI::CManager::SelectionData>*, shok::EntityId)>(0x721E7B);
bool GGUI::CManager::IsEntitySelected(shok::EntityId id) const
{
	return guimng_sel_isselected(&SelectedEntities, id);
}
inline bool(__thiscall* const guimng_selectent)(GGUI::CManager* th, shok::EntityId id) = reinterpret_cast<bool(__thiscall*)(GGUI::CManager*, shok::EntityId)>(0x525828);
bool GGUI::CManager::SelectEntity(shok::EntityId id)
{
	return guimng_selectent(this, id);
}
inline void(__thiscall* const guimng_selectionchanged)(GGUI::CManager* th, int uk) = reinterpret_cast<void(__thiscall*)(GGUI::CManager*, int)>(0x5235BC);
void GGUI::CManager::OnSelectionChanged()
{
	guimng_selectionchanged(this, 0); // TODO check what that param is, only call with 1 is from event handler of FEEDBACK_EVENT_ENTITIES_EXCHANGED via 0x5235D4
}
inline bool(__thiscall* const guimng_deselect)(GGUI::CManager* th, shok::EntityId id) = reinterpret_cast<bool(__thiscall*)(GGUI::CManager*, shok::EntityId)>(0x524A98);
bool GGUI::CManager::DeselectEntity(shok::EntityId id)
{
	return guimng_deselect(this, id);
}
inline bool(__thiscall* const guimng_clearsel)(GGUI::CManager* th) = reinterpret_cast<bool(__thiscall*)(GGUI::CManager*)>(0x523590);
bool GGUI::CManager::ClearSelection()
{
	return guimng_clearsel(this);
}
inline bool(__thiscall* const guimng_isstatevalid)(GGUI::CManager* th, GGUI::CManager::StateIdData* s, shok::EntityId entity, GGUI::CBasicState::TargetData* tdata, GGUI::CBasicState::ExecuteData* edata) = reinterpret_cast<bool(__thiscall*)(GGUI::CManager*, GGUI::CManager::StateIdData*, shok::EntityId, GGUI::CBasicState::TargetData*, GGUI::CBasicState::ExecuteData*)>(0x5237A2);
bool GGUI::CManager::IsCommandStateValid(StateIdData* s, shok::EntityId entity, GGUI::CBasicState::TargetData* tdata, GGUI::CBasicState::ExecuteData* edata)
{
	return guimng_isstatevalid(this, s, entity, tdata, edata);
}
inline GGUI::CBasicState* (__thiscall* const guimng_getcmdstate)(GGUI::CManager* th, shok::EntityId entity, GGUI::CBasicState::TargetData* tdata, GGUI::CBasicState::ExecuteData* edata) = reinterpret_cast<GGUI::CBasicState * (__thiscall*)(GGUI::CManager*, shok::EntityId, GGUI::CBasicState::TargetData*, GGUI::CBasicState::ExecuteData*)>(0x523FEC);
GGUI::CBasicState* GGUI::CManager::GetCommandStateFor(shok::EntityId entity, GGUI::CBasicState::TargetData* tdata, GGUI::CBasicState::ExecuteData* edata)
{
	return guimng_getcmdstate(this, entity, tdata, edata);
}
inline shok::EntityId(__thiscall* const guimng_lastselnonsol)(const shok::Vector<GGUI::CManager::SelectionData>* th) = reinterpret_cast<shok::EntityId(__thiscall*)(const shok::Vector<GGUI::CManager::SelectionData>*)>(0x721D9C);
shok::EntityId GGUI::CManager::GetLastSelectedNonSoldier() const
{
	return guimng_lastselnonsol(&SelectedEntities);
}

bool GGUI::CManager::CanSelectPlayer(shok::PlayerId p) const {
	auto* f = reinterpret_cast<bool(__thiscall*)(const GGUI::CManager*, shok::PlayerId)>(0x52376b);
	return f(this, p);
}

bool(*GGUI::CManager::PostEventCallback)(BB::CEvent* ev) = nullptr;
bool(*GGUI::CManager::PostEventCallback2)(BB::CEvent* ev) = nullptr;
// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
struct PostEventHook_GUIManager : public BB::IPostEvent {
	BB::IPostEvent* Base = nullptr;

	virtual void __stdcall PostEvent(BB::CEvent* ev) override {
		if (GGUI::CManager::PostEventCallback)
			if (GGUI::CManager::PostEventCallback(ev))
				return;
		if (GGUI::CManager::PostEventCallback2)
			if (GGUI::CManager::PostEventCallback2(ev))
				return;
		if (Base)
			Base->PostEvent(ev);
	}
};
PostEventHook_GUIManager PostEventHook_GUIManagerObj{};
void GGUI::CManager::HackPostEvent()
{
	if (PostGUIEvent == &PostEventHook_GUIManagerObj)
		return;
	PostEventHook_GUIManagerObj.Base = PostGUIEvent;
	PostGUIEvent = &PostEventHook_GUIManagerObj;
}

void GGUI::CManager::DisableSelectionLimit(bool disable)
{
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x525959), 4 };
	*reinterpret_cast<byte*>(0x525959) = disable ? 0xEB : 0x7E; // jmp : jle
}

bool GGUI::CManager::IsModifierPressed(shok::Keys modif)
{
	return EGUIX::CEventManager::GlobalObj()->IsModifierPressed(modif);
}

void NAKED lua_getguicolor_getcolorasm() {
	__asm {
		mov eax, 0x858f9c;
		mov eax, [eax];
		lea eax, [eax + esi*4];
		nop;
		nop;
		nop;
		nop;
		nop;
		int3;
	};
}

bool CManager_HookExtraPlayers = false;
void GGUI::CManager::HookExtraPlayers() {
	if (CManager_HookExtraPlayers)
		return;
	CManager_HookExtraPlayers = true;
	{
		CppLogic::Hooks::SaveVirtualProtect vp{0x100, {
			reinterpret_cast<void*>(0x523721),
			reinterpret_cast<void*>(0x52372f),
			reinterpret_cast<void*>(0x53a7d6),
		}};
		CppLogic::Hooks::WriteNops(reinterpret_cast<void*>(0x523726), reinterpret_cast<void*>(0x52372b));
		CppLogic::Hooks::WriteNops(reinterpret_cast<void*>(0x52372f), reinterpret_cast<void*>(0x52373f));
		CppLogic::Hooks::ReplaceOpcodes(reinterpret_cast<void*>(0x53a7d6), &lua_getguicolor_getcolorasm, reinterpret_cast<void*>(0x53a7e2));
	}
	HookPlayerSelectable();
}

bool (*GGUI::CManager::IsPlayerSelectableOverride)(shok::PlayerId p) = nullptr;

void NAKED feedback_event_asm() {
	__asm {
		push edi;
		mov edi, [esp+0xc];


		mov eax, [GGUI::CManager::FeedbackEventCb];
		test eax, eax;
		jz cont;

		push esi;
		call eax;
		test al, al;
		jz cont;

		push 0x529243;
		ret;

	cont:
		push 0x52920a;
		ret;
	};
}

bool (__stdcall*GGUI::CManager::FeedbackEventCb)(BB::CEvent* ev) = nullptr;
bool CManager_HookFeedbackEvent = false;
void GGUI::CManager::HookFeedbackEvent() {
	if (CManager_HookFeedbackEvent)
		return;
	CManager_HookFeedbackEvent = true;
	CppLogic::Hooks::SaveVirtualProtect vp{0x100, {
		reinterpret_cast<void*>(0x529205),
	}};
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x529205), &feedback_event_asm, reinterpret_cast<void*>(0x52920a));
}

bool CManager_HookPlayerSelectable = false;
void GGUI::CManager::HookPlayerSelectable()
{
	if (CManager_HookPlayerSelectable)
		return;
	CManager_HookExtraPlayers = true;
	CppLogic::Hooks::SaveVirtualProtect vp{0x100, {
		reinterpret_cast<void*>(0x52376b),
		reinterpret_cast<void*>(0x5288b3),
	}};
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x52376b), CppLogic::Hooks::MemberFuncPointerToVoid(&CManager::CanSelectPlayerExtra, 0), reinterpret_cast<void*>(0x523772));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5288b3), &InitSelectionIterAsm, reinterpret_cast<void*>(0x5288bd));
}

bool GGUI::CManager::CanSelectPlayerExtra(shok::PlayerId p) const {
	if (IsPlayerSelectableOverride != nullptr)
		return IsPlayerSelectableOverride(p);
	return p == ControlledPlayer;
}

namespace CppLogic::UI {
	class SelectableEntityIter : public EGL::IGLEEntityPredicate {
	public:
		virtual bool Check(EGL::CGLEEntity* e) override {
			return Manager->CanSelectPlayer(e->PlayerId);
		}

		GGUI::CManager* Manager;

		explicit SelectableEntityIter(GGUI::CManager* m) : Manager(m) {}
	};
}

void __stdcall GGUI::CManager::InitSelectionIter(void *p) {
	new (p) CppLogic::UI::SelectableEntityIter{
		GlobalObj(),
	};
}

void NAKED_DEF GGUI::CManager::InitSelectionIterAsm() {
	__asm {
		lea eax, [ebp - 0x24];
		push eax;
		call GGUI::CManager::InitSelectionIter;

		push 0x5288bd;
		ret;
	};
}

void GGUI::SoundFeedback::SoundDatas::ReloadData(shok::FeedbackEventShortenedId id) {
	auto i = SoundData.find(id);
	if (i == SoundData.end())
		throw std::invalid_argument("SoundData not found");
	auto* data = i->second;
	for (auto* v : data->Sound)
		delete v;
	{
		auto v = data->Sound.SaveVector();
		v.Vector.clear();
	}
	auto file = std::format(R"(Data\Config\FeedbackEventSoundData\{}.xml)", CppLogic::GetIdManager<shok::FeedbackEventShortenedId>().GetNameByID(id));
	auto seri = BB::CXmlSerializer::CreateUnique();
	seri->Deserialize(file.c_str(), data, FeedbackEventSoundData::SerializationData);
}
