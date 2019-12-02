/*!
 * @file CTaskManager.h
 * @brief 任务管理器基类文件
 *
 * 任务管理器基类属性文件
 * @author FanKaiyu
 * @date 2019-02-18
 * @version 1.0
*/

#pragma once
#include <string>
#include <thread>
#include <mutex>
#include "ADO/ADOConn.h"

#ifndef _TASK_H
#define _TASK_H

#define WM_UPDATE_TASK WM_USER+3

/*!
 * 任务动作
*/
#ifdef _UNICODE
#define _TASKACTION_W_EMPTYMOVE		L"空载移动"
#define _TASKACTION_W_FULLMOVE		L"满载移动"
#define _TASKACTION_W_MOVE			L"移动"
#define _TASKACTION_W_LOAD			L"上料"
#define _TASKACTION_W_UNLOAD		L"下料"
#define _TASKACTION_W_END			L"结束"
#endif 

#define _TASKACTION_A_EMPTYMOVE		"空载移动"
#define _TASKACTION_A_FULLMOVE		"满载移动"
#define _TASKACTION_A_MOVE			"移动"
#define _TASKACTION_A_LOAD			"上料"
#define _TASKACTION_A_UNLOAD		"下料"
#define _TASKACTION_A_END			"结束"

#ifdef _AFX
#define _TASKACTION_EMPTYMOVE		_T("空载移动")
#define _TASKACTION_FULLMOVE		_T("满载移动")
#define _TASKACTION_MOVE			_T("移动")
#define _TASKACTION_LOAD			_T("上料")
#define _TASKACTION_UNLOAD			_T("下料")
#define _TASKACTION_END				_T("结束")
#endif 

/*!
 * 任务异常
*/
#ifdef _AFX
#define _TASKERROR_NONE				_T("正常")
#define _TASKERROR_TYPE				_T("任务无效:该类型AGV无法执行此类任务")
#define _TASKERROR_TARGET			_T("任务无效:不是有效的坐标值,无法执行任务")
#define _TASKERROR_ACTION			_T("任务无效:无效的任务动作")
#define _TASKERROR_ACTOVERTIME		_T("任务错误:任务动作执行超时")
#define _TASKERROR_OUTLINE			_T("任务错误:执行任务的AGV离线")
#define _TASKERROR_FULL				_T("任务错误:AGV已满载,无法上料")
#define _TASKERROR_EMPTY			_T("任务错误:AGV已空载,无法下料")
#endif

#ifdef _UNICODE
#define _TASKERROR_W_NONE			L"正常"
#define _TASKERROR_W_TYPE			L"任务无效:该类型AGV无法执行此类任务"
#define _TASKERROR_W_TARGET			L"任务无效:不是有效的坐标值,无法执行任务"
#define _TASKERROR_W_ACTION			L"任务无效:无效的任务动作"
#define _TASKERROR_W_ACTOVERTIME	L"任务错误:任务动作执行超时"
#define _TASKERROR_W_OUTLINE		L"任务错误:执行任务的AGV离线"
#define _TASKERROR_W_FULL			L"任务错误:AGV已满载,无法上料"
#define _TASKERROR_W_EMPTY			L"任务错误:AGV已空载,无法下料"
#endif

#define _TASKERROR_A_NONE			"正常"
#define _TASKERROR_A_TYPE			"任务无效:该类型AGV无法执行此类任务"
#define _TASKERROR_A_TARGET			"任务无效:不是有效的坐标值,无法执行任务"
#define _TASKERROR_A_ACTION			"任务无效:无效的任务动作"
#define _TASKERROR_A_ACTOVERTIME	"任务错误:任务动作执行超时"
#define _TASKERROR_A_OUTLINE		"任务错误:执行任务的AGV离线"
#define _TASKERROR_A_FULL			"任务错误:AGV已满载,无法上料"
#define _TASKERROR_A_EMPTY			"任务错误:AGV已空载,无法下料"


namespace Task
{
	/*!
	 * 任务状态
	*/
	enum _TASKSTATUS
	{
		_TASKSTA_NONE,		/*!< 未执行 */
		_TASKSTA_EXECUTE,	/*!< 执行 */
		_TASKSTA_FINISH,	/*!< 完成:完成的任务不可以恢复 */
		_TASKSTA_PAUSE,		/*!< 暂停:暂停的任务可以恢复 */
		_TASKSTA_CANCEL,	/*!< 取消:调过当前任务阶段跳至下一任务阶段 */
	};

	typedef struct _TaskTable_Struct
	{
		std::string m_strTable;			/*!< 任务表名 */
		std::string m_strHisTable;		/*!< 任务历史表名 */
		std::string m_strNo;			/*!< 字段:任务号 */
		std::string m_strLevel;			/*!< 字段:阶段号 */
		std::string m_strTarget;		/*!< 字段:任务坐标 */
		std::string m_strBegin;			/*!< 字段:起始坐标 */
		std::string m_strAction;		/*!< 字段:任务动作 */
		std::string m_strStatus;		/*!< 字段:任务状态 */
		std::string m_strExecuter;		/*!< 字段:执行者 */
		std::string m_strError;			/*!< 字段:异常信息 */
		std::string m_strPublish;		/*!< 字段:发布时间 */
		std::string m_strFinish;		/*!< 字段:完成时间 */

		_TaskTable_Struct()
		{
			m_strTable = "TASK_RUNTIME_TABLE";
			m_strHisTable = "TASK_HISTORY_TABLE";
			m_strNo = "task_no";
			m_strLevel = "task_level";
			m_strTarget = "task_target";
			m_strBegin = "task_begin";
			m_strAction = "task_action";
			m_strStatus = "task_status";
			m_strExecuter = "task_executer";
			m_strError = "task_error";
			m_strPublish = "task_publish_time";
			m_strFinish = "task_finish_time";
		}

		_TaskTable_Struct(const _TaskTable_Struct& table)
		{
			m_strTable = table.m_strTable;
			m_strHisTable = table.m_strHisTable;
			m_strNo = table.m_strNo;
			m_strLevel = table.m_strLevel;
			m_strTarget = table.m_strTarget;
			m_strBegin = table.m_strBegin;
			m_strAction = table.m_strAction;
			m_strStatus = table.m_strStatus;
			m_strExecuter = table.m_strExecuter;
			m_strError = table.m_strError;
			m_strPublish = table.m_strPublish;
			m_strFinish = table.m_strFinish;
		}

		void operator=(const _TaskTable_Struct& table)
		{
			m_strTable = table.m_strTable;
			m_strHisTable = table.m_strHisTable;
			m_strNo = table.m_strNo;
			m_strLevel = table.m_strLevel;
			m_strTarget = table.m_strTarget;
			m_strBegin = table.m_strBegin;
			m_strAction = table.m_strAction;
			m_strStatus = table.m_strStatus;
			m_strError = table.m_strError;
			m_strExecuter = table.m_strExecuter;
			m_strPublish = table.m_strPublish;
			m_strFinish = table.m_strFinish;
		}

		/*!
		 * @brief 设置执行者字段名
		 * @param string 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetExecuter(std::string strExecuter)
		{
			if (m_strExecuter == strExecuter)
			{
				return false;
			}

			m_strExecuter = strExecuter;

			return true;
		}

		/*!
		 * @brief 设置执行者字段名
		 * @param wstring 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetExecuter(std::wstring wstrExecuter)
		{
			std::string strExecuter = "";
			BufferString::std_wstring_to_string(strExecuter, wstrExecuter);

			return SetExecuter(strExecuter);
		}

		/*!
		 * @brief 设置执行者字段名
		 * @param CString 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetExecuter(CString cstrExecuter)
		{
			std::string strExecuter = "";

			BufferString::CString_to_std_string(strExecuter, cstrExecuter);

			return SetExecuter(strExecuter);
		}

		/*!
		 * @brief 获取执行者字段名
		 * @param string& 字段名
		*/
		std::string GetExecuter() const
		{
			return m_strExecuter;
		}

		/*!
		 * @brief 获取执行者字段名
		 * @param string& 字段名
		*/
		void GetExecuter(std::string& strExecuter)
		{
			strExecuter = m_strExecuter;

			return;
		}

		/*!
		 * @brief 获取执行者字段名
		 * @param wstring& 字段名
		*/
		void GetExecuter(std::wstring& wstrExecuter)
		{
			BufferString::std_string_to_wstring(wstrExecuter, m_strExecuter);

			return;
		}

		/*!
		 * @brief 获取执行者字段名
		 * @param CString& 字段名
		*/
		void GetExecuter(CString& cstrExecuter)
		{
			BufferString::std_string_to_CString(cstrExecuter, m_strExecuter);

			return;
		}

		/*!
		 * @brief 设置完成时间字段名
		 * @param string 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetFinish(std::string strFinish)
		{
			if (m_strFinish == strFinish)
			{
				return false;
			}

			m_strFinish = strFinish;

			return true;
		}

		/*!
		 * @brief 设置完成时间字段名
		 * @param wstring 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetFinish(std::wstring wstrFinish)
		{
			std::string strFinish = "";
			BufferString::std_wstring_to_string(strFinish, wstrFinish);

			return SetFinish(strFinish);
		}

		/*!
		 * @brief 获取完成时间字段名
		 * @param string& 字段名
		*/
		std::string GetFinish() const
		{
			return m_strFinish;
		}

		/*!
		 * @brief 获取完成时间字段名
		 * @param string& 字段名
		*/
		void GetFinish(std::string& strFinish)
		{
			strFinish = m_strFinish;

			return;
		}

		/*!
		 * @brief 获取完成时间字段名
		 * @param wstring& 字段名
		*/
		void GetFinish(std::wstring& wstrFinish)
		{
			BufferString::std_string_to_wstring(wstrFinish, m_strFinish);

			return;
		}
		
		/*!
		 * @brief 设置发布时间字段名
		 * @param string 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetPublish(std::string strPublish)
		{
			if (m_strPublish == strPublish)
			{
				return false;
			}

			m_strPublish = strPublish;

			return true;
		}

		/*!
		 * @brief 设置发布时间字段名
		 * @param wstring 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetPublish(std::wstring wstrPublish)
		{
			std::string strPublish = "";
			BufferString::std_wstring_to_string(strPublish, wstrPublish);

			return SetPublish(strPublish);
		}

		/*!
		 * @brief 获取发布时间字字段名
		 * @return string 字段名
		*/
		std::string GetPublish() const
		{
			return m_strPublish;
		}

		/*!
		 * @brief 获取发布时间字段名
		 * @param string& 字段名
		*/
		void GetPublish(std::string& strPublish)
		{
			strPublish = m_strPublish;

			return;
		}

		/*!
		 * @brief 获取发布时间字段名
		 * @param wstring& 字段名
		*/
		void GetPublish(std::wstring& wstrPublish)
		{
			BufferString::std_string_to_wstring(wstrPublish, m_strPublish);

			return;
		}

		/*!
		 * @brief 设置异常信息字段名
		 * @param string 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetError(std::string strError)
		{
			if (m_strError == strError)
			{
				return false;
			}

			m_strError = strError;

			return true;
		}

		/*!
		 * @brief 设置异常信息字段名
		 * @param wstring 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetError(std::wstring wstrError)
		{
			std::string strError = "";
			BufferString::std_wstring_to_string(strError, wstrError);

			return SetError(strError);
		}

		/*!
		 * @brief 获取异常信息字字段名
		 * @return string 字段名
		*/
		std::string GetError() const
		{
			return m_strError;
		}

		/*!
		 * @brief 获取异常信息字段名
		 * @param string& 字段名
		*/
		void GetError(std::string& strError)
		{
			strError = m_strError;

			return;
		}

		/*!
		 * @brief 获取异常信息字段名
		 * @param wstring& 字段名
		*/
		void GetError(std::wstring& wstrError)
		{
			BufferString::std_string_to_wstring(wstrError, m_strError);

			return;
		}

		/*!
		 * @brief 设置任务状态字段名
		 * @param string 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetStatus(std::string strStatus)
		{
			if (m_strStatus == strStatus)
			{
				return false;
			}

			m_strStatus = strStatus;

			return true;
		}

		/*!
		 * @brief 设置任务状态字段名
		 * @param wstring 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetStatus(std::wstring wstrStatus)
		{
			std::string strStatus = "";
			BufferString::std_wstring_to_string(strStatus, wstrStatus);

			return SetStatus(strStatus);
		}

		/*!
		 * @brief 获取任务状态字字段名
		 * @return string 字段名
		*/
		std::string GetStatus() const
		{
			return m_strStatus;
		}

		/*!
		 * @brief 获取任务状态字段名
		 * @param string& 字段名
		*/
		void GetStatus(std::string strStatus)
		{
			strStatus = m_strStatus;

			return;
		}

		/*!
		 * @brief 获取务任状态字段名
		 * @param wstring& 字段名
		*/
		void GetStatus(std::wstring wstrStatus)
		{
			BufferString::std_string_to_wstring(wstrStatus, m_strStatus);

			return;
		}

		/*!
		 * @brief 设置任务动作字段名
		 * @param string 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetAction(std::string strAction)
		{
			if (m_strAction == strAction)
			{
				return false;
			}

			m_strAction = strAction;

			return true;
		}

		/*!
		 * @brief 设置任务动作字段名
		 * @param wstring 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetAction(std::wstring wstrAction)
		{
			std::string strAction = "";
			BufferString::std_wstring_to_string(strAction, wstrAction);

			return SetAction(strAction);
		}

		/*!
		 * @brief 获取任务动作字段名
		 * @return string 字段名
		*/
		std::string GetAction() const
		{
			return m_strAction;
		}

		/*!
		 * @brief 获取任务动作字段名
		 * @param string& 字段名
		*/
		void GetAction(std::string& strAction)
		{
			strAction = m_strAction;

			return;
		}

		/*!
		 * @brief 获取任务动作字段名
		 * @param wstring& 字段名
		*/
		void GetAction(std::wstring& wstrAction)
		{
			BufferString::std_string_to_wstring(wstrAction, m_strAction);

			return;
		}

		/*!
		 * @brief 设置起始坐标字段名
		 * @param string 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetBegin(std::string strBegin)
		{
			if (m_strBegin == strBegin)
			{
				return false;
			}

			m_strBegin = strBegin;

			return true;
		}

		/*!
		 * @brief 设置起始坐标字段名
		 * @param wstring 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetBegin(std::wstring wstrBegin)
		{
			std::string strBegin = "";
			BufferString::std_wstring_to_string(strBegin, wstrBegin);

			return SetBegin(strBegin);
		}

		/*!
		 * @brief 获取起始坐标字段名
		 * @return string 字段名
		*/
		std::string GetBegin() const
		{
			return m_strBegin;
		}

		/*!
		 * @brief 获取起始坐标字段名
		 * @param string& 字段名
		*/
		void GetBegin(std::string& strBegin)
		{
			strBegin = m_strBegin;

			return;
		}

		/*!
		 * @brief 获取起始坐标字段名
		 * @param wstring& 字段名
		*/
		void GetBegin(std::wstring& wstrBegin)
		{
			BufferString::std_string_to_wstring(wstrBegin, m_strBegin);

			return;
		}

		/*!
		 * @brief 设置任务坐标字段名
		 * @param string 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetTarget(std::string strTarget)
		{
			if (m_strTarget == strTarget)
			{
				return false;
			}

			m_strTarget = strTarget;

			return true;
		}

		/*!
		 * @brief 设置任务坐标字段名
		 * @param wstring 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetTarget(std::wstring wstrTarget)
		{
			std::string strTarget = "";
			BufferString::std_wstring_to_string(strTarget, wstrTarget);

			return SetTarget(strTarget);
		}

		/*!
		 * @brief 获取任务坐标字段名
		 * @return string 字段名
		*/
		std::string GetTarget() const
		{
			return m_strTarget;
		}

		/*!
		 * @brief 获取任务坐标字段名
		 * @param string& 字段名
		*/
		void GetTarget(std::string& strTarget)
		{
			strTarget = m_strTarget;

			return;
		}

		/*!
		 * @brief 获取任务坐标字段名
		 * @param wstring& 字段名
		*/
		void GetTarget(std::wstring& wstrTarget)
		{
			BufferString::std_string_to_wstring(wstrTarget, m_strTarget);

			return;
		}

		/*!
		 * @brief 设置阶段号字段名
		 * @param string 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetLevel(std::string strLevel)
		{
			if (m_strLevel == strLevel)
			{
				return false;
			}

			m_strLevel = strLevel;

			return true;
		}

		/*!
		 * @brief 设置阶段号字段名
		 * @param wstring 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetLevel(std::wstring wstrLevel)
		{
			std::string strLevel = "";
			BufferString::std_wstring_to_string(strLevel, wstrLevel);

			return SetLevel(strLevel);
		}

		/*!
		 * @brief 获取阶段号字段名
		 * @return string 字段名
		*/
		std::string GetLevel() const
		{
			return m_strLevel;
		}

		/*!
		 * @brief 获取阶段号字段名
		 * @param string& 字段名
		*/
		void GetLevel(std::string& strLevel)
		{
			strLevel = m_strLevel;

			return;
		}

		/*!
		 * @brief 获取阶段号字段名
		 * @param wstring& 字段名
		*/
		void GetLevel(std::wstring& wstrLevel)
		{
			BufferString::std_string_to_wstring(wstrLevel, m_strLevel);

			return;
		}

		/*!
		 * @brief 设置任务号字段名
		 * @param string 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetNo(std::string strNo)
		{
			if (m_strNo == strNo)
			{
				return false;
			}

			m_strNo = strNo;
			
			return true;
		}

		/*!
		 * @brief 设置任务号字段名
		 * @param wstring 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetNo(std::wstring wstrNo)
		{
			std::string strNo = "";
			BufferString::std_wstring_to_string(strNo, wstrNo);

			return SetNo(strNo);
		}

		/*!
		 * @brief 获取任务号字段名
		 * @return string 字段名
		*/
		std::string GetNo() const
		{
			return m_strNo;
		}

		/*!
		 * @brief 获取任务号字段名
		 * @param string& 字段名
		*/
		void GetNo(std::string strNo)
		{
			m_strNo = strNo;

			return;
		}

		/*!
		 * @brief 获取任务号字段名
		 * @param wstring& 字段名
		*/
		void GetNo(std::wstring& wstrNo)
		{
			BufferString::std_string_to_wstring(wstrNo, m_strNo);

			return;
		}

		/*!
		 * @brief 设置任务表名
		 * @param string 表名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetTable(std::string strTable)
		{
			if (m_strTable == strTable)
			{
				return false;
			}

			m_strTable = strTable;

			return true;
		}

		/*!
		 * @brief 设置任务表名
		 * @param wstring 表名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetTable(std::wstring wstrTable)
		{
			std::string strTable = "";

			BufferString::std_wstring_to_string(strTable, wstrTable);

			return SetTable(strTable);
		}

		/*!
		 * @brief 获取任务表名
		 * @param string& 表名
		*/
		void GetTable(std::string& strTable)
		{
			strTable = m_strTable;

			return;
		}

		/*!
		 * @brief 获取任务表名
		 * @return string 表名
		*/
		std::string GetTable() const
		{
			return m_strTable;
		}

		/*!
		 * @brief 获取任务表名
		 * @param wstring& 表名
		*/
		void GetTable(std::wstring& wstrTable)
		{
			BufferString::std_string_to_wstring(wstrTable, m_strTable);

			return;
		}

		/*!
		 * @brief 设置任务历史表名
		 * @param string 表名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetHisTable(std::string strHisTable)
		{
			if (m_strHisTable == strHisTable)
			{
				return false;
			}

			m_strHisTable = strHisTable;

			return true;
		}

		/*!
		 * @brief 设置任务历史表名
		 * @param wstring 表名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetHisTable(std::wstring wstrHisTable)
		{
			std::string strHisTable = "";

			BufferString::std_wstring_to_string(strHisTable, wstrHisTable);

			return SetHisTable(strHisTable);
		}

		/*!
		 * @brief 获取任务历史表名
		 * @param string& 表名
		*/
		void GetHisTable(std::string& strHisTable)
		{
			strHisTable = m_strHisTable;

			return;
		}

		/*!
		 * @brief 获取任务历史表名
		 * @return string 表名
		*/
		std::string GetHisTable() const
		{
			return m_strHisTable;
		}

		/*!
		 * @brief 获取任务历史表名
		 * @param wstring& 表名
		*/
		void GetHisTable(std::wstring& wstrHisTable)
		{
			BufferString::std_string_to_wstring(wstrHisTable, m_strHisTable);

			return;
		}

#ifdef _AFX
		/*!
		 * @brief 设置任务表名
		 * @param CString 表名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetTable(CString cstrTable)
		{
			std::string strTable = "";

			BufferString::CString_to_std_string(strTable, cstrTable);

			return SetTable(strTable);
		}

		/*!
		 * @brief 获取任务表名
		 * @param CString& 表名
		*/
		void GetTable(CString& cstrTable)
		{
			BufferString::std_string_to_CString(cstrTable, m_strTable);

			return;
		}

		/*!
		 * @brief 设置任务历史表名
		 * @param CString 表名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetHisTable(CString cstrHisTable)
		{
			std::string strHisTable = "";

			BufferString::CString_to_std_string(strHisTable, cstrHisTable);

			return SetHisTable(strHisTable);
		}

		/*!
		 * @brief 获取任务历史表名
		 * @param CString& 表名
		*/
		void GetHisTable(CString& cstrHisTable)
		{
			BufferString::std_string_to_CString(cstrHisTable, m_strHisTable);

			return;
		}

		/*!
		 * @brief 设置任务号字段名
		 * @param CString 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetNo(CString cstrNo)
		{
			std::string strNo = "";
			BufferString::CString_to_std_string(strNo, cstrNo);

			return SetNo(strNo);
		}

		/*!
		 * @brief 获取任务号字段名
		 * @param CString& 字段名
		*/
		void GetNo(CString& cstrNo)
		{
			BufferString::std_string_to_CString(cstrNo, m_strNo);

			return;
		}

		/*!
		 * @brief 设置阶段号字段名
		 * @param CString 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetLevel(CString cstrLevel)
		{
			std::string strLevel = "";

			BufferString::CString_to_std_string(strLevel, cstrLevel);

			return SetLevel(strLevel);
		}

		/*!
		 * @brief 获取阶段号字段名
		 * @param CSstring& 字段名
		*/
		void GetLevel(CString& cstrLevel)
		{
			BufferString::std_string_to_CString(cstrLevel, m_strTable);

			return;
		}

		/*!
		 * @brief 设置任务坐标字段名
		 * @param CString 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetTarget(CString cstrTarget)
		{
			std::string strTarget = "";

			BufferString::CString_to_std_string(strTarget, cstrTarget);

			return SetTarget(cstrTarget);
		}

		/*!
		 * @brief 获取任务坐标字段名
		 * @param CString& 字段名
		*/
		void GetTarget(CString& cstrTarget)
		{
			BufferString::std_string_to_CString(cstrTarget, m_strTarget);

			return;
		}

		/*!
		 * @brief 设置起始坐标字段名
		 * @param CString 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetBegin(CString cstrBegin)
		{
			std::string strBegin = "";

			BufferString::CString_to_std_string(strBegin, cstrBegin);

			return SetBegin(strBegin);
		}

		/*!
		 * @brief 获取起始坐标字段名
		 * @param CString& 字段名
		*/
		void GetBegin(CString& cstrBegin)
		{
			BufferString::std_string_to_CString(cstrBegin, m_strBegin);

			return;
		}

		/*!
		 * @brief 设置任务动作字段名
		 * @param cstring 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetAction(CString cstrAction)
		{
			std::string strAction = "";

			BufferString::CString_to_std_string(strAction, cstrAction);

			return SetAction(strAction);
		}

		/*!
		 * @brief 获取任务动作字段名
		 * @param CString& 字段名
		*/
		void GetAction(CString& cstrAction)
		{
			BufferString::std_string_to_CString(cstrAction, m_strAction);

			return;
		}

		/*!
		 * @brief 设置任务状态字段名
		 * @param CString 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetStatus(CString cstrStatus)
		{
			std::string strStatus = "";

			BufferString::CString_to_std_string(strStatus, cstrStatus);

			return SetStatus(strStatus);
		}

		/*!
		 * @brief 获取任务状态字段名
		 * @param CString& 字段名
		*/
		void GetStatus(CString cstrStatus)
		{
			BufferString::std_string_to_CString(cstrStatus, m_strStatus);

			return;
		}

		/*!
		 * @brief 设置异常信息字段名
		 * @param CString 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetError(CString cstrError)
		{
			std::string strError = "";

			BufferString::CString_to_std_string(strError, cstrError);

			return SetError(strError);
		}

		/*!
		 * @brief 获取异常信息字段名
		 * @param CString& 字段名
		*/
		void GetError(CString& cstrError)
		{
			BufferString::std_string_to_CString(cstrError, m_strError);

			return;
		}

		/*!
		 * @brief 设置发布时间字段名
		 * @param CString 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetPublish(CString cstrPublish)
		{
			std::string strPublish = "";

			BufferString::CString_to_std_string(strPublish, cstrPublish);

			return SetPublish(strPublish);
		}

		/*!
		 * @brief 获取发布时间字段名
		 * @param CString& 字段名
		*/
		void GetPublish(CString& cstrPublish)
		{
			BufferString::std_string_to_CString(cstrPublish, m_strPublish);

			return;
		}

		/*!
		 * @brief 设置完成时间字段名
		 * @param CString 字段名
		 * @return bool 设置成功返回true,失败返回false
		*/
		bool SetFinish(CString cstrFinish)
		{
			std::string strFinish = "";

			BufferString::CString_to_std_string(strFinish, cstrFinish);

			return SetFinish(strFinish);
		}

		/*!
		 * @brief 获取完成时间字段名
		 * @param CString& 字段名
		*/
		void GetFinish(CString& cstrFinish)
		{
			BufferString::std_string_to_CString(cstrFinish, m_strFinish);

			return;
		}
#endif // _AFX
	} TaskTable;

	static TaskTable g_taskTable;

	/*!
	 * @brief 设置任务表
	 * @param TaskTable 任务表结构体
	*/
	static void SetTaskTable(const TaskTable task);
}

/*!< @brief 描述AGV任务的基本结构块 */
typedef struct _TaskStruct
{
protected:
	unsigned short m_usNo;		/*!< 任务号:为0时任务无效 */
	unsigned char m_byLevel;	/*!< 阶段号:为0时任务无效 */
	std::string m_strAction;	/*!< 任务动作:该阶段AGV应执行的动作 */
	unsigned char m_byStatus;	/*!< 任外状态:该阶段任务执行的状态 */
	unsigned char m_byExecuter;	/*!< 任务执行者:执行该阶段任务的AGV编号 */
	unsigned short m_usTarget;	/*!< 任务坐标:通常为工位所在RFID卡编号,为0时任务无效 */
	unsigned short m_usBegin;	/*!< 起始坐标:通常为AGV所在的起始坐标，为0时则由系统分配AGV执行任务 */
	std::string m_strError;		/*!< 任务异常信息 */

public:
	_TaskStruct()
	{
		m_usNo = 0;
		m_byLevel = 0;
		m_strAction = "";
		m_byStatus = Task::_TASKSTA_NONE;
		m_byExecuter = 0;
		m_usTarget = 0;
		m_usBegin = 0;
		m_strError = _TASKERROR_A_NONE;
	}

	_TaskStruct(unsigned short usNo, unsigned char byLevel, std::string strAction, unsigned short usTarget,
		unsigned short usBegin = 0,unsigned char byStatus = Task::_TASKSTA_NONE, unsigned char byExecuter = 0
		, std::string strError = _TASKERROR_A_NONE)
	{
		m_usNo = usNo;
		m_byLevel = byLevel;
		m_strAction = strAction;
		m_byStatus = byStatus;
		m_byExecuter = byExecuter;
		m_usTarget = usTarget;
		m_usBegin = usBegin;
		m_strError = strError;
	}

	_TaskStruct(unsigned short usNo, unsigned char byLevel, std::wstring wstrAction, unsigned short usTarget,
		unsigned short usBegin = 0, unsigned char byStatus = Task::_TASKSTA_NONE, unsigned char byExecuter = 0
		, std::string strError = _TASKERROR_A_NONE)
	{
		m_usNo = usNo;
		m_byLevel = byLevel;
		BufferString::std_wstring_to_string(m_strAction,wstrAction);
		m_byStatus = byStatus;
		m_byExecuter = byExecuter;
		m_usTarget = usTarget;
		m_usBegin = usBegin;
		m_strError = strError;
	}

#ifdef _AFX
	_TaskStruct(unsigned short usNo, unsigned char byLevel, CString cstrAction, unsigned short usTarget,
		unsigned short usBegin = 0, unsigned char byStatus = Task::_TASKSTA_NONE, unsigned char byExecuter = 0
		, std::string strError = _TASKERROR_A_NONE)
	{
		m_usNo = usNo;
		m_byLevel = byLevel;
		BufferString::CString_to_std_string(m_strAction,cstrAction);
		m_byStatus = byStatus;
		m_byExecuter = byExecuter;
		m_usTarget = usTarget;
		m_usBegin = usBegin;
		m_strError = strError;
	}
#endif

	_TaskStruct(const _TaskStruct& task)
	{
		m_usNo = task.m_usNo;
		m_byLevel = task.m_byLevel;
		m_strAction = task.m_strAction;
		m_byStatus = task.m_byStatus;
		m_byExecuter = task.m_byExecuter;
		m_usTarget = task.m_usTarget;
		m_usBegin = task.m_usBegin;
		m_strError = task.m_strError;
	}

	void operator=(const _TaskStruct& task)
	{
		m_usNo = task.m_usNo;
		m_byLevel = task.m_byLevel;
		m_strAction = task.m_strAction;
		m_byStatus = task.m_byStatus;
		m_byExecuter = task.m_byExecuter;
		m_usTarget = task.m_usTarget;
		m_usBegin = task.m_usBegin;
		m_strError = task.m_strError;
	}

public:
	/*!
	 * @brief 任务交换
	 *
	 * 交换除执行者以及执行状态外的全部信息
	 * 并重置异常信息
	 * @param _TaskStruct& 交换的任务结构体
	*/
	void Swop(_TaskStruct& task)
	{
		_TaskStruct tmp(task);

		task.m_usNo = m_usNo;
		task.m_byLevel = m_byLevel;
		task.m_strAction = m_strAction;
		task.m_usTarget = m_usTarget;
		task.m_usBegin = m_usBegin;
		task.m_strError = _TASKERROR_A_NONE;

		m_usNo = tmp.m_usNo;
		m_byLevel = tmp.m_byLevel;
		m_strAction = tmp.m_strAction;
		m_usTarget = tmp.m_usTarget;
		m_usBegin = tmp.m_usBegin;
		m_strError = _TASKERROR_A_NONE;

		return;
	}

	/*!
	 * @brief 任务更新
	 * 
	 * 更新任务内容
	 * @param const _TaskStruct& 更新的任务结构体
	*/
	bool Update(_TaskStruct task)
	{
		if (m_usNo != task.GetNo() || m_byLevel != task.GetLevel())
		{
			return false;
		}

		std::string strAction = "";	/*!< 任务动作 */
		std::string strError = "";	/*!< 异常信息 */
		task.GetAction(strAction);
		task.GetError(strError);

		if (SetTarget(task.GetTarget())
			|| SetBegin(task.GetBegin())
			|| SetAction(strAction)
			|| SetExecuter(task.GetExecuter())
			|| SetStatus(task.GetStatus())
			|| SetError(strError))
		{
			return true;
		}

		return false;
	}

	/*!
	 * @brief 任务是否有效
	 * @return bool 无效返回true,有效返回false
	*/
	bool IsNull()
	{
		if (m_usNo == 0 || m_byLevel == 0 || m_strAction == "")
		{
			return true;
		}

		return false;
	}

	/*!
	 * @brief 设置任务号
	 * @param unsigned short 新任务号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetNo(unsigned short usNo)
	{
		if (usNo == 0)
		{
			return false;
		}

		if (m_usNo == usNo)
		{
			return false;
		}

		m_usNo = usNo;

		return true;
	}
	
	/*!
	 * @brief 获取任务号
	 * @return unsigned short 任务号
	*/
	unsigned short GetNo() const
	{
		return m_usNo;
	}
	
	/*!
	 * @brief 获取任务号
	 * @param string& 任务号
	*/
	void GetNo(std::string &strNo)
	{
		BufferString::std_string_format(strNo, "%d", m_usNo);

		return;
	}

	/*!
	 * @brief 获取任务号
	 * @param wstring& 任务号
	*/
	void GetNo(std::wstring& wstrNo)
	{
		std::string strNo = "";
		GetNo(strNo);

		BufferString::std_string_to_wstring(wstrNo, strNo);

		return;
	}

	/*!
	 * @brief 设置阶段号
	 * @param unsigned char 新阶段号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetLevel(unsigned char byLevel)
	{
		if (byLevel == 0)
		{
			return false;
		}

		if (m_byLevel == byLevel)
		{
			return false;
		}

		m_byLevel = byLevel;
		
		return true;
	}

	/*!
	 * @brief 获取阶段号
	 * @return unsigned char 阶段号
	*/
	unsigned char GetLevel() const
	{
		return m_byLevel;
	}

	/*!
	 * @brief 获取阶段号
	 * @param string& 阶段号
	*/
	void GetLevel(std::string& strLevel)
	{
		BufferString::std_string_format(strLevel, "%d", m_byLevel);

		return;
	}

	/*!
	 * @brief 获取阶段号
	 * @param wstring& 阶段号
	*/
	void GetLevel(std::wstring& wstrLevel)
	{
		std::string strLevel = "";
		GetLevel(strLevel);

		BufferString::std_string_to_wstring(wstrLevel, strLevel);

		return;
	}

	/*!
	 * @brief 设置动作
	 * @param string 动作
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetAction(std::string strAction)
	{
		if (m_strAction == strAction)
		{
			return false;
		}

		m_strAction = strAction;

		return true;
	}

	/*!
	 * @brief 设置动作
	 * @param wstring 动作
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetAction(std::wstring wstrAction)
	{
		std::string strAction = "";
		BufferString::std_wstring_to_string(strAction, wstrAction);

		return SetAction(strAction);
	}

	/*!
	 * @brief 获取动作
	 * @param string& 动作
	*/
	void GetAction(std::string &strAction)
	{
		strAction = m_strAction;

		return;
	}

	/*!
	 * @brief 获取动作
	 * @param wstring& 动作
	*/
	void GetAction(std::wstring &wstrAction)
	{
		std::string strAction = "";
		GetAction(strAction);

		BufferString::std_string_to_wstring(wstrAction, strAction);

		return;
	}

	/*!
	 * @brief 获取动作
	 * @return string 动作
	*/
	std::string GetAction() const
	{
		return m_strAction;
	}

	/*!
	 * @brief 设置状态
	 * @param unsigned char 新执行状态
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetStatus(unsigned char byStatus)
	{
		if (m_byStatus == byStatus)
		{
			return false;
		}

		if (m_strAction == _TASKACTION_A_END && (byStatus == Task::_TASKSTA_PAUSE || byStatus == Task::_TASKSTA_CANCEL))
		{
			return false;
		}

		m_byStatus = byStatus;

		return true;
	}

	/*!
	 * @brief 获取状态
	 * @return unsigned char 执行状态
	*/
	unsigned char GetStatus() const
	{
		return m_byStatus;
	}

	/*
	 * @brief 获取状态
	 * @param string& 执行状态
	*/
	void GetStatus(std::string& strStatus)
	{
		GetStatus(m_byStatus, strStatus);
		switch (m_byStatus)
		{
		case Task::_TASKSTA_FINISH:
			if (m_strAction != _TASKACTION_A_END)
			{
				strStatus = "任务完成,等待执行下一阶段任务";
			}
			else
			{
				strStatus = "所有任务完成";
			}
			break;
		case Task::_TASKSTA_CANCEL:
			if (m_strAction != _TASKACTION_A_END)
			{
				strStatus = "任务取消,跳至下一阶段";
			}
			else
			{
				strStatus = "任务取消";
			}	
			break;
		}

		return;
	}

	/*
	 * @brief 获取状态
	 * @param wstring& 执行状态
	*/
	void GetStatus(std::wstring& wstrStatus)
	{
		std::string strStatus = "";
		GetStatus(strStatus);

		BufferString::std_string_to_wstring(wstrStatus, strStatus);

		return;
	}

	/*!
	 * @brief 设置执行者
	 * @param unsigned char 新执行者编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetExecuter(unsigned char byExecuter)
	{
		if (m_byExecuter == byExecuter)
		{
			return false;
		}

		m_byExecuter = byExecuter;

		return true;
	}

	/*!
	 * @brief 获取执行者
	 * @return unsigned char 执行者编号
	*/
	unsigned char GetExecuter() const
	{
		return m_byExecuter;
	}

	/*!
	 * @brief 获取执行者
	 * @param string& 执行者编号
	*/
	void GetExecuter(std::string& strExecuter)
	{
		if (m_byExecuter == 0)
		{
			strExecuter = "无";
		}
		else
		{
			BufferString::std_string_format(strExecuter, "%d", m_byExecuter);
		}

		return;
	}

	/*!
	 * @brief 获取执行者
	 * @param wstring& 执行者编号
	*/
	void GetExecuter(std::wstring& wstrExecuter)
	{
		std::string strExecuter = "";
		GetExecuter(strExecuter);

		BufferString::std_string_to_wstring(wstrExecuter, strExecuter);

		return;
	}

	/*!
	 * @brief 设置任务坐标
	 * @param unsigned short 新任务坐标
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetTarget(unsigned short usTarget)
	{
		if (usTarget == 0)
		{
			return false;
		}

		if (m_usTarget == usTarget)
		{
			return false;
		}

		m_usTarget = usTarget;

		return true;
	}

	/*!
	 * @brief 获取任务坐标
	 * @return unsigned short 任务坐标
	*/
	unsigned short GetTarget() const
	{
		return m_usTarget;
	}

	/*!
	 * @brief 获取任务坐标
	 * @param string 任务坐标
	*/
	void GetTarget(std::string& strTarget)
	{
		BufferString::std_string_format(strTarget, "%d", m_usTarget);

		return;
	}

	/*!
	 * @brief 获取任务坐标
	 * @param wstring 任务坐标
	*/
	void GetTarget(std::wstring& wstrTarget)
	{
		std::string strTarget = "";
		GetTarget(strTarget);

		BufferString::std_string_to_wstring(wstrTarget, strTarget);

		return;
	}

	/*!
	 * @brief 设置异常信息
	 * @param string 异常信息
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetError(std::string strError)
	{
		if (strError == "")
		{
			return false;
		}

		if (m_strError == strError)
		{
			return false;
		}

		m_strError = strError;

		return true;
	}

	/*!
	 * @brief 设置异常信息
	 * @param wstring 异常信息
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetError(std::wstring wstrError)
	{
		std::string strError = "";
		BufferString::std_wstring_to_string(strError, wstrError);

		return SetError(strError);
	}

	/*!
	 * @brief 获取异常信息
	 * @param string& 异常信息
	*/
	void GetError(std::string& strError)
	{
		strError = m_strError;

		return;
	}

	/*!
	 * @brief 获取异常信息
	 * @param wstring& 异常信息
	*/
	void GetError(std::wstring& wstrError)
	{
		std::string strError = "";
		GetError(strError);

		BufferString::std_string_to_wstring(wstrError, strError);

		return;
	}

	/*!
	 * @brief 获取异常信息
	 * @return string 异常信息
	*/
	std::string GetError() const
	{
		return m_strError;
	}

	/*!
	 * @brief 设置起始坐标
	 * @param unsigned short 新起始坐标
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetBegin(unsigned short usBegin)
	{
		if (m_usBegin == usBegin)
		{
			return false;
		}

		m_usBegin = usBegin;

		return true;
	}

	/*!
	 * @brief 获取起始坐标
	 * @return unsigned short 起始坐标
	*/
	unsigned short GetBegin() const
	{
		return m_usBegin;
	}

	/*!
	 * @brief 获取起始坐标
	 * @param string& 起始坐标
	*/
	void GetBegin(std::string& strBegin)
	{
		if (m_usBegin == 0)
		{
			strBegin = "由调度系统分配AGV";
		}
		else
		{
			BufferString::std_string_format(strBegin, "%d", m_usBegin);
		}

		return;
	}

	/*!
	 * @brief 获取起始坐标
	 * @param wstring& 起始坐标
	*/
	void GetBegin(std::wstring& wstrBegin)
	{
		std::string strBegin = "";
		GetBegin(strBegin);

		BufferString::std_string_to_wstring(wstrBegin, strBegin);

		return;
	}

	/*!
	 * @brief 获取任务状态
	 * @param string 任务状态
	 * @return unsigned char 状态码
	*/
	static unsigned char GetStatusCode(std::string strStatus)
	{
		if (strStatus == "未执行任务")
		{
			return Task::_TASKSTA_NONE;
		}
		else if (strStatus == "执行任务中")
		{
			return Task::_TASKSTA_EXECUTE;
		}
		else if (strStatus == "任务完成,等待执行下一阶段任务" || strStatus == "所有任务完成" || strStatus == "任务完成")
		{
			return Task::_TASKSTA_FINISH;
		}
		else if (strStatus == "任务暂停")
		{
			return Task::_TASKSTA_PAUSE;
		}
		else if (strStatus == "任务取消,跳至下一阶段" || strStatus == "任务取消")
		{
			return Task::_TASKSTA_CANCEL;
		}

		return 0xFF;
	}

	/*!
	 * @brief 获取任务状态
	 * @param wstring 任务状态
	 * @return unsigned char 状态码
	*/
	static unsigned char GetStatusCode(std::wstring wstrStatus)
	{
		std::string strStatus = "";
		BufferString::std_wstring_to_string(strStatus, wstrStatus);

		return GetStatusCode(strStatus);
	}

	/*!
	 * @brief 获取状态
	 * @param unsigned char 状态码
	 * @param string& 状态
	*/
	static void GetStatus(unsigned char byStatus, std::string& strStatus)
	{
		switch (byStatus)
		{
		case Task::_TASKSTA_NONE:
			strStatus = "未执行任务";
			break;
		case Task::_TASKSTA_EXECUTE:
			strStatus = "执行任务中";
			break;
		case Task::_TASKSTA_FINISH:
			strStatus = "任务完成";
			break;
		case Task::_TASKSTA_PAUSE:
			strStatus = "任务暂停";
			break;
		case Task::_TASKSTA_CANCEL:
			strStatus = "任务取消";
			break;
		default:
			strStatus = "未知任务状态";
			break;
		}

		return;
	}

	/*!
	 * @brief 获取状态
	 * @param unsigned char 状态码
	 * @param wstring& 状态
	*/
	static void GetStatus(unsigned char byStatus, std::wstring& wstrStatus)
	{
		std::string strStatus = "";
		GetStatus(byStatus, strStatus);

		return BufferString::std_string_to_wstring(wstrStatus, strStatus);
	}

#ifdef _AFX
	/*!
	 * @brief 获取任务号
	 * @param CString& 任务号
	*/
	void GetNo(CString& cstrNo)
	{
		std::string strNo = "";
		GetNo(strNo);

		BufferString::std_string_to_CString(cstrNo, strNo);

		return;
	}

	/*!
	 * @brief 获取阶段号
	 * @oaram CString& 阶段号
	*/
	void GetLevel(CString& cstrLevel)
	{
		std::string strLevel = "";
		GetLevel(strLevel);

		BufferString::std_string_to_CString(cstrLevel, strLevel);

		return;
	}

	/*!
	 * @brief 设置动作
	 * @param CString 动作
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetAction(CString cstrAction)
	{
		std::string strAction = "";
		BufferString::CString_to_std_string(strAction, cstrAction);

		return SetAction(strAction);
	}

	/*!
	 * @brief 获取动作
	 * @param CString& 动作
	*/
	void GetAction(CString& cstrAction)
	{
		std::string strAction = "";
		GetAction(strAction);

		BufferString::std_string_to_CString(cstrAction, strAction);

		return;
	}

	/*!
	 * @brief 获取动作
	 * @param CString& 执行状态
	*/
	void GetStatus(CString& cstrStatus)
	{
		std::string strStatus = "";
		GetStatus(strStatus);

		BufferString::std_string_to_CString(cstrStatus, strStatus);

		return;
	}

	/*!
	 * @brief 获取执行者
	 * @param CString& 执行者编号
	*/
	void GetExecuter(CString& cstrExecuter)
	{
		std::string strExecuter = "";
		GetExecuter(strExecuter);

		BufferString::std_string_to_CString(cstrExecuter, strExecuter);

		return;
	}

	/*!
	 * @brief 获取任务坐标
	 * @param CString& cstrTarget
	*/
	void GetTarget(CString& cstrTarget)
	{
		std::string strTarget = "";
		GetTarget(strTarget);

		BufferString::std_string_to_CString(cstrTarget, strTarget);

		return;
	}

	/*!
	 * @brief 设置异常信息
	 * @param CString 异常信息
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetError(CString cstrError)
	{
		std::string strError = "";
		BufferString::CString_to_std_string(strError, cstrError);

		return SetError(strError);
	}

	/*!
	 * @brief 获取异常信息
	 * @param CString& 异常信息
	*/
	void GetError(CString& cstrError,COLORREF color)
	{
		std::string strError = "";
		GetError(strError);

		BufferString::std_string_to_CString(cstrError, strError);

		if (strError != _TASKERROR_A_NONE)
		{
			color = COLOR_RED;
		}

		return;
	}

	/*!
	 * @brief 获取起始坐标
	 * @param CString& 起始坐标
	*/
	void GetBegin(CString& cstrBegin)
	{
		std::string strBegin = "";
		GetBegin(strBegin);

		BufferString::std_string_to_CString(cstrBegin, strBegin);

		return;
	}

	/*!
	 * @brief 获取任务状态
	 * @param CString 任务状态
	 * @return unsigned char 状态码
	*/
	static unsigned char GetStatusCode(CString cstrStatus)
	{
		std::string strStatus = "";
		BufferString::CString_to_std_string(strStatus, cstrStatus);

		return GetStatusCode(strStatus);
	}

	/*!
	 * @brief 获取状态
	 * @param unsigned char 状态码
	 * @param CString& 状态
	*/
	static void GetStatus(unsigned char byStatus, CString& cstrStatus)
	{
		std::string strStatus = "";
		GetStatus(byStatus, strStatus);

		return BufferString::std_string_to_CString(cstrStatus, strStatus);
	}

#endif // !_AFX
} AGVTask;

/*!< @brief 描述AGV一组任务的基本结构块 */
typedef struct _TaskArrayStruct
{
protected:
	unsigned short m_usNo;						/*!< 任务号 */
	std::map<unsigned char,AGVTask*> m_mapTask;	/*!< 任务成员列表 */

public:
	_TaskArrayStruct()
	{
		m_usNo = 0;
	}

	_TaskArrayStruct(unsigned short usNo)
	{
		m_usNo = usNo;
	}

	_TaskArrayStruct(unsigned short usNo, std::map<unsigned char, AGVTask*> mapTask)
	{
		m_usNo = usNo;
		m_mapTask = mapTask;
	}

	_TaskArrayStruct(const _TaskArrayStruct& taskArr)
	{
		m_usNo = taskArr.m_usNo;
		m_mapTask = taskArr.m_mapTask;
	}

	~_TaskArrayStruct()
	{
		Clear();
	}

	void operator=(const _TaskArrayStruct& taskArr)
	{
		m_usNo = taskArr.m_usNo;
		m_mapTask = taskArr.m_mapTask;
	}

public:
	/*!
	 * @brief 任务组是否有效
	 * @return bool 无效返回true,有效返回false
	*/
	bool IsNull()
	{
		if (m_mapTask.size() > 0)
		{
			return false;
		}

		return true;
	}

	/*!
	 * @brief 设置任务号
	 *
	 * 设置新任务号成功,将失去原任务号的所有任务
	 * @param unsigned short 新任务号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetNo(unsigned short usNo)
	{
		if (usNo == 0)
		{
			return false;
		}

		if (m_usNo == usNo)
		{
			return false;
		}

		Clear();

		m_usNo = usNo;

		return true;
	}

	/*!
	 * @brief 获取任务号
	 * @return unsigned short 任务号
	*/
	unsigned short GetNo() const
	{
		return m_usNo;
	}

	/*!
	 * @brief 获取任务成员列表
	 * @return map<unsigend char,AGVTask*>&  任务成员列表
	*/
	std::map<unsigned char, AGVTask*>& GetTaskList() 
	{
		return m_mapTask;
	}

	/*!
	 * @brief 添加任务
	 * @param AGVTask 任务结构体
	 * @return bool 添加成功返回true,失败返回false
	*/
	bool AddTask(AGVTask task)
	{
		unsigned short usNo = task.GetNo();	/*!< 任务号 */
		if (usNo == 0)
		{
			return false;
		}

		unsigned char byLevel = task.GetLevel();	/*!< 阶段号 */
		if (byLevel == 0)
		{
			return false;
		}

		if (m_usNo != usNo)
		{
			return false;
		}

		if (m_mapTask.find(byLevel) != m_mapTask.end())
		{
			return false;
		}

		m_mapTask[byLevel] = new AGVTask(task);

		return true;
	}

	/*!
	 * @brief 获取任务
	 * @param unsigned char 任务阶段号
	 * @return AGVTask* 任务指针
	*/
	AGVTask* GetTask(unsigned char byLevel)
	{
		if (m_mapTask.find(byLevel) == m_mapTask.end())
		{
			return nullptr;
		}

		return m_mapTask[byLevel];
	}

	/*!
	 * @brief 更新任务
	 *
	 * 不存在则添加,存在则更新
	 * @param AGVTask 任务结构体
	 * @return bool 更新成功返回true,失败返回false
	*/
	bool UpdateTask(AGVTask task)
	{
		unsigned char byLevel = task.GetLevel();	/*!< 阶段号 */
		if (m_mapTask.find(byLevel) != m_mapTask.end())
		{
			return m_mapTask[byLevel]->Update(task);
			
		}
		else
		{
			return AddTask(task);
		}

		return true;
	}

	/*!
	 * @brief 清空任务
	*/
	void Clear()
	{
		for (std::map<unsigned char, AGVTask*>::iterator it = m_mapTask.begin(); it != m_mapTask.end();)
		{
			delete it->second;
			it = m_mapTask.erase(it);
		}
	}
	
} AGVTaskArr;

/*!
 * @class CTaskManager
 * @brief 任务管理器基类
 *
 * 描述任务管理器基本属性的类
*/
class CTaskManager
{
public:
	CTaskManager();
	~CTaskManager();

protected:
	std::map<unsigned short, AGVTaskArr*> m_mapTaskList; /*!< 任务列表 */
	ADOConn m_ado;										 /*!< 任务数据库ADO链接 */
	std::thread* m_pTheadUpdate;						 /*!< 任务更新线程 */
	HANDLE m_hShutdownEvent;							 /*!< 关闭事件 */
	std::mutex m_mutexLock;								 /*!< 互斥锁 */
#ifdef _AFX
	HWND m_hParent;										 /*!< 父窗口句柄 */
#endif // _AFX

public:
	/*!
	 * @brief 启动管理器
	 * @param const ADOConn 任务数据库ADO链接
	 * @return bool 启动成功返回true,失败返回false
	*/
	bool OpenManager(const ADOConn ado);

#ifdef _AFX
	/*!
	 * @brief 启动管理器
	 * @param const ADOConn 任务数据库ADO链接
	 * @return bool 启动成功返回true,失败返回false
	*/
	bool OpenManager(const ADOConn ado, HWND hParent);
#endif // _AFX

	/*!
	 * @brief 关闭管理器
	*/
	void CloseManager();

protected:
	/*!
	 * @brief 启动任务更新线程
	*/
	void StartUpdateThread();

	/*!
	 * @brief 关闭任务更新线程
	*/
	void EndUpdateThread();

	/*!
	 * @brief 任务更新线程
	*/
	void UpdateThread();

public:
	/*!
	 * @brief 获取任务
	 * @param unsigned short 任务号
	 * @param unsigend char 任务阶段号
	 * @return AGVTask 任务
	*/
	AGVTask Get(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief 获取任务成员列表
	 * @param unsigned short 任务号
	 * @return AGVTaskArr 任务成员列表
	*/
	AGVTaskArr GetArray(unsigned short usNo);

	/*!
	 * @brief 获取任务列表
	 * @param map<unsigned short, AGVTaskArr> 任务列表
	*/
	std::map<unsigned short, AGVTaskArr> GetList();

	/*!
	 * @brief 发布任务
	 * @param AGVTask 任务结构体
	 * @return bool 发布成功返回true,失败返回false
	*/
	bool Publish(AGVTask task);

	/*!
	 * @brief 更新任务
	 * @param AGVTask 任务结构体
	 * @return bool 更新成功返回true,失败返回false
	*/
	bool Update(AGVTask task);

	/*!
	 * @brief 删除任务
	 * @param unsigned short 任务号
	 * @return bool 删除成功返回true,失败返回false
	*/
	bool Delete(unsigned short usNo);

	/*!
	 * @brief 编辑任务
	 * @param AGVTask 任务结构体
	 * @return bool 编辑成功返回true,失败返回false
	 */
	bool Edit(AGVTask task);

	/*!
	 * @brief 取消任务
	 * @param unsigned short 任务号
	 * @param unsigned char 阶段号
	 * @return bool 取消成功返回true,失败返回false
	*/
	bool Cancel(unsigned short usNo,unsigned char byLevel);

	/*!
	 * @breif 暂停任务
	 * @param unsigned short 任务号
	 * @param unsigned char 阶段号
	 * @return bool 暂停成功返回true,失败返回false
	*/
	bool Pause(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief 强制完成任务
	 * @param unsigned short 任务号
	 * @param unsigned char 阶段号
	 * @return bool 强制完成成功返回true,失败返回false
	*/
	bool Finish(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief 重置任务
	 * @param unsigned short 任务号
	 * @param unsigned char 阶段号
	 * @return bool 强制完成成功返回true,失败返回false
	*/
	bool Reset(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief 取消全部任务
	 * @return bool 取消成功返回true,失败返回false
	*/
	bool CancelAll();

	/*!
	 * @brief 暂停全部任务
	 * @return bool 暂停成功返回true,失败返回false
	*/
	bool PauseAll();

	/*!
	 * @brief 重置全部任务
	 * @return bool 强制完成成功返回true,失败返回false
	*/
	bool ResetAll();

	/*!
	 * @brief 插入任务
	 *
	 * 将新任务插入至指定任务点前
	 * @param unsigned short 插入点任务号
	 * @param unsigned char 插入点阶段号
	 * @param AGVTask 新任务
	 * @return bool 插入成功返回true,失败返回false
	*/
	bool Insert(unsigned short usNo, unsigned char byLevel, AGVTask newTask);

	/*!
	 * @brief 添加任务
	 * @param AGVTask 新任务
	 * @return bool 添加成功返回true,失败返回false
	*/
	bool Add(AGVTask newTask);

	/*!
	 * @brief 获取当前最大任务号
	 * @return unsigned short 当前最大任务号 
	*/
	unsigned short GetCurMaxNo();
protected:

	/*!
	 * @brief 获取任务
	 * @param unsigned short 任务号
	 * @param unsigend char 任务阶段号
	 * @return AGVTask* 任务指针
	*/
	AGVTask* GetTask(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief 获取任务成员列表
	 * @param unsigned short 任务号
	 * @return AGVTaskArr* 任务成员列表指针
	*/
	AGVTaskArr* GetTaskArray(unsigned short usNo);

	/*!
	 * @brief 获取任务列表
	 * @param map<unsigned short, AGVTaskArr*>& 任务列表
	*/
	std::map<unsigned short, AGVTaskArr*>& GetTaskList();

	/*!
	 * @brief 发布任务
	 * @param AGVTask 任务结构体
	 * @return bool 发布成功返回true,失败返回false
	*/
	bool PublishTask(AGVTask task);

	/*!
	 * @brief 更新任务
	 * @param AGVTask 任务结构体
	 * @return bool 更新成功返回true,失败返回false
	*/
	bool UpdateTask(AGVTask task);

	/*!
	 * @brief 删除任务
	 * @param unsigned short 任务号
	*/
	void DeleteTask(unsigned short usNo);

	/*!
	 * @brief 清除任务管理器
	*/
	void Clear();
};

#endif // !_TASK_H