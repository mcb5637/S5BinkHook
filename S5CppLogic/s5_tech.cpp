#include "pch.h"
#include "s5_tech.h"
#include "s5_classfactory.h"
#include "s5_idmanager.h"

float shok::Technology::Modifier::ModifyValue(float i) const
{
	switch (Operator.c_str()[0]) {
	case '+':
		i += Value;
		break;
	case '-':
		i -= Value;
		break;
	case '*':
		i *= Value;
		break;
	case '/':
		i /= Value;
		break;
	}
    return i;
}

shok::Technology::Modifier::Modifier()
{
	Operator.assign("#");
}

void* shok::Technology::operator new(size_t s)
{
	return shok::Malloc(s);
}
void shok::Technology::operator delete(void* p) {
	shok::Free(p);
}

static inline shok::Technology::Modifier* (__thiscall* const tech_getmodif)(shok::Technology* th, shok::TechModifierType t) = reinterpret_cast<shok::Technology::Modifier* (__thiscall*)(shok::Technology*, shok::TechModifierType)>(0x4C78CC);
shok::Technology::Modifier& shok::Technology::GetModifierOfType(TechModifierType t)
{
	return *tech_getmodif(this, t);
}

void shok::TechManager::FreeTech(int id)
{
	--id;
	Technology* t = Techs[id];
	delete t;
	Techs[id] = nullptr;
}

void shok::TechManager::LoadTech(int id)
{
	--id;
	if (id > static_cast<int>(Techs.size()))
		throw std::logic_error{ "somehow the id is too big" };
	if (id == static_cast<int>(Techs.size())) {
		auto v = Techs.SaveVector();
		v.Vector.push_back(nullptr);
	}
	Technology* t = new Technology{};
	std::string filename = "Data\\Config\\Technologies\\";
	filename.append((*BB::CIDManagerEx::TechnologiesManager)->GetNameByID(id + 1));
	filename.append(".xml");
	(*BB::CClassFactory::GlobalObj)->LoadObject(t, filename.c_str(), t->SerializationData);
	Techs[id] = t;
}

void shok::TechManager::PopTech(int id)
{
	if (id != static_cast<int>(Techs.size()))
		throw std::out_of_range("invalid id");
	FreeTech(id);
	auto v = Techs.SaveVector();
	v.Vector.pop_back();
}

float shok::AdditionalTechModifier::ModifyValue(float i) const
{
	switch (Operator) {
	case '+':
		i += Value;
		break;
	case '-':
		i -= Value;
		break;
	case '*':
		i *= Value;
		break;
	case '/':
		i /= Value;
		break;
	}
	return i;
}
