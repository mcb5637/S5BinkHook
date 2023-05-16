#include "pch.h"
#include "ModUI.h"

#include "s5_classfactory.h"
#include "s5_events.h"

unsigned int CppLogic::Mod::CustomWidgetRenderTest::GetClassIdentifier() const
{
	return Identifier;
}
void* __stdcall CppLogic::Mod::CustomWidgetRenderTest::CastToIdentifier(unsigned int id)
{
	if (id == EGUIX::ICustomWidget::Identifier)
		return static_cast<EGUIX::ICustomWidget*>(this);
	return nullptr;
}

void CppLogic::Mod::CustomWidgetRenderTest::Initialize()
{
}
void CppLogic::Mod::CustomWidgetRenderTest::Destroy()
{
	delete this;
}
void CppLogic::Mod::CustomWidgetRenderTest::Render(EGUIX::CCustomWidget* widget, const EGUIX::Rect* screenCoords)
{
	auto* r = shok::UIRenderer::GlobalObj();
	EGUIX::CMaterial m{};
	m.Color.Blue = 0;
	m.Color.Red = 0;
	r->RenderMaterial(&m, true, screenCoords);
	m.Color.Green = 0;
	m.Color.Red = 255;
	if (a)
		RenderLine(screenCoords, &m.Color, 0.5f, 0.25f, 1, 0.75f);
}
bool CppLogic::Mod::CustomWidgetRenderTest::HandleEvent(EGUIX::CCustomWidget* widget, BB::CEvent* ev, BB::CEvent* evAgain)
{
	auto* me = BB::IdentifierCast<BB::CMouseEvent>(ev);
	if (me) {
		if (me->IsEvent(shok::InputEventIds::MouseWheel)) {
			a = me->Delta > 0;
			me->EventHandeled = true;
			me->KeyData = shok::Keys::MouseMButton;
		}
	}
	return false;
}


void* CppLogic::Mod::CustomWidgetRenderTest::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::CustomWidgetRenderTest::operator delete(void* p) {
	shok::Free(p);
}

void CppLogic::Mod::CustomWidgetRenderTest::Register()
{
	(*BB::CClassFactory::GlobalObj)->AddClassToFactory<CustomWidgetRenderTest>();
}