#include "stdafx.h"
#include "CStation.h"

#ifdef _STATION_H

CStation::CStation(unsigned char byNo, std::string strName)
{
	m_byNo = byNo;
	m_strName = strName;
}

CStation::CStation(unsigned char byNo, std::wstring wstrName)
{
	m_byNo = byNo;

	BufferString::std_wstring_to_string(m_strName, wstrName);
}

#ifdef _AFX
CStation::CStation(unsigned char byNo, CString cstrName)
{
	m_byNo = byNo;

	BufferString::CString_to_std_string(m_strName, cstrName);
}
#endif // !_AFX

CStation::~CStation()
{
}

bool CStation::SetNo(unsigned char byNo)
{
	if (m_byNo == byNo)
	{
		return false;
	}

	m_byNo = byNo;

	return true;
}

unsigned char CStation::GetNo() const
{
	return m_byNo;
}

bool CStation::SetName(std::string strName)
{
	if (m_strName == strName)
	{
		return false;
	}
	
	m_strName = strName;

	return true;
}

bool CStation::SetName(std::wstring wstrName)
{
	std::string strName = "";

	BufferString::std_wstring_to_string(strName, wstrName);

	return SetName(strName);
}

bool CStation::SetName(CString cstrName)
{
	std::string strName = "";

	BufferString::CString_to_std_string(strName, cstrName);

	return SetName(strName);
}

void CStation::GetName(std::string & strName)
{
	strName = m_strName;

	return;
}

void CStation::GetName(std::wstring & wstrName)
{
	BufferString::std_string_to_wstring(wstrName, m_strName);

	return;
}

#ifdef _AFX
void CStation::GetName(CString & cstrName)
{
	BufferString::std_string_to_CString(cstrName, m_strName);

	return;
}

void CStation::GetNo(std::string & strNo)
{
	BufferString::std_string_format(strNo,"%d",m_byNo);

	return;
}

void CStation::GetNo(std::wstring & wstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_wstring(wstrNo,strNo);

	return;
}

void CStation::GetNo(CString & cstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_CString(cstrNo, strNo);

	return;
}
#endif // !_AFX

#endif // !_STATION_H