#include "stdafx.h"
#include "CAGV.h"

#ifdef _AGV_H

CAGV::CAGV(unsigned char byNo, unsigned char byType, unsigned char byDriver, float fMaxSepped, bool bAuto)
{
	m_byNo = byNo;
	m_byType = byType;
	m_byDriver = byDriver;
	m_fMaxSpeed = fMaxSepped;

	m_byPower = 0;
	m_chSpeed = 0;
	m_byStatus = _AGVSTA_WAIT;
	m_chError = _AGVERR_NONE;
	m_usAction = _AGVACT_NONE;
	m_byActionStatus = _AGVACTSTA_NONE;
	m_byCargo = 0;
	m_bAuto = bAuto;
	m_pTask = nullptr;
}

CAGV::~CAGV()
{
	m_pTask = nullptr;
}

bool CAGV::SetNo(unsigned char byNo)
{
	if (m_byNo == byNo)
	{
		return false;
	}

	m_byNo = byNo;
	
	return true;
}

unsigned char CAGV::GetNo() const
{
	return m_byNo;
}

bool CAGV::SetType(unsigned char byType)
{
	if (m_byType == byType)
	{
		return false;
	}

	m_byType = byType;

	return true;
}

unsigned char CAGV::GetType() const
{
	return m_byType;
}

bool CAGV::SetDriver(unsigned char byDriver)
{
	if (m_byDriver == byDriver)
	{
		return false;
	}

	m_byDriver = byDriver;

	return true;
}

unsigned char CAGV::GetDriver() const
{
	return m_byDriver;
}

bool CAGV::SetMaxSpeed(float fMaxSpeed)
{
	if (m_fMaxSpeed != fMaxSpeed)
	{
		return false;
	}

	m_fMaxSpeed = fMaxSpeed;

	return true;
}

float CAGV::GetMaxSpeed() const
{
	return m_fMaxSpeed;
}

bool CAGV::SetPower(unsigned char byPower)
{
	if (m_byPower == byPower)
	{
		return false;
	}

	m_byPower = byPower;

	return true;
}

unsigned char CAGV::GetPower() const
{
	return m_byPower;
}

bool CAGV::SetSpeed(char chSpeed)
{
	if (m_chSpeed == chSpeed)
	{
		return false;
	}

	m_chSpeed = chSpeed;

	return true;
}

char CAGV::GetSpeed() const
{
	return m_chSpeed;
}

float CAGV::GetRealitySpeed() const
{
	return (float)m_chSpeed * m_fMaxSpeed;
}

bool CAGV::SetStatus(unsigned char byStatus)
{
	if (m_byStatus == byStatus)
	{
		return false;
	}

	m_byStatus = byStatus;

	return false;
}

unsigned char CAGV::GetStatus() const
{
	return m_byStatus;
}

bool CAGV::SetAction(unsigned short usAction, unsigned char byStatus)
{
	if (m_usAction != usAction || m_byStatus != byStatus)
	{
		m_usAction = usAction;
		m_byActionStatus = byStatus;

		return true;
	}
	return false;
}

void CAGV::GetAction(unsigned short & usAction, unsigned char & byStatus)
{
	usAction = m_usAction;
	byStatus = m_byActionStatus;

	return;
}

bool CAGV::SetError(char chError)
{
	if(m_chError == chError)
	{
		return false;
	}

	m_chError = chError;

	return true;
}

char CAGV::GetError() const
{
	return m_chError;
}

void CAGV::DownloadTask(void * pTask)
{
	m_pTask = pTask;

	return;
}

void CAGV::ClearTask()
{
	m_pTask = nullptr;

	return;
}

void * CAGV::GetTask() const
{
	return m_pTask;
}

bool CAGV::SetMode(bool bAuto)
{
	if (m_bAuto == bAuto)
	{
		return false;
	}

	m_bAuto = bAuto;

	return true;
}

bool CAGV::GetMode() const
{
	return m_bAuto;
}

bool CAGV::SetCargo(unsigned char byCargo)
{
	if (m_byCargo == byCargo)
	{
		return false;
	}

	m_byCargo = byCargo;

	return true;
}

unsigned char CAGV::GetCargo() const
{
	return m_byCargo;
}

void CAGV::GetNo(std::string & strNo)
{
	BufferString::std_string_format(strNo, "%d", m_byNo);

	return;
}

void CAGV::GetNo(std::wstring & wstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_wstring(wstrNo,strNo);

	return;
}

void CAGV::GetType(std::string & strType)
{
	return GetType(m_byType,strType);
}

void CAGV::GetType(std::wstring & wstrType)
{
	std::string strType = "";
	GetType(strType);

	BufferString::std_string_to_wstring(wstrType, strType);

	return;
}

void CAGV::GetDriver(std::string & strDriver)
{
	return GetDrive(m_byDriver, strDriver);
}

void CAGV::GetDriver(std::wstring & wstrDriver)
{
	std::string strDriver = "";
	GetDriver(strDriver);

	BufferString::std_string_to_wstring(wstrDriver, strDriver);

	return;
}

void CAGV::GetMaxSpeed(std::string & strSpeed)
{
	BufferString::std_string_format(strSpeed, "%3.2fm/min", m_fMaxSpeed);

	return;
}

void CAGV::GetMaxSpeed(std::wstring & wstrSpeed)
{
	std::string strSpeed = "";
	GetMaxSpeed(strSpeed);

	BufferString::std_string_to_wstring(wstrSpeed, strSpeed);

	return;
}

void CAGV::GetPower(std::string & strPower)
{
	BufferString::std_string_format(strPower, "%d%%", m_byPower);

	return;
}

void CAGV::GetPower(std::wstring & wstrPower)
{
	std::string strPower = "";
	GetPower(strPower);

	BufferString::std_string_to_wstring(wstrPower, strPower);

	return;
}

void CAGV::GetSpeed(std::string & strSpeed)
{
	BufferString::std_string_format(strSpeed, "%d%%", m_chSpeed);

	return;
}

void CAGV::GetSpeed(std::wstring & wstrSpeed)
{
	std::string strSpeed = "";
	GetSpeed(strSpeed);

	BufferString::std_string_to_wstring(wstrSpeed, strSpeed);

	return;
}

void CAGV::GetRealitySpeed(std::string & strSpeed)
{
	BufferString::std_string_format(strSpeed, "%3.2fm/min", GetRealitySpeed());

	return;
}

void CAGV::GetRealitySpeed(std::wstring & wstrSpeed)
{
	std::string strSpeed = "";
	GetRealitySpeed(strSpeed);

	BufferString::std_string_to_wstring(wstrSpeed, strSpeed);

	return;
}

void CAGV::GetStatus(std::string & strStatus)
{
	GetStatus(m_byStatus, strStatus);

	return;
}

void CAGV::GetStatus(std::wstring & wstrStatus)
{
	std::string strStatus = "";
	GetStatus(strStatus);

	BufferString::std_string_to_wstring(wstrStatus, strStatus);

	return;
}

void CAGV::GetAction(std::string & strAction, std::string & strStatus)
{
	GetAction(m_usAction, strAction);

	GetActionStatus(m_byActionStatus, strStatus);

	return;
}

void CAGV::GetAction(std::wstring & wstrAction, std::wstring & wstrStatus)
{
	std::string strAction = "";
	std::string strStatus = "";
	GetAction(strAction,strStatus);

	BufferString::std_string_to_wstring(wstrAction, strAction);

	BufferString::std_string_to_wstring(wstrStatus, strStatus);

	return;
}

void CAGV::GetError(std::string & strError)
{
	return GetErrorMessage(m_chError, strError);
}

void CAGV::GetError(std::wstring & wstrError)
{
	std::string strError = "";
	GetError(strError);

	BufferString::std_string_to_wstring(wstrError, strError);

	return;
}

void CAGV::GetMode(std::string & strMode)
{
	if (m_bAuto)
	{
		strMode = "自动模式";
	}
	else
	{
		strMode = "手动模式";
	}

	return;
}

void CAGV::GetMode(std::wstring & wstrMode)
{
	std::string strMode = "";
	GetMode(strMode);

	BufferString::std_string_to_wstring(wstrMode, strMode);

	return;
}

void CAGV::GetCargo(std::string & strCargo)
{
	switch (m_byCargo)
	{
	case 0:
		strCargo = "空载";
		break;
	case 0xFF:
		strCargo = "满载";
		break;
	default:
		BufferString::std_string_format(strCargo, "%d", m_byCargo);
		break;
	}

	return;
}

void CAGV::GetCargo(std::wstring & wstrCargo)
{
	std::string strCargo = "";
	GetCargo(strCargo);

	BufferString::std_string_to_wstring(wstrCargo, strCargo);

	return;
}

unsigned char CAGV::GetActionCode(std::string strAction)
{
	if (strAction == "无动作")
	{
		return _AGVACT_NONE;
	}
	else if (strAction == "上料")
	{
		return _AGVACT_LOAD;
	}
	else if (strAction == "下料")
	{
		return _AGVACT_UNLOAD;
	}
	else if (strAction == "移动")
	{
		return _AGVACT_MOVE;
	}
	else if (strAction == "交通管制")
	{
		return _AGVACT_TRAFFIC;
	}
	else if (strAction == "重新移动")
	{
		return _AGVACT_REMOVE;
	}

	return 0xFF;
}

unsigned char CAGV::GetActionCode(std::wstring wstrAction)
{
	std::string strAction = "";
	BufferString::std_wstring_to_string(strAction, wstrAction);

	return GetActionCode(strAction);
}

void CAGV::GetAction(unsigned char byActCode, std::string & strAction)
{
	switch (byActCode)
	{
	case _AGVACT_NONE:
		strAction = "无动作";
		break;
	case _AGVACT_LOAD:
		strAction = "上料";
		break;
	case _AGVACT_UNLOAD:
		strAction = "下料";
		break;
	case _AGVACT_MOVE:
		strAction = "移动";
		break;
	case _AGVACT_TRAFFIC:
		strAction = "交通管制";
		break;
	case _AGVACT_REMOVE:
		strAction = "重新移动";
		break;
	}

	return;
}

void CAGV::GetAction(unsigned char byActCode, std::wstring & wstrAction)
{
	std::string strAction = "";
	GetAction(byActCode, strAction);

	BufferString::std_string_to_wstring(wstrAction, strAction);

	return;
}

unsigned char CAGV::GetStatusCode(std::string strStatus)
{
	if (strStatus == "待机")
	{
		return _AGVSTA_WAIT;
	}
	else if (strStatus == "运行")
	{
		return _AGVSTA_RUN;
	}
	else if (strStatus == "停止")
	{
		return _AGVSTA_STOP;
	}
	else if (strStatus == "急停")
	{
		return _AGVSTA_SCREAM;
	}
	else if (strStatus == "定位中")
	{
		return _AGVSTA_FIND;
	}
	else if (strStatus == "避障减速")
	{
		return _AGVSTA_OBSDOWN;
	}
	else if (strStatus == "避障停止")
	{
		return _AGVSTA_OBSSTOP;
	}
	else if (strStatus == "碰撞停止")
	{
		return _AGVSTA_MOBSSTOP;
	}
	else if (strStatus == "交通管制中")
	{
		return _AGVSTA_TRAFFIC;
	}

	return 0xFF;
}

unsigned char CAGV::GetStatusCode(std::wstring wstrStatus)
{
	std::string strStatus = "";
	BufferString::std_wstring_to_string(strStatus, wstrStatus);

	return GetStatusCode(strStatus);
}

void CAGV::GetStatus(unsigned char byStaCode, std::string & strStatus)
{
	switch (byStaCode)
	{
	case _AGVSTA_WAIT:
		strStatus = "待机";
		break;
	case _AGVSTA_RUN:
		strStatus = "运行";
		break;
	case _AGVSTA_STOP:
		strStatus = "停止";
		break;
	case _AGVSTA_SCREAM:
		strStatus = "急停";
		break;
	case _AGVSTA_FIND:
		strStatus = "定位中";
		break;
	case _AGVSTA_OBSDOWN:
		strStatus = "避障减速";
		break;
	case _AGVSTA_OBSSTOP:
		strStatus = "避障停止";
		break;
	case _AGVSTA_MOBSSTOP:
		strStatus = "碰撞停止";
		break;
	case _AGVSTA_TRAFFIC:
		strStatus = "交通管制中";
		break;
	}

	return;
}

void CAGV::GetStatus(unsigned char byStaCode, std::wstring & wstrStatus)
{
	std::string strStatus = "";
	GetStatus(byStaCode, strStatus);

	BufferString::std_string_to_wstring(wstrStatus, strStatus);

	return;
}

char CAGV::GetErrorCode(std::string strError)
{
	if (strError == "正常")
	{
		return _AGVERR_NONE;
	}
	else if (strError == "无法定位")
	{
		return _AGVERR_MISS;
	}
	else if (strError == "路径不存在")
	{
		return _AGVERR_PATH;
	}
	else if (strError == "坐标异常")
	{
		return _AGVERR_LOCATE;
	}
	else if (strError == "定向异常")
	{
		return _AGVERR_DIRECTION;
	}
	else if (strError == "动作异常")
	{
		return _AGVERR_ACT;
	}
	else if (strError == "通信异常")
	{
		return _AGVERR_COMM;
	}

	return _AGVERR_NONE;
}

char CAGV::GetErrorCode(std::wstring wstrError)
{
	std::string strError = "";
	BufferString::std_wstring_to_string(strError, wstrError);

	return GetErrorCode(strError);
}

void CAGV::GetErrorMessage(char chErrorCode, std::string & strErrorMsg)
{
	switch (chErrorCode)
	{
	case _AGVERR_NONE:
		strErrorMsg = "正常";
		break;
	case _AGVERR_MISS:
		strErrorMsg = "无法定位";
		break;
	case _AGVERR_PATH:
		strErrorMsg = "路径不存在";
		break;
	case _AGVERR_LOCATE:
		strErrorMsg = "坐标异常";
		break;
	case _AGVERR_DIRECTION:
		strErrorMsg = "定向异常";
		break;
	case _AGVERR_ACT:
		strErrorMsg = "动作异常";
		break;
	case _AGVERR_COMM:
		strErrorMsg = "通信异常";
		break;
	}

	return;
}

void CAGV::GetErrorMessage(char chErrorCode, std::wstring & wstrErrorMsg)
{
	std::string strErrorMsg = "";
	GetErrorMessage(chErrorCode, strErrorMsg);

	BufferString::std_string_to_wstring(wstrErrorMsg, strErrorMsg);

	return;
}

unsigned char CAGV::GetActionStatusCode(std::string strActStatus)
{
	if (strActStatus == "未执行")
	{
		return _AGVACTSTA_NONE;
	}
	else if (strActStatus == "执行中")
	{
		return _AGVACTSTA_EXE;
	}
	else if (strActStatus == "执行完成")
	{
		return _AGVACTSTA_FINISH;
	}

	return 0xFF;
}

unsigned char CAGV::GetActionStatusCode(std::wstring wstrActStatus)
{
	std::string strActStatus = "";
	BufferString::std_wstring_to_string(strActStatus, wstrActStatus);

	return GetActionStatusCode(strActStatus);
}

void CAGV::GetActionStatus(unsigned char byActStaCode, std::string & strActStatus)
{
	switch (byActStaCode)
	{
	case _AGVACTSTA_NONE:
		strActStatus = "未执行";
		break;
	case _AGVACTSTA_EXE:
		strActStatus = "执行中";
		break;
	case _AGVACTSTA_FINISH:
		strActStatus = "执行完成";
		break;
	}

	return;
}

void CAGV::GetActionStatus(unsigned char byActStaCode, std::wstring & wstrActStatus)
{
	std::string strActStatus = "";
	GetActionStatus(byActStaCode, strActStatus);

	BufferString::std_string_to_wstring(wstrActStatus, strActStatus);

	return;
}

unsigned char CAGV::GetTypeCode(std::string strType)
{
	if (strType == "机械手式")
	{
		return _AGVTYPE_ARM;
	}
	else if (strType == "叉车式")
	{
		return _AGVTYPE_FORK;
	}
	else if (strType == "举升式")
	{
		return _AGVTYPE_LIFT;
	}
	else if (strType == "牵引式")
	{
		return _AGVTYPE_PULL;
	}
	else if (strType == "潜入式")
	{
		return _AGVTYPE_SNEAK;
	}
	else if (strType == "移载式")
	{
		return _AGVTYPE_TRAVEL;
	}

	return 0xFF;
}

unsigned char CAGV::GetTypeCode(std::wstring wstrType)
{
	std::string strType = "";
	BufferString::std_wstring_to_string(strType, wstrType);

	return GetTypeCode(strType);
}

void CAGV::GetType(unsigned char byTypeCode, std::string & strType)
{
	switch (byTypeCode)
	{
	case _AGVTYPE_ARM:
		strType = "机械手式";
		break;
	case _AGVTYPE_FORK:
		strType = "叉车式";
		break;
	case _AGVTYPE_LIFT:
		strType = "举升式";
		break;
	case _AGVTYPE_PULL:
		strType = "牵引式";
		break;
	case _AGVTYPE_SNEAK:
		strType = "潜入式";
		break;
	case _AGVTYPE_TRAVEL:
		strType = "移载式";
		break;
	}

	return;
}

void CAGV::GetType(unsigned char byTypeCode, std::wstring & wstrType)
{
	std::string strType = "";
	GetActionStatus(byTypeCode, strType);

	BufferString::std_string_to_wstring(wstrType, strType);

	return;
}

unsigned char CAGV::GetDriveCode(std::string strDrive)
{
	if (strDrive == "单向")
	{
		return _AGVDRIVER_SINGLE;
	}
	else if (strDrive == "双向")
	{
		return _AGVDRIVER_DOUBLE;
	}
	else if (strDrive == "全向")
	{
		return _AGVDRIVER_FULL;
	}

	return 0xFF;
}

unsigned char CAGV::GetDriveCode(std::wstring wstrDrive)
{
	std::string strDrive = "";
	BufferString::std_wstring_to_string(strDrive, wstrDrive);

	return GetDriveCode(strDrive);
}

void CAGV::GetDrive(unsigned char byDriveCode, std::string& strDriveMsg)
{
	switch (byDriveCode)
	{
	case _AGVDRIVER_SINGLE:
		strDriveMsg = "单向";
		break;
	case _AGVDRIVER_DOUBLE:
		strDriveMsg = "双向";
		break;
	case _AGVDRIVER_FULL:
		strDriveMsg = "全向";
		break;
	}

	return;
}

void CAGV::GetDrive(unsigned char byDriveCode, std::wstring& wstrDriveMsg)
{
	std::string strDriveMsg = "";
	GetDrive(byDriveCode, strDriveMsg);

	BufferString::std_string_to_wstring(wstrDriveMsg, strDriveMsg);

	return;
}

#ifdef _AFX
void CAGV::GetNo(CString & cstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_CString(cstrNo, strNo);

	return;
}

void CAGV::GetType(CString & cstrType)
{
	std::string strType = "";
	GetType(strType);

	BufferString::std_string_to_CString(cstrType, strType);

	return;
}

void CAGV::GetDriver(CString & cstrDriver)
{
	std::string strDriver = "";
	GetDriver(strDriver);

	BufferString::std_string_to_CString(cstrDriver, strDriver);

	return;
}

void CAGV::GetMaxSpeed(CString & cstrSpeed)
{
	std::string strSpeed = "";
	GetMaxSpeed(strSpeed);

	BufferString::std_string_to_CString(cstrSpeed, strSpeed);

	return;
}

void CAGV::GetPower(CString & cstrPower, COLORREF & color)
{
	std::string strPower = "";
	GetPower(strPower);

	BufferString::std_string_to_CString(cstrPower, strPower);

	if (m_byPower >= 0 && m_byPower <= 20)
	{
		color = COLOR_RED;
	}
	else if (m_byPower <= 50)
	{
		color = COLOR_YELLOW;
	}
	else if (m_byPower <= 50)
	{
		color = COLOR_GREEN;
	}

	return;
}

void CAGV::GetSpeed(CString & cstrSpeed, COLORREF & color)
{
	std::string strSpeed = "";
	GetSpeed(strSpeed);

	BufferString::std_string_to_CString(cstrSpeed, strSpeed);

	if (m_chSpeed == 0)
	{
		color = COLOR_RED;
	}
	else
	{
		color = COLOR_GREEN;
	}

	return;
}

void CAGV::GetRealitySpeed(CString & cstrSpeed, COLORREF & color)
{
	std::string strSpeed = "";
	GetRealitySpeed(strSpeed);

	BufferString::std_string_to_CString(cstrSpeed, strSpeed);

	if (GetRealitySpeed() == 0.0f)
	{
		color = COLOR_RED;
	}
	else
	{
		color = COLOR_GREEN;
	}

	return;
}

void CAGV::GetStatus(CString & cstrStatus, COLORREF & color)
{
	std::string strStatus = "";
	GetStatus(strStatus);

	BufferString::std_string_to_CString(cstrStatus, strStatus);

	switch (m_byStatus)
	{
	case _AGVSTA_WAIT:
		color = COLOR_YELLOW;
		break;
	case _AGVSTA_RUN:
		color = COLOR_GREEN;
		break;
	case _AGVSTA_STOP:
		color = COLOR_RED;
		break;
	case _AGVSTA_SCREAM:
		color = COLOR_RED;
		break;
	case _AGVSTA_FIND:
		color = COLOR_RED;
		break;
	case _AGVSTA_OBSDOWN:
		color = COLOR_YELLOW;
		break;
	case _AGVSTA_OBSSTOP:
		color = COLOR_RED;
		break;
	case _AGVSTA_MOBSSTOP:
		color = COLOR_RED;
		break;
	case _AGVSTA_TRAFFIC:
		color = COLOR_YELLOW;
		break;
	}

	return;
}

void CAGV::GetAction(CString & cstrAction, CString & cstrStatus, COLORREF & color)
{
	std::string strAction = "";
	std::string strStatus = "";
	GetAction(strAction,strStatus);

	BufferString::std_string_to_CString(cstrAction, strAction);
	BufferString::std_string_to_CString(cstrStatus, strStatus);

	if (m_usAction != _AGVACT_NONE)
	{
		switch (m_byActionStatus)
		{
		case _AGVACTSTA_NONE:
			color = COLOR_YELLOW;
			break;
		case _AGVACTSTA_EXE:
			color = COLOR_GREEN;
			break;
		case _AGVACTSTA_FINISH:
			color = COLOR_RED;
			break;
		}
	}

	return;
}

void CAGV::GetError(CString & cstrError, COLORREF & color)
{
	std::string strError = "";
	GetError(strError);

	BufferString::std_string_to_CString(cstrError, strError);

	switch (m_chError)
	{
	case _AGVERR_NONE:
		break;
	case _AGVERR_MISS:
		color = COLOR_RED;
		break;
	case _AGVERR_PATH:
		color = COLOR_RED;
		break;
	case _AGVERR_LOCATE:
		color = COLOR_RED;
		break;
	case _AGVERR_DIRECTION:
		color = COLOR_RED;
		break;
	case _AGVERR_ACT:
		color = COLOR_RED;
		break;
	case _AGVERR_COMM:
		color = COLOR_RED;
		break;
	}

	return;
}

void CAGV::GetMode(CString & cstrMode)
{
	std::string strMode = "";
	GetMode(strMode);

	BufferString::std_string_to_CString(cstrMode, strMode);

	return;
}

void CAGV::GetCargo(CString & cstrCargo)
{
	std::string strCargo = "";
	GetCargo(strCargo);

	BufferString::std_string_to_CString(cstrCargo, strCargo);

	return;
}

unsigned char CAGV::GetActionCode(CString cstrAction)
{
	std::string strAction = "";
	BufferString::CString_to_std_string(strAction, cstrAction);

	return GetActionCode(strAction);
}

void CAGV::GetAction(unsigned char byActCode, CString & cstrAction)
{
	std::string strAction = "";
	GetAction(byActCode, strAction);

	BufferString::std_string_to_CString(cstrAction, strAction);

	return;
}

unsigned char CAGV::GetStatusCode(CString cstrStatus)
{
	std::string strStatus = "";
	BufferString::CString_to_std_string(strStatus, cstrStatus);

	return GetStatusCode(strStatus);
}

void CAGV::GetStatus(unsigned char byStaCode, CString & cstrStatus)
{
	std::string strStatus = "";
	GetStatus(byStaCode, strStatus);

	BufferString::std_string_to_CString(cstrStatus, strStatus);

	return;
}

char CAGV::GetErrorCode(CString cstrError)
{
	std::string strError = "";
	BufferString::CString_to_std_string(strError, cstrError);

	return GetErrorCode(strError);
}

void CAGV::GetErrorMessage(char chErrorCode, CString & cstrErrorMsg)
{
	std::string strErrorMsg = "";
	GetErrorMessage(chErrorCode, strErrorMsg);

	BufferString::std_string_to_CString(cstrErrorMsg, strErrorMsg);

	return;
}

unsigned char CAGV::GetActionStatusCode(CString cstrActStatus)
{
	std::string strActStatus = "";
	BufferString::CString_to_std_string(strActStatus, cstrActStatus);

	return GetActionStatusCode(strActStatus);
}

void CAGV::GetActionStatus(unsigned char byActStaCode, CString & cstrActStatus)
{
	std::string strActStatus = "";
	GetActionStatus(byActStaCode, strActStatus);

	BufferString::std_string_to_CString(cstrActStatus, strActStatus);

	return;
}

unsigned char CAGV::GetTypeCode(CString cstrType)
{
	std::string strType = "";
	BufferString::CString_to_std_string(strType, cstrType);

	return GetTypeCode(strType);
}

void CAGV::GetType(unsigned char byTypeCode, CString & cstrType)
{
	std::string strType = "";
	GetType(byTypeCode, strType);

	BufferString::std_string_to_CString(cstrType, strType);

	return;
}

unsigned char CAGV::GetDriveCode(CString cstrDrive)
{
	std::string strDrive = "";
	BufferString::CString_to_std_string(strDrive, cstrDrive);

	return GetDriveCode(strDrive);
}

void CAGV::GetDrive(unsigned char byDriveCode, CString & cstrDriveMsg)
{
	std::string strDriveMsg = "";
	GetDrive(byDriveCode, strDriveMsg);

	BufferString::std_string_to_CString(cstrDriveMsg, strDriveMsg);

	return;
}
#endif // !_AFX

#endif //!_AGV_H