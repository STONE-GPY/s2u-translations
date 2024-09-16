#include <translations.hpp>

CUtlString Translations::CPhrase::CContent::Format(const CFormat &aData, size_t nCount, ...) const
{
	const auto &mapFrames = aData.GetFrames();

	CUtlString sResult = Get();

	{
		va_list aParams;

		va_start(aParams, nCount);

		for(size_t n = 1; n <= nCount; n++)
		{
			auto iFound = mapFrames.Find(n);

			if(iFound == mapFrames.InvalidIndex())
			{
				continue;
			}

			CBufferStringGrowable<MAX_TRANSLATIONS_FORMAT_FRAME_TARGET_LENGTH> sFrameTarget;

			sFrameTarget.Format("{%zd}", n);

			auto aFrame = mapFrames.Element(iFound);

			CBufferStringGrowable<MAX_TRANSLATIONS_FORMAT_FRAME_RESULT_LENGTH> sFrameResult;

			switch(*aFrame.GetArgument())
			{
				case 'b':
				{
					auto nValue = va_arg(aParams, int);

					do
					{
						if(nValue & 1)
						{
							sFrameResult.AppendRepeat('1', 1);
						}
						else
						{
							sFrameResult.AppendRepeat('0', 1);
						}

						nValue >>= 1;
					}
					while(nValue);

					break;
				}

				case 'c':
				{
					sFrameResult.AppendRepeat((char)va_arg(aParams, int), 1);

					break;
				}

				case 'd':
				case 'i':
				{
					sFrameResult.Format("%d", va_arg(aParams, int));

					break;
				}

				case 'u':
				{
					sFrameResult.Format("%u", va_arg(aParams, uint));

					break;
				}

				case 'f':
				{
					sFrameResult.Format("%lf", va_arg(aParams, double));

					break;
				}

				case 's':
				{
					const char *pszConcat[] = {va_arg(aParams, const char *)};

					sFrameResult.AppendConcat(ARRAYSIZE(pszConcat), pszConcat, NULL);

					break;
				}

				case 'x':
				{
					sFrameResult.Format("%x", va_arg(aParams, int));

					break;
				}

				case 'X':
				{
					sFrameResult.Format("%X", va_arg(aParams, int));

					break;
				}
			}

			sResult = sResult.Replace(sFrameTarget.Get(), sFrameResult.Get());
		}

		va_end(aParams);
	}

	return sResult;
}
