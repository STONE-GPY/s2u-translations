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

#include <tier0/commonmacros.h>
#include <tier1/keyvalues3.h>

Translations::Translations()
 :  m_mapPhrases(DefLessFunc(const CUtlSymbolLarge))
{
}

Translations::CKey_t Translations::GetKeyT(const char *pszInit)
{
	CKey_t nResult;

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
		nResult = *(uint16 *)pszInit << 8 | *pszInit;
	}
	else
	{
		AssertMsg(0, "Translations key over 4 characters");
	}

	return nResult;
}

bool Translations::FindPhrase(const char *pszName, int &iFound) const
{
	iFound = m_mapPhrases.Find(FindPhraseSymbol(pszName));

	return iFound != m_mapPhrases.InvalidIndex();
}

const Translations::CPhrase &Translations::GetPhrase(int iFound) const
{
	return m_mapPhrases.Element(iFound);
}

bool Translations::Parse(const KeyValues3 *pRoot, CBufferStringVector &vecMessages)
{
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

bool Translations::ParsePhrase(const char *pszName, const KeyValues3 *pDataKeys, CBufferStringVector &vecMessages)
{
	int iMemberCount = pDataKeys->GetMemberCount();

	if(!iMemberCount)
	{
		static const char *s_pszMessageConcat[] = {"No members"};

		vecMessages.AddToTail(s_pszMessageConcat);

		return false;
	}

	CPhrase aPhrase {};

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
			aPhrase.InsertContent(GetKeyT(pszKey), pszValue);
		}
	}

	m_mapPhrases.Insert(GetPhraseSymbol(pszName), aPhrase);

	return true;
}

void Translations::Purge()
{
	m_mapPhrases.Purge();
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
