#include <translations.hpp>

Translations::CPhrase::CPhrase()
 :  m_map()
{
}

const Translations::CPhrase::CFormat &Translations::CPhrase::GetFormat() const
{
	return m_aFormat;
}

const char *Translations::CPhrase::ParseFormatString(const char *psz, CBufferStringVector &vecMessages)
{
	return m_aFormat.ParseString(psz, vecMessages);
}

void Translations::CPhrase::InsertContent(const CKey_t nKey, const CContent &aData)
{
	m_map.emplace(nKey, aData);
}

bool Translations::CPhrase::Find(const CKey &sKey, const CContent *&psOutput) const
{
	auto iFound = m_map.find((CKey_t)sKey);

	bool bResult = iFound != m_map.end();

	if(bResult)
	{
		psOutput = &iFound->second;
	}

	return bResult;
}
