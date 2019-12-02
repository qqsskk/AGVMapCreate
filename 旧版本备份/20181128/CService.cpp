#include "stdafx.h"
#include "CService.h"

#ifdef _SERVICE_H

CService::CService()
{
	m_pTcpServer = nullptr;
	m_pCA = nullptr;
	m_pArm = nullptr;
	m_pStocker = nullptr;

	m_pThreadService = nullptr;
	m_hShutdownEvent = nullptr;

	for (int i = 0; i < 5; i++)
	{
		m_strLed[i] = "";
	}

	m_unNo = 0;
	m_unRow = 0;
	m_unCol = 0;
}

CService::~CService()
{
	Close();
}

bool CService::Open()
{
	LoadConfig(L"../Data/WMSConfig.xml");

	if (m_adoDB.OnInitADOConn() == false)
	{
		TRACE(_T("连接服务器失败.\r\n"));
		AfxMessageBox(L"连接服务器失败.");
		return false;
	}

	if (m_pTcpServer == nullptr || m_pTcpServer->BeginListen() == false)
	{
		TRACE(_T("创建服务端失败.\r\n"));
		AfxMessageBox(L"创建服务端失败.");
		return false;
	}

	StartThread();

	OpenScreen();

	return true;
}

void CService::Close()
{
	CloseThread();

	lock_guard<mutex> lock(m_lock);

	if (m_pTcpServer)
	{
		delete m_pTcpServer;
		m_pTcpServer = nullptr;
	}

	if (m_pCA)
	{
		delete m_pCA;
		m_pCA = nullptr;
	}

	if (m_pArm)
	{
		delete m_pArm;
		m_pArm = nullptr;
	}

	for (map<unsigned int, CPipeline*>::iterator it = m_mapPipeline.begin(); it != m_mapPipeline.end();)
	{
		delete it->second;
		it = m_mapPipeline.erase(it);
	}

	if (m_pStocker)
	{
		delete m_pStocker;
		m_pStocker = nullptr;
	}

	for (list<CTcpClient*>::iterator it = m_listTcpClients.begin(); it != m_listTcpClients.end();)
	{
		delete *it;
		it = m_listTcpClients.erase(it);
	}

	CloseScreen();
}

bool CService::AutoGetOut(unsigned int unNo, unsigned int unRow, unsigned int unCol)
{
	_CARGO cargo = GetCargo(m_adoDB, unNo, unRow, unCol);

	if (cargo.IsNull())
	{
		AfxMessageBox(_T("出库失败!该货位不存在货物!"));
		return false;
	}

	if (GetAllWMSTask(m_adoDB).size() != 0)
	{
		AfxMessageBox(_T("出库失败!当前任务未完成!"));
		return false;
	}

	return CreateWMSTask(m_adoDB,unNo,unRow,unCol,WMSTASKTYPE_OUT);
}

bool CService::GetOut(unsigned int unNo, unsigned int unRow, unsigned int unCol)
{
	return ClearIn(m_adoDB,unNo,unRow,unCol);
}

bool CService::PutIn(unsigned int unNo, unsigned int unRow, unsigned int unCol)
{
	_CARGO cargo = GetCargo(m_adoDB, unNo, unRow, unCol);

	if (cargo.IsNull() == false)
	{
		AfxMessageBox(_T("入库失败!该货位已存在货物!"));
		return false;
	}

	return RecordIn(m_adoDB,unNo,unRow,unCol,"瓷砖","块",1);
}

void CService::LoadConfig(wstring wstrPath)
{
	string strPath = _bstr_t(wstrPath.c_str);

	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(strPath.c_str()) == false)
	{
		TRACE(_T("读取配置信息失败.\r\n"));
		return;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL || pXmlRoot->NoChildren())
	{
		TRACE(_T("读取配置信息失败.\r\n"));
		return;
	}

	TiXmlElement *pXmlElemt = pXmlRoot->FirstChildElement(); /*!< 子节点*/

	while (pXmlElemt)
	{
		CString cstrElemtName(pXmlElemt->Value()); /*!< 子节点名称 */

		/*!
		 * 读取服务端信息
		*/
		if (cstrElemtName == _T("Server"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			wstring wstrIP = L"";		/*!< IP地址 */
			unsigned int unPort = 0;	/*!< 端口 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("IP"))
				{
					wstrIP = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Port"))
				{
					unPort = _ttoi(cstrAttrValue);
				}
			}

			if (m_pTcpServer)
			{
				delete m_pTcpServer;
			}

			m_pTcpServer = new CTcpServer(unPort, wstrIP);
		}

		/*!
		 * 读取CA主机信息
		*/
		else if (cstrElemtName == _T("CA"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			unsigned int unNo = 0;		/*!< 编号 */
			wstring wstrIP = L"";		/*!< IP地址 */
			unsigned int unPort = 0;	/*!< 端口 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					unNo = _ttoi(cstrAttrValue);
				}
				else if(cstrAttrName == _T("IP"))
				{
					wstrIP = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Port"))
				{
					unPort = _ttoi(cstrAttrValue);
				}
			}

			if (m_pCA)
			{
				delete m_pCA;
			}

			m_pCA = new CCA(unNo, wstrIP, unPort);
		}

		/*!
		 * 读取机械手信息
		*/
		else if (cstrElemtName == _T("Arm"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			unsigned int unNo = 0;		/*!< 编号 */
			wstring wstrIP = L"";		/*!< IP地址 */
			unsigned int unPort = 0;	/*!< 端口 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					unNo = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("IP"))
				{
					wstrIP = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Port"))
				{
					unPort = _ttoi(cstrAttrValue);
				}
			}

			if (m_pArm)
			{
				delete m_pArm;
			}

			m_pArm = new CArm(unNo, wstrIP, unPort);
		}

		/*!
		 * 读取输送线信息
		*/
		else if (cstrElemtName == _T("Pipeline"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			unsigned int unNo = 0;		/*!< 编号 */
			wstring wstrIP = L"";		/*!< IP地址 */
			unsigned int unPort = 0;	/*!< 端口 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					unNo = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("IP"))
				{
					wstrIP = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Port"))
				{
					unPort = _ttoi(cstrAttrValue);
				}
			}

			if (m_mapPipeline.find(unNo) != m_mapPipeline.end())
			{
				delete m_mapPipeline[unNo];
			}

			m_mapPipeline[unNo] = new CPipeline(unNo, wstrIP, unPort);
		}

		/*!
		 * 读取堆垛机信息
		*/
		else if (cstrElemtName == _T("Stocker"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			unsigned int unNo = 0;		/*!< 编号 */
			wstring wstrIP = L"";		/*!< IP地址 */
			unsigned int unPort = 0;	/*!< 端口 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					unNo = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("IP"))
				{
					wstrIP = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Port"))
				{
					unPort = _ttoi(cstrAttrValue);
				}
			}

			if (m_pStocker)
			{
				delete m_pStocker;
			}

			m_pStocker = new CStocker(unNo, wstrIP, unPort);
		}


		pXmlElemt = pXmlElemt->NextSiblingElement();
	}

	TRACE(_T("读取配置信息完成.\r\n"));
	return;
}

void CService::ClientConnect()
{
	if (m_pTcpServer == nullptr)
	{
		return;
	}

	SOCKET socket = m_pTcpServer->GetClient();

	if (socket == INVALID_SOCKET)
	{
		return;
	}

	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);

	//根据套接字获取地址信息
	if (::getpeername(socket, (SOCKADDR*)&addrClient, &addrClientlen) != 0)
	{
		TRACE(_T("从SOCKET套接字中获取IP地址信息失败！\r\n"));
		return;
	}

	wstring wstrIp = L""; /*!< IP地址 */
	wstrIp = _bstr_t(inet_ntoa(addrClient.sin_addr));

	unsigned int unPort = 0; /*!< 端口 */
	unPort = htons(addrClient.sin_port);

	CTcpClient *pTcpClient = new CTcpClient(wstrIp, unPort);
	pTcpClient->Connect(socket);

	/*!
	 * 与CA主机进行匹配
	*/
	if (m_pCA)
	{
		if (m_pCA->m_wstrIP == wstrIp && m_pCA->m_unPort == unPort)
		{
			m_pCA->Connect(pTcpClient);
		}
	}

	/*!
	 * 与机械手进行匹配
	*/
	if (m_pArm)
	{
		if (m_pArm->m_wstrIP == wstrIp &&(m_pArm->m_unPort == 0 || m_pArm->m_unPort == unPort))
		{
			m_pArm->Connect(pTcpClient);
		}
	}

	/*!
	 * 与输送线进行匹配
	*/
	for (map<unsigned int, CPipeline*>::iterator it = m_mapPipeline.begin(); it != m_mapPipeline.end();it++)
	{
		CPipeline *pPipeline = it->second;

		if (pPipeline->m_wstrIP == wstrIp && (pPipeline->m_unPort == 0 || pPipeline->m_unPort == unPort))
		{
			pPipeline->Connect(pTcpClient);
		}
	}

	/*!
	 * 与堆垛机进行匹配
	*/
	if (m_pStocker)
	{
		if (m_pStocker->m_wstrIP == wstrIp && (m_pStocker->m_unPort == 0 || m_pStocker->m_unPort == unPort))
		{
			m_pStocker->Connect(pTcpClient);
		}
	}

	if (pTcpClient->GetLinkCount() == 0)
	{
		pTcpClient->Break();
		delete pTcpClient;
	}
	else
	{
		if (m_listTcpClients.max_size() == m_listTcpClients.size())
		{
			ClearFreeClient();
		}

		lock_guard<mutex> lock(m_lock);

		m_listTcpClients.push_back(pTcpClient);
	}

	return;
}

void CService::ClearFreeClient()
{
	lock_guard<mutex> lock(m_lock);

	for (list<CTcpClient*>::iterator it = m_listTcpClients.begin(); it != m_listTcpClients.end();)
	{
		CTcpClient* pTcpClient = *it;

		if (pTcpClient->GetLinkCount() != 0)
		{
			it++;
			continue;
		}

		if (pTcpClient->IsConnected())
		{
			pTcpClient->Break();
		}

		delete *it;

		it = m_listTcpClients.erase(it);
	}

	return;
}

void CService::StartThread()
{
	if (m_pThreadService != nullptr)
	{
		CloseThread();
	}

	// 初始化线程关闭事件
	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(nullptr, true, false, nullptr);
	}

	m_pThreadService = new thread(&CService::ServiceThread, this);

	return;
}

void CService::CloseThread()
{
	if (m_pThreadService)
	{
		if (m_pThreadService->joinable())
		{
			SetEvent(m_hShutdownEvent);

			m_pThreadService->join();
		}

		delete m_pThreadService;
		m_pThreadService = nullptr;
	}

	return;
}

void CService::ServiceThread()
{
	while (true)
	{
		ClientConnect();

		ProcessTask();

		GetCallMsg();

		if (WaitForSingleObject(m_hShutdownEvent, 1000) == WAIT_OBJECT_0)
		{
			break;
		}
	}

	return;
}

void CService::ProcessTask()
{
	list<_WMSTASK> listWMS;

	listWMS = GetAllWMSTask(m_adoDB);

	for (list<_WMSTASK>::iterator it = listWMS.begin(); it != listWMS.end(); it++)
	{
		_WMSTASK& task = *it;

		if (task.m_strType == WMSTASKTYPE_IN)
		{
			ProcessTaskIn(task);
		}
		else if (task.m_strType == WMSTASKTYPE_OUT)
		{
			ProcessTaskOut(task);
		}

		/*!
		 * 每次只处理1条任务
		 * 当1条任务完成后,再处理下一条任务
		*/
		if (task.m_unStatus != TASKIN_FINISH || task.m_unStatus != TASKOUT_FINISH)
		{
			break;
		}
	}

	return;
}

void CService::ProcessTaskOut(_WMSTASK & task)
{
	/*!
	 * 接收任务,开始处理任务
	 * 任务进入阶段1
	*/
	if (task.m_unStatus == TASKOUT_NONE)
	{
		UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL1_OUT);

		CStringA cstraText = "";
		cstraText.Format("出库开始:%d行,%d列.\r\n", task.m_unRow, task.m_unCol);
		SendToLED(cstraText.GetBuffer());
	}
	/*!
	 * 阶段1.1 出库
	 * 此阶段从立体库取出货物运送至平台
	 * 控制机械手AGV移动至平台
	 * 控制潜入式AGV到达等待位
	 * 控制背负式AGV到达输送线1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL1_OUT)
	{
		_STOCKERTASK stockerTask = m_pStocker->GetTask();	/*!< 堆垛机任务 */

		if (stockerTask.m_unRow != task.m_unRow || stockerTask.m_unCol != task.m_unCol || stockerTask.m_unAction != STOCKERACT_REMOVAL)
		{
			m_pStocker->Removal(task.m_unRow, task.m_unCol);

			CStringA cstraText = "";
			cstraText.Format("堆垛机开始取货:%d行,%d列.\r\n", task.m_unRow, task.m_unCol);
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 出库完成
			if (stockerTask.m_unStatus == 0x02 && ClearIn(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol))
			{
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL1_MOVETOTABLE);

				CStringA cstraText = "";
				cstraText.Format("堆垛机取货完成:%d行,%d列.\r\n", task.m_unRow, task.m_unCol);
				SendToLED(cstraText.GetBuffer());
			}
		}

		_AGVTASK taskAGV;
		
		taskAGV = GetAGVTask(m_adoDB,AGVTYPE_A,1,AGVTASKACT_MOVE);

		if (taskAGV.IsNull())
		{
			CreateAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_MOVE);

			CStringA cstraText = "通知机械手AGV移动至平台.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

		if (taskAGV.IsNull())
		{
			CreateAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至等待位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 背负式AGV移动至输送线1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		if (taskAGV.IsNull())
		{
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "通知背负式AGV移动至输送线1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * 阶段1.2 机械手AGV移动至平台
	 * 此阶段判断机械手AGV是否到位
	 * 控制潜入式AGV到达等待位
	 * 控制背负式AGV到达输送线1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL1_MOVETOTABLE)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_MOVE);

		if (taskAGV.IsNull())
		{
			CreateAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_MOVE);

			CStringA cstraText = "通知机械手AGV移动至平台.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL1_MOVETOTABLE);

				CStringA cstraText = "机械手AGV到达平台.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * 潜入式AGV移动至等待位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至等待位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 背负式AGV移动至输送线1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// 不存在背负式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "通知背负式AGV移动至输送线1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * 阶段1.3 机械手AGV从平台上取走货物
	 * 此阶段判断机械手AGV上料是否完成
	 * 控制潜入式AGV到达等待位
	 * 控制背负式AGV到达输送线1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL1_LOAD)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_LOAD);

		if (taskAGV.IsNull())
		{
			CreateAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_LOAD);

			CStringA cstraText = "通知机械手AGV上料.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL1_MOVETOUNLOAD);

				CStringA cstraText = "机械手AGV上料完成.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * 潜入式AGV移动至等待位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至等待位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 背负式AGV移动至输送线1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// 不存在背负式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "通知背负式AGV移动至输送线1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * 阶段1.3 机械手AGV移动至卸车位
	 * 此阶段判断机械手AGV是否到达卸车位
	 * 控制潜入式AGV到达等待位
	 * 控制背负式AGV到达输送线1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL1_MOVETOUNLOAD)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		if (taskAGV.IsNull())
		{
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "通知机械手AGV移动至下料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL1_UNLOAD);

				CStringA cstraText = "机械手AGV到达下料位.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * 潜入式AGV移动至等待位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至等待位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 背负式AGV移动至输送线1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// 不存在背负式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "通知背负式AGV移动至输送线1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * 阶段1.4 机械手AGV将货物放至卸车位料车上
	 * 此阶段判断机械手AGV是否下料完成
	 * 控制潜入式AGV到达等待位
	 * 控制背负式AGV到达输送线1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL1_UNLOAD)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_UNLOAD);

		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_UNLOAD);

			CStringA cstraText = "通知机械手AGV下料.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL2_MOVETOWAIT);

				CStringA cstraText = "机械手AGV到达下料完成.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * 潜入式AGV移动至等待位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至等待位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 背负式AGV移动至输送线1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// 不存在背负式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "通知背负式AGV移动至输送线1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * 阶段2.1 潜入式AGV移动至等待位,等待机械手AGV下料完成
	 * 此阶段判断潜入式AGV是否到达等待位
	 * 控制背负式AGV到达输送线1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL2_MOVETOWAIT)
	{
		_AGVTASK taskAGV;

		/*!
		 * 潜入式AGV移动至等待位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至等待位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 潜入式AGV移动任务完成
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL2_EXCHANGE);

				CStringA cstraText = "潜入式AGV到达等待位.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * 背负式AGV移动至输送线1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// 不存在背负式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "通知背负式AGV移动至输送线1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * 阶段2.3 潜入式AGV换车
	 * 此阶段判断潜入式AGV更换料车是否完成
	 * 控制背负式AGV到达输送线1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL2_EXCHANGE)
	{
		_AGVTASK taskAGV;

		/*!
		 * 背负式AGV移动至输送线1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// 不存在背负式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "通知背负式AGV移动至输送线1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 1、潜入式AGV移动至卸车位
		 * 2、潜入式AGV卸车
		 * 3、潜入式AGV移动至换车位
		 * 4、潜入式AGV换车
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至下料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 潜入式AGV移动未完成
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "潜入式AGV到达下料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_UNLOAD);

		// 不存在潜入式AGV下料
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_UNLOAD);

			CStringA cstraText = "通知潜入式AGV下料.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 潜入式AGV下料未完成
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "潜入式AGV下料完成.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至上料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 潜入式AGV移动未完成
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "潜入式AGV到达上料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_LOAD);

		// 不存在潜入式AGV上料
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_LOAD);

			CStringA cstraText = "通知潜入式AGV上料.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 潜入式AGV上料任务完成
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL2_MOVETOARM);

				CStringA cstraText = "潜入式AGV上料位完成.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * 阶段2.4 潜入式AGV移动至固定机械手
	 * 此阶段判断潜入式AGV是否到达固定机械手
	 * 控制背负式AGV到达输送线1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL2_MOVETOARM)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至固定机械手.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 潜入式AGV移动任务完成
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL2_LOAD);

				CStringA cstraText = "潜入式AGV到达固定机械手.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * 背负式AGV移动至输送线1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// 不存在背负式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "通知背负式AGV移动至输送线1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * 阶段2.5 固定机械手上料至输送线1
	 * 此阶段判断固定机械手是否上料完成以及输送线1是否满载
	 * 控制背负式AGV到达输送线1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL2_LOAD)
	{
		unsigned int unAction = 0;	/*!< 机械手当前动作 */
		unsigned int unStatus = 0;	/*!< 机械手当前状态 */
		m_pArm->GetActionStatus(unAction, unStatus);

		// 未开始上料
		if (unAction != ARMACT_LOAD || unStatus == 0)
		{
			m_pArm->Load();

			CStringA cstraText = "固定机械手开始上料.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		// 上料完成
		else if (unStatus == 2)
		{
			CStringA cstraText = "固定机械手上料完成.\r\n";
			SendToLED(cstraText.GetBuffer());

			if (m_mapPipeline.find(1) != m_mapPipeline.end())
			{
				// 输送线1满载
				if (m_mapPipeline[1]->GetCargoStatus())
				{
					// 更新任务状态
					UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL3_MOVETOPIPELINE1);

					cstraText = "输送线1满载.\r\n";
					SendToLED(cstraText.GetBuffer());
				}
			}
		}

		_AGVTASK taskAGV;

		/*!
		 * 背负式AGV移动至输送线1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// 不存在背负式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "通知背负式AGV移动至输送线1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * 阶段3.1 背负式AGV移动至输送线1
	 * 此阶段判断背负式AGV是否到达输送线1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL3_MOVETOPIPELINE1)
	{
		_AGVTASK taskAGV;

		/*!
		 * 背负式AGV移动至输送线1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// 不存在背负式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "通知背负式AGV移动至输送线1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 背负式AGV移动任务完成
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL3_LOADFROMPIPELINE1);

				CStringA cstraText = "背负式AGV到达输送线1.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * 阶段3.2 背负式AGV将输送线1上的货物取走
	 * 此阶段判断背负式AGV是否上料完成
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL3_LOADFROMPIPELINE1)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_LOAD);

		// 不存在背负式AGV上料任务
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_LOAD);

			CStringA cstraText = "通知背负式AGV上料.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 背负式AGV上料任务完成
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "背负式AGV上料完成.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		if (m_mapPipeline.find(1) != m_mapPipeline.end())
		{
			// 输送线1空载
			if (m_mapPipeline[1]->GetCargoStatus() == false)
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_FINISH);

				CStringA cstraText = "输送线1空载.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * 任务完成
	 * 创建入库任务
	*/
	else if (task.m_unStatus == TASKOUT_FINISH)
	{
		if (GetWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, WMSTASKTYPE_IN).IsNull())
		{
			// 清除全部AGV任务
			if (ClearAGVTask(m_adoDB))
			{
				// 创建入库任务
				CreateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, WMSTASKTYPE_IN);
			}
		}
		else
		{
			// 清除出库任务
			ClearWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, WMSTASKTYPE_OUT);
		}

		CStringA cstraText = "";
		cstraText.Format("出库结束:%d行,%d列.\r\n", task.m_unRow, task.m_unCol);
		SendToLED(cstraText.GetBuffer());
	}

	return;
}

void CService::ProcessTaskIn(_WMSTASK & task)
{
	/*!
	 * 接收任务,开始处理任务
	 * 任务进入阶段1
	*/
	if (task.m_unStatus == TASKIN_NONE)
	{
		// 更新任务状态
		UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL1_MOVETOPIPELINE2);

		CStringA cstraText = "";
		cstraText.Format("入库开始:%d行,%d列.\r\n", task.m_unRow, task.m_unCol);
		SendToLED(cstraText.GetBuffer());
	}
	/*!
	 * 阶段1.1 背负式AGV移动至输送线2
	 * 此阶段检测背负式AGV是否到达输送线2
	 * 控制潜入式AGV移动至固定机械手
	 * 控制机械手AGV移动至卸车位
	*/
	else if (task.m_unStatus == TASKIN_LEVEL1_MOVETOPIPELINE2)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 8, AGVTASKACT_MOVE);

		// 不存在背负式AGV移动任务
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 8, AGVTASKACT_MOVE);

			CStringA cstraText = "通知背负式AGV移动至输送线2.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 背负式AGV移动完成
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL1_UNLOADTOPIPELINE2);

				CStringA cstraText = "背负式AGV到达输送线2.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * 潜入式AGV移动至固定机械手
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV到达固定机械手.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 机械手AGV移动至卸车位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// 不存在机械手AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "通知机械手AGV到达固定下料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * 阶段1.2 背负式AGV下料至输送线2
	 * 此阶段检测背负式AGV是否下料完成
	 * 控制潜入式AGV移动至固定机械手
	 * 控制机械手AGV移动至卸车位
	*/
	else if (task.m_unStatus == TASKIN_LEVEL1_UNLOADTOPIPELINE2)
	{
		_AGVTASK taskAGV;

		/*!
		 * 潜入式AGV移动至固定机械手
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV到达固定机械手.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 机械手AGV移动至卸车位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// 不存在机械手AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "通知机械手AGV到达固定下料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 背负式AGV下料
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 8, AGVTASKACT_UNLOAD);

		// 不存在背负式AGV下料任务
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 8, AGVTASKACT_UNLOAD);

			CStringA cstraText = "通知背负式AGV下料.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 背负式AGV下料完成
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "背负式AGV下料完成.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		if (m_mapPipeline.find(2) != m_mapPipeline.end())
		{
			// 输送线2满载
			if (m_mapPipeline[2]->GetCargoStatus())
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL1_MOVETOPIPELINE1);

				CStringA cstraText = "输送线2满载.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * 阶段1.3 背负式AGV移动至输送线1
	 * 此阶段检测背负式AGV到达输送线1
	 * 控制潜入式AGV移动至固定机械手
	 * 控制机械手AGV移动至卸车位
	*/
	else if (task.m_unStatus == TASKIN_LEVEL1_MOVETOPIPELINE1)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// 不存在背负式AGV移动任务
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "通知背负式AGV移动至输送线1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 背负式AGV移动完成
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL1_PASSTOPIPELINE1);

				CStringA cstraText = "背负式AGV到达输送线1.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * 潜入式AGV移动至固定机械手
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至固定机械书.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 机械手AGV移动至卸车位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// 不存在机械手AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "通知机械手AGV移动至下料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * 阶段1.4 使输送线2的货物通过背负式AGV到达输送线1
	 * 此阶段检测输送线2是否空载，输送线1是否满载
	 * 控制潜入式AGV移动至固定机械手
	 * 控制机械手AGV移动至卸车位
	*/
	else if (task.m_unStatus == TASKIN_LEVEL1_PASSTOPIPELINE1)
	{
		_AGVTASK taskAGV;

		/*!
		 * 潜入式AGV移动至固定机械手
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至固定机械书.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 机械手AGV移动至卸车位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// 不存在机械手AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "通知机械手AGV移动至下料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 背负式AGV通行
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_PASS);

		// 不存在背负式AGV通行任务
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_PASS);

			CStringA cstraText = "通知背负式AGV桥接输送线1和2.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 背负式AGV通行完成
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "背负式AGV桥接输送线1和2完成.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		if (m_mapPipeline.find(2) != m_mapPipeline.end())
		{
			// 输送线2满载
			if (m_mapPipeline[2]->GetCargoStatus())
			{
				return;
			}

			CStringA cstraText = "输送线2空载.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		if (m_mapPipeline.find(1) != m_mapPipeline.end())
		{
			// 输送线1满载
			if (m_mapPipeline[1]->GetCargoStatus())
			{
				CStringA cstraText = "输送线2满载.\r\n";
				SendToLED(cstraText.GetBuffer());

				taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 1, AGVTASKACT_RETURN);

				// 不存在背负式AGV返回任务
				if (taskAGV.IsNull())
				{
					// 创建任务
					CreateAGVTask(m_adoDB, AGVTYPE_T, 1, AGVTASKACT_RETURN);

					CStringA cstraText = "通知背负式AGV返回待机位.\r\n";
					SendToLED(cstraText.GetBuffer());
				}
				else
				{
					// 更新任务状态
					UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL1_UNLOADFROMPIPELINE1);
				}
			}
		}
	}
	/*!
	 * 阶段1.5 固定机械手将货物从输送线1放至潜入式AGV料车上
	 * 此阶段检测机械手是否下料完成，输送线1是否空载,潜入式AGV是否到达固定机械手
	 * 控制机械手AGV移动至卸车位
	*/
	else if (task.m_unStatus == TASKIN_LEVEL1_UNLOADFROMPIPELINE1)
	{
		_AGVTASK taskAGV;

		/*!
		 * 潜入式AGV移动至固定机械手
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至固定机械手.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 潜入式AGV移动完成
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "潜入式AGV到达固定机械手.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 机械手AGV移动至卸车位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// 不存在机械手AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "通知机械手AGV移动至下料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		unsigned int unAction = 0;	/*!< 机械手当前动作 */
		unsigned int unStatus = 0;	/*!< 机械手当前状态 */
		m_pArm->GetActionStatus(unAction, unStatus);

		// 未开始下料
		if (unAction != ARMACT_UNLOAD || unStatus == 0)
		{
			m_pArm->Unload();

			CStringA cstraText = "通知固定机械手下料.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		// 下料完成
		else if (unStatus == 2)
		{
			CStringA cstraText = "固定机械手下料完成.\r\n";
			SendToLED(cstraText.GetBuffer());

			if (m_mapPipeline.find(1) != m_mapPipeline.end())
			{
				// 输送线1空载
				if (m_mapPipeline[1]->GetCargoStatus() == false)
				{
					// 更新任务状态
					UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL2_MOVETOUNLOAD);

					CStringA cstraText = "输送线1空载.\r\n";
					SendToLED(cstraText.GetBuffer());
				}
			}
		}
	}
	/*!
	 * 阶段2.1 潜入式AGV移动至卸车点
	 * 此阶段检测潜入式AGV是否到达卸车点
	 * 控制机械手AGV移动至卸车位
	*/
	else if (task.m_unStatus == TASKIN_LEVEL2_MOVETOUNLOAD)
	{
		_AGVTASK taskAGV;

		/*!
		 * 潜入式AGV移动至等待位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至下料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 潜入式AGV移动完成
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL3_LOAD);

				CStringA cstraText = "潜入式AGV到达下料位.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * 机械手AGV移动至卸车位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// 不存在机械手AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "通知机械手AGV移动至下料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * 阶段3.1 机械手AGV移动至卸车点
	 * 此阶段检测机械手AGV是否到达卸车点
	 * 控制潜入式AGV返回
	*/
	else if (task.m_unStatus == TASKIN_LEVEL3_MOVETOLOAD)
	{
		_AGVTASK taskAGV;

		/*!
		 * 机械手AGV移动至卸车位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// 不存在机械手AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "通知机械手AGV移动至下料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 机械手AGV移动完成
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL3_LOAD);

				CStringA cstraText = "机械手AGV到达下料位.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * 阶段3.1 机械手AGV将料车上的货物取回
	 * 此阶段检测机械手AGV是否上料完成
	 * 控制潜入式AGV返回
	*/
	else if (task.m_unStatus == TASKIN_LEVEL3_LOAD)
	{
		_AGVTASK taskAGV;

		/*!
		 * 机械手AGV上料
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_LOAD);

		// 不存在机械手AGV上料
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_LOAD);

			CStringA cstraText = "通知机械手AGV上料.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 机械手AGV上料完成
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL3_MOVETOTABLE);

				CStringA cstraText = "机械手AGV上料完成.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * 阶段3.2 机械手AGV移动至平台
	 * 此阶段检测机械手AGV是否到达平台
	 * 控制潜入式AGV返回
	*/
	else if (task.m_unStatus == TASKIN_LEVEL3_MOVETOTABLE)
	{
		_AGVTASK taskAGV;

		/*!
		 * 机械手AGV移动至平台
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_MOVE);

		// 不存在机械手AGV移动至平台
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_MOVE);

			CStringA cstraText = "通知机械手AGV移动至上料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 机械手AGV移动完成
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL3_UNLOAD);

				CStringA cstraText = "机械手AGV到达上料位.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * 阶段3.3 机械手AGV将货物放至平台上
	 * 此阶段检测机械手AGV是否下料完成
	 * 控制潜入式AGV返回
	*/
	else if (task.m_unStatus == TASKIN_LEVEL3_UNLOAD)
	{
		_AGVTASK taskAGV;

		/*!
		 * 机械手AGV下料
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_UNLOAD);

		// 不存在机械手AGV下料
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_UNLOAD);

			CStringA cstraText = "通知机械手AGV下料.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 机械手AGV下料完成
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// 更新任务状态
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL4_IN);

				CStringA cstraText = "机械手AGV下料完成.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * 阶段4.1 堆垛机将平台上的货物放至货架
	 * 此阶段检测堆垛机是否入库完成
	 * 控制潜入式AGV返回
	*/
	else if (task.m_unStatus == TASKIN_LEVEL4_IN)
	{
		_STOCKERTASK stockerTask = m_pStocker->GetTask();	/*!< 堆垛机任务 */

		if (stockerTask.m_unRow != task.m_unRow || stockerTask.m_unCol != task.m_unCol || stockerTask.m_unAction != STOCKERACT_WAREHOUSING)
		{
			m_pStocker->Warehousing(task.m_unRow, task.m_unCol);

			CStringA cstraText = "";
			cstraText.Format("堆垛机开始存货:%d行,%d列.\r\n", task.m_unRow, task.m_unCol);
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// 入库完成
			if (stockerTask.m_unStatus == 0x02 && RecordIn(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol,"瓷砖","块",1))
			{
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_FINISH);

				CStringA cstraText = "";
				cstraText.Format("堆垛机存货完成:%d行,%d列.\r\n", task.m_unRow, task.m_unCol);
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	else if (task.m_unStatus == TASKIN_FINISH)
	{
		_AGVTASK taskAGV;

		/*!
		 * 背负式AGV返回待机位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 1, AGVTASKACT_RETURN);

		// 存在背负式AGV返回
		if (taskAGV.IsNull() == false)
		{
			// 背负式AGV返回未完成
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "背负式AGV到达待机位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 潜入式AGV返回待机位
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 2, AGVTASKACT_RETURN);

		// 存在潜入式AGV返回
		if (taskAGV.IsNull() == false)
		{
			// 潜入式AGV返回未完成
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "潜入式AGV到达待机位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		if (ClearAGVTask(m_adoDB))
		{
			ClearAllWMSTask(m_adoDB);
		}

		CStringA cstraText = "";
		cstraText.Format("入库完成:%d行,%d列.\r\n", task.m_unRow, task.m_unCol);
		SendToLED(cstraText.GetBuffer());
	}

	_AGVTASK taskAGV;

	taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_MOVE);

	// 移动至卸车位任务不存在或任务未完成
	if (taskAGV.IsNull() || taskAGV.m_strStatus != AGVTASKSTA_FINISH)
	{
		return;
	}
	else
	{
		/*!
		 * 潜入式AGV换车返回
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_UNLOAD);

		// 不存在潜入式AGV下料
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_UNLOAD);

			CStringA cstraText = "通知潜入式AGV下料.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "潜入式AGV下料完成.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_MOVE);

		// 不存在潜入式AGV移动
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_MOVE);

			CStringA cstraText = "通知潜入式AGV移动至上料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "潜入式AGV到达上料位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_LOAD);

		// 不存在潜入式AGV上料
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_LOAD);

			CStringA cstraText = "通知潜入式AGV上料.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "潜入式AGV到达上料完成.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 2, AGVTASKACT_RETURN);

		// 不存在潜入式AGV返回
		if (taskAGV.IsNull())
		{
			// 创建任务
			CreateAGVTask(m_adoDB, AGVTYPE_S, 2, AGVTASKACT_RETURN);


			CStringA cstraText = "通知潜入式AGV返回待机位.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}

	return;
}

void CService::OpenScreen()
{
	if (!m_led.OpenScreen())
	{
		AfxMessageBox(_T("开屏失败"));
	}

	return;
}

void CService::CloseScreen()
{
	if (!m_led.CloseScreen())
	{
		AfxMessageBox(_T("关屏失败"));
	}

	return;
}

void CService::SendToLED(string strText)
{
	// 已输出相同的文本
	for (int i = 0; i < 10; i++)
	{
		if (strText == m_strLed[i])
		{
			return;
		}
	}

	for (int i = 0; i < 9; i++)
	{
		m_strLed[i + 1] = m_strLed[i];
	}

	m_strLed[0] = strText;

	if (!m_led.Connect())
	{
		//AfxMessageBox(_T("建立连接失败"));
		return;
	}

	string strOutput = "";

	for (int i = 0; i < 10; i++)
	{
		strOutput += m_strLed[i];
	}

	USES_CONVERSION;
	if (!m_led.SendText((char*)strOutput.c_str()))
	{
		//AfxMessageBox(_T("发送文本失败"));
	}

	if (!m_led.Disconnect())
	{
		//AfxMessageBox(_T("断开连接失败"));
	}

	return;
}

void CService::GetCallMsg()
{
	string strMsg = CCA::GetRecvMsg();

	if (strMsg == "")
	{
		return;
	}

	if (strstr(strMsg.c_str(), "呼叫"))
	{
		GetOut(m_unNo,m_unRow,m_unCol);
	}

	return;
}

#endif // !_SERVICE_H