#include "pch.h"
#include "ModUI.h"

#include <format>
#include <locale>

#include "s5_classfactory.h"
#include "s5_events.h"
#include "s5_idmanager.h"
#include "s5_scriptsystem.h"

void CppLogic::Mod::UI::RegisterClasses()
{
	(*BB::CClassFactory::GlobalObj)->AddClassToFactory<AutoScrollCustomWidget>();
	(*BB::CClassFactory::GlobalObj)->AddClassToFactory<TextInputCustomWidget>();
	(*BB::CClassFactory::GlobalObj)->AddClassToFactory<FreeCamCustomWidget>();
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
		float childsiz = Widgets[0]->PosAndSize.H + ScollableSpacing();
		auto rend = shok::UIRenderer::GlobalObj();
		EGUIX::Rect containerpos = WidgetContainer->CalcGlobalPosAndSize();
		EGUIX::Rect r = Widgets[0]->PosAndSize;
		r.X = containerpos.X + Widgets[0]->PosAndSize.X;
		r.Y = containerpos.Y + ScollableSpacing();
		r.H = UIOffset() * childsiz - ScollableSpacing();
		float texoff = (Widgets[0]->PosAndSize.H - r.H) / Widgets[0]->PosAndSize.H * PartialWidget.TextureCoordinates.H;
		PartialWidget.TextureCoordinates.Y += texoff;
		PartialWidget.TextureCoordinates.H -= texoff;
		if (r.H > 1.0f)
			rend->RenderMaterial(&PartialWidget, true, &r);
		PartialWidget.TextureCoordinates = texcoords;
		r.Y += r.H + ScollableSpacing();
		r.H = containerpos.H - ScollableSpacing() * 3 - r.H;
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
		auto id = mng->GetIdByName(ScollableName().c_str());
		if (id == shok::WidgetId::Invalid)
			return;
		auto* w = mng->GetWidgetByID(id);
		if (w == nullptr)
			return;
		w->UserVariable[0] = 0;
		Widgets.push_back(w);
		WidgetContainer = static_cast<EGUIX::CContainerWidget*>(mng->GetWidgetByID(w->MotherWidgetID));
		if (auto* m = w->GetMaterial(0)) {
			PartialWidget = *m;
		}
		else {
			PartialWidget.Color.Alpha = 0;
		}

		id = mng->GetIdByName(SliderName().c_str());
		if (id != shok::WidgetId::Invalid) {
			Slider = mng->GetWidgetByID(id);
			if (Slider) {
				SliderTravel = mng->GetWidgetByID(Slider->MotherWidgetID);
			}
		}
	}
	auto* cwid = mng->GetWidgetByID(WidgetId);
	float childs = Widgets[0]->PosAndSize.H + ScollableSpacing();
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
	float childsiz = Widgets[0]->PosAndSize.H + ScollableSpacing();
	float cpos = ScollableSpacing() + UIOffset() * childsiz;
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

bool CppLogic::Mod::UI::InputFocusWidget::CheckFocusEvent(BB::CEvent* ev)
{
	if (ev->IsEvent(shok::InputEventIds::ClearFocus)) {
		Active = false;
		return true;
	}
	return false;
}
void CppLogic::Mod::UI::InputFocusWidget::GetFocus()
{
	BB::CEvent ev{ shok::InputEventIds::ClearFocus };
	EGUIX::WidgetLoader::GlobalObj()->RootWid->HandleEvent(&ev, &ev);
	Active = true;
}
void CppLogic::Mod::UI::InputFocusWidget::ClearFocus()
{
	Active = false;
}
bool CppLogic::Mod::UI::InputFocusWidget::HasFocus()
{
	return Active;
}


shok::ClassId __stdcall CppLogic::Mod::UI::TextInputCustomWidget::GetClassIdentifier() const
{
	return Identifier;
}

void* __stdcall CppLogic::Mod::UI::TextInputCustomWidget::CastToIdentifier(shok::ClassId id)
{
	if (id == EGUIX::ICustomWidget::Identifier)
		return static_cast<EGUIX::ICustomWidget*>(this);
	return nullptr;
}

void CppLogic::Mod::UI::TextInputCustomWidget::Initialize()
{
	const char* f = "Data\\Menu\\Fonts\\standard10.met";
	if (FontName().size() > 0)
		f = FontName().c_str();
	Font.LoadFont(f);
}

void CppLogic::Mod::UI::TextInputCustomWidget::Destroy()
{
	delete this;
}

void CppLogic::Mod::UI::TextInputCustomWidget::Render(EGUIX::CCustomWidget* widget, const EGUIX::Rect* screenCoords)
{
	auto rend = shok::UIRenderer::GlobalObj();
	EGUIX::Color col{ *reinterpret_cast<const shok::Color*>(&BackgroundColor()) };
	if (col.Alpha > 0) {
		EGUIX::CMaterial m{};
		m.Color = col;
		rend->RenderMaterial(&m, true, screenCoords);
	}
	col = EGUIX::Color{ *reinterpret_cast<const shok::Color*>(&TextColor()) };
	if (col.Alpha == 0) {
		col = EGUIX::Color{};
	}
	rend->RenderText(CurrentTextDisplay.c_str(), Font.FontID, screenCoords->X, screenCoords->Y, screenCoords->X + screenCoords->W, &col, 1);
	if (!HasFocus())
		return;
	if (!(static_cast<int>(shok::GetCurrentTimeFloat() * 3.0f) & 1)) {
		col = EGUIX::Color{ *reinterpret_cast<const shok::Color*>(&BlinkColor()) };
		if (col.Alpha == 0) {
			col.Red = 100;
			col.Green = 100;
			col.Blue = 100;
			col.Alpha = 255;
		}
	}
	char back = CurrentTextDisplay[CurrentPos];
	CurrentTextDisplay[CurrentPos] = '\0';
	float textw = rend->GetTextWidth(CurrentTextDisplay.c_str(), Font.FontID);
	CurrentTextDisplay[CurrentPos] = back;
	float x = screenCoords->X + textw * 768.0f;
	float y = screenCoords->Y;
	rend->RenderLine(&col, true, x, y, x, y + rend->GetTextHeight(Font.FontID) * 768.0f);
}

bool CppLogic::Mod::UI::TextInputCustomWidget::HandleEvent(EGUIX::CCustomWidget* widget, BB::CEvent* ev, BB::CEvent* evAgain)
{
	if (CheckFocusEvent(ev))
		return false;
	if (ev->IsEvent(shok::InputEventIds::MouseButtonUp)) {
		if (auto* me = BB::IdentifierCast<BB::CMouseEvent>(ev)) {
			if (me->IsKey(shok::Keys::MouseLButton)) {
				GetFocus();
				EGUIX::WidgetLoader::KeyStrokeLuaCallback();
				return true;
			}
		}
	}
	else if (ev->IsEvent(shok::InputEventIds::MouseButtonDown)) {
		return true;
	}
	else if (ev->IsEvent(shok::InputEventIds::MouseWheel) && ScrollDelta() != 0) {
		if (auto* me = BB::IdentifierCast<BB::CMouseEvent>(ev)) {
			std::string r{};
			bool changed = false;
			if (Mode() == Modes::Int || Mode() == Modes::UInt) {
				int i = 0;
				std::from_chars(CurrentTextDisplay.data(), CurrentTextDisplay.data() + CurrentTextDisplay.size(), i);
				i += me->Delta > 0 ? ScrollDelta() : -ScrollDelta();
				if (Mode() == Modes::UInt) {
					i = std::max(0, i);
				}
				r = std::format("{}", i);
				changed = true;
			}
			else if (Mode() == Modes::Double || Mode() == Modes::UDouble) {
				double i = 0;
				std::from_chars(CurrentTextDisplay.data(), CurrentTextDisplay.data() + CurrentTextDisplay.size(), i);
				i += me->Delta > 0 ? ScrollDelta() : -ScrollDelta();
				if (Mode() == Modes::UDouble) {
					i = std::max(0.0, i);
				}
				r = std::format("{}", i);
				changed = true;
			}
			if (changed) {
				bool adv = true;
				if (HasFlag(Event::Validate)) {
					if (!CallFunc(EventFunc(), Event::Validate)) {
						return true;
					}
				}
				CurrentTextRaw = r;
				CurrentPos = std::min(CurrentPos, CurrentTextRaw.size());
				RefreshDisplayText();
				
				if (!HasFocus()) {
					CallFunc(EventFunc(), Event::Confirm);
				}
			}
		}
		return true;
	}
	else if (!HasFocus()) {
		return false;
	}
	else if (ev->IsEvent(shok::InputEventIds::KeyDown)) {
		if (auto* me = BB::IdentifierCast<BB::CKeyEvent>(ev)) {
			if (me->IsKey(shok::Keys::Back)) {
				auto s = CurrentTextRaw.size();
				if (s > 0 && CurrentPos > 0) {
					CurrentTextRaw.erase(CurrentPos-1, 1);
					--CurrentPos;
					RefreshDisplayText();
					EGUIX::WidgetLoader::KeyStrokeLuaCallback();
				}
			}
			else if (me->IsKey(shok::Keys::Escape)) {
				ClearFocus();
				EGUIX::WidgetLoader::KeyStrokeLuaCallback();
				if (HasFlag(Event::Cancel))
					CallFunc(EventFunc(), Event::Cancel);
			}
			else if (me->IsKey(shok::Keys::Left)) {
				if (CurrentPos > 0)
					--CurrentPos;
				EGUIX::WidgetLoader::KeyStrokeLuaCallback();
			}
			else if (me->IsKey(shok::Keys::Right)) {
				if (CurrentPos < CurrentTextRaw.size())
					++CurrentPos;
				EGUIX::WidgetLoader::KeyStrokeLuaCallback();
			}
			else if (me->IsKey(shok::Keys::Enter)) {
				EGUIX::WidgetLoader::KeyStrokeLuaCallback();
				CallFunc(EventFunc(), Event::Confirm);
			}
		}
		return true;
	}
	else if (ev->IsEvent(shok::InputEventIds::KeyUp)) {
		return true;
	}
	else if (ev->IsEvent(shok::InputEventIds::KeyPressed)) {
		if (IgnoreNextChar) {
			IgnoreNextChar = false;
			return true;
		}
		if (auto* me = BB::IdentifierCast<BB::CKeyPressEvent>(ev)) {
			char c = static_cast<char>(me->KeyChar);
			if ((Mode() == Modes::Double || Mode() == Modes::UDouble) && c == ',')
				c = '.';
			if (CharValid(c)) {
				CurrentTextRaw.insert(CurrentPos, 1, c);
				bool adv = true;
				if (HasFlag(Event::Validate)) {
					if (!CallFunc(EventFunc(), Event::Validate)) {
						CurrentTextRaw.erase(CurrentTextRaw.begin() + CurrentPos);
						adv = false;
					}
				}
				if (adv) {
					++CurrentPos;
					RefreshDisplayText();
					EGUIX::WidgetLoader::KeyStrokeLuaCallback();
				}
			}
		}
		return true;
	}
	return false;
}

void* CppLogic::Mod::UI::TextInputCustomWidget::operator new(size_t s)
{
	return shok::Malloc(s);
}

void CppLogic::Mod::UI::TextInputCustomWidget::operator delete(void* p)
{
	shok::Free(p);
}

bool CppLogic::Mod::UI::TextInputCustomWidget::CharValid(char c) const
{
	static std::locale l("C");
	if (Mode() == Modes::Int) {
		return std::isdigit(c, l) || (c == '-' && NegativeNumberValid());
	}
	if (Mode() == Modes::UInt) {
		return std::isdigit(c, l);
	}
	if (Mode() == Modes::Double) {
		return std::isdigit(c, l) || (c == '-' && NegativeNumberValid()) || (c == '.' && CurrentTextRaw.find('.') == std::string::npos);
	}
	if (Mode() == Modes::UDouble) {
		return std::isdigit(c, l) || (c == '.' && CurrentTextRaw.find('.') == std::string::npos);
	}
	if (c == '@')
		return false;
	static std::string_view valids{ reinterpret_cast<const char*>(0x780A84) };
	return std::isalnum(c, l)
		|| valids.find(c) != std::string::npos;
}

bool CppLogic::Mod::UI::TextInputCustomWidget::NegativeNumberValid() const
{
	if (CurrentTextRaw.empty())
		return true;
	if (CurrentPos == 0 && CurrentTextRaw[0] != '-')
		return true;
	return false;
}

void CppLogic::Mod::UI::TextInputCustomWidget::RefreshDisplayText()
{
	CurrentTextDisplay.clear();
	if (Mode() == Modes::Password) {
		CurrentTextDisplay.append(CurrentTextRaw.length(), '*');
		return;
	}
	CurrentTextDisplay = ClearTextOutput();
}

std::string CppLogic::Mod::UI::TextInputCustomWidget::ClearTextOutput() const
{
	std::string r{};
	for (char cr : CurrentTextRaw) {
		auto c = static_cast<unsigned char>(cr);
		if (c > 0x7F) {
			r.append(1, static_cast<char>((c >> 6) | 0xC0));
			c = static_cast<char>(c & 0x3F | 0x80);
		}
		r.append(1, c);
	}
	return r;
}

bool CppLogic::Mod::UI::TextInputCustomWidget::CallFunc(std::string_view funcname, Event ev)
{
	if (funcname.empty())
		return true;
	lua::State L{ *EScr::GetCurrentLuaState() };
	int t = L.GetTop();
	std::string s = std::format("return {}", funcname);
	bool r = false;
	try {
		L.DoStringT(s, "TextInputCustomWidget::CallFunc");
		if (L.IsFunction(-1)) {
			L.Push(ClearTextOutput());
			L.Push(static_cast<int>(WidgetId));
			L.Push(static_cast<int>(ev));
			L.PCall(3, 1);
			r = L.ToBoolean(-1);
			L.Pop(1);
		}
	}
	catch (const lua::LuaException&) {}
	L.SetTop(t);
	return r;
}

shok::ClassId __stdcall CppLogic::Mod::UI::FreeCamCustomWidget::GetClassIdentifier() const
{
	return Identifier;
}

void* __stdcall CppLogic::Mod::UI::FreeCamCustomWidget::CastToIdentifier(shok::ClassId id)
{
	if (id == EGUIX::ICustomWidget::Identifier)
		return static_cast<EGUIX::ICustomWidget*>(this);
	return nullptr;
}

void CppLogic::Mod::UI::FreeCamCustomWidget::Initialize()
{
}

void CppLogic::Mod::UI::FreeCamCustomWidget::Destroy()
{
	delete this;
}

void CppLogic::Mod::UI::FreeCamCustomWidget::Render(EGUIX::CCustomWidget* widget, const EGUIX::Rect* screenCoords)
{
	float tick = shok::GetCurrentTimeFloat();
	if (tick > LastTick) {
		LastTick = tick + 0.05f;

		auto* cam = static_cast<ERwTools::CRwCameraHandler*>(*ERwTools::CRwCameraHandler::GlobalObj);

		if (Status == MouseStatus::None) {
			shok::Position p{ cam->CameraInfo.LookAtX, cam->CameraInfo.LookAtY };
			bool writeback = false;

			if (RotateRight) {
				cam->HorizontalAngle -= static_cast<float>(CurrentSensitivity(widget)) * 0.25f;
			}
			if (RotateLeft) {
				cam->HorizontalAngle += static_cast<float>(CurrentSensitivity(widget)) * 0.25f;
			}
			if (RotateUp) {
				cam->VerticalAngle -= static_cast<float>(CurrentSensitivity(widget)) * 0.25f;
			}
			if (RotateDown) {
				cam->VerticalAngle += static_cast<float>(CurrentSensitivity(widget)) * 0.25f;
			}
			ClampCamera(cam);
			shok::Position move{ static_cast<float>(CurrentSensitivity(widget) * 10), 0.0f };

			if (Forward) {
				p += move.Rotate(CppLogic::DegreesToRadians(cam->HorizontalAngle + 90.0f));
				writeback = true;
			}
			if (Backward) {
				p += move.Rotate(CppLogic::DegreesToRadians(cam->HorizontalAngle - 90.0f));
				writeback = true;
			}
			if (Right) {
				p += move.Rotate(CppLogic::DegreesToRadians(cam->HorizontalAngle));
				writeback = true;
			}
			if (Left) {
				p += move.Rotate(CppLogic::DegreesToRadians(cam->HorizontalAngle + 180.0f));
				writeback = true;
			}

			if (Up) {
				cam->CameraInfo.LookAtZ += move.X;
			}
			if (Down) {
				cam->CameraInfo.LookAtZ -= move.X;
			}

			if (writeback) {
				cam->CameraInfo.LookAtX = p.X;
				cam->CameraInfo.LookAtY = p.Y;
				ClampLookAt(cam);
			}
		}
		else {
			cam->VerticalAngle += MouseInverted(widget) * std::min(std::max(static_cast<float>(MouseStartY - MouseY) / 20 * 8, -1.0f), 1.0f);
			cam->HorizontalAngle += MouseInverted(widget) * std::min(std::max(static_cast<float>(MouseX - MouseStartX) / 20 * 8, -1.0f), 1.0f);
			ClampCamera(cam);

			float d = 0.0f;
			if (Status == MouseStatus::Forward)
				d = 1.0f;
			else if (Status == MouseStatus::Backward)
				d = -1.0f;

			if (d != 0.0f) {
				d *= static_cast<float>(CurrentSensitivity(widget) * 10);

				float pitch = CppLogic::DegreesToRadians(cam->VerticalAngle);
				float yaw = CppLogic::DegreesToRadians(cam->HorizontalAngle);

				float dz = -(d * std::sin(pitch));
				float dxy = (d * std::cos(pitch));
				float dx = -(dxy * std::sin(yaw));
				float dy = (dxy * std::cos(yaw));

				cam->CameraInfo.LookAtX += dx;
				cam->CameraInfo.LookAtY += dy;
				cam->CameraInfo.LookAtZ += dz;
				ClampLookAt(cam);
			}
		}
	}
	if (Status != MouseStatus::None) {
		EGUIX::Color c{};
		shok::UIRenderer::GlobalObj()->RenderLine(&c, true, static_cast<float>(MouseStartX), static_cast<float>(MouseStartY), static_cast<float>(MouseX), static_cast<float>(MouseY));
	}
}

bool CppLogic::Mod::UI::FreeCamCustomWidget::HandleEvent(EGUIX::CCustomWidget* widget, BB::CEvent* ev, BB::CEvent* evAgain)
{
	if (CheckFocusEvent(ev))
		return false;
	if (ev->IsEvent(shok::InputEventIds::WidgetShow)) {
		if (CurrentSensitivity(widget) == 0) {
			CurrentSensitivity(widget) = DefaultScrollSpeed();
		}
		return false;
	}
	else if (ev->IsEvent(shok::InputEventIds::MouseButtonUp)) {
		if (auto* me = BB::IdentifierCast<BB::CMouseEvent>(ev)) {
			if (!HasFocus()) {
				if (me->IsKey(shok::Keys::MouseLButton)) {
					GetFocus();
					EGUIX::WidgetLoader::KeyStrokeLuaCallback();
				}
			}
			else {
				Status = MouseStatus::None;
			}
			return true;
		}
	}
	else if (ev->IsEvent(shok::InputEventIds::MouseButtonDown)) {
		if (Status == MouseStatus::None && HasFocus()) {
			if (auto* me = BB::IdentifierCast<BB::CMouseEvent>(ev)) {
				if (me->IsKey(shok::Keys::MouseLButton)) {
					Status = MouseStatus::Forward;
					MouseX = me->X;
					MouseY = me->Y;
					MouseStartX = me->X;
					MouseStartY = me->Y;
				}
				else if (me->IsKey(shok::Keys::MouseRButton)) {
					Status = MouseStatus::Backward;
					MouseX = me->X;
					MouseY = me->Y;
					MouseStartX = me->X;
					MouseStartY = me->Y;
				}
				else if (me->IsKey(shok::Keys::MouseMButton)) {
					Status = MouseStatus::Rotate;
					MouseX = me->X;
					MouseY = me->Y;
					MouseStartX = me->X;
					MouseStartY = me->Y;
				}
			}
		}
		return true;
	}
	else if (!HasFocus()) {
		Forward = false;
		Backward = false;
		Left = false;
		Right = false;
		RotateLeft = false;
		RotateRight = false;
		Up = false;
		Down = false;
		RotateUp = false;
		RotateDown = false;
		Status = MouseStatus::None;
		return false;
	}
	else if (ev->IsEvent(shok::InputEventIds::MouseWheel)) {
		if (auto* me = BB::IdentifierCast<BB::CMouseEvent>(ev)) {
			CurrentSensitivity(widget) = std::max(0, CurrentSensitivity(widget) + (me->Delta > 0 ? 5 : -5));
		}
		return true;
	}
	else if (ev->IsEvent(shok::InputEventIds::MouseMove) && Status != MouseStatus::None) {
		if (auto* me = BB::IdentifierCast<BB::CMouseEvent>(ev)) {
			MouseX = me->X;
			MouseY = me->Y;
		}
		return false;
	}
	else if (ev->IsEvent(shok::InputEventIds::KeyDown)) {
		if (auto* ev2 = BB::IdentifierCast<BB::CKeyEvent>(ev)) {
			if (ev2->IsKey(shok::Keys::W)) {
				Forward = true;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::S)) {
				Backward = true;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::A)) {
				Left = true;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::D)) {
				Right = true;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::Q)) {
				RotateLeft = true;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::E)) {
				RotateRight = true;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::R)) {
				Up = true;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::F)) {
				Down = true;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::T)) {
				RotateUp = true;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::G)) {
				RotateDown = true;
				return true;
			}
		}
	}
	else if (ev->IsEvent(shok::InputEventIds::KeyUp)) {
		if (auto* ev2 = BB::IdentifierCast<BB::CKeyEvent>(ev)) {
			if (ev2->IsKey(shok::Keys::W)) {
				Forward = false;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::S)) {
				Backward = false;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::A)) {
				Left = false;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::D)) {
				Right = false;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::Q)) {
				RotateLeft = false;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::E)) {
				RotateRight = false;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::R)) {
				Up = false;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::F)) {
				Down = false;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::T)) {
				RotateUp = false;
				return true;
			}
			else if (ev2->IsKey(shok::Keys::G)) {
				RotateDown = false;
				return true;
			}
		}
	}
	return false;
}

void* CppLogic::Mod::UI::FreeCamCustomWidget::operator new(size_t s)
{
	return shok::Malloc(s);
}

void CppLogic::Mod::UI::FreeCamCustomWidget::operator delete(void* p)
{
	shok::Free(p);
}

void CppLogic::Mod::UI::FreeCamCustomWidget::ClampCamera(ERwTools::CRwCameraHandler* cam)
{
	if (cam->VerticalAngle > 90.0f)
		cam->VerticalAngle = 90.0f;
	if (cam->VerticalAngle < -90.0f)
		cam->VerticalAngle = -90.0f;
	while (cam->HorizontalAngle < -180.0f)
		cam->HorizontalAngle += 360.0f;
	while (cam->HorizontalAngle >= 180.0f)
		cam->HorizontalAngle -= 360.0f;
}
void CppLogic::Mod::UI::FreeCamCustomWidget::ClampLookAt(ERwTools::CRwCameraHandler* cam)
{
	if (cam->CameraInfo.LookAtX < 0.0f)
		cam->CameraInfo.LookAtX = 0.0f;
	if (cam->CameraInfo.LookAtY < 0.0f)
		cam->CameraInfo.LookAtY = 0.0f;
	auto p = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->GetMapSize();
	if (cam->CameraInfo.LookAtX > p.X)
		cam->CameraInfo.LookAtX = p.X;
	if (cam->CameraInfo.LookAtY > p.Y)
		cam->CameraInfo.LookAtY = p.Y;
}
