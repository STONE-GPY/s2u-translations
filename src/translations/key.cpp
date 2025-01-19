#include <translations.hpp>

Translations::CKey::CKey(const char *pszInit)
{
	*(CKey_t *)this = GetKeyT(pszInit);
}

Translations::CKey::CKey(const CKey_t nInit)
{
	*(CKey_t *)this = nInit;
}

Translations::CKey_t Translations::CKey::Get() const
{
	return *(CKey_t *)m_sCode;
}

const char *Translations::CKey::String() const
{
	return m_sCode;
}

bool Translations::CKey::operator==(const CKey &aRigth) const
{
	return Get() == aRigth.Get();
}

bool Translations::CKey::operator!=(const CKey &aRigth) const
{
	return Get() != aRigth.Get();
}

bool Translations::CKey::operator<(const CKey &aRigth) const
{
	return Get() < aRigth.Get();
}
