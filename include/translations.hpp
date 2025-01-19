/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * ======================================================
 * Translationss util for Source2 games.
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

#ifndef _INCLUDE_TRANSLATIONS_HPP_
#define _INCLUDE_TRANSLATIONS_HPP_

#include <tier0/bufferstring.h>
#include <tier0/platform.h>
#include <tier0/strtools.h>
#include <tier1/utlmap.h>
#include <tier1/utlsymbollarge.h>

#define MAX_TRANSLATIONS_MESSAGE_LENGTH 1024

#define MAX_TRANSLATIONS_FORMAT_FRAME_TARGET_LENGTH 32 // 2 ("{}") + 11 ("-2147483648") + 1 (null terminated) = 14
#define MAX_TRANSLATIONS_FORMAT_FRAME_RESULT_LENGTH 512

class KeyValues3;

class Translations
{
public:
	using CBufferStringSection = CBufferStringN<MAX_TRANSLATIONS_MESSAGE_LENGTH>;

	class CBufferStringConcat : public CBufferStringSection
	{
	public:
		using Base = CBufferStringSection;
		using Base::Base;

		template<uintp N>
		CBufferStringConcat(const char *(&pszSplit)[N])
		{
			AppendConcat(N, pszSplit, NULL);
		}

		template<uintp N>
		CBufferStringConcat(const char *pszStartWtih, const char *(&pszSplit)[N])
		{
			Insert(0, pszStartWtih);
			AppendConcat(N, pszSplit, NULL);
		}
	}; // GameData::CBufferStringConcat

	using CBufferStringVector = CUtlVector<CBufferStringConcat>;

public:
	Translations();

public:
	using CKey_t = uint32;

	static CKey_t GetKeyT(const char *pszInit);

	struct CKey
	{
		char m_sCode[4];

		CKey(const char *pszInit);
		CKey(const CKey_t nInit);
		CKey_t Get() const;
		const char *String() const;

		operator CKey_t() const { return Get(); }
		bool operator==(const CKey &aRigth) const;
		bool operator!=(const CKey &aRigth) const;
		bool operator<(const CKey &aRigth) const;
	}; // CKey

	class CPhrase
	{
		friend class Translations;

	public:
		CPhrase();

	public:
		class CFormat;

		using CContentBase = CUtlString;

		class CContent : public CContentBase
		{
		public:
			using CBase = CContentBase;
			using CBase::CBase;

		public:
			CUtlString Format(const CFormat &aData, size_t nCount, ...) const;
		}; // CContent

		class CFormat
		{
			friend class CPhrase;

		public:
			CFormat();

			using CFrame_t = uint32;

			class CFrame
			{
				friend class CFormat;

			public:
				const char *GetArgument() const;

			protected:
				const char *ParseString(const char *psz, CBufferStringVector &vecMessages);

			private:
				char m_sArgument[8];
			}; // CFrame

		public:
			const CUtlMap<CFrame_t, CFrame> &GetFrames() const;
			CUtlString GenerateString() const;

		protected:
			const char *ParseString(const char *psz, CBufferStringVector &vecMessages);

		private:
			CUtlMap<CFrame_t, CFrame> m_mapFrames;
		}; // CFormat

		const CFormat &GetFormat() const;
		bool Find(const CKey &sCountryCode, const CContent *&psOutput) const;

	protected:
		const char *ParseFormatString(const char *psz, CBufferStringVector &vecMessages);
		void InsertContent(const CKey_t nKey, const CContent &aData);

	private:
		using CFormat_t = uint32;
		CFormat m_aFormat;
		CUtlMap<CKey_t, CContent> m_map;
	}; // CPhrase

public:
	bool FindPhrase(const char *pszName, int &iFound) const;
	const CPhrase &GetPhrase(int iFound) const;

public:
	bool Parse(const KeyValues3 *pRoot, CBufferStringVector &vecMessages);
	bool ParsePhrase(const char *pszName, const KeyValues3 *pDataKeys, CBufferStringVector &vecMessages);

public:
	void Purge();

protected:

protected:
	CUtlSymbolLarge GetPhraseSymbol(const char *pszName);
	CUtlSymbolLarge FindPhraseSymbol(const char *pszName) const;

private:
	CUtlSymbolTableLarge_CI m_aPhraseSymbolTable;
	CUtlMap<CUtlSymbolLarge, CPhrase> m_mapPhrases;
}; // Translations

#endif // _INCLUDE_TRANSLATIONS_HPP_
