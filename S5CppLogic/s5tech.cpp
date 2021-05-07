#include "pch.h"
#include "s5data.h"

float shok_technologyModifier::ModifyValue(float i)
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
