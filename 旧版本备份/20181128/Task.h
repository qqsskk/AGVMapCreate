/*!
 * @file Task.h
 * @brief Task头文件
 * @author FanKaiyu
 * @date 2018-11-26
 * @version 1.0
*/

#pragma once
#include "stdafx.h"
#include <string>
#include <list>
#include <map>
#include "ADOConn.h"

using namespace std;

/*!
 * 工位说明:
 * 工位1：平台
 * 工位2：机械手AGV下料/上料点
 * 工位3：潜入式AGV等待点
 * 工位4：潜入式AGV卸料车点
 * 工位5：潜入式AGV换料车点
 * 工位6：固定机械手
 * 工位7：输送线1
 * 工位8：输送线2
*/

/*!
 * 待机位说明:
 * 待机位1：背负式AGV待机位
 * 待机位2：潜入式AGV待机位
*/

/*!
 * WMS任务表表结构
 * 表名 WMS_TASK_TABLE
 * 字段 WMSTask_Type 类型 varchar(50) 说明：任务类型，字符串类型。值为“入库”/“出库”
 * 字段 WMSTask_No 类型 int 说明：货架编号
 * 字段 WMSTask_Row 类型 int 说明：货架行号
 * 字段 WMSTask_Col 类型 int 说明：货架列号
 * 字段 WMSTask_Status 类型 int 说明：任务执行流程阶段，详见TASKIN/TASKOUT结构体
 * 字段 WMSTask_Publish 类型 datetime 说明：任务发布时间
 * 字段 WMSTask_Finish 类型 datetiem 说明：任务完成的时间
 *
 * 在任务表中仅能够存在1条任务。
*/

/*!
 * AGV任务表表结构
 * 表明 Task
 * 字段 AgvNo 类型 int 说明：执行AGV编号
 * 字段 AgvType 类型 nvarchar(50) 说明：执行AGV类型。值为“P”牵引式、“S”潜入式、“T”背负式、“A”机械手式、“L”激光式、“F”叉车式
 * 字段 Station 类型 int 说明：任务工位编号，执行返回任务时此字段代表待机位编号
 * 字段 Action 类型 nvarchar(50) 说明：任务动作。值为“移动”、“上料”、“下料”、“通行”、“返回”
 * 字段 Status 类型 nvarchar(20) 说明：任务状态。值为“未执行”，“执行”，“执行完成”
 * 字段 Erro 类型 nvarchar(20) 说明：任务异常信息。默认值:"正常"
 * 字段 IssueTime 类型 datetime 说明：任务发布时间
 * 字段 FinishTime 类型 datetime 说明：任务完成时间
*/

#define AGVTASKSTA_NONE "未执行"
#define AGVTASKSTA_EXE "执行"
#define AGVTASKSTA_FINISH "完成"
#define AGVTASKACT_MOVE "移动"
#define AGVTASKACT_LOAD "上料"
#define AGVTASKACT_UNLOAD "下料"
#define AGVTASKACT_PASS "通行"
#define AGVTASKACT_RETURN "返回"
#define AGVTYPE_P "P"
#define AGVTYPE_S "S"
#define AGVTYPE_T "T"
#define AGVTYPE_A "A"
#define AGVTYPE_F "F"
#define AGVTYPE_L "L"
#define WMSTASKTYPE_IN "入库"
#define WMSTASKTYPE_OUT "出库"

/*! @brief 入库阶段任务流程 */
enum TASKIN
{
	TASKIN_NONE,							/*!< 未执行 */
	/*!
	 * 阶段1 从输送线开始
	 * 背负->输送线2->通道->输送线1->机械手->潜入
	 */
	TASKIN_LEVEL1_MOVETOPIPELINE2,			/*!< 移动至输送线2,此时发送背负式AGV任务 */
	TASKIN_LEVEL1_UNLOADTOPIPELINE2,		/*!< 下料至输送线2,此时发送背负式AGV任务 */
	TASKIN_LEVEL1_MOVETOPIPELINE1,			/*!< 移动至输送线1,此时发送背负式AGV以及潜入式AGV任务　*/
	TASKIN_LEVEL1_PASSTOPIPELINE1,			/*!< 获取通过至输送线1,此时发送背负式AGV任务 */
	TASKIN_LEVEL1_UNLOADFROMPIPELINE1,		/*!< 从输送线1下料至潜入式AGV,此时控制机械手 */
	/*!
	 * 阶段2 从潜入式AGV开始
	 * 潜入->卸车->换车->返回
	*/
	TASKIN_LEVEL2_MOVETOUNLOAD,				/*!< 移动至卸车点,此时发送潜入式以及机械手式AGV任务 */
	/*!
	 * 阶段3 从机械手式AGV开始
	 * 机械手->上料->移动至平台->下料
	*/
	TASKIN_LEVEL3_MOVETOLOAD,				/*!< 移动至卸车点,此时发送机械手式AGV任务,潜入式卸车、移动至换车点、装车、返回任务*/
	TASKIN_LEVEL3_LOAD,						/*!< 从上料至机械手AGV */
	TASKIN_LEVEL3_MOVETOTABLE,				/*!< 机械手AGV移动至平台 */
	TASKIN_LEVEL3_UNLOAD,					/*!< 下料至平台 */
	/*!
	 * 阶段4 从平台开始
	 * 平台->入库
	*/
	TASKIN_LEVEL4_IN,						/*!< 入库至平台 */
	TASKIN_FINISH,							/*!< 任务完成 */
};

/*! @brief 出库阶段任务流程 */
enum TASKOUT
{
	TASKOUT_NONE,							/*!< 未执行 */
	/*!
	 * 阶段1 从平台开始
	 * 出库->平台->机械手AGV->卸车
	*/
	TASKOUT_LEVEL1_OUT,						/*!< 从货架出库至平台,此时发送机械手、潜入式、背负式AGV任务 */
	TASKOUT_LEVEL1_MOVETOTABLE,				/*! <机械手AGV移动至平台 */
	TASKOUT_LEVEL1_LOAD,					/*!< 从平台上料至机械手AGV */
	TASKOUT_LEVEL1_MOVETOUNLOAD,			/*!< 机械手AGV移动至卸车 */
	TASKOUT_LEVEL1_UNLOAD,					/*!< 下料至卸车点料车 */
	/*!
	 * 阶段2 从潜入式开始
	 * 等待->卸车->换车->机械手
	*/
	TASKOUT_LEVEL2_MOVETOWAIT,				/*!< 移动至等待位 */
	TASKOUT_LEVEL2_EXCHANGE,				/*!< 换车 */
	TASKOUT_LEVEL2_MOVETOARM,				/*!< 移动至机械手 */
	TASKOUT_LEVEL2_LOAD,					/*!< 机械手上料至输送线1 */
	/*!
	 * 阶段3 从输送线开始
	*/
	TASKOUT_LEVEL3_MOVETOPIPELINE1,			/*!< 移动至输送线1 */
	TASKOUT_LEVEL3_LOADFROMPIPELINE1,		/*!< 从输送线1上料 */
	TASKOUT_FINISH,					
};

/*!< @brief AGV任务结构体 */
typedef struct _AGVTaskStruct
{
	int m_unNo;			/*!< 执行AGV编号 */
	string m_strType;	/*!< 执行AGV类型 */
	int m_unStation;	/*!< 任务工位编号，执行返回任务时此字段代表待机位编号 */
	string m_strAction; /*!< 任务动作 */
	string m_strStatus; /*!< 任务状态 */
	string m_strError;	/*!< 任务异常信息 */
	string m_strIssueTime; /*!< 任务发布时间 */
	string m_strFinishTime; /*!< 任务完成时间 */

	_AGVTaskStruct()
	{
		m_unNo = 0;		
		m_strType = "";
		m_unStation = 0;
		m_strAction = "";
		m_strStatus = "";
		m_strError = "";
		m_strIssueTime = "";
		m_strFinishTime = "";
	}

	_AGVTaskStruct(const _AGVTaskStruct& agvTask)
	{
		m_unNo = agvTask.m_unNo;
		m_strType = agvTask.m_strType;
		m_unStation = agvTask.m_unStation;
		m_strAction = agvTask.m_strAction;
		m_strStatus = agvTask.m_strStatus;
		m_strError = agvTask.m_strError;
		m_strIssueTime = agvTask.m_strIssueTime;
		m_strFinishTime = agvTask.m_strFinishTime;
	}

	void operator= (const _AGVTaskStruct& agvTask)
	{
		m_unNo = agvTask.m_unNo;
		m_strType = agvTask.m_strType;
		m_unStation = agvTask.m_unStation;
		m_strAction = agvTask.m_strAction;
		m_strStatus = agvTask.m_strStatus;
		m_strError = agvTask.m_strError;
		m_strIssueTime = agvTask.m_strIssueTime;
		m_strFinishTime = agvTask.m_strFinishTime;
	}

	bool IsNull()
	{
		if (m_strType == "" || m_unStation == 0 || m_strAction == "" || m_strStatus == "" || m_strIssueTime == "")
		{
			return true;
		}

		return false;
	}

} _AGVTASK;

/*!< @brief WMS任务结构体 */
typedef struct _WMSTaskStruct
{
	unsigned int m_unNo;		/*!< 货架编号 */
	unsigned int m_unRow;		/*!< 货架行 */
	unsigned int m_unCol;		/*!< 货架列 */
	string m_strType;			/*!< 任务类型 */
	unsigned int m_unStatus;	/*!< 任务状态 */
	string m_strPublish;		/*!< 任务发布时间 */

	_WMSTaskStruct()
	{
		m_unNo = 0;
		m_unRow = 0;
		m_unCol = 0;
		m_strType = "";
		m_unStatus = 0;
		m_strPublish = "";
	}

	_WMSTaskStruct(const _WMSTaskStruct& WMSTask)
	{
		m_unNo = WMSTask.m_unNo;
		m_unRow = WMSTask.m_unRow;
		m_unCol = WMSTask.m_unCol;
		m_strType = WMSTask.m_strType;
		m_unStatus = WMSTask.m_unStatus;
		m_strPublish = WMSTask.m_strPublish;
	}

	void operator= (const _WMSTaskStruct& WMSTask)
	{
		m_unNo = WMSTask.m_unNo;
		m_unRow = WMSTask.m_unRow;
		m_unCol = WMSTask.m_unCol;
		m_strType = WMSTask.m_strType;
		m_unStatus = WMSTask.m_unStatus;
		m_strPublish = WMSTask.m_strPublish;
	}

	bool IsNull()
	{
		if (m_unNo == 0 || m_unRow == 0 || m_unCol == 0 || m_strType == "" || m_strPublish=="")
		{
			return true;
		}

		return false;
	}

} _WMSTASK;

/*!
 * @brief 创建AGV任务
 * @param ADOConn& 数据库连接串
 * @param string 执行任务的AGV类型
 * @param int 任务工位编号，执行返回任务时此字段代表待机位编号
 * @param string 执行的任务动作
 * @return bool 创建成功返回true,失败返回false
*/
static bool CreateAGVTask(ADOConn& ado,string strType, int Station, string strAction)
{
	CString cstrSql = _T("");
	cstrSql.Format(_T("insert into Task([AgvNo],[AgvType],[Station],[Action],[Status]) values(0,'%s',%d,'%s','%s')"), strType.c_str(), Station, strAction.c_str(), AGVTASKSTA_NONE);

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief 获取AGV任务
 * @param ADOConn& 数据库连接串
 * @param string 执行任务的AGV类型
 * @param int 任务工位编号，执行返回任务时此字段代表待机位编号
 * @param string 执行的任务动作
 * @return _AGVSTASK AGV任务结构体
*/
static _AGVTASK GetAGVTask(ADOConn& ado, string strType, int Station, string strAction)
{
	_AGVTASK agvTask;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from Task where [AgvType]='%s' and [Station]=%d and [Action]='%s'"), strType, Station, strAction);

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	if (pRecordset == nullptr || pRecordset->adoEOF)
	{
		return agvTask;
	}

	_variant_t var;	/*!< 字段值 */

	var = pRecordset->GetCollect(_T("AgvNo"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_unNo = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("AgvType"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_strType = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("Station"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_unStation = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("Action"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_strAction = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("Status"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_strStatus = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("Error"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_strError = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("IssueTime"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_strIssueTime = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("FinishTime"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_strFinishTime = _bstr_t(var);
	}

	return agvTask;
}

/*!
 * @brief 获取全部AGV任务
 * @param ADOConn& 数据库连接串
 * @return _AGVSTASK AGV任务结构体列表
*/
static list<_AGVTASK> GetAllAGVTask(ADOConn& ado)
{
	list<_AGVTASK> listAGV;

	CString cstrSql = _T("select * from Task order by [IssueTime]");
	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_AGVTASK agvTask;

		_variant_t var;	/*!< 字段值 */

		var = pRecordset->GetCollect(_T("AgvNo"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("AgvType"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("Station"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_unStation = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("Action"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_strAction = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("Status"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_strStatus = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("Error"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_strError = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("IssueTime"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_strIssueTime = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("FinishTime"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_strFinishTime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listAGV.push_back(agvTask);
	}

	return listAGV;
}

/*!
 * @brief 清除AGV任务
 * @param ADOConn& 数据库连接串
 * @return bool 清除成功返回true,失败返回false
*/
static bool ClearAGVTask(ADOConn& ado)
{
	CString cstrSql = _T("delete Task");

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief 创建WMS任务
 * @param ADOConn& 数据库连接串
 * @param unsigend int 货架编号
 * @param unsigend int 货架行
 * @param unsigned int 货架列
 * @param string 任务类型
 * @return bool 创建成功返回true,失败返回false
*/
static bool CreateWMSTask(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol, string strType)
{
	CString cstrSql = _T("");
	if (strType == WMSTASKTYPE_IN)
	{
		cstrSql.Format(_T("insert into WMS_TASK_TABLE([WMSTask_No],[WMSTask_Row],[WMSTask_Col],[WMSTask_Type],[WMSTask_Status]) values(%d,%d,%d,'%s',%d)"), unNo, unRow, unCol, strType.c_str(),TASKIN_NONE);
	}
	else if (strType == WMSTASKTYPE_OUT)
	{
		cstrSql.Format(_T("insert into WMS_TASK_TABLE([WMSTask_No],[WMSTask_Row],[WMSTask_Col],[WMSTask_Type],[WMSTask_Status]) values(%d,%d,%d,'%s',%d)"), unNo, unRow, unCol, strType.c_str(), TASKOUT_NONE);
	}

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief 更新WMS任务
 * @param ADOConn& 数据库连接串
 * @param unsigend int 货架编号
 * @param unsigend int 货架行
 * @param unsigned int 货架列
 * @param string 任务类型
 * @param unsigned int 任务状态
 * @return bool 创建成功返回true,失败返回false
*/
static bool UpdateWMSTask(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol, string strType,unsigned int unStatus)
{
	CString cstrSql = _T("");

	if (unStatus == TASKOUT_FINISH || unStatus == TASKIN_FINISH)
	{
		cstrSql.Format(_T("update WMS_TASK_TABLE set [WMSTask_Status]=%d,[WMSTask_Finish]=getdate() where [WMSTask_No]=%d and [WMSTask_Row]=%d and [WMSTask_Col]=%d and [WMSTask_Type]='%s'"), unStatus, unNo, unRow, unCol, strType.c_str());
	}
	else
	{
		cstrSql.Format(_T("update WMS_TASK_TABLE set [WMSTask_Status]=%d where [WMSTask_No]=%d and [WMSTask_Row]=%d and [WMSTask_Col]=%d and [WMSTask_Type]='%s'"), unStatus, unNo, unRow, unCol, strType.c_str());
	}

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief 清除全部WMS任务
 * @param ADOConn& 数据库连接串
 * @return bool 创建成功返回true,失败返回false
*/
static bool ClearAllWMSTask(ADOConn& ado)
{
	CString cstrSql = _T("delete WMS_TASK_TABLE");

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief 清除WMS任务
 * @param ADOConn& 数据库连接串
 * @param unsigend int 货架编号
 * @param unsigend int 货架行
 * @param unsigned int 货架列
 * @param string 任务类型
 * @return bool 创建成功返回true,失败返回false
*/
static bool ClearWMSTask(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol, string strType)
{
	CString cstrSql = _T("");
	cstrSql.Format(_T("delete WMS_TASK_TABLE where [WMSTask_No]=%d and [WMSTask_Row]=%d and [WMSTask_Col]=%d and [WMSTask_Type]='%s'"), unNo, unRow, unCol, strType.c_str());

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief 获取全部WMS任务
 * @param ADOConn& 数据库连接串
 * @return list<_WMSTASK> WMS任务结构体列表
*/
static list<_WMSTASK> GetAllWMSTask(ADOConn& ado)
{
	list<_WMSTASK> listWMS;

	CString cstrSql = _T("select * from WMS_TASK_TABLE order by [WMSTask_Publish]");
	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_WMSTASK task;

		_variant_t var;	/*!< 字段值 */

		var = pRecordset->GetCollect(_T("WMSTask_No"));
		if (var.vt != VT_NULL)
		{
			task.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSTask_Row"));
		if (var.vt != VT_NULL)
		{
			task.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSTask_Col"));
		if (var.vt != VT_NULL)
		{
			task.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSTask_Type"));
		if (var.vt != VT_NULL)
		{
			task.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSTask_Status"));
		if (var.vt != VT_NULL)
		{
			task.m_unStatus = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSTask_Publish"));
		if (var.vt != VT_NULL)
		{
			task.m_strPublish = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listWMS.push_back(task);
	}

	return listWMS;
}

/*!
 * @brief 获取WMS任务
 * @param ADOConn& 数据库连接串
 * @param unsigend int 货架编号
 * @param unsigend int 货架行
 * @param unsigned int 货架列
 * @param string 任务类型
 * @return _WMSTASK WMS任务结构体
*/
static _WMSTASK GetWMSTask(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol, string strType)
{
	_WMSTASK task;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_TASK_TABLE where [WMSTask_No]=%d and [WMSTask_Row]=%d and [WMSTask_Col]=%d and [WMSTask_Type]='%s'"),unNo, unRow, unCol, strType.c_str());

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	if (pRecordset == nullptr || pRecordset->adoEOF)
	{
		return task;
	}

	_variant_t var;	/*!< 字段值 */

	var = pRecordset->GetCollect(_T("WMSTask_No"));
	if (var.vt != VT_NULL)
	{
		task.m_unNo = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSTask_Row"));
	if (var.vt != VT_NULL)
	{
		task.m_unRow = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSTask_Col"));
	if (var.vt != VT_NULL)
	{
		task.m_unCol = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSTask_Type"));
	if (var.vt != VT_NULL)
	{
		task.m_strType = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("WMSTask_Status"));
	if (var.vt != VT_NULL)
	{
		task.m_unStatus = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSTask_Publish"));
	if (var.vt != VT_NULL)
	{
		task.m_strPublish = _bstr_t(var);
	}

	return task;
}