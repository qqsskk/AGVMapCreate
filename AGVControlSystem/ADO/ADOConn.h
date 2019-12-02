/*!
* @file ADOConn.h
* @brief interface for the ADOConn class.
* @author 
* @date 
* @version 1.0
*/

#if !defined(AFX_ADOCONN_H__5FB9A9B2_8D94_44F7_A2DA_1F37A4F33D10__INCLUDED_)
#define AFX_ADOCONN_H__5FB9A9B2_8D94_44F7_A2DA_1F37A4F33D10__INCLUDED_

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace\
	rename("EOF", "adoEOF")rename("BOF", "adoBOF")

#if _MSC_VER > 1000

#pragma once
#include <map>
#include <mutex>
//using namespace std;
#endif // _MSC_VER > 1000

/*!
* @class ADOConn  
* @brief 数据库ADO连接
*
* 数据库ADO连接类,包含查询、修改数据库等操作
*/
class ADOConn  
{
public:
	/*!
	* @brief 无参构造
	*/
	ADOConn();

	/*!
	* @brief 拷贝构造
	* @param cAdo 拷贝的ADO连接
	*/
	ADOConn(const ADOConn& cAdo);

	/*!
	* @brief 析构函数
	*/
	virtual ~ADOConn();

protected:
	_ConnectionPtr m_pConnection; /*!< 连接对象指针*/
	_RecordsetPtr m_pRecordset; /*!< 结果集指针*/

protected:
	std::mutex m_mutexLock; /*!< 互斥锁*/

public:
	_bstr_t m_bstrConnect; /*!< 连接串 */
	_bstr_t m_bstrUser; /*!< 用户名 */
	_bstr_t m_bstrPassword; /*!< 用户密码 */

public:
	/*!
	* @brief 初始化ADO连接
	* @return bool 成功返回true,失败返回false
	*/
	bool OnInitADOConn();

	/*!
	* @brief 查询数据库
	* @return _RecordsetPtr 返回查询结果集
	*/
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);

	/*!
	* @brief 执行SQL语句
	* @return bool 成功返回true,失败返回false
	*/
    BOOL ExecuteSQL(_bstr_t bstrSQL);

	/*!
	* @brief 关闭ADO连接
	*/
	void ExitConnect();

public:
	/*!
	* @brief 重载=函数
	* @param cAdo 拷贝的ADO连接
	*/
	void operator=(const ADOConn& cAdo);
};

#endif // !defined(AFX_ADOCONN_H__5FB9A9B2_8D94_44F7_A2DA_1F37A4F33D10__INCLUDED_)
