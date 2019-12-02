#include "stdafx.h"
#include "CService.h"
#include "XML/Xml.h"

#ifdef _SERVICE_H

CService::CService()
{
	m_pServer = nullptr;
	m_pThreadFunc = nullptr;
	m_hShutdownEvent = nullptr;
	m_hParent = nullptr;
	m_bPermission = true;
}

CService::~CService()
{
	Destroy();
}

void CService::DownLoadConfig()
{
}

void CService::UpLoadConfig()
{

}

void CService::LoadXMLConfig()
{
}

void CService::LoadDatabaseConfig()
{
}

void CService::Connect()
{
	if (m_pServer == nullptr)
	{
		return;
	}

	SOCKET socket = m_pServer->GetClient();
	if (socket == INVALID_SOCKET)
	{
		return;
	}

	std::string strClientIP = "";
	unsigned short usClientPort = 0;

	CTCPServer::GetSocketAddress(socket, strClientIP, usClientPort);

#ifdef _MAGNETICTRACK_H

	/*!
	 * AGV检测
	*/
	for (std::map<unsigned char, CTCPMNavAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		CTCPMNavAGV *pAGV = it->second;	/*!< AGV指针 */

		std::string strIP = "";		/*!< AGV绑定IP */
		short sPort = 0;			/*!< AGV绑定端口 */
		pAGV->GetIP(strIP);
		sPort = pAGV->GetPort();

		if (strClientIP == strIP && (sPort == -1 || (unsigned short)sPort == usClientPort))
		{
			if (pAGV->Connect(socket))
			{
				/*!
				 * 更新设备连接状态
				*/

				std::string strRecord = "";
				BufferString::std_string_format(strRecord, "【系统System】已连接AGV【编号:%d】【IP:%s】【端口:%d】", pAGV->GetNo(), strClientIP.c_str(), usClientPort);
				// 输出日志
				DebugPrint(strRecord.c_str());
				// 记录日志
				Record(strRecord);
			}

			return;
		}
	}

#ifdef _SELECT_FUNC

#ifdef _CHARGERCONTROLER_H
	/*!
	 * 充电控制器检测
	*/
	for (std::map<unsigned char, CChargerControler*>::iterator it = m_mapChargerCtrlers.begin(); it != m_mapChargerCtrlers.end(); ++it)
	{
		CChargerControler *pChargerCtrler = it->second;	/*!< AGV指针 */

		std::string strIP = "";		/*!< AGV绑定IP */
		short sPort = 0;			/*!< AGV绑定端口 */
		pChargerCtrler->GetIP(strIP);
		sPort = pChargerCtrler->GetPort();

		if (strClientIP == strIP && (sPort == -1 || (unsigned short)sPort == usClientPort))
		{
			if (pChargerCtrler->Connect(socket))
			{
				/*!
				 * 更新设备连接状态
				*/

				std::string strRecord = "";
				BufferString::std_string_format(strRecord, "【系统System】已连接充电控制器【编号:%d】【IP:%s】【端口:%d】", pChargerCtrler->GetNo(), strClientIP.c_str(), usClientPort);
				// 输出日志
				DebugPrint(strRecord.c_str());
				// 记录日志
				Record(strRecord);
			}

			return;
		}
	}
#endif // !_CHARGERCONTROLER_H

#endif // !_SELECT_FUNC

#endif // !_MAGNETICTRACK_H

#ifdef _SELECT_FUNC

#ifdef _CA_H
	/*!
	 * CA主机检测
	*/
	for (std::map<unsigned short, CCA*>::iterator it = m_mapCAs.begin(); it != m_mapCAs.end(); ++it)
	{
		CCA *pCA = it->second;		/*!< CA指针 */

		std::string strIP = "";		/*!< AGV绑定IP */
		short sPort = 0;			/*!< AGV绑定端口 */
		pCA->GetIP(strIP);
		sPort = pCA->GetPort();

		if (strClientIP == strIP && (sPort == -1 || (unsigned short)sPort == usClientPort))
		{
			if (pCA->Connect(socket))
			{
				/*!
				 * 更新设备连接状态
				*/

				std::string strRecord = "";
				BufferString::std_string_format(strRecord, "【系统System】已连接CA无线主机【编号:%d】【IP:%s】【端口:%d】", pCA->GetNo(), strClientIP.c_str(), usClientPort);
				// 输出日志
				DebugPrint(strRecord.c_str());
				// 记录日志
				Record(strRecord);
			}

			return;
		}
	}

#endif // _CA_H

#endif // !_SELECT_FUNC

	return;
}

void CService::Disconnect()
{
	if (m_pServer && m_pServer->GetListenStatus())
	{
		m_pServer->EndListen();

		std::string strIP = "";
		m_pServer->GetIP(strIP);

		std::string strRecord = "";
		BufferString::std_string_format(strRecord, "【系统System】中断服务端监听【IP:%s】【端口:%d】", strIP.c_str(), m_pServer->GetPort());
		// 输出日志
		DebugPrint(strRecord.c_str());
		// 记录日志
		Record(strRecord);
	}

#ifdef _MAGNETICTRACK_H

	/*!
	 * AGV检测
	*/
	for (std::map<unsigned char, CTCPMNavAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		CTCPMNavAGV *pAGV = it->second;	/*!< AGV指针 */

		if (pAGV->IsConnected())
		{
			pAGV->Break();

			std::string strIP = "";
			pAGV->GetIP(strIP);

			std::string strRecord = "";
			BufferString::std_string_format(strRecord, "【系统System】中断与AGV连接【IP:%s】【端口:%d】", strIP.c_str(), pAGV->GetPort());
			// 输出日志
			DebugPrint(strRecord.c_str());
			// 记录日志
			Record(strRecord);
		}
	}

#ifdef _SELECT_FUNC

#ifdef _CHARGERCONTROLER_H
	/*!
	 * 充电控制器检测
	*/
	for (std::map<unsigned char, CChargerControler*>::iterator it = m_mapChargerCtrlers.begin(); it != m_mapChargerCtrlers.end(); ++it)
	{
		CChargerControler *pChargerCtrler = it->second;	/*!< AGV指针 */

		if (pChargerCtrler->IsConnected())
		{
			pChargerCtrler->Break();

			std::string strIP = "";
			pChargerCtrler->GetIP(strIP);

			std::string strRecord = "";
			BufferString::std_string_format(strRecord, "【系统System】中断与充电控制器连接【IP:%s】【端口:%d】", strIP.c_str(), pChargerCtrler->GetPort());
			// 输出日志
			DebugPrint(strRecord.c_str());
			// 记录日志
			Record(strRecord);
		}
	}
#endif // !_CHARGERCONTROLER_H

#ifdef _CA_H
	/*!
	 * CA主机检测
	*/
	for (std::map<unsigned short, CCA*>::iterator it = m_mapCAs.begin(); it != m_mapCAs.end(); ++it)
	{
		CCA *pCA = it->second;		/*!< CA指针 */

		if (pCA->IsConnected())
		{
			pCA->Break();

			std::string strIP = "";
			pCA->GetIP(strIP);

			std::string strRecord = "";
			BufferString::std_string_format(strRecord, "【系统System】中断与CA无线主机连接【IP:%s】【端口:%d】", strIP.c_str(), pCA->GetPort());
			// 输出日志
			DebugPrint(strRecord.c_str());
			// 记录日志
			Record(strRecord);
		}
	}

#endif // _CA_H

#ifdef _LED_H
	/*!< LED组 */
#endif // !_LED_H

#endif // !_SELECT_FUNC

#endif // !_MAGNETICTRACK_H

	return;
}

void CService::FunctionThread()
{
	while (true)
	{

#ifdef _SELECT_FUNC

#ifdef _CA_H
		ProcessCallMsg();
#endif // _CA_H

#endif // !_SELECT_FUNC

#ifdef _MAGNETICTRACK_H
		TrafficControl();

#ifdef _SELECT_FUNC

#ifdef _SELECT_AUTO_GOUP
		RestPositionControl();
#endif // !_SELECT_AUTO_GOUP

#ifdef _CHARGERCONTROLER_H
		ProcessChargerControlerData();

#endif // !_CHARGERCONTROLER_H

#endif // !_SELECT_FUNC

#endif // !_MAGNETICTRACK_H

		if (WaitForSingleObject(m_hShutdownEvent, 100) == WAIT_OBJECT_0)
		{
			break;
		}
	}

	return;
}

void CService::StartThread()
{
	EndThread();

	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(nullptr, true, false, nullptr);
	}

	m_pThreadFunc = new std::thread(&CService::FunctionThread, this);

	// 记录日志
	Record("【系统System】业务功能【启动】.");
	// 调试输出
	DebugPrint("AGVContorlSystem/CService/StartThread 启动功能线程.");

	return;
}

void CService::EndThread()
{
	if (m_pThreadFunc)
	{
		if (m_pThreadFunc->joinable())
		{
			SetEvent(m_hShutdownEvent);

			m_pThreadFunc->join();
		}

		delete m_pThreadFunc;
		m_pThreadFunc = nullptr;

		// 记录日志
		Record("【系统System】业务功能【关闭】.");
		// 调试输出
		DebugPrint("AGVContorlSystem/CService/StartThread 关闭功能线程.");
	}

	return;
}

void CService::DestroyData()
{
	// 调试输出
	DebugPrint("AGVContorlSystem/CService/StartThread 销毁数据.");

	if (m_pServer)
	{
		delete m_pServer;

		m_pServer = nullptr;
	}

#ifdef _SELECT_FUNC

#ifdef _CHARGERCONTROLER_H
	for (std::map<unsigned char, CChargerControler*>::iterator it = m_mapChargerCtrlers.begin(); it != m_mapChargerCtrlers.end(); )
	{
		delete it->second;

		it = m_mapChargerCtrlers.erase(it);
	}
#endif // !_CHARGERCONTROLER_H

#ifdef _CA_H
	for (std::map<unsigned short, CCA*>::iterator it = m_mapCAs.begin(); it != m_mapCAs.end(); )
	{
		delete it->second;

		it = m_mapCAs.erase(it);
	}

	for (std::map<unsigned char, CAWatch*>::iterator it = m_mapCAWatchs.begin(); it != m_mapCAWatchs.end(); )
	{
		delete it->second;

		it = m_mapCAWatchs.erase(it);
	}
#endif // !_CA_H

#ifdef _LED_H
	/*!< LED组 */
#endif // !_LED_H

#endif // !_SELECT_FUNC

#ifdef _MAGNETICTRACK_H
	for (std::map<unsigned char, CTCPMNavAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); )
	{
		delete it->second;

		it = m_mapAGVs.erase(it);
	}

	for (std::map<unsigned short, CRFID*>::iterator it = m_mapRFIDs.begin(); it != m_mapRFIDs.end(); )
	{
		delete it->second;

		it = m_mapRFIDs.erase(it);
	}

	for (std::map<unsigned char, CMNavStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end(); )
	{
		delete it->second;

		it = m_mapStations.erase(it);
	}

	for (std::map<unsigned char, CMNavRestPosition*>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); )
	{
		delete it->second;

		it = m_mapRests.erase(it);
	}

	for (std::map<unsigned short, CMTTrafficCtrlArea*>::iterator it = m_mapTraffic.begin(); it != m_mapTraffic.end(); )
	{
		delete it->second;

		it = m_mapTraffic.erase(it);
	}

#ifdef _SELECT_FUNC
	for (std::map<unsigned char, CMNavCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); )
	{
		delete it->second;

		it = m_mapChargers.erase(it);
	}

#ifdef _CA_H
	for (std::map<unsigned short, CACall*>::iterator it = m_mapCACalls.begin(); it != m_mapCACalls.end(); )
	{
		delete it->second;

		it = m_mapCACalls.erase(it);
	}
#endif // _CA_H

#endif // !_SELECT_FUNC

#endif 

	if (m_hShutdownEvent)
	{
		CloseHandle(m_hShutdownEvent);

		m_hShutdownEvent = nullptr;
	}

	return;
}

bool CService::Open()
{
	std::string strRecord = "";

	LoadXMLConfig();

	if (m_adoDB.OnInitADOConn())
	{
		BufferString::std_string_format(strRecord, "【系统System】连接数据库【链接串:%s】【用户名:%s】【密码:%s】成功.",
			(char*)m_adoDB.m_bstrConnect,(char*)m_adoDB.m_bstrUser,(char*)m_adoDB.m_bstrPassword);

		// 记录日志
		Record(strRecord);
		// 调试输出
		DebugPrint(strRecord.c_str());
	}
	else
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("连接数据库失败!"), _T("启动失败"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "【系统System】连接数据库【链接串:%s】【用户名:%s】【密码:%s】失败.",
			(char*)m_adoDB.m_bstrConnect, (char*)m_adoDB.m_bstrUser, (char*)m_adoDB.m_bstrPassword);

		// 记录日志
		Record(strRecord);
		// 调试输出
		DebugPrint(strRecord.c_str());

		return false;
	}

	LoadDatabaseConfig();

	if (m_bPermission == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("缺少使用权限!"), _T("启动失败"), MB_ICONERROR);
		}
#endif // _AFX

		// 记录日志
		Record(strRecord);
		// 调试输出
		DebugPrint(strRecord.c_str());

		return false;
	}

	if (m_pServer && m_pServer->GetListenStatus() == false)
	{
		std::string strIP = "";
		m_pServer->GetIP(strIP);

		if (m_pServer->StartListen())
		{
			BufferString::std_string_format(strRecord, "【系统System】服务端【IP:%s:%d】监听启动.", strIP.c_str(), m_pServer->GetPort());

			// 记录日志
			Record(strRecord);
			// 调试输出
			DebugPrint(strRecord.c_str());
		}
		else
		{
#ifdef _AFX
			CString cstrMsg = _T("");
			CString cstrIP = _T("");
			m_pServer->GetIP(cstrIP);
			cstrMsg.Format(_T("服务端IP:%s:%d监听失败!"), cstrIP, m_pServer->GetPort());
			if (m_hParent)
			{
				MessageBox(m_hParent, cstrMsg, _T("启动失败"), MB_ICONERROR);
			}
#endif // _AFX

			BufferString::std_string_format(strRecord, "【系统System】服务端【IP:%s:%d】监听失败.", strIP.c_str(), m_pServer->GetPort());

			// 记录日志
			Record(strRecord);
			// 调试输出
			DebugPrint(strRecord.c_str());

			return false;
		}
	}

#ifdef _AFX
	m_TaskManager.OpenManager(m_adoDB, m_hParent);
#else
	m_TaskManager.OpenManager(m_adoDB);
#endif // _AFX

	StartThread();

	return true;
}

#ifdef _AFX
bool CService::Open(const HWND hParent)
{
	if (hParent == nullptr)
	{
		DebugPrint("无效的父窗口句柄");
		return false;
	}

	m_hParent = hParent;

	return Open();
}
#endif //_AFX

void CService::Close()
{
	EndThread();
	Disconnect();
}

void CService::Destroy()
{
	Close();
	DestroyData();
}

bool CService::GetAdmin(std::string strPassword)
{
	std::string strSql = "";
	BufferString::std_string_format(strSql,
		"select * from [%s]",
		"USER_MANAGER_TABLE"
	);

	_RecordsetPtr pRecordset = m_adoDB.GetRecordSet(_bstr_t(strSql.c_str())); /*!< 结果集 */

	if (pRecordset == nullptr || pRecordset->adoEOF)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("管理员权限获取失败!读取数据库失败."), _T("管理员权限获取"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	std::string strAesAdminPassword = ""; /*!< AES加密后的管理员密码 */

	_variant_t var;	/*!< 值 */

	// 获取AES加密后的管理员密码
	var = pRecordset->GetCollect("admin_password");
	// 值不为空
	if (var.vt != VT_NULL)
	{
		strAesAdminPassword = (char*)_bstr_t(var);
	}

	if (strAesAdminPassword == "")
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("管理员权限获取失败!未设置管理员密码."), _T("管理员权限获取"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	std::string strAdminPassword = m_aes.DecryptionAES(strAesAdminPassword);	/*!< 明文管理员密码 */

	if (strAdminPassword == strPassword)
	{
		return true;
	}

#ifdef _AFX
	if (m_hParent)
	{
		MessageBox(m_hParent, _T("管理员权限获取失败!密码不正确."), _T("管理员权限获取"), MB_ICONERROR);
	}
#endif // _AFX

	return false;
}

bool CService::GetAdmin(std::wstring wstrPassword)
{
	std::string strPassword = "";
	BufferString::std_wstring_to_string(strPassword, wstrPassword);

	return GetAdmin(strPassword);
}

#ifdef _AFX
bool CService::GetAdmin(CString cstrPassword)
{
	std::string strPassword = "";
	BufferString::CString_to_std_string(strPassword, cstrPassword);

	return GetAdmin(strPassword);
}
#endif //_AFX

bool CService::SetAdmin(std::string strOldPassword, std::string strNewPassword)
{
	if(strNewPassword == "")
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("管理员权限设置失败!密码不能为空."), _T("管理员权限设置"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	std::string strSql = "";
	BufferString::std_string_format(strSql,
		"select * from [%s]",
		"USER_MANAGER_TABLE"
	);

	_RecordsetPtr pRecordset = m_adoDB.GetRecordSet(_bstr_t(strSql.c_str())); /*!< 结果集 */

	if (pRecordset == nullptr || pRecordset->adoEOF)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("管理员权限设置失败!读取数据库失败."), _T("管理员权限设置"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}
	std::string strAesAdminPassword = ""; /*!< AES加密后的管理员密码 */

	_variant_t var;	/*!< 值 */

	// 获取AES加密后的管理员密码
	var = pRecordset->GetCollect("admin_password");

	// 值不为空
	if (var.vt != VT_NULL)
	{
		strAesAdminPassword = (char*)_bstr_t(var);
	}

	if (strAesAdminPassword != "")
	{
		std::string strAdminPassword = m_aes.DecryptionAES(strAesAdminPassword);	/*!< 明文管理员密码 */

		if (strOldPassword != strAdminPassword)
		{
#ifdef _AFX
			if (m_hParent)
			{
				MessageBox(m_hParent, _T("管理员权限设置失败!密码不正确."), _T("管理员权限获取"), MB_ICONERROR);
			}
#endif // _AFX

			return false;
		}
	}

	std::string strAesNewPassword = m_aes.EncryptionAES(strNewPassword);

	BufferString::std_string_format(strSql,
		"update [%s] set [%s]=%s where [%s]=%s",
		"AGV_SYSTEM_CONFIG", "Value" , strAesNewPassword.c_str(),"Name", "Admin"
	);

	// 执行SQL语句
	if (m_adoDB.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// 调试输出
		DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

		BufferString::std_string_format(strSql,
			"insert into[%s]([%s],[%s]) values('%s','%s')",
			"AGV_SYSTEM_CONFIG", "Name","Value", "Admin", strAesNewPassword.c_str()
		);

		// 执行SQL语句
		if (m_adoDB.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
		{
			// 调试输出
			DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

#ifdef _AFX
			if (m_hParent)
			{
				MessageBox(m_hParent, _T("管理员权限设置失败!数据库写入失败."), _T("管理员权限获取"), MB_ICONERROR);
			}
#endif // _AFX
			return false;
		}
	}

	return true;
}

bool CService::SetAdmin(std::wstring wstrOldPassword, std::wstring wstrNewPassword)
{
	std::string strOldPassword = "", strNewPassword = "";
	BufferString::std_wstring_to_string(strOldPassword, wstrOldPassword);
	BufferString::std_wstring_to_string(strNewPassword, wstrNewPassword);

	return SetAdmin(strOldPassword, strNewPassword);
}

#ifdef _AFX
bool CService::SetAdmin(CString cstrOldPassword, CString cstrNewPassword)
{
	std::string strOldPassword = "", strNewPassword = "";
	BufferString::CString_to_std_string(strOldPassword, cstrOldPassword);
	BufferString::CString_to_std_string(strNewPassword, cstrNewPassword);

	return SetAdmin(strOldPassword, strNewPassword);
}
#endif //_AFX

#ifdef _SELECT_FUNC

#ifdef _CA_H
void CService::ProcessCallMsg()
{
	std::string strMsg = "";	/*!< 呼叫器传回的消息 */
	strMsg = CCA::GetRecvMsg();

	if (strMsg == "")
	{
		return;
	}

	/*!
	 * 根据规则对呼叫器传回的消息进行解析
	 * 最后生成相应的任务
	 * 远程呼叫器仅能完成较为单一的任务
	 * 但是可以通过使用多个呼叫器组合的方式完成较为复杂的任务
	 * 如上料下料等
	 * 为防止因没有呼叫反馈,多次呼叫引起的任务重复下发的情况发生.
	 * 每个呼叫器仅能呼叫1次任务，只有当任务完成时才能进行下一次的呼叫
	 * 即任务中不存在相同相同内容的任务
	*/
	unsigned int unCallNo = 0;	/*!< 呼叫器编号 */
	std::string strText = "";	/*!< 呼叫器信息文本 */

	/*!
	 * 解析信息
	*/

	// 发布任务
	PublishCallTask(unCallNo, strText);
	
	return;
}

void CService::SendMsgToWatch(std::string strMsg, unsigned char byStation)
{
	for (std::map<unsigned char, CAWatch*>::iterator it = m_mapCAWatchs.begin(); it != m_mapCAWatchs.end(); ++it)
	{
		CAWatch* pWatch = it->second;

		if (pWatch->m_byStation != byStation && pWatch->m_byStation != 0)
		{
			continue;
		}

		if (pWatch->m_usCA == 0)
		{
			std::map<unsigned short, CCA*> m_mapCAs;

			for (std::map<unsigned short, CCA*>::iterator itCA = m_mapCAs.begin(); itCA != m_mapCAs.end(); ++itCA)
			{
				CCA* pCA = itCA->second;

				pCA->SendToWatch(pWatch->m_byNo, strMsg);
			}
		}
		else
		{
			if (m_mapCAs.find(pWatch->m_usCA) == m_mapCAs.end())
			{
				continue;
			}

			m_mapCAs[pWatch->m_usCA]->SendToWatch(pWatch->m_byNo, strMsg);
		}
	}

	return;
}

void CService::SendMsgToWatch(std::wstring wstrMsg, unsigned char byStation)
{
	std::string strMsg = "";
	BufferString::std_wstring_to_string(strMsg, wstrMsg);

	SendMsgToWatch(strMsg, byStation);

	return;
}

#ifdef _AFX
void CService::SendMsgToWatch(CString cstrMsg, unsigned char byStation)
{
	std::string strMsg = "";
	BufferString::CString_to_std_string(strMsg, cstrMsg);

	SendMsgToWatch(strMsg, byStation);

	return;
}
#endif // _AFX

#endif // !_CA_H

#ifdef _CHARGERCONTROLER_H
void CService::ProcessChargerControlerData()
{

}

#endif // !_CHARGERCONTROLER_H

#endif // !_SELECT_FUNC

#ifdef _MAGNETICTRACK_H
void CService::TrafficControl()
{
	for (std::map<unsigned short, CMTTrafficCtrlArea*>::iterator it = m_mapTraffic.begin(); it != m_mapTraffic.end(); ++it)
	{
		CMTTrafficCtrlArea* pTrafficArea = it->second;

		unsigned short usCtrlRFID = pTrafficArea->GetCtrlRFID();	/*!< 管制RFID卡编号 */

		assert(usCtrlRFID != 0);

		if (m_mapRFIDs.find(usCtrlRFID) == m_mapRFIDs.end())
		{
			// 管制点无效
			continue;
		}

		CRFID* pCtrlRFID = m_mapRFIDs[usCtrlRFID];	/*!< 管制RFID卡结构体 */

		CTCPMNavAGV *pAGV = nullptr;	/*!< 管制点AGV指针 */

		// AGV不处于待机状态或处于手动模式或无电
		if (pAGV->GetStatus() != _AGVSTA_TRAFFIC || pAGV->GetMode() == false || pAGV->GetPower() == 0)
		{
			continue;
		}

#ifdef _WIN64
		unsigned long long ullUseTime = 0;	/*!< RFID使用时间 */
		pAGV = (CTCPMNavAGV *)pCtrlRFID->GetUser(ullUseTime);
#elif _WIN32
		unsigned long ulUseTime = 0;	/*!< RFID使用时间 */
		pAGV = (CTCPMNavAGV *)pCtrlRFID->GetUser(ulUseTime);
#endif //!_WIN64

		if (pAGV == nullptr)
		{
			// 无AGV在管制点
			continue;
		}

		bool bNext = true;	/*!< 下一阶段标识 */

		/*!
		 * 禁行RFID卡检测
		*/
		std::set<unsigned short> setStop = pTrafficArea->GetStopRFIDList();	/*!< 禁行RFID卡编号组 */

		for (std::set<unsigned short>::iterator itStop = setStop.begin(); itStop != setStop.end(); ++itStop)
		{
			unsigned short usStopRFID = *itStop;	/*!< 禁行RFID卡编号 */

			if (usStopRFID == 0)
			{
				continue;
			}

			if (m_mapRFIDs.find(usStopRFID) == m_mapRFIDs.end())
			{
				// 禁行点无效
				continue;
			}

			CRFID* pStopRFID = m_mapRFIDs[usStopRFID];	/*!< 禁行RFID卡结构体 */
			
			if (pStopRFID->GetUser())
			{
				// 存在AGV在禁行RFID卡上
				bNext = false;
				break;
			}
		}

		if (bNext == false)
		{
			continue;
		}

		/*!
		 * 对比交通管制点检测
		*/
		std::set<unsigned short> setCompare = pTrafficArea->GetCompareAreaList();	/*!< 对比交通管制点组 */

		for (std::set<unsigned short>::iterator itCompare = setCompare.begin(); itCompare != setCompare.end(); ++itCompare)
		{
			unsigned short usArea = *itCompare;	/*!< 交通管制点编号 */

			if (m_mapTraffic.find(usArea) == m_mapTraffic.end())
			{
				// 对比点无效
				continue;
			}

			CMTTrafficCtrlArea *pCompareArea = m_mapTraffic[usArea];	/*!< 对比交通管制点结构体 */

			/*!
			 * 禁行RFID卡检测
			*/
			std::set<unsigned short> setCompareStop = pCompareArea->GetStopRFIDList();	/*!< 禁行RFID卡编号组 */

			for (std::set<unsigned short>::iterator itStop = setCompareStop.begin(); itStop != setCompareStop.end(); ++itStop)
			{
				unsigned short usStopRFID = *itStop;	/*!< 禁行RFID卡编号 */

				if (usStopRFID == 0)
				{
					// 禁行点无效
					continue;
				}

				if (m_mapRFIDs.find(usStopRFID) == m_mapRFIDs.end())
				{
					// 禁行点无效
					continue;
				}

				CRFID* pStopRFID = m_mapRFIDs[usStopRFID];	/*!< 禁行RFID卡结构体 */

				if (pStopRFID->GetUser())
				{
					// 存在AGV在禁行RFID卡上
					bNext = false;
					break;
				}
			}

			if (bNext == false)
			{
				break;
			}

			unsigned short usCompareRFID = pCompareArea->GetCtrlRFID();	/*!< 对比交通管制点管制RFID卡编号 */

			if(usCompareRFID == 0)
			{
				// 对比点的管制点无效
				continue;
			}

			if (m_mapRFIDs.find(usCompareRFID) == m_mapRFIDs.end())
			{
				// 对比点的管制点无效
				continue;
			}

			CRFID *pCompareCtrlRFID = m_mapRFIDs[usCompareRFID];	/*!< 禁行RFID卡结构体 */

#ifdef _WIN64
			unsigned long long ullCompareUseTime = 0;		/*!< 对比RFID使用时间 */
			if (pCompareCtrlRFID->GetUser(ullCompareUseTime))
			{
				// 对比点的管制点存在AGV
				if (ullCompareUseTime < ullUseTime)
				{
					// 对比点使用时间早于管制点使用时间
					bNext = false;
					break;
				}
			}
			else
			{
				// 对比点的管制点不存在AGV
				continue;
			}
#elif _WIN32
			unsigned long ulCompareUseTime = 0;		/*!< 对比RFID使用时间 */
			if (pCompareCtrlRFID->GetUser(ulCompareUseTime))
			{
				// 对比点的管制点存在AGV
				if (ulCompareUseTime < ulUseTime)
				{
					// 对比点使用时间早于管制点使用时间
					bNext = false;
					break;
				}
			}
			else
			{
				// 对比点的管制点不存在AGV
				continue;
			}
#endif // !_WIN64
		}

		if (bNext == false)
		{
			continue;
		}

		/*!
		 * 通知AGV允许通行
		*/
		pAGV->TrafficPass();

	}

	return;
}

void CService::UpdateAGVStatus(unsigned char byNo,unsigned char byPower, char chSpeed, unsigned char byStatus,
	char chError, unsigned short usAction, unsigned char byActStatus, unsigned char byCargo,
	unsigned short usRFID, unsigned short usNextRFID)
{
	if (m_mapAGVs.find(byNo) == m_mapAGVs.end())
	{
		return;
	}

	CTCPMNavAGV* pAGV = nullptr;	/*!< AGV指针 */
	pAGV = m_mapAGVs[byNo];
		
	bool bNew = false;		/*!< 更新标识 */
		
	if (pAGV->SetPower(byPower)
		|| pAGV->SetSpeed(chSpeed) 
		|| pAGV->SetStatus(byStatus)
		|| pAGV->SetError(chError) 
		|| pAGV->SetAction(usAction, byActStatus) 
		|| pAGV->SetCargo(byCargo) 
		|| pAGV->SetNextRFID(usNextRFID)
		)
	{
		bNew = true;
	}

	if (pAGV->SetCurRFID(usRFID))
	{
		std::string strSql = ""; /*!< SQL语句 */

		// 合成SQL语句
		BufferString::std_string_format(strSql,
			"update [%s] set [%s]=%d where [%s]=%d",
			"AGV_RUNTIME_TABLE", "agv_cur_rfid", pAGV->GetCurRFID(), "agv_no", pAGV->GetNo()
		);

		// 执行SQL语句
		if (m_adoDB.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
		{
			// 调试输出
			DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

			// 合成SQL语句
			BufferString::std_string_format(strSql,
				"insert into [%s]([%s],[%s]) values(%d,%d)",
				"AGV_RUNTIME_TABLE", "agv_no", "agv_cur_rfid", pAGV->GetNo(), pAGV->GetCurRFID()
			);

			m_adoDB.ExecuteSQL(_bstr_t(strSql.c_str()));
		}

		bNew = true;
	}

#ifdef _AFX
	if (bNew && m_hParent)
	{
		::PostMessage(m_hParent, WM_SERVICE_UPDATE, ID_DATA_AGV, (LPARAM)pAGV);
	}
#endif //_AFX

	CRFID* pCurRDIF = nullptr;	/*!< AGV当前RFID卡指针 */
	CRFID* pOldRDIF = nullptr;	/*!< AGV上一RFID卡指针 */

	if (m_mapRFIDs.find(pAGV->GetCurRFID()) == m_mapRFIDs.end())
	{
		pCurRDIF = m_mapRFIDs[pAGV->GetCurRFID()];
	}

	// 锁定RFID卡
	if (pCurRDIF)
	{
		pCurRDIF->Lock(pAGV);
	}

	if (m_mapRFIDs.find(pAGV->GetOldRFID()) == m_mapRFIDs.end())
	{
		pOldRDIF = m_mapRFIDs[pAGV->GetOldRFID()];
	}

	// 解锁RFID卡
	if (pOldRDIF)
	{
		pOldRDIF->Unlock(pAGV);
	}

	return;
}

void CService::UpdateAGVError(unsigned char byNo, char chError)
{
	if (m_mapAGVs.find(byNo) == m_mapAGVs.end())
	{
		return;
	}

	CTCPMNavAGV* pAGV = nullptr;	/*!< AGV指针 */
	pAGV = m_mapAGVs[byNo];

	if (pAGV->SetError(chError) == false)
	{
		return;
	}

#ifdef _AFX
	if (m_hParent)
	{
		::PostMessage(m_hParent, WM_SERVICE_UPDATE, ID_DATA_AGV, (LPARAM)pAGV);
	}
#endif //_AFX

	return;
}

void CService::UpdateAGVRFID(unsigned char byNo, unsigned short usRFID)
{
	if (m_mapAGVs.find(byNo) == m_mapAGVs.end())
	{
		return;
	}

	CTCPMNavAGV* pAGV = nullptr;	/*!< AGV指针 */
	pAGV = m_mapAGVs[byNo];

	if (pAGV->SetCurRFID(usRFID))
	{
#ifdef _AFX
		if (m_hParent)
		{
			::PostMessage(m_hParent, WM_SERVICE_UPDATE, ID_DATA_AGV, (LPARAM)pAGV);
		}
#endif //_AFX
	}

	CRFID* pCurRDIF = nullptr;	/*!< AGV当前RFID卡指针 */
	CRFID* pOldRDIF = nullptr;	/*!< AGV上一RFID卡指针 */

	if (m_mapRFIDs.find(pAGV->GetCurRFID()) == m_mapRFIDs.end())
	{
		pCurRDIF = m_mapRFIDs[pAGV->GetCurRFID()];
	}

	// 锁定RFID卡
	if (pCurRDIF)
	{
		pCurRDIF->Lock(pAGV);
	}

	if (m_mapRFIDs.find(pAGV->GetOldRFID()) == m_mapRFIDs.end())
	{
		pOldRDIF = m_mapRFIDs[pAGV->GetOldRFID()];
	}

	// 解锁RFID卡
	if (pOldRDIF)
	{
		pOldRDIF->Unlock(pAGV);
	}

	return;
}

#ifdef _TASK_H
void CService::ProcessTask()
{
	std::map<unsigned short,AGVTaskArr> taskList = m_TaskManager.GetList();

	if (taskList.size() == 0)
	{
		return;
	}

	for (std::map<unsigned short, AGVTaskArr>::iterator itArr = taskList.begin(); itArr != taskList.end(); ++itArr)
	{
		AGVTaskArr& arr = itArr->second;	/*!< 任务组 */

		std::map<unsigned char, AGVTask*> list = arr.GetTaskList();

		for (std::map<unsigned char, AGVTask*>::iterator it = list.begin(); it != list.end(); ++it)
		{
			AGVTask* pTask = it->second;	/*!< 任务指针 */

			if (pTask->GetStatus() == Task::_TASKSTA_NONE)
			{
			}
			else if (pTask->GetStatus() == Task::_TASKSTA_EXECUTE)
			{
			}
			else if (pTask->GetStatus() == Task::_TASKSTA_FINISH)
			{
				break;
			}
			else if (pTask->GetStatus() == Task::_TASKSTA_PAUSE)
			{
				break;
			}
			else if (pTask->GetStatus() == Task::_TASKSTA_CANCEL)
			{
				continue;
			}
		}
	}

	return;
}
#endif _TASK_H

#ifdef _SELECT_AUTO_GOUP
void CService::RestPositionControl()
{
	for (std::map<unsigned char, CMNavRestPosition*>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); ++it)
	{
		CMNavRestPosition* pRest = it->second;	/*!< 待机位 */

		unsigned short usRestRFID = pRest->GetLocation();	/*!< 待机位RFID卡编号 */

		assert(usRestRFID != 0);

		if (m_mapRFIDs.find(usRestRFID) == m_mapRFIDs.end())
		{
			// 无效的待机位RFID卡
			continue;
		}

		CRFID *pRestRFID = m_mapRFIDs[usRestRFID];	/*!< 待机位RFID卡 */

		CTCPMNavAGV *pAGV = (CTCPMNavAGV *)pRestRFID->GetUser();	/*!< 待机位AGV指针 */
		if (pAGV == nullptr)
		{
			// 无AGV在待机位
			continue;
		}

		// AGV不处于待机状态或处于手动模式或无电
		if (pAGV->GetStatus() != _AGVSTA_WAIT || pAGV->GetMode() == false || pAGV->GetPower() == 0)
		{
			continue;
		}

		bool bNext = true;	/*!< 下一阶段标识 */

		/*!
		 * 禁行RFID卡检测
		*/
		std::set<unsigned short> setStop = pRest->GetStopRFIDList();	/*!< 禁行RFID卡列表 */

		for (std::set<unsigned short>::iterator itStop = setStop.begin(); itStop != setStop.end(); ++itStop)
		{
			unsigned short usStopRFID = *itStop;	/*!< 禁行RFID卡编号 */

			if (usStopRFID == 0)
			{
				// 禁行点无效
				continue;
			}

			if (m_mapRFIDs.find(usStopRFID) == m_mapRFIDs.end())
			{
				// 禁行点无效
				continue;
			}

			CRFID *pStopRFID = m_mapRFIDs[usStopRFID];	/*!< 禁行RFID卡结构体 */

			if (pStopRFID->GetUser())
			{
				// 存在AGV在禁行RFID卡上
				bNext = false;
				break;
			}
		}

		if (bNext == false)
		{
			continue;
		}

		if (pRest->GetNext() == 0)
		{
			// 无效的待机位编号
			continue;
		}

		if (m_mapRests.find(pRest->GetNext()) == m_mapRests.end())
		{
			// 无效的待机位
			continue;
		}

		// CMNavRestPosition restNext(m_mapRests[rest.GetNext()]); /*!< 下一待机位 */
		CMNavRestPosition *pRestNext = m_mapRests.find(pRest->GetNext())->second; /*!< 下一待机位 */

		unsigned short usNextRestRFID = pRestNext->GetLocation();	/*!< 下一待机位RFID卡编号 */

		if (usNextRestRFID == 0)
		{
			// 无效的下一待机位RFID卡
			continue;
		}

		if (m_mapRFIDs.find(usNextRestRFID) == m_mapRFIDs.end())
		{
			// 无效的下一待机位RFID卡
			continue;
		}

		CRFID *pNextRestRFID = m_mapRFIDs[usNextRestRFID];	/*!< 下一待机位RFID卡 */

		if (pNextRestRFID->GetUser())
		{
			// 存在AGV在下一待机位
			continue;
		}

		/*!
		 * 禁行RFID卡检测
		*/
		std::set<unsigned short> setNextStop = pRestNext->GetStopRFIDList();	/*!< 禁行RFID卡列表 */

		for (std::set<unsigned short>::iterator itStop = setNextStop.begin(); itStop != setNextStop.end(); ++itStop)
		{
			unsigned short usStopRFID = *itStop;	/*!< 禁行RFID卡编号 */

			if (usStopRFID == 0)
			{
				// 禁行点无效
				continue;
			}

			if (m_mapRFIDs.find(usStopRFID) == m_mapRFIDs.end())
			{
				// 禁行点无效
				continue;
			}

			CRFID *pStopRFID = m_mapRFIDs[usStopRFID];	/*!< 禁行RFID卡结构体 */

			if (pStopRFID->GetUser())
			{
				// 存在AGV在禁行RFID卡上
				bNext = false;
				break;
			}
		}

		if (bNext == false)
		{
			continue;
		}

		/*!
		 * 通知AGV移动至下一待机位
		*/
		pAGV->MoveTo(usNextRestRFID);
	}

	return;
}

#ifdef _CHARGER_H
void CService::ChargerControl()
{
}
#endif // _CHARGER_H

#endif // !_SELECT_AUTO_GOUP

#ifdef _CA_H
void CService::PublishCallTask(unsigned int unNo, std::string strMsg)
{
}

void CService::PublishCallTask(unsigned int unNo, std::wstring strMsg)
{
}

#ifdef _AFX
void CService::PublishCallTask(unsigned int unNo, CString strMsg)
{
}
#endif // _AFX
#endif // _CA_H

bool CService::AGVScream(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "【用户User】";
	std::string strCtrl = "【急停】";
	std::string strAGV = "AGV";

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d",byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	//std::string strAction = "";				/*!< 动作 */
	//std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	//pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< 动作 */
	unsigned char byActStatus = 0;			/*!< 动作状态 */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// 离线
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV离线."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV离线.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于自动模式
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【模式:%s】.",strMode.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 已处于急停状态
	if (pAGV->GetStatus() == _AGVSTA_SCREAM)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV已处于【急停】状态."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV已处于" + strStatus + "状态.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 正在执行动作
	if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			if (MessageBox(m_hParent, _T("急停会使AGV正在执行的动作中断,是否急停?"), _T("动作警告"), MB_ICONWARNING | MB_YESNO) == IDNO)
			{
				return false;
			}
		}
#endif // _AFX
	}

	pAGV->Scream();

	strRecord = strUser + "操作" + strAGV + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::AGVRestart(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "【用户User】";
	std::string strCtrl = "【复位】";
	std::string strAGV = "AGV";

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	//std::string strAction = "";				/*!< 动作 */
	//std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	//pAGV->GetAction(strAction, strActStatus);

	//unsigned short usAction = 0;			/*!< 动作 */
	//unsigned char byActStatus = 0;			/*!< 动作状态 */
	//pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// 离线
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV离线."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV离线.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于自动模式
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败!AGV不能接受系统的控制:【模式:%s】.", strMode.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 已处于待机状态
	if (pAGV->GetStatus() == _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV已处于待机状态."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV已处于" + strStatus + "状态.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	pAGV->Restart();

	strRecord = strUser + "操作" + strAGV + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::AGVMove(unsigned char byAGVNo, unsigned short usRFIDNo)
{
	std::string strRecord = "";
	std::string strUser = "【用户User】";
	std::string strCtrl = "【移动】";
	std::string strAGV = "AGV";

	BufferString::std_string_format(strCtrl, "【移动】至【坐标:%d】", usRFIDNo);

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 无效的RFID卡编号
	if (usRFIDNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的移动坐标."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的移动坐标:%d", usRFIDNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在RFID卡配置信息
	if (m_mapRFIDs.find(usRFIDNo) == m_mapRFIDs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的移动坐标."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的移动坐标:%d", usRFIDNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	std::string strAction = "";				/*!< 动作 */
	std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< 动作 */
	unsigned char byActStatus = 0;			/*!< 动作状态 */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// 离线
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV离线."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV离线.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于自动模式
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【模式:%s】.",strMode.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于待机状态
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【运行状态:%s】.",strStatus.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 已到达移动终点
	if (pAGV->GetCurRFID() == usRFIDNo)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV已处于移动坐标上."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV已处于移动坐标上.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 正在执行动作
	if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV正在执行动作."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV正在执行动作:【动作:" + strAction + "】【动作状态:" + strActStatus + "】";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	pAGV->MoveTo(usRFIDNo);

	strRecord = strUser + "操作" + strAGV + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::AGVStopAction(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "【用户User】";
	std::string strCtrl = "【动作停止】";
	std::string strAGV = "AGV";

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	std::string strAction = "";				/*!< 动作 */
	std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< 动作 */
	unsigned char byActStatus = 0;			/*!< 动作状态 */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// 离线
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV离线."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strAction + "失败!AGV离线.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于自动模式
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【模式:%s】.",strMode.c_str());
		strRecord = strUser + "操作" + strAGV + strAction + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于待机状态
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【运行状态:%s】.",strStatus.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	if (usAction == _AGVACT_NONE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV未执行动作."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV未执行动作.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	if (usAction == _AGVACT_TRAFFIC)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV无法停止该动作."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV无法停止该动作:" + strAction;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	if (byActStatus == _AGVACTSTA_FINISH || byActStatus == _AGVACTSTA_NONE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV动作已完成或未执行."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV动作已完成或未执行.【动作:" + strAction + "】【动作状态:" + strActStatus + "】";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	pAGV->StopAction();

	strRecord = strUser + "操作" + strAGV  + strCtrl + "【动作:" + strAction + "】【动作状态:" + strActStatus + "】";
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::AGVLoad(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "【用户User】";
	std::string strCtrl = "【上料】";
	std::string strAGV = "AGV";

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	std::string strAction = "";				/*!< 动作 */
	std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< 动作 */
	unsigned char byActStatus = 0;			/*!< 动作状态 */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// 离线
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV离线."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV离线.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于自动模式
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【模式:%s】.",strMode.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于待机状态
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【运行状态:%s】.",strStatus.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 已满载
	if (pAGV->GetCargo() == 0xFF)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV已满载."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV已满载.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 正在上料
	if (usAction == _AGVACT_LOAD)
	{
		// 未上料完成
		if (byActStatus != _AGVACTSTA_FINISH)
		{
#ifdef _AFX
			if (m_hParent)
			{
				MessageBox(m_hParent, _T("操作失败!AGV正在执行上料动作."), _T("用户操作"), MB_ICONERROR);
			}
#endif // _AFX

			strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV正在执行" + strAction;
			// 输出DEBUG
			DebugPrint(strRecord.c_str());
			// 记录用户操作
			Record(strRecord);
			return false;
		}
		else
		{
			if (MessageBox(m_hParent, _T("已执行过上料动作,是否再次执行?"), _T("动作警告"), MB_ICONWARNING | MB_YESNO) == IDNO)
			{
				return false;
			}
		}
	}
	// 有动作且正在执行
	else if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV正在执行其他动作."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV正在执行其他动作.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	pAGV->Load();

	strRecord = strUser + "操作" + strAGV + strCtrl + "【动作:" + strAction + "】【动作状态:" + strActStatus + "】";
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::AGVUnload(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "【用户User】";
	std::string strCtrl = "【下料】";
	std::string strAGV = "AGV";

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	std::string strAction = "";				/*!< 动作 */
	std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< 动作 */
	unsigned char byActStatus = 0;			/*!< 动作状态 */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// 离线
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV离线."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV离线.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于自动模式
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【模式:%s】.",strMode.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于待机状态
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【运行状态:%s】.",strStatus.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 空载
	if (pAGV->GetCargo() == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV空载."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV空载.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 下料
	if (usAction == _AGVACT_UNLOAD)
	{
		// 未下料完成
		if (byActStatus != _AGVACTSTA_FINISH)
		{
#ifdef _AFX
			if (m_hParent)
			{
				MessageBox(m_hParent, _T("操作失败!AGV正在执行下料动作."), _T("用户操作"), MB_ICONERROR);
			}
#endif // _AFX

			strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV正在执行" + strAction;
			// 输出DEBUG
			DebugPrint(strRecord.c_str());
			// 记录用户操作
			Record(strRecord);
			return false;
		}
		else
		{
			if (MessageBox(m_hParent, _T("已执行过下料动作,是否再次执行?"), _T("动作警告"), MB_ICONWARNING | MB_YESNO) == IDNO)
			{
				return false;
			}
		}
	}
	// 有动作且未执行完成
	else if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV正在执行其他动作."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV正在执行其他动作.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	pAGV->Unload();

	strRecord = strUser + "操作" + strAGV + strCtrl + "【动作:" + strAction + "】【动作状态:" + strActStatus + "】";
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::AGVAction(unsigned char byAGVNo, unsigned short usActionCode)
{
	std::string strRecord = "";
	std::string strUser = "【用户User】";
	std::string strCtrl = "【其他动作】";
	std::string strAGV = "AGV";

	BufferString::std_string_format(strCtrl, "【执行动作:%d】", usActionCode);

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	std::string strAction = "";				/*!< 动作 */
	std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< 动作 */
	unsigned char byActStatus = 0;			/*!< 动作状态 */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// 离线
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV离线."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV离线.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于自动模式
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【模式:%s】.",strMode.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于待机状态
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【运行状态:%s】.",strStatus.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 正在执行此动作
	if (usAction == usActionCode)
	{
		// 动作未完成
		if (byActStatus != _AGVACTSTA_FINISH)
		{
#ifdef _AFX
			if (m_hParent)
			{
				MessageBox(m_hParent, _T("操作失败!AGV正在执行此动作."), _T("用户操作"), MB_ICONERROR);
			}
#endif // _AFX

			strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV正在执行" + strAction;
			// 输出DEBUG
			DebugPrint(strRecord.c_str());
			// 记录用户操作
			Record(strRecord);
			return false;
		}
		else
		{
			if (MessageBox(m_hParent, _T("已执行过此动作,是否再次执行?"), _T("动作警告"), MB_ICONWARNING | MB_YESNO) == IDNO)
			{
				return false;
			}
		}
	}
	// 有动作且未执行完成
	else if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV正在执行其他动作."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV正在执行其他动作.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	pAGV->Action(usAction);

	strRecord = strUser + "操作" + strAGV + strCtrl + "【动作:" + strAction + "】【动作状态:" + strActStatus + "】";
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::AGVRemove(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "【用户User】";
	std::string strCtrl = "【重新移动】";
	std::string strAGV = "AGV";

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	std::string strAction = "";				/*!< 动作 */
	std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< 动作 */
	unsigned char byActStatus = 0;			/*!< 动作状态 */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// 离线
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV离线."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV离线.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于自动模式
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【模式:%s】.",strMode.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于待机状态
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【运行状态:%s】.",strStatus.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 正在执行动作
	if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV正在执行动作."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV正在执行动作:【动作:" + strAction + "】【动作状态:" + strActStatus + "】";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	unsigned short usEndRFID = pAGV->GetEndRFID();	/*!< 终点RFID卡编号 */
	unsigned short usCurRFID = pAGV->GetCurRFID();	/*!< 当前RFID卡编号 */

	// 终点坐标与当前坐标均无效
	if (usEndRFID == 0 && usCurRFID == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!不存在有效的移动坐标."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!不存在有效的移动坐标.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	unsigned short usMoveRFID = 0;	/*!< 移动RFID卡编号 */

	if (usEndRFID != 0)
	{
		usMoveRFID = usEndRFID;
	}
	else
	{
		usMoveRFID = usCurRFID;
	}

	BufferString::std_string_format(strAction, "【重新移动】至【坐标:%d】", usMoveRFID);

	pAGV->Remove(usMoveRFID);

	strRecord = strUser + "操作" + strAGV + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::AGVReset(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "【管理员Admin】";
	std::string strCtrl = "【重置】";
	std::string strAGV = "AGV";

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	//std::string strAction = "";				/*!< 动作 */
	//std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	//pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< 动作 */
	unsigned char byActStatus = 0;			/*!< 动作状态 */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// 离线
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV离线."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV离线.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于自动模式
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【模式:%s】.",strMode.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于待机状态
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【运行状态:%s】.",strStatus.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 正在执行动作
	if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			if (MessageBox(m_hParent, _T("重置会使AGV正在执行的动作中断,是否重置?"), _T("动作警告"), MB_ICONWARNING | MB_YESNO) == IDNO)
			{
				return false;
			}
		}
#endif // _AFX
	}

	pAGV->Reset();

	strRecord = strUser + "操作" + strAGV + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return false;
}

bool CService::AGVSet(unsigned char byAGVNo, unsigned short usRFIDNo, unsigned char byStatus)
{
	std::string strRecord = "";
	std::string strUser = "【管理员Admin】";
	std::string strCtrl = "【设置当前坐标与状态】";
	std::string strAGV = "AGV";

	BufferString::std_string_format(strCtrl, "【设置当前坐标与状态】【坐标:%d】【状态:%d】", usRFIDNo, byStatus);

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	//std::string strAction = "";				/*!< 动作 */
	//std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	//pAGV->GetAction(strAction, strActStatus);

	//unsigned short usAction = 0;			/*!< 动作 */
	//unsigned char byActStatus = 0;			/*!< 动作状态 */
	//pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// 离线
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV离线."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV离线.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于自动模式
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【模式:%s】.",strMode.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	pAGV->ManualSet(usRFIDNo, byStatus);

	strRecord = strUser + "操作" + strAGV + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::AGVTrafficPass(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "【管理员Admin】";
	std::string strCtrl = "【交通管制通行】";
	std::string strAGV = "AGV";

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	std::string strAction = "";				/*!< 动作 */
	std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< 动作 */
	unsigned char byActStatus = 0;			/*!< 动作状态 */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	BufferString::std_string_format(strCtrl, "【交通管制通行】【坐标:%d】", pAGV->GetCurRFID());

	// 离线
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV离线."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV离线.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于自动模式

	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不能接受系统的控制:【模式:%s】.",strMode.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于交通管制状态
	if (pAGV->GetStatus() != _AGVSTA_TRAFFIC)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不处于交通管制状态."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"失败!AGV不处于交通管制状态:【运行状态:%s】.",strStatus.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 正在执行动作
	if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV正在执行动作."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV正在执行动作:【动作:" + strAction + "】【动作状态:" + strActStatus + "】";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	pAGV->TrafficPass();

	strRecord = strUser + "操作" + strAGV + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::AGVHandleMode(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "【管理员Admin】";
	std::string strCtrl = "【切换手动模式】";
	std::string strAGV = "AGV";

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	//std::string strAction = "";				/*!< 动作 */
	//std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	//pAGV->GetAction(strAction, strActStatus);

	//unsigned short usAction = 0;			/*!< 动作 */
	//unsigned char byActStatus = 0;			/*!< 动作状态 */
	//pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV已处于手动模式."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV已处于" + strMode;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

#ifdef _AFX
	AGVTask* pTask = (AGVTask*)pAGV->GetTask();	/*!< AGV任务 */
	// 有任务且正在执行
	if (pTask && pTask->GetStatus() == Task::_TASKSTA_EXECUTE)
	{
		if (m_hParent)
		{
			if (MessageBox(m_hParent, _T("切换为手动模式将会中断AGV正在执行的任务,是否切换?"), _T("用户操作"), MB_ICONWARNING | MB_YESNO) == IDNO)
			{
				return false;
			}
		}
	}
#endif // _AFX

	pAGV->SetMode(false);

	strRecord = strUser + "操作" + strAGV + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::AGVAutoMode(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "【管理员Admin】";
	std::string strCtrl = "【切换自动模式】";
	std::string strAGV = "AGV";

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	//std::string strAction = "";				/*!< 动作 */
	//std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	//pAGV->GetAction(strAction, strActStatus);

	//unsigned short usAction = 0;			/*!< 动作 */
	//unsigned char byActStatus = 0;			/*!< 动作状态 */
	//pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	if (pAGV->GetMode() == true)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV已处于自动模式."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV已处于" + strMode;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	pAGV->SetMode(true);

	strRecord = strUser + "操作" + strAGV + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::AGVShutdown(unsigned char byAGVNo)
{
	std::string strRecord = "";	
	std::string strUser = "【管理员Admin】";
	std::string strCtrl = "【关机】";
	std::string strAGV = "AGV";

	// 无效的AGV编号
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 该编号不存在AGV配置信息
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无效的AGV编号."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败！无效的AGV编号:%d", byAGVNo);
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGV指针 */

	std::string strIP = "";					/*!< IP地址 */
	std::string strDrive = "";				/*!< 可运动方向 */
	std::string strType = "";				/*!< 类型 */
	std::string strMode = "";				/*!< 模式 */
	std::string strStatus = "";				/*!< 状态 */
	std::string strAction = "";				/*!< 动作 */
	std::string strActStatus = "";			/*!< 动作状态 */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< 动作 */
	unsigned char byActStatus = 0;			/*!< 动作状态 */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV【编号:%d】【IP:%s:%d】【类型:%s】【坐标:%d】【状态:%s】",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// 离线
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV离线."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV离线.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于自动模式
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败!AGV不能接受系统的控制:【模式:%s】.", strMode.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 不处于待机状态
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV不能接受系统的控制."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "失败!AGV不能接受系统的控制:【运行状态:%s】.", strStatus.c_str());
		strRecord = strUser + "操作" + strAGV + strCtrl + strRecord;
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 正在执行动作
	if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!AGV正在执行动作."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strAGV + strCtrl + "失败!AGV正在执行动作:【动作:" + strAction + "】【动作状态:" + strActStatus + "】";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	pAGV->Shutdown();

	strRecord = strUser + "操作" + strAGV + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

#ifdef _TASK_H
AGVTask CService::GetTask(unsigned short usNo, unsigned char byLevel)
{
	return m_TaskManager.Get(usNo, byLevel);
}

bool CService::EditTask(unsigned short usNo, unsigned char byLevel, AGVTask newTask)
{
	std::string strRecord = "";					/*!< 日志文本 */
	std::string strUser = "【管理员Admin】";	/*!< 操作用户 */
	std::string strCtrl = "【修改】";			/*!< 操作 */
	std::string strTask = "任务";				/*!< 任务 */

	BufferString::std_string_format(strTask, "任务【任务号:%d】【阶段号:%d】", usNo, byLevel);

	AGVTask task = m_TaskManager.Get(usNo, byLevel);	/*!< 任务指针 */

	// 未查找到任务
	if (task.IsNull())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!未查找到任务."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "操作" + strTask + strCtrl + "失败!未查找到任务.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	std::string strStatus = "";	/*!< 任务状态 */

	task.GetStatus(strStatus);

	BufferString::std_string_format(strTask, "任务【任务号:%d】【阶段号:%d】【任务坐标:%d】【起始坐标:%d】【任务动作:%s】【任务状态:%s】",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), strStatus.c_str());

	// 任务不符合
	if (task.GetNo() != newTask.GetNo() || task.GetLevel() != newTask.GetLevel())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!修改内容与原任务不符."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "操作" + strTask + strCtrl + "失败!修改内容与原任务不符.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	std::string strNewAction = "";		/*!, 新任务动作 */
	std::string strNewError = "";		/*!< 新异常信息 */
	newTask.GetAction(strNewAction);
	newTask.GetError(strNewError);

	// 任务内容不变
	if (task.GetAction() == strNewAction && newTask.GetTarget() == task.GetTarget() && newTask.GetBegin() == task.GetBegin())
	{
#ifdef _AFX
			if (m_hParent)
			{
				MessageBox(m_hParent, _T("操作失败!修改内容未修改."), _T("用户操作"), MB_ICONERROR);
			}
#endif // _AFX
		strRecord = strUser + "操作" + strTask + strCtrl + "失败!修改内容未修改.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

#ifdef _AFX
	if (m_hParent)
	{
		if (MessageBox(m_hParent, _T("修改后将重新执行修改后的任务,是否修改?"), _T("用户操作"), MB_ICONWARNING | MB_YESNO) != IDYES)
		{
			return false;
		}
	}
#endif // _AFX

	// *pTask = newTask;

	if (m_TaskManager.Edit(newTask) == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!数据库写入失败."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	strRecord = strUser + "操作" + strTask + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::CancelTask(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";					/*!< 日志文本 */
	std::string strUser = "【管理员Admin】";	/*!< 操作用户 */
	std::string strCtrl = "【取消】";			/*!< 操作 */
	std::string strTask = "任务";				/*!< 任务 */

	BufferString::std_string_format(strTask, "任务【任务号:%d】【阶段号:%d】", usNo, byLevel);

	AGVTask task = m_TaskManager.Get(usNo, byLevel);	/*!< 任务指针 */

	// 未查找到任务
	if (task.IsNull())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!未查找到任务."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "操作" + strTask + strCtrl + "失败!未查找到任务.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	std::string strStatus = "";	/*!< 任务状态 */

	task.GetStatus(strStatus);

	BufferString::std_string_format(strTask, "任务【任务号:%d】【阶段号:%d】【任务坐标:%d】【起始坐标:%d】【任务动作:%s】【任务状态:%s】",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), strStatus.c_str());

	if (task.GetStatus() == Task::_TASKSTA_CANCEL || task.GetStatus() == Task::_TASKSTA_FINISH)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无法取消此状态的任务."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "操作" + strTask + strCtrl + "失败!无法取消此状态的任务.【状态:" + strStatus + "】";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	// 执行SQL语句
	if (m_TaskManager.Cancel(usNo, byLevel) == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!数据库写入失败."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	strRecord = strUser + "操作" + strTask + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::FinishTask(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";					/*!< 日志文本 */
	std::string strUser = "【管理员Admin】";	/*!< 操作用户 */
	std::string strCtrl = "【强制完成】";		/*!< 操作 */
	std::string strTask = "任务";				/*!< 任务 */

	BufferString::std_string_format(strTask, "任务【任务号:%d】【阶段号:%d】", usNo, byLevel);

	AGVTask task = m_TaskManager.Get(usNo, byLevel);	/*!< 任务指针 */

	// 未查找到任务
	if (task.IsNull())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!未查找到任务."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "操作" + strTask + strCtrl + "失败!未查找到任务.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	std::string strStatus = "";	/*!< 任务状态 */

	task.GetStatus(strStatus);

	BufferString::std_string_format(strTask, "任务【任务号:%d】【阶段号:%d】【任务坐标:%d】【起始坐标:%d】【任务动作:%s】【任务状态:%s】",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), strStatus.c_str());

#ifdef _AFX
	if (m_hParent)
	{
		if (MessageBox(m_hParent, _T("强制完成任务存在风险,是否强制完成?"), _T("用户操作"), MB_ICONWARNING | MB_YESNO) != IDYES)
		{
			return false;
		}
	}
#endif // _AFX

	// 执行SQL语句
	if (m_TaskManager.Finish(usNo, byLevel) == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!数据库写入失败."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
	}

	strRecord = strUser + "操作" + strTask + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::PauseTask(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";					/*!< 日志文本 */
	std::string strUser = "【管理员Admin】";	/*!< 操作用户 */
	std::string strCtrl = "【暂停】";			/*!< 操作 */
	std::string strTask = "任务";				/*!< 任务 */

	BufferString::std_string_format(strTask, "任务【任务号:%d】【阶段号:%d】", usNo, byLevel);

	AGVTask task = m_TaskManager.Get(usNo, byLevel);	/*!< 任务指针 */

	// 未查找到任务
	if (task.IsNull())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!未查找到任务."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "操作" + strTask + strCtrl + "失败!未查找到任务.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);

		return false;
	}

	std::string strStatus = "";	/*!< 任务状态 */

	task.GetStatus(strStatus);

	BufferString::std_string_format(strTask, "任务【任务号:%d】【阶段号:%d】【任务坐标:%d】【起始坐标:%d】【任务动作:%s】【任务状态:%s】",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), strStatus.c_str());

	/*!
	 * 检测任务是否可以暂停
	*/
	// 已完成或已取消的任务
	if (task.GetStatus() == Task::_TASKSTA_FINISH || task.GetStatus() == Task::_TASKSTA_CANCEL || task.GetStatus() == Task::_TASKSTA_PAUSE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无法暂停此状态的任务."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "操作" + strTask + strCtrl + "失败!无法暂停此状态的任务.【状态:" + strStatus + "】";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);

		return false;
	}
	
	// 执行SQL语句
	if (m_TaskManager.Pause(usNo, byLevel) == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!数据库写入失败."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	strRecord = strUser + "操作" + strTask + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::ResetTask(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";					/*!< 日志文本 */
	std::string strUser = "【管理员Admin】";	/*!< 操作用户 */
	std::string strCtrl = "【重置】";			/*!< 操作 */
	std::string strTask = "任务";				/*!< 任务 */

	BufferString::std_string_format(strTask, "任务【任务号:%d】【阶段号:%d】", usNo, byLevel);

	AGVTask task = m_TaskManager.Get(usNo, byLevel);	/*!< 任务指针 */

	// 未查找到任务
	if (task.IsNull())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!未查找到任务."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "操作" + strTask + strCtrl + "失败!未查找到任务.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);
		return false;
	}

	std::string strStatus = "";	/*!< 任务状态 */

	task.GetStatus(strStatus);

	BufferString::std_string_format(strTask, "任务【任务号:%d】【阶段号:%d】【任务坐标:%d】【起始坐标:%d】【任务动作:%s】【任务状态:%s】",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), strStatus.c_str());

	// 执行SQL语句
	if (m_TaskManager.Reset(usNo, byLevel) == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!数据库写入失败."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	strRecord = strUser + "操作" + strTask + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

void CService::CancelAllTasks()
{
	std::string strRecord = "";					/*!< 日志文本 */
	std::string strUser = "【管理员Admin】";	/*!< 操作用户 */
	std::string strCtrl = "【取消】";			/*!< 操作 */
	std::string strTask = "全部任务";			/*!< 任务 */

	// 执行SQL语句
	if (m_TaskManager.CancelAll() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!数据库写入失败."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		return;
	}

	strRecord = strUser + "操作" + strTask + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return;
}

void CService::PauseAllTasks()
{
	std::string strRecord = "";					/*!< 日志文本 */
	std::string strUser = "【管理员Admin】";	/*!< 操作用户 */
	std::string strCtrl = "【暂停】";			/*!< 操作 */
	std::string strTask = "全部任务";			/*!< 任务 */

	// 执行SQL语句
	if (m_TaskManager.PauseAll() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!数据库写入失败."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		return;
	}

	strRecord = strUser + "操作" + strTask + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return;
}

void CService::ResetAllTasks()
{
	std::string strRecord = "";					/*!< 日志文本 */
	std::string strUser = "【管理员Admin】";	/*!< 操作用户 */
	std::string strCtrl = "【重置】";			/*!< 操作 */
	std::string strTask = "全部任务";			/*!< 任务 */

	// 执行SQL语句
	if (m_TaskManager.ResetAll() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!数据库写入失败."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		return;
	}

	strRecord = strUser + "操作" + strTask + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return;
}

bool CService::InsertTask(unsigned short usNo, unsigned char byLevel, AGVTask newTask)
{
	std::string strRecord = "";					/*!< 日志文本 */
	std::string strUser = "【管理员Admin】";	/*!< 操作用户 */
	std::string strCtrl = "【插入】";			/*!< 操作 */
	std::string strTask = "任务";				/*!< 任务 */

	BufferString::std_string_format(strTask, "任务【任务号:%d】【阶段号:%d】【任务坐标:%d】【起始坐标:%d】【任务动作:%s】",
		newTask.GetNo(), newTask.GetLevel(), newTask.GetTarget(), newTask.GetBegin(), newTask.GetAction().c_str());

	if (newTask.GetNo() != usNo)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!无法插入任务号不相同的任务."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strTask + strCtrl + "失败!无法插入任务号不相同的任务.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);

		return false;
	}

	AGVTask task = m_TaskManager.Get(usNo, byLevel);	/*!< 任务指针 */

	// 未查找到任务
	if (task.IsNull())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!未查找到任务."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "操作" + strTask + strCtrl + "失败!未查找到任务.";
		// 输出DEBUG
		DebugPrint(strRecord.c_str());
		// 记录用户操作
		Record(strRecord);

		return false;
	}

	if (task.GetStatus() == Task::_TASKSTA_FINISH)
	{
#ifdef _AFX
		if (m_hParent)
		{
			if (MessageBox(m_hParent, _T("插入新任务后，当前任务会重置.是否插入?"), _T("用户操作"), MB_ICONWARNING | MB_YESNO) != IDYES)
			{
				return false;
			}
		}
#endif //_AFX
	}

	if (m_TaskManager.Insert(usNo, byLevel, newTask) == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("操作失败!数据库写入失败."), _T("用户操作"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	strRecord = strUser + "操作" + strTask + strCtrl;
	// 输出DEBUG
	DebugPrint(strRecord.c_str());
	// 记录用户操作
	Record(strRecord);

	return true;
}

bool CService::AddTask(AGVTask newTask)
{
	return false;
}
#endif // _TASK_H
#endif // !_MAGNETICTRACK_H
#endif // !_SERVICE