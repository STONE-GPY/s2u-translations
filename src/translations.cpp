/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * ======================================================
 * Tranlation util for Source2 games.
 * Written by Wend4r (2024).
 * ======================================================

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <translations.hpp>

#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>

#include <tier0/commonmacros.h>
#include <tier1/keyvalues3.h>

Translations::Translations()
//  :  m_mapPhrases(DefLessFunc(const CUtlSymbolLarge))
 :  m_mapPhrases()
{
}

Translations::CKey_t Translations::GetKeyT(const char *pszInit)
{
	CKey_t nResult {};

	if(!pszInit[0])
	{
		nResult = 0;
	}
	else if(!pszInit[1])
	{
		nResult = pszInit[0];
	}
	else if(!pszInit[2])
	{
		nResult = *(uint16 *)pszInit;
	}
	else if(!pszInit[3])
	{
		nResult = *(uint16 *)(pszInit + 1) << 8 | *pszInit;
	}
	else if(!pszInit[4])
	{
		nResult = *(uint32 *)pszInit;
	}
	else
	{
		AssertMsg(0, "Translations key over 5 characters");
	}

	return nResult;
}

bool Translations::FindPhrase(const char *pszName, const_iterator &iFound) const
{
	iFound = m_mapPhrases.find(FindPhraseSymbol(pszName));
	return iFound != m_mapPhrases.end();
}

const Translations::CPhrase &Translations::GetPhrase(iterator iFound) const
{
	return iFound->second;
}

bool Translations::Parse(const KeyValues3 *pRoot, CBufferStringVector &vecMessages)
{
	// Entering to "Phrases" (if any).
	{
		const KeyValues3 *pPhrasesData = pRoot->FindMember("Phrases");

		if(pPhrasesData)
		{
			pRoot = pPhrasesData;
		}
	}

	int iMemberCount = pRoot->GetMemberCount();

	if(!iMemberCount)
	{
		static const char *s_pszMessageConcat[] = {"No members"};

		vecMessages.AddToTail(s_pszMessageConcat);

		return true;
	}

	for(KV3MemberId_t n = 0; n < iMemberCount; n++)
	{
		const char *pszPhraseName = pRoot->GetMemberName(n);

		const KeyValues3 *pPhraseDataKeys = pRoot->GetMember(n);

		ParsePhrase(pszPhraseName, pPhraseDataKeys, vecMessages);
	}

	return true;
}

std::map<std::string, std::string> m_ColorMap = {
	{"{DEFAULT}", "\x01"},
	{"{RED}", "\x02"},
	{"{LIGHTPURPLE}", "\x03"},
	{"{GREEN}", "\x04"},
	{"{LIME}", "\x05"},
	{"{LIGHTGREEN}", "\x06"},
	{"{LIGHTRED}", "\x07"},
	{"{GRAY}", "\x08"},
	{"{LIGHTOLIVE}", "\x09"},
	{"{OLIVE}", "\x10"},
	{"{LIGHTBLUE}", "\x0B"},
	{"{BLUE}", "\x0C"},
	{"{PURPLE}", "\x0E"},
	{"{GRAYBLUE}", "\x0A"},
	{"\\n", "\xe2\x80\xa9"}
};
std::string ReplaceAllColor(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;

	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}

	return str;
}
const char* _ParseColor(const char* pszValue)
{
	std::string text(pszValue);
	for(const auto& [from, to] : m_ColorMap){
		text = ReplaceAllColor(text, from, to);
	}
	return text.c_str();
}

bool Translations::ParsePhrase(const char *pszName, const KeyValues3 *pDataKeys, CBufferStringVector &vecMessages)
{
	int iMemberCount = pDataKeys->GetMemberCount();

	if(!iMemberCount)
	{
		static const char *s_pszMessageConcat[] = {"No members"};

		vecMessages.AddToTail(s_pszMessageConcat);

		return false;
	}

	auto &mapPhrases = m_mapPhrases;
	std::pair<CUtlSymbolLarge, CPhrase> pairPhrase;
	iterator iterPhrase;

	CUtlSymbolLarge sPhrase = GetPhraseSymbol(pszName);

	if((iterPhrase = mapPhrases.find(sPhrase)) == mapPhrases.end())
	{
		auto oPhraseInsertResult = mapPhrases.emplace(sPhrase, CPhrase());
		if(oPhraseInsertResult.second == false) return false;
		iterPhrase = oPhraseInsertResult.first;
	}
	CPhrase& aPhrase = iterPhrase->second;

	for(KV3MemberId_t n = 0; n < iMemberCount; n++)
	{
		const char *pszKey = pDataKeys->GetMemberName(n);

		const char *pszValue = pDataKeys->GetMember(n)->GetString();

		if(!V_strcmp(pszKey, "#format"))
		{
			aPhrase.ParseFormatString(pszValue, vecMessages);
		}
		else
		{
			const char* convertedValue = _ParseColor(pszValue);
			aPhrase.InsertContent(GetKeyT(pszKey), convertedValue);
		}
	}

	return true;
}

void Translations::Purge()
{
	m_mapPhrases.clear();
	m_aPhraseSymbolTable.Purge();
}

CUtlSymbolLarge Translations::GetPhraseSymbol(const char *pszName)
{
	return m_aPhraseSymbolTable.AddString(pszName);
}

CUtlSymbolLarge Translations::FindPhraseSymbol(const char *pszName) const
{
	return m_aPhraseSymbolTable.Find(pszName);
}