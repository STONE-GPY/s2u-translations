#include <translations.hpp>

Translations::CPhrase::CFormat::CFormat()
 :  m_mapFrames()
{
}

const std::map<Translations::CPhrase::CFormat::CFrame_t, Translations::CPhrase::CFormat::CFrame> &Translations::CPhrase::CFormat::GetFrames() const
{
	return m_mapFrames;
}

CUtlString Translations::CPhrase::CFormat::GenerateString() const
{
	CBufferStringN<1024> sResult;

	for (const auto& [key, value] : m_mapFrames)
	{
		sResult.AppendFormat("{%d:%s},", key, value.GetArgument());

	}

	sResult.SetLength(sResult.Length() - 1);

	return sResult;
}

const char *Translations::CPhrase::CFormat::ParseString(const char *psz, CBufferStringVector &vecMessages)
{
	do
	{
		const auto &iInvalid = m_mapFrames.end();

		auto iKey = iInvalid;

		while(*psz)
		{
			if(*psz == ',')
			{
				psz++;
			}

			if(*psz == '{')
			{
				psz++;
			}
			else
			{
				static const char *s_pszMessageConcat[] = {"Format: ", "no start with"};

				vecMessages.AddToTail(s_pszMessageConcat);

				return psz;
			}

			{
				char *psNextIterfator;

				auto res = m_mapFrames.emplace((CFormat_t)strtoul(psz, &psNextIterfator, 10), CFrame());
				if(res.second) iKey = res.first;
				psz = psNextIterfator;
			}

			if(*psz == ':')
			{
				psz++;
			}
			else
			{
				static const char *s_pszMessageConcat[] = {"Format: ", "no separator character"};

				vecMessages.AddToTail(s_pszMessageConcat);

				return psz;
			}

			if(iKey != iInvalid)
			{
				psz = iKey->second.ParseString(psz, vecMessages);
			}
		}
	}
	while(*psz && *psz == ',');

	return psz;
}
