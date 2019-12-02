#include "stdafx.h"
#include "CRestPosition.h"
#include "Buffer.h"

#ifdef _RESTPOSITION_H

CRestPosition::CRestPosition(unsigned char byNo, unsigned char byNext, std::string strName)
{
	m_byNo = byNo;
	m_byNext = byNext; 
	m_strName = strName;
}

CRestPosition::CRestPosition(unsigned char byNo, unsigned char byNext, std::wstring wstrName)
{
	m_byNo = byNo;
	m_byNext = byNext;
	BufferString::std_wstring_to_string(m_strName, wstrName);
}

#ifdef _AFX
CRestPosition::CRestPosition(unsigned char byNo, unsigned char byNext, CString cstrName)
{
	m_byNo = byNo;
	m_byNext = byNext;
	BufferString::CString_to_std_string(m_strName, cstrName);
}
#endif //!_AFX

CRestPosition::CRestPosition(const CRestPosition & rest)
{
	m_byNo = rest.m_byNo;
	m_byNext = rest.m_byNext;
	m_strName = rest.m_strName;
}

CRestPosition::~CRestPosition()
{
}

bool CRestPosition::SetNo(unsigned char byNo)
{
	if (m_byNo == byNo)
	{
		return false;
	}

	m_byNo = byNo;

	return true;
}

unsigned char CRestPosition::GetNo() const
{
	return m_byNo;
}

bool CRestPosition::SetNext(unsigned char byNext)
{
	if (m_byNext == byNext)
	{
		return false;
	}

	m_byNext = byNext;

	return true;
}

unsigned char CRestPosition::GetNext() const
{
	return m_byNext;
}

bool CRestPosition::SetName(std::string strName)
{
	if (m_strName == strName)
	{
		return false;
	}

	m_strName = strName;

	return true;
}

bool CRestPosition::SetName(std::wstring wstrName)
{
	std::string strName = "";

	BufferString::std_wstring_to_string(strName, wstrName);

	return SetName(strName);
}

bool CRestPosition::SetName(CString cstrName)
{
	std::string strName = "";

	BufferString::CString_to_std_string(strName, cstrName);

	return SetName(strName);
}

void CRestPosition::GetName(std::string & strName)
{
	strName = m_strName;

	return;
}

void CRestPosition::GetName(std::wstring & wstrName)
{
	BufferString::std_string_to_wstring(wstrName, m_strName);

	return;
}

#ifdef _AFX
void CRestPosition::GetName(CString & cstrName)
{
	BufferString::std_string_to_CString(cstrName, m_strName);

	return;
}
#endif // !_AFX

void CRestPosition::GetNo(std::string & strNo)
{
	BufferString::std_string_format(strNo, "%d", m_byNo);

	return;
}

void CRestPosition::GetNo(std::wstring & wstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_wstring(wstrNo, strNo);

	return;
}

void CRestPosition::GetNext(std::string & strNo)
{
	BufferString::std_string_format(strNo, "%d", m_byNext);

	return;
}

void CRestPosition::GetNext(std::wstring & wstrNo)
{
	std::string strNo = "";
	GetNext(strNo);

	BufferString::std_string_to_wstring(wstrNo, strNo);

	return;
}

#ifdef _AFX
void CRestPosition::GetNo(CString & cstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_CString(cstrNo, strNo);

	return;
}

void CRestPosition::GetNext(CString & cstrNo)
{
	std::string strNo = "";
	GetNext(strNo);

	BufferString::std_string_to_CString(cstrNo, strNo);

	return;
}
#endif // !_AFX

#endif // !_RESTPOSITION_H