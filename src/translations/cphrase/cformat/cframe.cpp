#include <translations.hpp>

const char *Translations::CPhrase::CFormat::CFrame::GetArgument() const
{
	return m_sArgument;
}

const char *Translations::CPhrase::CFormat::CFrame::ParseString(const char *psz, CBufferStringVector &vecMessages)
{
	size_t nLength = 0;

	while(true)
	{
		if(*psz)
		{
			if(*psz == '}')
			{
				m_sArgument[nLength] = '\0';

				return psz;
			}
			else if(nLength < sizeof(m_sArgument) - 1)
			{
				m_sArgument[nLength++] = *psz;
			}
		}
		else
		{
			m_sArgument[nLength] = '\0';

			static const char *s_pszMessageConcat[] = {"Format: ", "no end"};

			vecMessages.AddToTail(s_pszMessageConcat);

			return psz;
		}

		psz++;
	}

	return psz;
}
