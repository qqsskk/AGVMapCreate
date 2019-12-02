#include "stdafx.h"
#include "CLED.h"

#ifdef _LED_H
CLED::CLED(unsigned char byNo, unsigned short usWidth, unsigned short usHeight, std::string strIP, unsigned short usPort, std::string strText)
{
	m_byNo = byNo;
	m_usWidth = usWidth;
	m_usHeight = usHeight;
	m_strIP = strIP;
	m_usPort = usPort;
	m_strText = strText;
}

CLED::CLED(unsigned char byNo, unsigned short usWidth, unsigned short usHeight, std::wstring wstrIP, unsigned short usPort, std::wstring wstrText)
{
	m_byNo = byNo;
	m_usWidth = usWidth;
	m_usHeight = usHeight;
	BufferString::std_wstring_to_string(m_strIP, wstrIP);
	m_usPort = usPort;
	BufferString::std_wstring_to_string(m_strText, wstrText);
}

#ifdef _AFX
CLED::CLED(unsigned char byNo, unsigned short usWidth, unsigned short usHeight, CString cstrIP, unsigned short usPort, CString cstrText)
{
	m_byNo = byNo;
	m_usWidth = usWidth;
	m_usHeight = usHeight;
	BufferString::CString_to_std_string(m_strIP, cstrIP);
	m_usPort = usPort;
	BufferString::CString_to_std_string(m_strText, cstrText);
}
#endif //!_AFC

CLED::~CLED()
{
}

bool CLED::SetNo(unsigned char byNo)
{
	if(m_byNo == byNo)
	{
		return false;
	}

	m_byNo = byNo;

	return true;
}

unsigned char CLED::GetNo() const
{
	return m_byNo;
}

void CLED::GetNo(std::string & strNo)
{
	BufferString::std_string_format(strNo, "%d", m_byNo);

	return;
}

void CLED::GetNo(std::wstring & wstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_wstring(wstrNo, strNo);

	return;
}

bool CLED::SetWidth(unsigned short usWidth)
{
	if (m_usWidth == usWidth)
	{
		return false;
	}

	m_usWidth = usWidth;

	return true;
}

unsigned short CLED::GetWidth() const
{
	return m_usWidth;
}

void CLED::GetWidth(std::string & strWidth)
{
	BufferString::std_string_format(strWidth, "%d", m_usWidth);

	return;
}

void CLED::GetWidth(std::wstring & wstrWidth)
{
	std::string strWidth = "";
	GetWidth(strWidth);

	BufferString::std_string_to_wstring(wstrWidth, strWidth);

	return;
}

bool CLED::SetHeight(unsigned short usHeight)
{
	if (m_usHeight == usHeight)
	{
		return false;
	}

	m_usHeight = usHeight;

	return true;
}

unsigned short CLED::GetHeight() const
{
	return m_usHeight;
}

void CLED::GetHeight(std::string strHeight)
{
	BufferString::std_string_format(strHeight, "%d", m_usHeight);

	return;
}

void CLED::GetHeight(std::wstring wstrHeight)
{
	std::string strHeight = "";
	GetHeight(strHeight);

	BufferString::std_string_to_wstring(wstrHeight, strHeight);

	return;
}

bool CLED::SetIP(std::string strIP)
{
	if(m_strIP == strIP)
	{
		return false;
	}

	m_strIP = strIP;

	return true;
}

bool CLED::SetIP(std::wstring wstrIP)
{
	std::string strIP = "";
	BufferString::std_wstring_to_string(strIP,wstrIP);

	return SetIP(strIP);
}

void CLED::GetIP(std::string & strIP)
{
	strIP =  m_strIP;

	return;
}

void CLED::GetIP(std::wstring & wstrIP)
{
	BufferString::std_string_to_wstring(wstrIP,m_strIP);

	return;
}

bool CLED::SetPort(unsigned short usPort)
{
	if (m_usPort == usPort)
	{
		return false;
	}

	m_usPort = usPort;

	return true;
}

unsigned short CLED::GetPort() const
{
	return m_usPort;
}

void CLED::GetPort(std::string & strPort)
{
	BufferString::std_string_format(strPort, "%d", m_usPort);

	return;
}

void CLED::GetPort(std::wstring wstrPort)
{
	std::string strPort = "";
	GetPort(strPort);

	BufferString::std_string_to_wstring(wstrPort, strPort);

	return;
}

bool CLED::SetText(std::string strText)
{
	if (m_strText == strText)
	{
		return false;
	}

	m_strText = strText;

	return true;
}

bool CLED::SetText(std::wstring wstrText)
{
	std::string strText = "";
	BufferString::std_wstring_to_string(strText, wstrText);

	return SetText(strText);
}

void CLED::GetText(std::string & strText)
{
	strText = m_strText;

	return;
}

void CLED::GetText(std::wstring & wstrText)
{
	BufferString::std_string_to_wstring(wstrText, m_strText);

	return;
}

#ifdef _AFX
void CLED::GetNo(CString & cstrNo)
{
	cstrNo.Format(_T("%d"), m_byNo);

	return;
}

void CLED::GetWidth(CString & cstrWidth)
{
	cstrWidth.Format(_T("%d"), m_usWidth);

	return;
}

void CLED::GetHeight(CString & cstrHeight)
{
	cstrHeight.Format(_T("%d"), m_usHeight);

	return;
}

bool CLED::SetIP(CString cstrIP)
{
	std::string strIP = "";
	BufferString::CString_to_std_string(strIP, cstrIP);

	return SetIP(strIP);
}

void CLED::GetIP(CString & cstrIP)
{
	BufferString::std_string_to_CString(cstrIP, m_strIP);

	return;
}

void CLED::GetPort(CString & cstrPort)
{
	cstrPort.Format(_T("%d"), m_usPort);

	return;
}

bool CLED::SetText(CString cstrText)
{
	std::string strText = "";
	BufferString::CString_to_std_string(strText, cstrText);

	return SetText(strText);
}

void CLED::GetText(CString & cstrText)
{
	BufferString::std_string_to_CString(cstrText, m_strText);

	return;
}
#endif // !_AFX

#endif //!_LED_H