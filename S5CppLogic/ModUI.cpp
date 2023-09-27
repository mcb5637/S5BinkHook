#include "pch.h"
#include "ModUI.h"

#include <format>

#include "s5_classfactory.h"
#include "s5_events.h"
#include "s5_idmanager.h"

void CppLogic::Mod::UI::RegisterClasses()
{
	(*BB::CClassFactory::GlobalObj)->AddClassToFactory<AutoScrollCustomWidget>();
}

shok::ClassId CppLogic::Mod::UI::AutoScrollCustomWidget::GetClassIdentifier() const
{
	return Identifier;
}
void* __stdcall CppLogic::Mod::UI::AutoScrollCustomWidget::CastToIdentifier(shok::ClassId id)
{
	if (id == EGUIX::ICustomWidget::Identifier)
		return static_cast<EGUIX::ICustomWidget*>(this);
	return nullptr;
}

void CppLogic::Mod::UI::AutoScrollCustomWidget::Initialize()
{
}
void CppLogic::Mod::UI::AutoScrollCustomWidget::Destroy()
{
	delete this;
}
void CppLogic::Mod::UI::AutoScrollCustomWidget::Render(EGUIX::CCustomWidget* widget, const EGUIX::Rect* screenCoords)
{
	if (PartialWidget.Color.Alpha && ElementCount > WidgetCount) {
		if (Widgets.empty())
			return;
		EGUIX::Rect texcoords = PartialWidget.TextureCoordinates;
		float childsiz = Widgets[0]->PosAndSize.H + IntegerUserVariable0;
		auto rend = shok::UIRenderer::GlobalObj();
		EGUIX::Rect r = Widgets[0]->PosAndSize;
		r.X = screenCoords->X + WidgetContainer->PosAndSize.X + Widgets[0]->PosAndSize.X;
		r.Y = screenCoords->Y + WidgetContainer->PosAndSize.Y + IntegerUserVariable0;
		r.H = UIOffset() * childsiz - IntegerUserVariable0;
		float texoff = (Widgets[0]->PosAndSize.H - r.H) / Widgets[0]->PosAndSize.H * PartialWidget.TextureCoordinates.H;
		PartialWidget.TextureCoordinates.Y += texoff;
		PartialWidget.TextureCoordinates.H -= texoff;
		if (r.H > 1.0f)
			rend->RenderMaterial(&PartialWidget, true, &r);
		PartialWidget.TextureCoordinates = texcoords;
		r.Y += r.H + IntegerUserVariable0;
		r.H = WidgetContainer->PosAndSize.H - IntegerUserVariable0 * 3 - r.H;
		for (const auto w : Widgets) {
			if (w->IsShown) {
				r.Y += childsiz;
				r.H -= childsiz;
			}
			else {
				break;
			}
		}
		PartialWidget.TextureCoordinates.H *= r.H / Widgets[0]->PosAndSize.H;
		if (r.H > 1.0f && r.H < Widgets[0]->PosAndSize.H)
			rend->RenderMaterial(&PartialWidget, true, &r);
		PartialWidget.TextureCoordinates = texcoords;
	}
}
bool CppLogic::Mod::UI::AutoScrollCustomWidget::HandleEvent(EGUIX::CCustomWidget* widget, BB::CEvent* ev, BB::CEvent* evAgain)
{
	if (Widgets.empty())
		return false;
	if (ev->IsEvent(shok::InputEventIds::MouseLeave)) {
		Dragging = false;
	}
	else if (auto* me = BB::IdentifierCast<BB::CMouseEvent>(ev)) {
		if (me->IsEvent(shok::InputEventIds::MouseWheel)) {
			Offset += -me->Delta / 200.0f;
			Clamp();
			Update();
		}
		else if (me->IsEvent(shok::InputEventIds::MouseButtonDown) && me->IsKey(shok::Keys::MouseLButton)) {
			if (ClickedOnSlider(me->X, me->Y)) {
				UpdateBySlider(me->X, me->Y);
				Update();
				Dragging = true;
			}
		}
		else if (me->IsEvent(shok::InputEventIds::MouseMove) && Dragging) {
			UpdateBySlider(me->X, me->Y);
			Update();
		}
		else {
			Dragging = false;
		}
	}
	return false;
}


void* CppLogic::Mod::UI::AutoScrollCustomWidget::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::UI::AutoScrollCustomWidget::operator delete(void* p) {
	shok::Free(p);
}

void CppLogic::Mod::UI::AutoScrollCustomWidget::ReInit()
{
	auto* mng = EGUIX::WidgetManager::GlobalObj();
	if (Widgets.empty()) {
		auto id = mng->GetIdByName(StringUserVariable[1].c_str());
		if (id == shok::WidgetId::Invalid)
			return;
		auto* w = mng->GetWidgetByID(id);
		if (w == nullptr)
			return;
		w->UserVariable[0] = 0;
		Widgets.push_back(w);
		WidgetContainer = static_cast<EGUIX::CContainerWidget*>(mng->GetWidgetByID(w->MotherWidgetID));
		int matcount = 0;
		if (auto* m = w->GetMaterials(&matcount)) {
			PartialWidget = m[0];
		}
		else {
			PartialWidget.Color.Alpha = 0;
		}

		id = mng->GetIdByName(StringUserVariable[0].c_str());
		if (id == shok::WidgetId::Invalid) {
			Slider = mng->GetWidgetByID(id);
			if (Slider) {
				SliderTravel = mng->GetWidgetByID(Slider->MotherWidgetID);
			}
		}
	}
	auto* cwid = mng->GetWidgetByID(WidgetId);
	float childs = Widgets[0]->PosAndSize.H + IntegerUserVariable0;
	WidgetCount = static_cast<int>(WidgetContainer->PosAndSize.H / childs);
	for (int o = Widgets.size(); o < std::min(WidgetCount, ElementCount); ++o) {
		Widgets.push_back(WidgetContainer->CloneAsChild(Widgets[0], [o](const char* n, EGUIX::CBaseWidget* w) {
			return std::format("{}_{}", n, o);
			},
			Widgets.back()));
	}
	Clamp();
	Update();
}

void CppLogic::Mod::UI::AutoScrollCustomWidget::Update()
{
	float childsiz = Widgets[0]->PosAndSize.H + IntegerUserVariable0;
	float cpos = IntegerUserVariable0 + UIOffset() * childsiz;
	int curri = 0;
	for (auto* w : Widgets) {
		if (curri < std::min(WidgetCount, ElementCount) && cpos + childsiz <= WidgetContainer->PosAndSize.H) {
			w->PosAndSize.Y = cpos;
			w->UserVariable[0] = curri + static_cast<int>(std::ceil(Offset));
			w->SetVisibility(true);
		}
		else {
			w->SetVisibility(false);
		}
		++curri;
		cpos += childsiz;
	}
	if (Slider) {
		if (ElementCount > WidgetCount) {
			Slider->SetVisibility(true);
			float barlen = SliderTravel->PosAndSize.H - Slider->PosAndSize.H;
			Slider->PosAndSize.Y = Offset / (ElementCount - WidgetCount) * barlen;
		}
		else {
			Slider->SetVisibility(false);
		}
	}
}

void CppLogic::Mod::UI::AutoScrollCustomWidget::Clamp()
{
	if (Offset + WidgetCount > ElementCount)
		Offset = static_cast<float>(ElementCount - WidgetCount);
	if (Offset < 0)
		Offset = 0;
}

void CppLogic::Mod::UI::AutoScrollCustomWidget::UpdateBySlider(int x, int y)
{
	if (Slider && ElementCount > WidgetCount) {
		float barlen = SliderTravel->PosAndSize.H - Slider->PosAndSize.H;
		Offset = (y - SliderTravel->PosAndSize.Y - Slider->PosAndSize.H / 2) / barlen * (ElementCount - WidgetCount);
		Clamp();
	}
}

bool CppLogic::Mod::UI::AutoScrollCustomWidget::ClickedOnSlider(int x, int y)
{
	if (Slider && ElementCount > WidgetCount) {
		float xl = SliderTravel->PosAndSize.X;
		float xh = SliderTravel->PosAndSize.X + SliderTravel->PosAndSize.W;
		float yl = SliderTravel->PosAndSize.Y;
		float yh = SliderTravel->PosAndSize.Y + SliderTravel->PosAndSize.H;
		if (xl <= x && x <= xh && yl <= y && y <= yh) {
			return true;
		}
	}
	return false;
}

float CppLogic::Mod::UI::AutoScrollCustomWidget::UIOffset() const
{
	float m = std::fmod(Offset, 1.0f);
	if (m == 0.0f)
		return 0.0f;
	return 1.0f - m;
}
