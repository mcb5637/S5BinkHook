#include "pch.h"
#include "s5_tech.h"

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
