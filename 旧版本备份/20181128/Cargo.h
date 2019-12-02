#pragma once
#include "stdafx.h"
#include <string>
#include <list>
#include <map>
#include "ADOConn.h"

using namespace std;

/*!
 * WMS库存表表结构
 * 表名 WMS_CARGO_TABLE
 * 字段 WMSCargo_No 类型 int 说明：货架编号
 * 字段 WMSCargo_Row 类型 int 说明：货架行
 * 字段 WMSCargo_Col 类型 int  说明：货架列
 * 字段 WMSCargo_Unit 类型 vchar 说明：单位
 * 字段 WMSCargo_Type 类型 vchar 说明：货物类型
 * 字段 WMSCargo_Number 类型 int 说明：数量
 * 字段 WMSCargo_Intime 类型 datetime 说明：入库时间
*/

/*!< @brief 库存信息结构体 */
typedef struct _CargoStruct
{
	unsigned int m_unNo;		/*!< 货架编号 */
	unsigned int m_unRow;		/*!< 行 */
	unsigned int m_unCol;		/*!< 列 */
	string m_strType;			/*!< 货物类型 */
	string m_strUnit;			/*!< 单位 */
	unsigned int m_unNumber;	/*!< 数量 */
	string m_strIntime;			/*!< 入库时间 */

	_CargoStruct()
	{
		m_unNo = 0;
		m_unRow = 0;
		m_unCol = 0;
		m_strType = "";
		m_strUnit = "";
		m_unNumber = 0;
		m_strIntime = "";
	}

	_CargoStruct(const _CargoStruct& cargo)
	{
		m_unNo = cargo.m_unNo;
		m_unRow = cargo.m_unRow;
		m_unCol = cargo.m_unCol;
		m_strType = cargo.m_strType;
		m_strUnit = cargo.m_strUnit;
		m_unNumber = cargo.m_unNumber;
		m_strIntime = cargo.m_strIntime;
	}

	void operator=(const _CargoStruct& cargo)
	{
		m_unNo = cargo.m_unNo;
		m_unRow = cargo.m_unRow;
		m_unCol = cargo.m_unCol;
		m_strType = cargo.m_strType;
		m_strUnit = cargo.m_strUnit;
		m_unNumber = cargo.m_unNumber;
		m_strIntime = cargo.m_strIntime;
	}

	bool IsNull()
	{
		if (m_unNo == 0 || m_unRow == 0 || m_unCol == 0 || m_strType == "" || m_strUnit == "" || m_unNumber == 0 || m_strIntime == "")
		{
			return true;
		}

		return false;
	}
} _CARGO;

/*!
 * @brief 记录入库信息
 * @param ADOConn& 数据库连接串
 * @param unsigned int 货架编号
 * @param unsigned int 货架行
 * @param unsigned int 货架列
 * @param string 货物类型
 * @param string 单位
 * @param unsigend int 数量
 * @return bool 记录成功返回true,失败返回false
*/
static bool RecordIn(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol, string strType, string strUnit, unsigned int unNumber)
{
	CString cstrSql = _T("");
	cstrSql.Format(_T("insert into WMS_CARGO_TABLE([WMSCargo_No],[WMSCargo_Row],[WMSCargo_Col],[WMSCargo_Unit],[WMSCargo_Type],[WMSCargo_Number]) values(%d,%d,%d,'%s','%s',%d)"), unNo, unRow, unCol, strUnit.c_str(), strType.c_str(), unNumber);

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief 清除入库信息
 * @param ADOConn& 数据库连接串
 * @param unsigned int 货架编号
 * @param unsigned int 货架行
 * @param unsigned int 货架列
 * @return bool 清除成功返回true,失败返回false
*/
static bool ClearIn(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol)
{
	CString cstrSql = _T("");
	cstrSql.Format(_T("delete WMS_CARGO_TABLE where [WMSCargo_No]=%d and [WMSCargo_Row]=%d and [WMSCargo_Col]=%d"), unNo, unRow, unCol);

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief 清除全部库存信息
 * @param ADOConn& 数据库连接串
 * @return bool 清除成功返回true,失败返回false
*/
static bool ClearAllCargo(ADOConn& ado)
{
	CString cstrSql = _T("delete WMS_CARGO_TABLE");

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief 获取库存信息
 * @param ADOConn& 数据库连接串
 * @param unsigned int 货架编号
 * @param unsigned int 货架行
 * @param unsigned int 货架列
 * @return _CARGO 库存信息结构体
*/
static _CARGO GetCargo(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol)
{
	_CARGO cargo;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_CARGO_TABLE where [WMSCargo_No]=%d and [WMSCargo_Row]=%d and [WMSCargo_Col]=%d"), unNo, unRow, unCol);

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	if (pRecordset == nullptr || pRecordset->adoEOF)
	{
		return cargo;
	}

	_variant_t var;	/*!< 字段值 */

	var = pRecordset->GetCollect(_T("WMSCargo_No"));
	if (var.vt != VT_NULL)
	{
		cargo.m_unNo = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSCargo_Row"));
	if (var.vt != VT_NULL)
	{
		cargo.m_unRow = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSCargo_Col"));
	if (var.vt != VT_NULL)
	{
		cargo.m_unCol = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
	if (var.vt != VT_NULL)
	{
		cargo.m_strUnit = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("WMSCargo_Type"));
	if (var.vt != VT_NULL)

	{
		cargo.m_strType = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("WMSCargo_Number"));
	if (var.vt != VT_NULL)
	{
		cargo.m_unNumber = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
	if (var.vt != VT_NULL)
	{
		cargo.m_strIntime = _bstr_t(var);
	}

	return cargo;
}

/*!
 * @brief 获取全部库存信息
 * @param ADOConn& 数据库连接串
 * @return list<_CARGO> 库存信息列表
*/
static list<_CARGO> GetAllCargo(ADOConn& ado)
{
	list<_CARGO> listCargo;

	CString cstrSql = _T("select * from WMS_CARGO_TABLE");

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_CARGO cargo;

		_variant_t var;	/*!< 字段值 */

		var = pRecordset->GetCollect(_T("WMSCargo_No"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Row"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Col"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strUnit = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Type"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Number"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNumber = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strIntime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listCargo.push_back(cargo);
	}

	return listCargo;
}

/*!
 * @brief 获取指定货架上所有库存信息
 * @param ADOConn& 数据库连接串
 * @param unsigend int 货架编号
 * @return list<_CARGO> 库存信息列表
*/
static list<_CARGO> GetAllCargo(ADOConn& ado, unsigned int unNo)
{
	list<_CARGO> listCargo;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_CARGO_TABLE where [WMSCargo_No]=%d"), unNo);

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_CARGO cargo;

		_variant_t var;	/*!< 字段值 */

		var = pRecordset->GetCollect(_T("WMSCargo_No"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Row"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Col"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strUnit = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Type"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Number"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNumber = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strIntime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listCargo.push_back(cargo);
	}

	return listCargo;
}

/*!
 * @brief 获取指定货架上所有库存信息
 * @param ADOConn& 数据库连接串
 * @param string 单位
 * @param string 类型
 * @return list<_CARGO> 库存信息列表
*/
static list<_CARGO> GetAllCargo(ADOConn& ado, string strUnit, string strType)
{
	list<_CARGO> listCargo;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_CARGO_TABLE where [WMSCargo_Unit]='%s' and [WMSCargo_Type]='%s'"), strUnit.c_str(), strType.c_str());

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_CARGO cargo;

		_variant_t var;	/*!< 字段值 */

		var = pRecordset->GetCollect(_T("WMSCargo_No"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Row"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Col"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strUnit = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Type"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Number"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNumber = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strIntime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listCargo.push_back(cargo);
	}

	return listCargo;
}

/*!
 * @brief 获取指定货架上所有库存信息
 * @param ADOConn& 数据库连接串
 * @param unsigend int 货架编号
 * @param string 单位
 * @param string 类型
 * @return list<_CARGO> 库存信息列表
*/
static list<_CARGO> GetAllCargo(ADOConn& ado, unsigned int unNo,string strUnit, string strType)
{
	list<_CARGO> listCargo;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_CARGO_TABLE where [WMSCargo_No]=%d [WMSCargo_Unit]='%s' and [WMSCargo_Type]='%s'"), unNo, strUnit.c_str(), strType.c_str());

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_CARGO cargo;

		_variant_t var;	/*!< 字段值 */

		var = pRecordset->GetCollect(_T("WMSCargo_No"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Row"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Col"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strUnit = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Type"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Number"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNumber = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strIntime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listCargo.push_back(cargo);
	}

	return listCargo;
}

/*!
 * @brief unsigned int unNo
 * @param ADOConn& 数据库连接串
 * @param string 开始时间
 * @param string 结束时间
 * @return list<_CARGO> 库存信息列表
*/
static list<_CARGO> GetAllCargo(ADOConn& ado, string strStartTime, string strEndTime)
{
	list<_CARGO> listCargo;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_CARGO_TABLE where [WMSCargo_Intime] between '%s' and '%s'"), strStartTime.c_str(), strEndTime.c_str());

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_CARGO cargo;

		_variant_t var;	/*!< 字段值 */

		var = pRecordset->GetCollect(_T("WMSCargo_No"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Row"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Col"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strUnit = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Type"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Number"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNumber = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strIntime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listCargo.push_back(cargo);
	}

	return listCargo;
}

/*!
 * @brief unsigned int unNo
 * @param ADOConn& 数据库连接串
 * @param unsigned int 货架编号
 * @param string 开始时间
 * @param string 结束时间
 * @return list<_CARGO> 库存信息列表
*/
static list<_CARGO> GetAllCargo(ADOConn& ado, unsigned int unNo, string strStartTime, string strEndTime)
{
	list<_CARGO> listCargo;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_CARGO_TABLE where [WMSCargo_No]=%d and [WMSCargo_Intime] between '%s' and '%s'"), unNo, strStartTime.c_str(), strEndTime.c_str());

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_CARGO cargo;

		_variant_t var;	/*!< 字段值 */

		var = pRecordset->GetCollect(_T("WMSCargo_No"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Row"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Col"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strUnit = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Type"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Number"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNumber = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strIntime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listCargo.push_back(cargo);
	}

	return listCargo;
}