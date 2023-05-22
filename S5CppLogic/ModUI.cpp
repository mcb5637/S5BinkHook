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

unsigned int CppLogic::Mod::UI::AutoScrollCustomWidget::GetClassIdentifier() const
{
	return Identifier;
}
void* __stdcall CppLogic::Mod::UI::AutoScrollCustomWidget::CastToIdentifier(unsigned int id)
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
			Clamp(widget);
			Update(widget);
		}
		else if (me->IsEvent(shok::InputEventIds::MouseButtonDown) && me->IsKey(shok::Keys::MouseLButton)) {
			if (ClickedOnSlider(widget, me->X, me->Y)) {
				UpdateBySlider(widget, me->X, me->Y);
				Update(widget);
				Dragging = true;
			}
		}
		else if (me->IsEvent(shok::InputEventIds::MouseMove) && Dragging) {
			UpdateBySlider(widget, me->X, me->Y);
			Update(widget);
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
		int id = mng->GetIdByName(StringUserVariable[1].c_str());
		if (id == 0)
			return;
		auto* w = mng->GetWidgetByID(id);
		if (w == nullptr)
			return;
		w->UserVariable[0] = 0;
		Widgets.push_back(w);
		id = mng->GetIdByName(StringUserVariable[0].c_str());
		if (id) {
			Slider = mng->GetWidgetByID(id);
			if (Slider) {
				SliderTravel = mng->GetWidgetByID(Slider->MotherWidgetID);
			}
		}
	}
	auto* cwid = mng->GetWidgetByID(WidgetId);
	float childs = Widgets[0]->PosAndSize.H + IntegerUserVariable0;
	WidgetCount = static_cast<int>(cwid->PosAndSize.H / childs);
	auto* mother = static_cast<EGUIX::CContainerWidget*>(mng->GetWidgetByID(cwid->MotherWidgetID));
	for (int o = Widgets.size(); o < std::min(WidgetCount, ElementCount); ++o) {
		Widgets.push_back(CloneWidget(Widgets[0], mother, o, Widgets.back()));
	}
	Clamp(cwid);
	Update(cwid);
}

void CppLogic::Mod::UI::AutoScrollCustomWidget::Update(EGUIX::CBaseWidget* cw)
{
	float childsiz = Widgets[0]->PosAndSize.H + IntegerUserVariable0;
	float cpos = IntegerUserVariable0 + std::fmod(Offset, 1.0f) * childsiz;
	int curri = 0;
	for (auto* w : Widgets) {
		if (curri < std::min(WidgetCount, ElementCount) && cpos + childsiz <= cw->PosAndSize.H) {
			w->PosAndSize.Y = cpos;
			w->UserVariable[0] = curri + static_cast<int>(Offset);
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

void CppLogic::Mod::UI::AutoScrollCustomWidget::Clamp(EGUIX::CBaseWidget* cw)
{
	if (Offset + WidgetCount > ElementCount)
		Offset = static_cast<float>(ElementCount - WidgetCount);
	if (Offset < 0)
		Offset = 0;
}

void CppLogic::Mod::UI::AutoScrollCustomWidget::UpdateBySlider(EGUIX::CBaseWidget* cw, int x, int y)
{
	if (Slider && ElementCount > WidgetCount) {
		float barlen = SliderTravel->PosAndSize.H - Slider->PosAndSize.H;
		Offset = (y - SliderTravel->PosAndSize.Y - Slider->PosAndSize.H / 2) / barlen * (ElementCount - WidgetCount);
		Clamp(cw);
	}
}

bool CppLogic::Mod::UI::AutoScrollCustomWidget::ClickedOnSlider(EGUIX::CBaseWidget* cw, int x, int y)
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

EGUIX::CBaseWidget* CppLogic::Mod::UI::AutoScrollCustomWidget::CloneWidget(EGUIX::CBaseWidget* cw, EGUIX::CContainerWidget* mother, int o, EGUIX::CBaseWidget* prev)
{
	auto* mng = EGUIX::WidgetManager::GlobalObj();
	EGUIX::CBaseWidget* w;
	auto* cont = dynamic_cast<EGUIX::CContainerWidget*>(cw);
	EGUIX::CContainerWidget* contcl = nullptr;
	std::string n = std::format("{}_{}", mng->WidgetNameManager->GetNameByID(cw->WidgetID), o);
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
		w = cw->Clone();
	}
	if (auto* stxtw = BB::IdentifierCast<EGUIX::CStaticTextWidget>(cw)) {
		auto* cotxt = static_cast<EGUIX::CStaticTextWidget*>(w);
		cotxt->FirstLineToPrint = stxtw->FirstLineToPrint;
		cotxt->NumberOfLinesToPrint = stxtw->NumberOfLinesToPrint;
	}
	mother->AddWidget(w, n.c_str(), prev);
	if (cont) {
		for (auto* chi : cont->WidgetListHandler.SubWidgets) {
			CloneWidget(chi, contcl, o, nullptr);
		}
	}
	return w;
}
