#include "stdafx.h"
#include "CCharger.h"
#include "Buffer.h"

#ifdef _CHARGER_H

CCharger::CCharger(unsigned char byNo, unsigned char byController, std::string strName)
{
	m_byNo = byNo;

#ifdef _WIN64
	m_ullChargeTime = 0;
#elif _WIN32
	m_ulChargeTime = 0;
#endif // !_WIN64
	m_ulMinDuration = 0;
	m_pUser = nullptr;
	m_byStatus = _CHARGERSTATUS_NONE;
	m_byController = byController;
	m_byPower = 0;

	m_strName = strName;
}

CCharger::CCharger(unsigned char byNo, unsigned char byController, std::wstring wstrName)
{
	m_byNo = byNo;

#ifdef _WIN64
	m_ullChargeTime = 0;
#elif _WIN32
	m_ulChargeTime = 0;
#endif // !_WIN64
	m_ulMinDuration = 0;
	m_pUser = nullptr;
	m_byStatus = _CHARGERSTATUS_NONE;
	m_byController = byController;
	m_byPower = 0;

	BufferString::std_wstring_to_string(m_strName, wstrName);
}

CCharger::CCharger(unsigned char byNo, unsigned char byController, CString cstrName)
{
	m_byNo = byNo;

#ifdef _WIN64
	m_ullChargeTime = 0;
#elif _WIN32
	m_ulChargeTime = 0;
#endif // !_WIN64
	m_ulMinDuration = 0;
	m_pUser = nullptr;
	m_byStatus = _CHARGERSTATUS_NONE;
	m_byController = byController;
	m_byPower = 0;

	BufferString::CString_to_std_string(m_strName, cstrName);
}


CCharger::~CCharger()
{
	m_pUser = nullptr;
}

bool CCharger::SetNo(unsigned char byNo)
{
	if (m_byNo == byNo)
	{
		return false;
	}

	m_byNo = byNo;

	return true;
}

unsigned char CCharger::GetNo() const
{
	return m_byNo;
}

bool CCharger::SetName(std::string strName)
{
	if (m_strName == strName)
	{
		return false;
	}

	m_strName = strName;

	return true;
}

bool CCharger::SetName(std::wstring wstrName)
{
	std::string strName = "";

	BufferString::std_wstring_to_string(strName, wstrName);

	return SetName(strName);
}

bool CCharger::SetName(CString cstrName)
{
	std::string strName = "";

	BufferString::CString_to_std_string(strName, cstrName);

	return SetName(strName);
}

void CCharger::GetName(std::string & strName)
{
	strName = m_strName;

	return;
}

void CCharger::GetName(std::wstring & wstrName)
{
	BufferString::std_string_to_wstring(wstrName, m_strName);

	return;
}

#ifdef _AFX
void CCharger::GetName(CString & cstrName)
{
	BufferString::std_string_to_CString(cstrName, m_strName);

	return;
}
#endif // !_AFX

void CCharger::StartCharging(unsigned long ulMinDuration, void* pUser)
{
	m_ulMinDuration = ulMinDuration;

#ifdef _WIN64
	if (m_ullChargeTime > 0)
	{
		return;
	}

	m_ullChargeTime = GetTickCount64();
#elif _WIN32
	if (m_ulChargeTime > 0)
	{
		return;
	}

	m_ulChargeTime = GetTickCount();
#endif // !_WIN64

	if (pUser)
	{
		m_pUser = pUser;
	}

	m_byStatus = _CHARGERSTATUS_USING;

	return;
}

void CCharger::EndCharging(void *pUser)
{
	m_ulMinDuration = 0;

#ifdef _WIN64
	m_ullChargeTime = 0;
#elif _WIN32
	m_ulChargeTime = 0;
#endif // !_WIN64

	if (m_pUser == pUser)
	{
		m_pUser = nullptr;
	}

	m_byStatus = _CHARGERSTATUS_FINISH;

	return;
}

void CCharger::Reset()
{
#ifdef _WIN64
	m_ullChargeTime = 0;
#elif _WIN32
	m_ulChargeTime = 0;
#endif // !_WIN64

	m_ulMinDuration = 0;

	m_pUser = nullptr;

	m_byStatus = _CHARGERSTATUS_NONE;

	return;
}

bool CCharger::Lock(void * pUser)
{
	if (m_pUser)
	{
		if(m_pUser != pUser)
		{
			return false;
		}
		else
		{
			return true;
		}
		
	}

	m_pUser = pUser;

	m_byStatus = _CHARGERSTATUS_USING;

	return true;
}

void CCharger::Unlock(void * pUser)
{
	if(m_pUser == pUser)
	{
		m_pUser = nullptr;
	}

	return;
}

#ifdef _WIN64
unsigned long long CCharger::GetStartTime()
{
	return m_ullChargeTime;
}
#elif _WIN32
unsigned long CCharger::GetStartTime()
{
	return m_ulChargeTime;
}
#endif // !_WIN64

#ifdef _WIN64
unsigned long long CCharger::GetDurativeTime()
{
	if (m_ullChargeTime > 0)
	{
		return GetTickCount64() - m_ullChargeTime;
	}
	
	return 0;
}
#elif _WIN32
unsigned long CCharger::GetDurativeTime()
{
	if (m_ulChargeTime > 0)
	{
		return GetTickCount() - m_ulChargeTime;
	}

	return 0;
}
#endif // !_WIN64

unsigned long CCharger::GetMinDurativeTime()
{
	return m_ulMinDuration;
}

unsigned char CCharger::GetStatus() const
{
	return m_byStatus;
}

void * CCharger::GetUser() const
{
	return m_pUser;
}

bool CCharger::SetController(unsigned char byController)
{
	if(m_byController == byController)
	{
		return false;
	}

	m_byController = byController;

	return true;
}

unsigned char CCharger::GetController() const
{
	return m_byController;
}

bool CCharger::SetPower(unsigned char byPower)
{
	if(m_byPower > 100)
	{
		return false;
	}

	if (m_byPower == byPower)
	{
		return false;
	}

	m_byPower = byPower;

	return true;
}

unsigned char CCharger::GetPower() const
{
	return m_byPower;
}

void CCharger::GetNo(std::string & strNo)
{
	BufferString::std_string_format(strNo, "%d", m_byNo);

	return;
}

void CCharger::GetNo(std::wstring & wstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_wstring(wstrNo, strNo);

	return;
}

void CCharger::GetDurativeTime(std::string & strTime)
{
	unsigned int unDay = 0;		/*!< 持续天数 */
	unsigned int unHour = 0;	/*!< 持续小时 */
	unsigned int unMin = 0;		/*!< 持续分钟 */
	unsigned int unSec = 0;		/*!< 持续秒 */
	unsigned int unMinSec = 0;	/*!< 持续毫秒 */

#ifdef _WIN64
	unsigned long long ullDurativeTime = 0;
	ullDurativeTime = GetDurativeTime();

	unDay = ullDurativeTime / ONEDAY;
	unHour = (ullDurativeTime % ONEDAY) / ONEHOUR;
	unMin = ((ullDurativeTime % ONEDAY) % ONEHOUR) / ONEMINUTE;
	unSec = (((ullDurativeTime % ONEDAY) % ONEHOUR) % ONEMINUTE) / ONESECOND;
	unMinSec = (((ullDurativeTime % ONEDAY) % ONEHOUR) % ONEMINUTE) % ONESECOND;
#elif _WIN32
	unsigned long ulDurativeTime = 0;
	ulDurativeTime = GetDurativeTime();

	unDay = ulDurativeTime / ONEDAY;
	unHour = (ulDurativeTime % ONEDAY) / ONEHOUR;
	unMin = ((ulDurativeTime % ONEDAY) % ONEHOUR) / ONEMINUTE;
	unSec = (((ulDurativeTime % ONEDAY) % ONEHOUR) % ONEMINUTE) / ONESECOND;
	unMinSec = (((ulDurativeTime % ONEDAY) % ONEHOUR) % ONEMINUTE) % ONESECOND;
#endif //!_WIN64

	BufferString::std_string_format(strTime, "%d天:%d小时:%d分:%d秒", unDay, unHour, unMin, unSec);

	return;
}

void CCharger::GetDurativeTime(std::wstring & wstrTime)
{
	std::string strTime = "";
	GetDurativeTime(strTime);

	BufferString::std_string_to_wstring(wstrTime, strTime);

	return;
}

void CCharger::GetMinDurativeTime(std::string & strTime)
{
	unsigned int unDay = 0;		/*!< 最短持续天数 */
	unsigned int unHour = 0;	/*!< 最短持续小时 */
	unsigned int unMin = 0;		/*!< 最短持续分钟 */
	unsigned int unSec = 0;		/*!< 最短持续秒 */
	unsigned int unMinSec = 0;	/*!< 最短持续毫秒 */

	unDay = m_ulMinDuration / ONEDAY;
	unHour = (m_ulMinDuration % ONEDAY) / ONEHOUR;
	unMin = ((m_ulMinDuration % ONEDAY) % ONEHOUR) / ONEMINUTE;
	unSec = (((m_ulMinDuration % ONEDAY) % ONEHOUR) % ONEMINUTE) / ONESECOND;
	unMinSec = (((m_ulMinDuration % ONEDAY) % ONEHOUR) % ONEMINUTE) % ONESECOND;

	BufferString::std_string_format(strTime, "%d天:%d小时:%d分:%d秒", unDay, unHour, unMin, unSec);

	return;
}

void CCharger::GetMinDurativeTime(std::wstring & wstrTime)
{
	std::string strTime = "";
	GetMinDurativeTime(strTime);

	BufferString::std_string_to_wstring(wstrTime, strTime);

	return;
}

void CCharger::GetStatus(std::string & strStatus)
{
	return GetStatus(m_byStatus, strStatus);;
}

void CCharger::GetStatus(std::wstring & wstrStatus)
{
	std::string strStatus = "";
	GetStatus(strStatus);

	BufferString::std_string_to_wstring(wstrStatus, strStatus);

	return;
}

void CCharger::GetController(std::string & strController)
{
	BufferString::std_string_format(strController, "%d", m_byController);

	return;
}

void CCharger::GetController(std::wstring & wstrController)
{
	std::string strController = "";
	GetController(strController);

	BufferString::std_string_to_wstring(wstrController, strController);

	return;
}

void CCharger::GetPower(std::string & strPower)
{
	BufferString::std_string_format(strPower, "%d%%", m_byPower);

	return;
}

void CCharger::GetPower(std::wstring & wstrPower)
{
	std::string strPower = "";
	GetPower(strPower);

	BufferString::std_string_to_wstring(wstrPower, strPower);

	return;
}

unsigned char CCharger::GetStatusCode(std::string strStatus)
{
	if (strStatus == "未使用")
	{
		return _CHARGERSTATUS_NONE;
	}
	else if (strStatus == "使用中")
	{
		return _CHARGERSTATUS_USING;
	}
	else if (strStatus == "使用完成")
	{
		return _CHARGERSTATUS_FINISH;
	}

	return 0xFF;
}

unsigned char CCharger::GetStatusCode(std::wstring wstrStatus)
{
	std::string strStatus = "";
	BufferString::std_wstring_to_string(strStatus, wstrStatus);

	return GetStatusCode(strStatus);
}

void CCharger::GetStatus(unsigned char byStatus, std::string & strStatus)
{
	switch (byStatus)
	{
	case _CHARGERSTATUS_NONE:
		strStatus = "未使用";
		break;
	case _CHARGERSTATUS_USING:
		strStatus = "使用中";
		break;
	case _CHARGERSTATUS_FINISH:
		strStatus = "使用完成";
		break;
	}

	return;
}

void CCharger::GetStatus(unsigned char byStatus, std::wstring & wstrStatus)
{
	std::string strStatus = "";
	GetStatus(byStatus, strStatus);

	BufferString::std_string_to_wstring(wstrStatus, strStatus);

	return;
}

#ifdef _AFX
void CCharger::GetNo(CString & cstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_CString(cstrNo, strNo);

	return;
}

void CCharger::GetDurativeTime(CString & cstrTime)
{
	std::string strTime = "";
	GetDurativeTime(strTime);

	BufferString::std_string_to_CString(cstrTime, strTime);

	return;
}

void CCharger::GetMinDurativeTime(CString & cstrTime)
{
	std::string strTime = "";
	GetMinDurativeTime(strTime);

	BufferString::std_string_to_CString(cstrTime, strTime);

	return;
}

void CCharger::GetStatus(CString & cstrStatus, COLORREF & color)
{
	std::string strStatus = "";
	GetStatus(strStatus);

	BufferString::std_string_to_CString(cstrStatus, strStatus);

	switch (m_byStatus)
	{
	case _CHARGERSTATUS_NONE:
		color = COLOR_GREEN;
		break;
	case _CHARGERSTATUS_USING:
		color = COLOR_YELLOW;
		break;
	case _CHARGERSTATUS_FINISH:
		color = COLOR_RED;
		break;
	}

	return;
}

void CCharger::GetController(CString & cstrController)
{
	std::string strController = "";
	GetController(strController);

	BufferString::std_string_to_CString(cstrController, strController);

	return;
}

void CCharger::GetPower(CString & cstrPower)
{
	std::string strPower = "";
	GetPower(strPower);

	BufferString::std_string_to_CString(cstrPower, strPower);

	return;
}

unsigned char CCharger::GetStatusCode(CString cstrStatus)
{
	std::string strStatus = "";
	BufferString::CString_to_std_string(strStatus, cstrStatus);

	return GetStatusCode(strStatus);
}

void CCharger::GetStatus(unsigned char byStatus, CString & cstrStatus)
{
	std::string strStatus = "";
	GetStatus(byStatus, strStatus);

	BufferString::std_string_to_CString(cstrStatus, strStatus);

	return;
}

#endif //!_AFX

#endif // !_CHARGER_H