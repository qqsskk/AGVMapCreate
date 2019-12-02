#include "stdafx.h"
#include "CTaskManager.h"

#ifdef _TASK_H

CTaskManager::CTaskManager()
{
	m_pTheadUpdate = nullptr;
}

CTaskManager::~CTaskManager()
{
	Clear();
}

bool CTaskManager::OpenManager(const ADOConn ado)
{
	m_ado = ado;

	if (m_ado.OnInitADOConn() == false)
	{
		return false;
	}

	StartUpdateThread();

	return true;
}

bool CTaskManager::OpenManager(const ADOConn ado, HWND hParent)
{
	if (hParent == nullptr)
	{
		DebugPrint("无效的父窗口句柄");
		return false;
	}

	m_hParent = hParent;

	return OpenManager(ado);
}

void CTaskManager::CloseManager()
{
	EndUpdateThread();

	return;
}

void CTaskManager::StartUpdateThread()
{
	EndUpdateThread();

	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(nullptr, true, false, nullptr);
	}

	m_pTheadUpdate = new std::thread(&CTaskManager::UpdateThread, this);

	// 记录日志
	Record("【系统System】任务管理器【启动】.");
	// 调试输出
	DebugPrint("AGVContorlSystem/CTaskManager/StartUpdateThread 启动任务更新线程.");

	return;
}

void CTaskManager::EndUpdateThread()
{
	if (m_pTheadUpdate)
	{
		if (m_pTheadUpdate->joinable())
		{
			SetEvent(m_hShutdownEvent);

			m_pTheadUpdate->join();
		}

		delete m_pTheadUpdate;
		m_pTheadUpdate = nullptr;

		// 记录日志
		Record("【系统System】任务管理器【关闭】.");
		// 调试输出
		DebugPrint("AGVContorlSystem/CTaskManager/StartUpdateThread 关闭任务更新线程.");
	}

	return;
}

void CTaskManager::UpdateThread()
{
	ADOConn ado = m_ado;		/*!< ADO链接 */
	_RecordsetPtr pRecordset = nullptr; /*!< 结果集 */
	std::string strSql = "";	/*!< SQL 语句 */

	BufferString::std_string_format(strSql, "select * from [%s] order by [%s]", \
		Task::g_taskTable.m_strTable.c_str(),Task::g_taskTable.m_strPublish.c_str());

	while (true)
	{
		// 获取结果集
		pRecordset =  ado.GetRecordSet(_bstr_t(strSql.c_str()));

		// 无结果
		if (pRecordset == nullptr || pRecordset->adoEOF)
		{
			continue;
		}

		while (!pRecordset->adoEOF)
		{
			AGVTask agvTask;

			_variant_t var;	/*!< 值 */

			// 获取任务号
			var = pRecordset->GetCollect(Task::g_taskTable.m_strNo.c_str());
			// 值不为空
			if (var.vt != VT_NULL)
			{
				// 无效的任务号
				if (agvTask.SetNo((unsigned short)var) == false)
				{
					continue;
				}
			}

			// 获取阶段号
			var = pRecordset->GetCollect(Task::g_taskTable.m_strLevel.c_str());
			// 值不为空
			if (var.vt != VT_NULL)
			{
				// 无效的阶段号
				if (agvTask.SetLevel((unsigned char)var) == false)
				{
					continue;
				}
			}

			// 获取任务坐标
			var = pRecordset->GetCollect(Task::g_taskTable.m_strTarget.c_str());
			// 值不为空
			if (var.vt != VT_NULL)
			{
				agvTask.SetTarget((unsigned short)var);
			}

			// 获取起始坐标
			var = pRecordset->GetCollect(Task::g_taskTable.m_strBegin.c_str());
			// 值不为空
			if (var.vt != VT_NULL)
			{
				agvTask.SetBegin((unsigned short)var);
			}

			// 获取任务动作
			var = pRecordset->GetCollect(Task::g_taskTable.m_strAction.c_str());
			// 值不为空
			if (var.vt != VT_NULL)
			{
				agvTask.SetAction((char*)_bstr_t(var));
			}

			// 获取任务状态
			var = pRecordset->GetCollect(Task::g_taskTable.m_strStatus.c_str());
			// 值不为空
			if (var.vt != VT_NULL)
			{
				agvTask.SetStatus((unsigned char)var);
			}

			// 获取异常信息
			var = pRecordset->GetCollect(Task::g_taskTable.m_strError.c_str());
			// 值不为空
			if (var.vt != VT_NULL)
			{
				agvTask.SetError((char*)_bstr_t(var));
			}

			if (UpdateTask(agvTask) == false)
			{
				continue;
			}

			AGVTask* pTask = GetTask(agvTask.GetNo(), agvTask.GetLevel());

			if (pTask == nullptr)
			{
				continue;
			}

#ifdef _AFX
			if (m_hParent)
			{
				::PostMessage(m_hParent, WM_UPDATE_TASK, (WPARAM)pTask, 0);
			}
#endif // _AFX
		}

		if (WaitForSingleObject(m_hShutdownEvent, 100) == WAIT_OBJECT_0)
		{
			break;
		}
	}

	return;
}

bool CTaskManager::PublishTask(AGVTask task)
{
	unsigned short usNo = task.GetNo();

	if (task.GetNo() == 0)
	{
		return false;
	}

	if (m_mapTaskList.find(usNo) == m_mapTaskList.end())
	{
		m_mapTaskList[usNo] = new AGVTaskArr(usNo);
	}

	return m_mapTaskList[usNo]->AddTask(task);
}

bool CTaskManager::Add(AGVTask newTask)
{
	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		// insert into 表名(任务号,阶段号,任务坐标,起始坐标,任务动作,任务状态,执行者,异常信息,发布时间) values(%d,%d,%d,%d,%s,%d,%d,%s,getdate())
		"insert into [%s]([%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]) values(%d,%d,%d,%d,'%s',%d,%d,'%s',getdate());",
		// 表名								  任务号字段						 阶段号字段
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strNo.c_str(), Task::g_taskTable.m_strLevel.c_str(),
		//任务坐标字段							起始坐标字段						任务动作字段
		Task::g_taskTable.m_strTarget.c_str(), Task::g_taskTable.m_strBegin.c_str(), Task::g_taskTable.m_strAction.c_str(),
		//任务状态字段							执行者字段								异常信息字段
		Task::g_taskTable.m_strStatus.c_str(), Task::g_taskTable.m_strExecuter.c_str(), Task::g_taskTable.m_strError.c_str(),
		//发布时间
		Task::g_taskTable.m_strPublish,
		//任务号		阶段号			任务坐标			起始坐标		任务动作				任务状态
		newTask.GetNo(), newTask.GetLevel(), newTask.GetTarget(), newTask.GetBegin(), newTask.GetAction().c_str(), newTask.GetStatus(),
		//执行者			异常信息
		newTask.GetExecuter(), newTask.GetError().c_str()
	);

	// 合成SQL语句
	BufferString::std_string_format(strSql,
		// insert into 表名(任务号,阶段号,任务坐标,起始坐标,任务动作,任务状态,执行者,异常信息,发布时间) values(%d,%d,%d,%d,%s,%d,%d,%s,getdate())
		"%s insert into [%s]([%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]) values(%d,%d,%d,%d,'%s',%d,%d,'%s',getdate());",
		//				表名								  任务号字段						 阶段号字段
		strSql.c_str(),Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strNo.c_str(), Task::g_taskTable.m_strLevel.c_str(),
		//任务坐标字段							起始坐标字段						任务动作字段
		Task::g_taskTable.m_strTarget.c_str(), Task::g_taskTable.m_strBegin.c_str(), Task::g_taskTable.m_strAction.c_str(),
		//任务状态字段							执行者字段								异常信息字段
		Task::g_taskTable.m_strStatus.c_str(), Task::g_taskTable.m_strExecuter.c_str(), Task::g_taskTable.m_strError.c_str(),
		//发布时间
		Task::g_taskTable.m_strPublish,
		//任务号		阶段号					任务坐标			起始坐标				任务动作				任务状态
		newTask.GetNo(), newTask.GetLevel() + 1, newTask.GetTarget(), newTask.GetTarget(), _TASKACTION_A_END, Task::_TASKSTA_NONE,
		//执行者				异常信息
		newTask.GetExecuter(), _TASKERROR_A_NONE
	);

	return true;
}

unsigned short CTaskManager::GetCurMaxNo()
{
	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		// select max(任务号字段) as MaxValue form 表名
		"select max([%s]) as MaxValue from [%s]",
		//任务号字段						表名
		Task::g_taskTable.m_strNo.c_str(), Task::g_taskTable.m_strTable.c_str()
	);

	_RecordsetPtr pRecordset = m_ado.GetRecordSet(_bstr_t(strSql.c_str())); /*!< 结果集 */

	if (pRecordset == nullptr || pRecordset->adoEOF)
	{
		return 0;
	}

	unsigned short usMaxNo = 0;	/*!< 最大任务号 */

	_variant_t var;	/*!< 值 */

	// 获取最大任务号
	var = pRecordset->GetCollect(Task::g_taskTable.m_strNo.c_str());
	// 值不为空
	if (var.vt != VT_NULL)
	{
		usMaxNo = (unsigned short)var;
	}

	if (usMaxNo == UINT16_MAX)
	{
		usMaxNo = 0;
	}

	return usMaxNo;
}

AGVTask* CTaskManager::GetTask(unsigned short usNo, unsigned char byLevel)
{
	AGVTaskArr* pTaskArr = GetTaskArray(usNo);

	if (pTaskArr == nullptr)
	{
		return nullptr;
	}

	if (byLevel == 0)
	{
		return nullptr;
	}

	std::map<unsigned char,AGVTask*>& mapTask = pTaskArr->GetTaskList();

	if (mapTask.find(byLevel) == mapTask.end())
	{
		return nullptr;
	}

	return mapTask[byLevel];
}

AGVTask CTaskManager::Get(unsigned short usNo, unsigned char byLevel)
{
	AGVTask *pTask = GetTask(usNo, byLevel);

	return *pTask;
}

AGVTaskArr CTaskManager::GetArray(unsigned short usNo)
{
	if (usNo == 0)
	{
		return AGVTaskArr();
	}

	if (m_mapTaskList.find(usNo) == m_mapTaskList.end())
	{
		return AGVTaskArr();
	}

	return *m_mapTaskList[usNo];
}

std::map<unsigned short, AGVTaskArr> CTaskManager::GetList()
{
	// TODO: 在此处插入 return 语句
	std::map<unsigned short, AGVTaskArr> list;

	for (std::map<unsigned short, AGVTaskArr*>::iterator it = m_mapTaskList.begin(); it != m_mapTaskList.end(); ++it)
	{
		list[it->first] = *(it->second);
	}

	return list;
}

bool CTaskManager::Publish(AGVTask task)
{
	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		// insert into 表名(任务号,阶段号,任务坐标,起始坐标,任务动作,任务状态,执行者,异常信息,发布时间) values(%d,%d,%d,%d,%s,%d,%d,%s,getdate())
		"insert into [%s]([%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]) values(%d,%d,%d,%d,'%s',%d,%d,'%s',getdate());",
		// 表名								  任务号字段						 阶段号字段
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strNo.c_str(), Task::g_taskTable.m_strLevel.c_str(),
		//任务坐标字段							起始坐标字段						任务动作字段
		Task::g_taskTable.m_strTarget.c_str(), Task::g_taskTable.m_strBegin.c_str(), Task::g_taskTable.m_strAction.c_str(),
		//任务状态字段							执行者字段								异常信息字段
		Task::g_taskTable.m_strStatus.c_str(), Task::g_taskTable.m_strExecuter.c_str(), Task::g_taskTable.m_strError.c_str(),
		//发布时间
		Task::g_taskTable.m_strPublish,
		//任务号		阶段号			任务坐标			起始坐标		任务动作				任务状态
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), task.GetStatus(),
		//执行者			异常信息
		task.GetExecuter(),task.GetError().c_str()
		);

	// 执行SQL语句
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// 调试输出
		DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());
		return false;
	}

	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"【系统System】发布任务【任务号:%d】【阶段号:%d】【任务坐标:%d】【起始坐标:%d】【任务动作:%s】【任务状态:%d】【执行者:%s】",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), task.GetStatus(), task.GetExecuter()
	);

	// 记录日志
	Record(strRecord);
	// 调试输出
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Update(AGVTask task)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"【系统System】更新任务【任务号:%d】【阶段号:%d】【任务坐标:%d】【起始坐标:%d】【任务动作:%s】【任务状态:%d】【执行者:%s】【异常信息:%s】",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), task.GetStatus(), task.GetExecuter(),task.GetError().c_str()
	);

	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		// update 表名 set 任务状态=%d,执行者=%d,异常信息=%s
		"update [%s] set [%s]=%d,[%s]=%d,[%s]='%s'",
		// 表名									任务状态字段							任务状态
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), task.GetStatus(),
		//执行者字段								执行者				异常信息字段
		Task::g_taskTable.m_strExecuter.c_str(), task.GetExecuter(), Task::g_taskTable.m_strError.c_str(),
		// 异常信息
		task.GetError().c_str()
	);

	// 任务完成
	if (task.GetStatus() == Task::_TASKSTA_FINISH)
	{
		// 合成SQL语句
		BufferString::std_string_format(strSql,
			"%s,[%s]=getdate()",
			//					完成时间字段
			strSql.c_str(), Task::g_taskTable.m_strFinish.c_str()
		);
	}

	// 合成SQL语句
	BufferString::std_string_format(strSql,
		"%s where [%s]=%d and [%s]=%d;",
		//				任务号字段							任务号			阶段号字段
		strSql.c_str(), Task::g_taskTable.m_strNo.c_str(), task.GetNo(), Task::g_taskTable.m_strLevel.c_str(),
		//阶段号
		task.GetLevel()
	);

	// 执行SQL语句
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// 调试输出
		DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

		if (Publish(task) == false)
		{
			strRecord += "失败!";

			// 记录日志
			Record(strRecord);
			// 调试输出
			DebugPrint(strRecord.c_str());

			return false;
		}
		else
		{
			return true;
		}
	}

	// 记录日志
	Record(strRecord);
	// 调试输出
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Delete(unsigned short usNo)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"【系统System】删除任务【任务号:%d】",
		usNo
	);

	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		// insert into 历史表名 select * form 表名 where 表名.任务号=%d;delete from 表名 where 任务号=%ds
		"insert into [%s] select * from [%s] where [%s].[%s]=%d;delete from [%s] where [%s]=%d;",
		// 历史表名									表名								表名
		Task::g_taskTable.m_strHisTable.c_str(), Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strTable.c_str(),
		// 任务号字段						任务号	表名								任务号字段							任务号
		Task::g_taskTable.m_strNo.c_str(), usNo, Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strNo.c_str(), usNo
	);

	// 执行SQL语句
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// 调试输出
		DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

		strRecord += "失败!";

		// 记录日志
		Record(strRecord);
		// 调试输出
		DebugPrint(strRecord.c_str());

		return false;
	}

	DeleteTask(usNo);

	// 记录日志
	Record(strRecord);
	// 调试输出
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Edit(AGVTask task)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"【系统System】编辑任务【任务号:%d】【阶段号:%d】【任务坐标:%d】【起始坐标:%d】【任务动作:%s】【任务状态:%d】【执行者:%s】【异常信息:%s】",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), task.GetStatus(), task.GetExecuter(), task.GetError().c_str()
	);

	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		// update 表名 set 任务坐标=%d,起始坐标=%d,任务动作=%s,任务状态=%d,执行者=%d,异常信息=%s
		"update [%s] set [%s]=%d,[%s]=%d,[%s]='%s',[%s]=%d,[%s]=%d,[%s]='%s'",
		// 表名									任务坐标字段							任务坐标
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strTarget.c_str(), task.GetTarget(),
		// 起始坐标字段							起始坐标		任务动作字段						任务动作
		Task::g_taskTable.m_strBegin.c_str(), task.GetBegin(), Task::g_taskTable.m_strAction.c_str(),task.GetAction().c_str(),
		// 任务状态字段							任务状态		执行者字段									执行者
		Task::g_taskTable.m_strStatus.c_str(), task.GetStatus(),Task::g_taskTable.m_strExecuter.c_str(), task.GetExecuter(),
		// 异常信息字段							异常信息
		Task::g_taskTable.m_strError.c_str(), task.GetError().c_str()
	);

	// 任务完成
	if (task.GetStatus() == Task::_TASKSTA_FINISH)
	{
		// 合成SQL语句
		BufferString::std_string_format(strSql,
			"%s,[%s]=getdate()",
			//					完成时间字段
			strSql.c_str(), Task::g_taskTable.m_strFinish.c_str()
		);
	}

	// 合成SQL语句
	BufferString::std_string_format(strSql,
		"%s where [%s]=%d and [%s]=%d;",
		//				任务号字段							任务号			阶段号字段
		strSql.c_str(), Task::g_taskTable.m_strNo.c_str(), task.GetNo(), Task::g_taskTable.m_strLevel.c_str(),
		//阶段号
		task.GetLevel()
	);

	// 执行SQL语句
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// 调试输出
		DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

		strRecord += "失败!";

		// 记录日志
		Record(strRecord);
		// 调试输出
		DebugPrint(strRecord.c_str());
		
		return false;
	}

	// 记录日志
	Record(strRecord);
	// 调试输出
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Cancel(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"【系统System】取消任务【任务号:%d】【阶段号:%d】",
		usNo, byLevel
	);

	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		//update 表名 set 状态字段=取消 where 任务号字段=%d and 阶段号字段=%d and 状态字段 not int(完成，取消)
		"update [%s] set [%s]=%d where [%s]=%d and [%s]=%d and [%s] not in(%d,%d);",
		// 表名									状态字段							取消状态
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_CANCEL,
		//任务号字段						任务号	阶段号字段							阶段号
		Task::g_taskTable.m_strNo.c_str(), usNo, Task::g_taskTable.m_strLevel.c_str(), byLevel,
		//状态字段								完成状态				取消状态
		Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_FINISH, Task::_TASKSTA_CANCEL
	);

	// 执行SQL语句
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// 调试输出
		DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

		strRecord += "失败!";

		// 记录日志
		Record(strRecord);
		// 调试输出
		DebugPrint(strRecord.c_str());

		return false;
	}

	// 记录日志
	Record(strRecord);
	// 调试输出
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Pause(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"【系统System】暂停任务【任务号:%d】【阶段号:%d】",
		usNo, byLevel
	);

	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		//update 表名 set 状态字段=暂停 where 任务号字段=%d and 阶段号字段=%d and 状态字段 not int(完成，取消)
		"update [%s] set [%s]=%d where [%s]=%d and [%s]=%d and [%s] not in(%d,%d,%d);",
		// 表名									状态字段							暂停状态
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_PAUSE,
		//任务号字段						任务号	阶段号字段							阶段号
		Task::g_taskTable.m_strNo.c_str(), usNo, Task::g_taskTable.m_strLevel.c_str(), byLevel,
		//状态字段								完成状态				取消状态			暂停状态
		Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_FINISH, Task::_TASKSTA_CANCEL, Task::_TASKSTA_PAUSE
	);

	// 执行SQL语句
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// 调试输出
		DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

		strRecord += "失败!";

		// 记录日志
		Record(strRecord);
		// 调试输出
		DebugPrint(strRecord.c_str());

		return false;
	}

	// 记录日志
	Record(strRecord);
	// 调试输出
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Finish(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"【系统System】强制完成任务【任务号:%d】【阶段号:%d】",
		usNo, byLevel
	);

	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		//update 表名 set 状态字段=完成 where 任务号字段=%d and 阶段号字段=%d and 状态字段 not int(完成)
		"update [%s] set [%s]=%d where [%s]=%d and [%s]=%d and [%s] not in(%d);",
		// 表名									状态字段							完成状态
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_FINISH,
		//任务号字段						任务号	阶段号字段							阶段号
		Task::g_taskTable.m_strNo.c_str(), usNo, Task::g_taskTable.m_strLevel.c_str(), byLevel,
		//状态字段								完成状态
		Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_FINISH
	);

	// 执行SQL语句
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// 调试输出
		DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

		strRecord += "失败!";

		// 记录日志
		Record(strRecord);
		// 调试输出
		DebugPrint(strRecord.c_str());

		return false;
	}

	// 记录日志
	Record(strRecord);
	// 调试输出
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Reset(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"【系统System】重置任务【任务号:%d】【阶段号:%d】",
		usNo, byLevel
	);

	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		//update 表名 set 状态字段=未执行 where 任务号字段=%d and 阶段号字段>=%d
		"update [%s] set [%s]=%d where [%s]=%d and [%s]>=%d;",
		// 表名									状态字段							重置状态
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_NONE,
		//任务号字段						任务号	阶段号字段							阶段号
		Task::g_taskTable.m_strNo.c_str(), usNo, Task::g_taskTable.m_strLevel.c_str(), byLevel
	);

	// 执行SQL语句
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// 调试输出
		DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

		strRecord += "失败!";

		// 记录日志
		Record(strRecord);
		// 调试输出
		DebugPrint(strRecord.c_str());

		return false;
	}

	// 记录日志
	Record(strRecord);
	// 调试输出
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::CancelAll()
{
	std::string strRecord = "【系统System】取消全部任务";

	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		//update 表名 set 状态字段=取消 where 状态字段 not in (取消,完成)
		"update [%s] set [%s]=%d where [%s] not in(%d,%d);",
		// 表名									状态字段							重置状态
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_CANCEL,
		//状态字段								取消					完成
		Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_CANCEL,Task::_TASKSTA_FINISH
	);

	// 执行SQL语句
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// 调试输出
		DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

		strRecord += "失败!";

		// 记录日志
		Record(strRecord);
		// 调试输出
		DebugPrint(strRecord.c_str());

		return false;
	}

	// 记录日志
	Record(strRecord);
	// 调试输出
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::PauseAll()
{
	std::string strRecord = "【系统System】暂停全部任务";

	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		//update 表名 set 状态字段=暂停 where 状态字段 not in (取消,完成,暂停)
		"update [%s] set [%s]=%d where [%s] not in(%d,%d,%d);",
		// 表名									状态字段							暂停状态
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_PAUSE,
		//状态字段								取消					完成				暂停
		Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_CANCEL, Task::_TASKSTA_FINISH,Task::_TASKSTA_PAUSE
	);

	// 执行SQL语句
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// 调试输出
		DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

		strRecord += "失败!";

		// 记录日志
		Record(strRecord);
		// 调试输出
		DebugPrint(strRecord.c_str());

		return false;
	}

	// 记录日志
	Record(strRecord);
	// 调试输出
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::ResetAll()
{
	std::string strRecord = "【系统System】重置全部任务";

	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		//update 表名 set 状态字段=重置
		"update [%s] set [%s]=%d;",
		// 表名									状态字段							重置状态
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_NONE
	);

	// 执行SQL语句
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// 调试输出
		DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

		strRecord += "失败!";

		// 记录日志
		Record(strRecord);
		// 调试输出
		DebugPrint(strRecord.c_str());

		return false;
	}

	// 记录日志
	Record(strRecord);
	// 调试输出
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Insert(unsigned short usNo, unsigned char byLevel, AGVTask newTask)
{
	std::string strRecord = "";					/*!< 日志文本 */

	BufferString::std_string_format(strRecord, "【系统System】插入任务【任务号:%d】【阶段号:%d】【任务坐标:%d】【起始坐标:%d】【任务动作:%s】",
		newTask.GetNo(), newTask.GetLevel(), newTask.GetTarget(), newTask.GetBegin(), newTask.GetAction().c_str());

	std::string strSql = ""; /*!< SQL语句 */
	// 合成SQL语句
	BufferString::std_string_format(strSql,
		//update 表名 set 阶段号=阶段号+1 where 任务号=%d and 阶段号>=%d;
		"update [%s] set [%s]=[%s]+1 where [%s]=%d and [%s]>=%d;",
		// 表名									阶段号字段							阶段号		任务号字段
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strLevel.c_str(), byLevel, Task::g_taskTable.m_strNo.c_str(),
		//任务号	阶段号字段						阶段号	
		usNo, Task::g_taskTable.m_strLevel.c_str(), byLevel
	);

	// 合成SQL语句
	BufferString::std_string_format(strSql,
		//update 表名 set 状态=未执行 where 任务号=%d and 阶段号>=%d and 状态 not in (取消,暂停)
		"%s update[%s] set [%s]=%d where [%s]=%d and [%s]>=%d and [%s]not in(%d, %d);",
		//				表名									状态字段								状态:未执行
		strSql.c_str(), Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_NONE,
		//任务号字段						任务号	阶段号字段							阶段号	状态字段
		Task::g_taskTable.m_strNo.c_str(), usNo, Task::g_taskTable.m_strLevel.c_str(), byLevel, Task::g_taskTable.m_strStatus.c_str(),
		//状态:取消				状态:暂停
		Task::_TASKSTA_CANCEL,Task::_TASKSTA_PAUSE
	);

	// 合成SQL语句
	BufferString::std_string_format(strSql,
		// insert into 表名(任务号,阶段号,任务坐标,起始坐标,任务动作,任务状态,执行者,异常信息,发布时间) values(%d,%d,%d,%d,%s,%d,%d,%s,getdate())
		"%s insert into [%s]([%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]) values(%d,%d,%d,%d,'%s',%d,%d,'%s',getdate());",
		//				表名								  任务号字段						 阶段号字段
		strSql.c_str(), Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strNo.c_str(), Task::g_taskTable.m_strLevel.c_str(),
		//任务坐标字段							起始坐标字段						任务动作字段
		Task::g_taskTable.m_strTarget.c_str(), Task::g_taskTable.m_strBegin.c_str(), Task::g_taskTable.m_strAction.c_str(),
		//任务状态字段							执行者字段								异常信息字段
		Task::g_taskTable.m_strStatus.c_str(), Task::g_taskTable.m_strExecuter.c_str(), Task::g_taskTable.m_strError.c_str(),
		//发布时间
		Task::g_taskTable.m_strPublish,
		//任务号		阶段号			任务坐标			起始坐标		任务动作				任务状态
		newTask.GetNo(), newTask.GetLevel(), newTask.GetTarget(), newTask.GetBegin(), newTask.GetAction().c_str(), newTask.GetStatus(),
		//执行者			异常信息
		newTask.GetExecuter(), newTask.GetError().c_str()
	);

	// 执行SQL语句
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// 调试输出
		DebugPrint("执行SQL语句失败!语句:%s", strSql.c_str());

		strRecord += "失败!";

		// 记录日志
		Record(strRecord);
		// 调试输出
		DebugPrint(strRecord.c_str());

		return false;
	}

	// 记录日志
	Record(strRecord);
	// 调试输出
	DebugPrint(strRecord.c_str());

	return true;
}

AGVTaskArr* CTaskManager::GetTaskArray(unsigned short usNo)
{
	if (usNo == 0)
	{
		return nullptr;
	}

	if (m_mapTaskList.find(usNo) == m_mapTaskList.end())
	{
		return nullptr;
	}

	return m_mapTaskList[usNo];
}

std::map<unsigned short, AGVTaskArr*>& CTaskManager::GetTaskList()
{
	// TODO: 在此处插入 return 语句
	return m_mapTaskList;
}

bool CTaskManager::UpdateTask(AGVTask task)
{
	unsigned short usNo = task.GetNo();

	if (task.GetNo() == 0)
	{
		return false;
	}

	if (m_mapTaskList.find(usNo) == m_mapTaskList.end())
	{
		return PublishTask(task);
	}
	else
	{
		AGVTaskArr* pArr = GetTaskArray(usNo);

		return pArr->UpdateTask(task);
	}

	return false;
}

void CTaskManager::DeleteTask(unsigned short usNo)
{
	if (m_mapTaskList.find(usNo) != m_mapTaskList.end())
	{
		delete m_mapTaskList[usNo];
		m_mapTaskList.erase(m_mapTaskList.find(usNo));
	}

	return;
}

void CTaskManager::Clear()
{
	for (std::map<unsigned short, AGVTaskArr*>::iterator it = m_mapTaskList.begin(); it != m_mapTaskList.end();)
	{
		delete it->second;
		it = m_mapTaskList.erase(it);
	}
}

#endif // !_TASK_H

void Task::SetTaskTable(const TaskTable task)
{
	g_taskTable = task;

	return;
}
