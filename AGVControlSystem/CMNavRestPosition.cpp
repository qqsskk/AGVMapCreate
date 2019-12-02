#include "stdafx.h"
#include "CMNavRestPosition.h"

#ifdef _RESTPOSITION_H

CMNavRestPosition::CMNavRestPosition(unsigned char byNo, unsigned short usLocation, std::string strName) :CRestPosition(byNo, 0,strName), CMagneticTrackLocation(usLocation)
{
}

CMNavRestPosition::CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::string strName) : CRestPosition(byNo, byNext, strName), CMagneticTrackLocation(usLocation)
{
}

CMNavRestPosition::CMNavRestPosition(unsigned char byNo, unsigned short usLocation, std::set<unsigned short> setStopList, std::string strName) : CRestPosition(byNo, 0,strName), CMagneticTrackLocation(usLocation)
{
	m_setStop = setStopList;
}

CMNavRestPosition::CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::set<unsigned short> setStopList, std::string strName) : CRestPosition(byNo, byNext, strName), CMagneticTrackLocation(usLocation)
{
	m_setStop = setStopList;
}

CMNavRestPosition::CMNavRestPosition(unsigned char byNo, unsigned short usLocation, std::wstring wstrName) : CRestPosition(byNo,0, wstrName), CMagneticTrackLocation(usLocation)
{
}

CMNavRestPosition::CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::wstring wstrName) : CRestPosition(byNo, byNext, wstrName), CMagneticTrackLocation(usLocation)
{
}

CMNavRestPosition::CMNavRestPosition(unsigned char byNo, unsigned short usLocation, std::set<unsigned short> setStopList, std::wstring wstrName) : CRestPosition(byNo, 0, wstrName), CMagneticTrackLocation(usLocation)
{
	m_setStop = setStopList;
}

CMNavRestPosition::CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::set<unsigned short> setStopList, std::wstring wstrName) : CRestPosition(byNo, byNext, wstrName), CMagneticTrackLocation(usLocation)
{
	m_setStop = setStopList;
}

#ifdef _AFX
CMNavRestPosition::CMNavRestPosition(unsigned char byNo, unsigned short usLocation, CString cstrName) : CRestPosition(byNo, 0, cstrName), CMagneticTrackLocation(usLocation)
{
}

CMNavRestPosition::CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, CString cstrName) : CRestPosition(byNo, byNext, cstrName), CMagneticTrackLocation(usLocation)
{
}

CMNavRestPosition::CMNavRestPosition(unsigned char byNo, unsigned short usLocation, std::set<unsigned short> setStopList, CString cstrName) : CRestPosition(byNo, 0, cstrName), CMagneticTrackLocation(usLocation)
{
	m_setStop = setStopList;
}

CMNavRestPosition::CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::set<unsigned short> setStopList, CString cstrName) : CRestPosition(byNo, byNext, cstrName), CMagneticTrackLocation(usLocation)
{
	m_setStop = setStopList;
}
#endif // _AFX


CMNavRestPosition::CMNavRestPosition(const CMNavRestPosition & rest):CRestPosition(rest.m_byNo, rest.m_byNext, rest.m_strName), CMagneticTrackLocation(rest.m_usLocateRFID)
{
	m_setStop = rest.m_setStop;
}

CMNavRestPosition::~CMNavRestPosition()
{
}

void CMNavRestPosition::operator=(const CMNavRestPosition & rest)
{
	m_byNo = rest.m_byNo;
	m_byNext = rest.m_byNext;
	m_setStop = rest.m_setStop;

	return;
}

bool CMNavRestPosition::AddStopRFID(unsigned short usRFID)
{
	if (m_setStop.find(usRFID) != m_setStop.end())
	{
		return false;
	}

	m_setStop.insert(usRFID);

	return true;
}

void CMNavRestPosition::DeleteStopRFID(unsigned short usRFID)
{
	if (m_setStop.find(usRFID) != m_setStop.end())
	{
		m_setStop.erase(m_setStop.find(usRFID));
	}

	return;
}

std::set<unsigned short> CMNavRestPosition::GetStopRFIDList() const
{
	return m_setStop;
}


void CMNavRestPosition::GetStopRFIDList(std::string & strRFIDList)
{
	strRFIDList = "";

	for (std::set<unsigned short>::iterator it = m_setStop.begin(); it != m_setStop.end(); it++)
	{
		std::string strRFID = "";

		if (it == m_setStop.begin())
		{
			BufferString::std_string_format(strRFID, "%d", *it);
		}
		else
		{
			BufferString::std_string_format(strRFID, ",%d", *it);
		}

		strRFIDList += strRFID;
	}

	return;
}

void CMNavRestPosition::GetStopRFIDList(std::wstring & wstrRFIDList)
{
	std::string strRFIDList = "";
	GetStopRFIDList(strRFIDList);

	BufferString::std_string_to_wstring(wstrRFIDList, strRFIDList);

	return;
}

void CMNavRestPosition::SetStopRFIDList(std::string strRFIDList)
{
	// 将，置换为,
	std::string strList = strRFIDList;	/*!< 置换后的字符串 */
	BufferString::std_string_replace(strList, '，', ',');

	// 查找进行RFID卡
	int nPost = 0;	/*!< 查找位置 */
	int nNext = 0;	/*!< ,所在位置 */
	do
	{
		nNext = strList.find(',', nPost);

		if (nNext == -1)
		{
			break;
		}

		std::string strRFID = strList.substr(nPost, nNext - nPost);

		AddStopRFID(BufferString::std_string_to_int(strRFID));

		nPost = nNext + 1;
	} while (true);

	return;
}

void CMNavRestPosition::SetStopRFIDList(std::wstring wstrRFIDList)
{
	std::string strList = "";
	BufferString::std_wstring_to_string(strList, wstrRFIDList);

	SetStopRFIDList(strList);

	return;
}


#ifdef _AFX

void CMNavRestPosition::GetStopRFIDList(CString & cstrRFIDList)
{
	std::string strList = "";
	GetStopRFIDList(strList);

	BufferString::std_string_to_CString(cstrRFIDList, strList);

	return;
}

void CMNavRestPosition::SetStopRFIDList(CString cstrRFIDList)
{
	std::string strList = "";
	BufferString::CString_to_std_string(strList, cstrRFIDList);

	SetStopRFIDList(strList);

	return;
}
#endif // !_AFX

#endif // !_RESTPOSITION_H