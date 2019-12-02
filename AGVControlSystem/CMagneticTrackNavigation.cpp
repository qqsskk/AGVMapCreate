#include "stdafx.h"
#include "CMagneticTrackNavigation.h"

#ifdef _MAGNETICTRACK_H

/*----------------------------------CRFID--------------------------------------*/
CRFID::CRFID(unsigned short usNo)
{
	m_usNo = usNo;
	m_pUser = nullptr;
#ifdef _WIN64
	m_ullUseTime = 0;
#elif _WIN32
	m_ulUseTime = 0;
#endif //  _WIN64
	m_pLocker = nullptr;
}

CRFID::CRFID(const CRFID & rfid)
{
	m_usNo = rfid.m_usNo;
	m_pUser = rfid.m_pUser;
#ifdef _WIN64
	m_ullUseTime = rfid.m_ullUseTime;
#elif _WIN32
	m_ulUseTime = rfid.m_ulUseTime;
#endif //  _WIN64
	m_pLocker = rfid.m_pLocker;
}

CRFID::~CRFID()
{
	m_usNo = 0;
	m_pUser = nullptr;
#ifdef _WIN64
	m_ullUseTime = 0;
#elif _WIN32
	m_ulUseTime = 0;
#endif //  _WIN64
	m_pLocker = nullptr;
}

unsigned short CRFID::GetNo() const
{
	return m_usNo;
}

bool CRFID::SetNo(unsigned short usNo)
{
	if(m_usNo == usNo)
	{
		return false;
	}

	m_usNo = usNo;

	return true;
}

void* CRFID::Use(void * pUser)
{
	if (m_pUser == nullptr)
	{
		m_pUser = pUser;
#ifdef _WIN64
		m_ullUseTime = GetTickCount64();
#elif _WIN32
		m_ulUseTime = GetTickCount();
#endif //  _WIN64
	}

	if (m_pLocker == m_pUser)
	{
		Unlock(m_pLocker);
	}

	return m_pUser;
}

void CRFID::Finish(void * pUser)
{
	if(m_pUser == pUser)
	{
#ifdef _WIN64
		m_ullUseTime = 0;
#elif _WIN32
		m_ulUseTime = 0;
#endif //  _WIN64
		m_pUser = nullptr;
	}

	return;
}

void * CRFID::GetUser() const
{
	return m_pUser;
}

#ifdef  _WIN64
void * CRFID::GetUser(unsigned long long & ullUseTime) const
{
	ullUseTime = m_ullUseTime;
	return m_pUser;
}
#elif _WIN32
void * CRFID::GetUser(unsigned long & ulUseTime) const
{
	ulUseTime = m_ulUseTime;
	return m_pUser;
}
#endif //  _WIN64

void * CRFID::Lock(void * pLocker)
{
	if (m_pLocker == nullptr)
	{
		m_pLocker = pLocker;
	}

	return m_pLocker;
}

void CRFID::Unlock(void * pLocker)
{
	if (m_pLocker == pLocker)
	{
		m_pLocker = nullptr;
	}

	return;
}

void * CRFID::GetLocker() const
{
	return m_pLocker;
}

void CRFID::operator=(const CRFID & rfid)
{
	m_usNo = rfid.m_usNo;
	m_pUser = rfid.m_pUser;
#ifdef _WIN64
	m_ullUseTime = rfid.m_ullUseTime;
#elif _WIN32
	m_ulUseTime = rfid.m_ulUseTime;
#endif //  _WIN64
	m_pLocker = rfid.m_pLocker;
}

/*----------------------------------CMagneticTrackLocation--------------------------------------*/
CMagneticTrackLocation::CMagneticTrackLocation(unsigned short usRFID)
{
	m_usLocateRFID = usRFID;
}

CMagneticTrackLocation::~CMagneticTrackLocation()
{
	m_usLocateRFID = 0;
}

bool CMagneticTrackLocation::SetLocation(unsigned short usRFID)
{
	if(m_usLocateRFID == usRFID)
	{
		return false;
	}

	m_usLocateRFID = usRFID;

	return true;
}

unsigned short CMagneticTrackLocation::GetLocation() const
{
	return m_usLocateRFID;
}

void CMagneticTrackLocation::GetLocation(std::string & strRFID)
{
	BufferString::std_string_format(strRFID, "%d", m_usLocateRFID);

	return;
}

void CMagneticTrackLocation::GetLocation(std::wstring & wstrRFID)
{
	std::string strRFID = "";
	GetLocation(strRFID);

	BufferString::std_string_to_wstring(wstrRFID, strRFID);

	return;
}

#ifdef _AFX
void CMagneticTrackLocation::GetLocation(CString & cstrRFID)
{
	std::string strRFID = "";
	GetLocation(strRFID);

	BufferString::std_string_to_CString(cstrRFID, strRFID);

	return;
}
#endif // !_AFX

/*----------------------------------CMagneticTrackNavigation--------------------------------------*/
CMagneticTrackNavigation::CMagneticTrackNavigation()
{
	m_usCurRFID = 0;
	m_usOldRFID = 0;
	m_usNextRFID = 0;
	m_usEndRFID = 0;
}


CMagneticTrackNavigation::~CMagneticTrackNavigation()
{
	m_listOldRFID.clear();
}

bool CMagneticTrackNavigation::SetCurRFID(unsigned short usRFID)
{
	if (m_usCurRFID == usRFID)
	{
		return false;
	}

	m_usOldRFID = m_usCurRFID;
	m_usCurRFID = usRFID;

	if (m_usCurRFID == m_usEndRFID)
	{
		m_listOldRFID.clear();
	}

	m_listOldRFID.push_back(m_usOldRFID);

	return true;
}

bool CMagneticTrackNavigation::SetCurRFID(unsigned short usRFID, unsigned short & usOldRFID)
{
	if (SetCurRFID(usRFID))
	{
		usOldRFID = m_usOldRFID;

		return true;
	}
	
	return false;
}

unsigned short CMagneticTrackNavigation::GetCurRFID() const
{
	return m_usCurRFID;
}

unsigned short CMagneticTrackNavigation::GetOldRFID() const
{
	return m_usOldRFID;
}

bool CMagneticTrackNavigation::SetNextRFID(unsigned short usRFID)
{
	if (m_usNextRFID == usRFID)
	{
		return false;
	}

	m_usNextRFID = usRFID;

	return true;
}

unsigned short CMagneticTrackNavigation::GetNextRFID() const
{
	return m_usNextRFID;
}

bool CMagneticTrackNavigation::SetEndRFID(unsigned short usRFID)
{
	if (m_usEndRFID == usRFID)
	{
		return false;
	}

	m_usEndRFID = usRFID;

	return true;
}

unsigned short CMagneticTrackNavigation::GetEndRFID() const
{
	return m_usEndRFID;
}

std::list<unsigned short> CMagneticTrackNavigation::GetOldRFIDList() const
{
	return m_listOldRFID;
}

void CMagneticTrackNavigation::GetCurRFID(std::string & strRFID)
{
	BufferString::std_string_format(strRFID, "%d", m_usCurRFID);

	return;
}

void CMagneticTrackNavigation::GetCurRFID(std::wstring & wstrRFID)
{
	std::string strRFID = "";
	GetCurRFID(strRFID);

	BufferString::std_string_to_wstring(wstrRFID, strRFID);

	return;
}

void CMagneticTrackNavigation::GetOldRFID(std::string & strRFID)
{
	BufferString::std_string_format(strRFID, "%d", m_usOldRFID);

	return;
}

void CMagneticTrackNavigation::GetOldRFID(std::wstring & wstrRFID)
{
	std::string strRFID = "";
	GetOldRFID(strRFID);

	BufferString::std_string_to_wstring(wstrRFID, strRFID);

	return;
}

void CMagneticTrackNavigation::GetNextRFID(std::string & strRFID)
{
	BufferString::std_string_format(strRFID, "%d", m_usNextRFID);

	return;
}

void CMagneticTrackNavigation::GetNextRFID(std::wstring & wstrRFID)
{
	std::string strRFID = "";
	GetNextRFID(strRFID);

	BufferString::std_string_to_wstring(wstrRFID, strRFID);

	return;
}

void CMagneticTrackNavigation::GetEndRFID(std::string & strRFID)
{
	BufferString::std_string_format(strRFID, "%d", m_usEndRFID);

	return;
}

void CMagneticTrackNavigation::GetEndRFID(std::wstring & wstrRFID)
{
	std::string strRFID = "";
	GetEndRFID(strRFID);

	BufferString::std_string_to_wstring(wstrRFID, strRFID);

	return;
}

void CMagneticTrackNavigation::GetOldRFIDList(std::string & strList)
{
	strList = "";

	for (std::list<unsigned short>::iterator it = m_listOldRFID.begin(); it != m_listOldRFID.end(); it++)
	{
		std::string strRFID = "";

		if (it == m_listOldRFID.begin())
		{
			BufferString::std_string_format(strRFID, "%d", *it);
		}
		else
		{
			BufferString::std_string_format(strRFID, ",%d", *it);
		}

		strList += strRFID;
	}

	return;
}

void CMagneticTrackNavigation::GetOldRFIDList(std::wstring & wstrList)
{
	std::string strList = "";
	GetOldRFIDList(strList);

	BufferString::std_string_to_wstring(wstrList, strList);

	return;
}

#ifdef _AFX
void CMagneticTrackNavigation::GetCurRFID(CString & cstrRFID)
{
	std::string strRFID = "";
	GetCurRFID(strRFID);

	BufferString::std_string_to_CString(cstrRFID, strRFID);

	return;
}

void CMagneticTrackNavigation::GetOldRFID(CString & cstrRFID)
{
	std::string strRFID = "";
	GetOldRFID(strRFID);

	BufferString::std_string_to_CString(cstrRFID, strRFID);

	return;
}

void CMagneticTrackNavigation::GetNextRFID(CString & cstrRFID)
{
	std::string strRFID = "";
	GetNextRFID(strRFID);

	BufferString::std_string_to_CString(cstrRFID, strRFID);

	return;
}

void CMagneticTrackNavigation::GetEndRFID(CString & cstrRFID)
{
	std::string strRFID = "";
	GetEndRFID(strRFID);

	BufferString::std_string_to_CString(cstrRFID, strRFID);

	return;
}

void CMagneticTrackNavigation::GetOldRFIDList(CString & cstrList)
{
	std::string strList = "";
	GetOldRFIDList(strList);

	BufferString::std_string_to_CString(cstrList, strList);

	return;
}
#endif //!_AFX

/*----------------------------------CMagneticTrackTrafficControlArea--------------------------------------*/
CMagneticTrackTrafficControlArea::CMagneticTrackTrafficControlArea()
{
	m_usBefore = 0;
	m_usCtrl = 0;
}

CMagneticTrackTrafficControlArea::CMagneticTrackTrafficControlArea(unsigned short usCtrl, unsigned short usBefore)
{
	m_usBefore = usBefore;
	m_usCtrl = usCtrl;
}

CMagneticTrackTrafficControlArea::CMagneticTrackTrafficControlArea(unsigned short usCtrl, std::set<unsigned short> setStop, std::set<unsigned short> setComparison, unsigned short usBefore)
{
	m_usBefore = usBefore;
	m_usCtrl = usCtrl;
	m_setStop = setStop;
	m_setComparison = setComparison;
}

CMagneticTrackTrafficControlArea::CMagneticTrackTrafficControlArea(const CMagneticTrackTrafficControlArea & area)
{
	m_usBefore = area.m_usBefore;
	m_usCtrl = area.m_usCtrl;
	m_setStop = area.m_setStop;
	m_setComparison = area.m_setComparison;
}

CMagneticTrackTrafficControlArea::~CMagneticTrackTrafficControlArea()
{
	m_setStop.clear();
	m_setComparison.clear();
}

bool CMagneticTrackTrafficControlArea::SetBeforeRFID(unsigned short usRFID)
{
	if (m_usBefore == usRFID)
	{
		return false;
	}

	m_usBefore = usRFID;

	return true;
}

unsigned short CMagneticTrackTrafficControlArea::GetBeforeRFID() const
{
	return m_usBefore;
}

bool CMagneticTrackTrafficControlArea::SetCtrlRFID(unsigned short usRFID)
{
	if (m_usCtrl == usRFID)
	{
		return false;
	}

	m_usCtrl = usRFID;

	return true;
}

unsigned short CMagneticTrackTrafficControlArea::GetCtrlRFID() const
{
	return m_usCtrl;
}

bool CMagneticTrackTrafficControlArea::AddStopRFID(unsigned short usRFID)
{
	if (m_setStop.find(usRFID) != m_setStop.end())
	{
		return false;
	}

	m_setStop.insert(usRFID);

	return true;
}

void CMagneticTrackTrafficControlArea::DeleteStopRFID(unsigned short usRFID)
{
	if (m_setStop.find(usRFID) != m_setStop.end())
	{
		m_setStop.erase(m_setStop.find(usRFID));
	}

	return;
}

std::set<unsigned short> CMagneticTrackTrafficControlArea::GetStopRFIDList() const
{
	return m_setStop;
}

bool CMagneticTrackTrafficControlArea::AddCompareArea(unsigned short usAreaCtrl)
{
	if (m_setComparison.find(usAreaCtrl) != m_setComparison.end())
	{
		return false;
	}

	m_setComparison.insert(usAreaCtrl);

	return false;
}

void CMagneticTrackTrafficControlArea::DeleteCompareArea(unsigned short usAreaCtrl)
{
	if (m_setComparison.find(usAreaCtrl) != m_setComparison.end())
	{
		m_setComparison.erase(m_setComparison.find(usAreaCtrl));
	}

	return;
}

std::set<unsigned short> CMagneticTrackTrafficControlArea::GetCompareAreaList() const
{
	return m_setComparison;
}

void CMagneticTrackTrafficControlArea::operator=(const CMagneticTrackTrafficControlArea & area)
{
	m_usBefore = area.m_usBefore;
	m_usCtrl = area.m_usCtrl;
	m_setStop = area.m_setStop;
	m_setComparison = area.m_setComparison;
}

void CMagneticTrackTrafficControlArea::GetBeforeRFID(std::string & strRFID)
{
	BufferString::std_string_format(strRFID, "%d", m_usBefore);

	return;
}

void CMagneticTrackTrafficControlArea::GetBeforeRFID(std::wstring & wstrRFID)
{
	std::string strRFID = "";
	GetBeforeRFID(strRFID);

	BufferString::std_string_to_wstring(wstrRFID, strRFID);

	return;
}

void CMagneticTrackTrafficControlArea::GetCtrlRFID(std::string & strRFID)
{
	BufferString::std_string_format(strRFID, "%d", m_usCtrl);

	return;
}

void CMagneticTrackTrafficControlArea::GetCtrlRFID(std::wstring & wstrRFID)
{
	std::string strRFID = "";
	GetCtrlRFID(strRFID);

	BufferString::std_string_to_wstring(wstrRFID, strRFID);

	return;
}

void CMagneticTrackTrafficControlArea::GetStopRFIDList(std::string & strRFIDList)
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

void CMagneticTrackTrafficControlArea::GetStopRFIDList(std::wstring & wstrRFIDList)
{
	std::string strRFIDList = "";
	GetStopRFIDList(strRFIDList);

	BufferString::std_string_to_wstring(wstrRFIDList, strRFIDList);

	return;
}

void CMagneticTrackTrafficControlArea::SetStopRFIDList(std::string strRFIDList)
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
	}
	while (true);

	return;
}

void CMagneticTrackTrafficControlArea::SetStopRFIDList(std::wstring wstrRFIDList)
{
	std::string strList = "";
	BufferString::std_wstring_to_string(strList, wstrRFIDList);

	SetStopRFIDList(strList);

	return;
}

void CMagneticTrackTrafficControlArea::GetCompareAreaList(std::string & strCompareList)
{
	strCompareList = "";

	for (std::set<unsigned short>::iterator it = m_setComparison.begin(); it != m_setComparison.end(); it++)
	{
		std::string strCompare = "";

		if (it == m_setComparison.begin())
		{
			BufferString::std_string_format(strCompare, "%d", *it);
		}
		else
		{
			BufferString::std_string_format(strCompare, ",%d", *it);
		}

		strCompareList += strCompare;
	}

	return;
}

void CMagneticTrackTrafficControlArea::GetCompareAreaList(std::wstring & wstrCompareList)
{
	std::string strCompareList = "";
	GetCompareAreaList(strCompareList);

	BufferString::std_string_to_wstring(wstrCompareList, strCompareList);

	return;
}

void CMagneticTrackTrafficControlArea::SetCompareAreaList(std::string strCompareList)
{
	// 将，置换为,
	std::string strList = strCompareList;	/*!< 置换后的字符串 */
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

		std::string strArea = strList.substr(nPost, nNext - nPost);

		AddCompareArea(BufferString::std_string_to_int(strArea));

		nPost = nNext + 1;
	} while (true);

	return;
}

void CMagneticTrackTrafficControlArea::SetCompareAreaList(std::wstring wstrCompareList)
{
	std::string strList = "";
	BufferString::std_wstring_to_string(strList, wstrCompareList);

	SetCompareAreaList(strList);

	return;
}

#ifdef _AFX
void CMagneticTrackTrafficControlArea::GetBeforeRFID(CString & cstrRFID)
{
	std::string strRFID = "";
	GetBeforeRFID(strRFID);

	BufferString::std_string_to_CString(cstrRFID, strRFID);

	return;
}

void CMagneticTrackTrafficControlArea::GetCtrlRFID(CString & cstrRFID)
{
	std::string strRFID = "";
	GetCtrlRFID(strRFID);

	BufferString::std_string_to_CString(cstrRFID, strRFID);

	return;
}

void CMagneticTrackTrafficControlArea::GetStopRFIDList(CString & cstrRFIDList)
{
	std::string strList = "";
	GetStopRFIDList(strList);

	BufferString::std_string_to_CString(cstrRFIDList, strList);

	return;
}

void CMagneticTrackTrafficControlArea::SetStopRFIDList(CString cstrRFIDList)
{
	std::string strList = "";
	BufferString::CString_to_std_string(strList, cstrRFIDList);

	SetStopRFIDList(strList);

	return;
}

void CMagneticTrackTrafficControlArea::GetCompareAreaList(CString & cstrCompareList)
{
	std::string strList = "";
	GetCompareAreaList(strList);

	BufferString::std_string_to_CString(cstrCompareList, strList);

	return;
}

void CMagneticTrackTrafficControlArea::SetCompareAreaList(CString cstrCompareList)
{
	std::string strList = "";
	BufferString::CString_to_std_string(strList, cstrCompareList);

	SetCompareAreaList(strList);

	return;
}
#endif // !_AFX

#endif //_MAGNTICTRACK_H