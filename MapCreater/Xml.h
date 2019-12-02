#pragma once
#include <string>
#include "tinystr.h"
#include "tinyxml.h"

static CString UTF8toUnicode(const char* utf8Str, UINT length)
{
	CString unicodeStr;
	unicodeStr = _T("");

	if (!utf8Str)
		return unicodeStr;

	if (length == 0)
		return unicodeStr;

	//×ª»» 
	WCHAR chr = 0;
	for (UINT i = 0; i < length;)
	{
		if ((0x80 & utf8Str[i]) == 0) // ASCII 
		{
			chr = utf8Str[i];
			i++;
		}
		else if ((0xE0 & utf8Str[i]) == 0xC0) // 110xxxxx 10xxxxxx 
		{
			chr = (utf8Str[i + 0] & 0x3F) << 6;
			chr |= (utf8Str[i + 1] & 0x3F);
			i += 2;
		}
		else if ((0xF0 & utf8Str[i]) == 0xE0) // 1110xxxx 10xxxxxx 10xxxxxx 
		{
			chr = (utf8Str[i + 0] & 0x1F) << 12;
			chr |= (utf8Str[i + 1] & 0x3F) << 6;
			chr |= (utf8Str[i + 2] & 0x3F);
			i += 3;
		}
		/*
		else if() // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		{}
		else if() // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		{}
		else if() // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		{}
		*/
		else // ²»ÊÇUTF-8×Ö·û´® 
		{
			return unicodeStr;
		}
		unicodeStr.AppendChar(chr);
	}

	return unicodeStr;
}

static CString UTF8toUnicode(const char* utf8Str)
{
	UINT theLength = strlen(utf8Str);
	return UTF8toUnicode(utf8Str, theLength);
}