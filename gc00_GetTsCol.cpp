
/// <summary>
/// 功能说明:根据前台出入的多个表名返回多个表的记录块(指定字段)
/// </summary>
/// Copyright: Baosight Software LTD.co Copyright (c) 2010
/// Company: 上海宝信软件股份有限公司
/// Author:  项目组
/// Version:  1.0
/// History:
///		2010-12-20 张金金 [创建] 
///		2012-04-26 严蕾 [修改] 修改名称，从si00-->gc00 

#include "stdafx.h"


// Service 入口
BM2F_ENTERACE(gc00_GetTsCol)

int f_gc00_GetTsCol(EIClass * bcls_rec, EIClass * bcls_ret,CDbConnection * conn) 
{
	CTracer log(__FUNCTION__);
	// 数据库SQL操作字符串
	CString  sqlstr("");    

	int doFlag = 0;
	try
	{
		CString table_columns = (CString)bcls_rec->Tables[0].Rows[0]["TABLE_COLUMNS"];
		CString table_name = (CString)bcls_rec->Tables[0].Rows[0]["TABLE_NAME"];
		CDbCommand cmd(conn);
		cmd.SetCommandText("select " + table_columns + " from " + table_name);
		cmd.ExecuteQuery(bcls_ret->Tables[0]);
		bcls_ret->Tables[0].set_TableName(table_name);

		if(bcls_rec->Tables[0].Rows.get_Count() > 1)
		{
			for(int i = 1;i <bcls_rec->Tables[0].Rows.get_Count();i++)
			{
				table_columns = (CString)bcls_rec->Tables[0].Rows[i]["TABLE_COLUMNS"];
				table_name = (CString)bcls_rec->Tables[0].Rows[i]["TABLE_NAME"];
				cmd.SetCommandText("select " + table_columns + " from " + table_name);
				bcls_ret->Tables.Add(table_name);
				cmd.ExecuteQuery(bcls_ret->Tables[table_name]);
			}
		}		
		cmd.Close();
	}
	catch(CDbException& ex)  //捕获数据库操作异常
	{
		CFormattable arguments[] = { ex.GetCode() };
		CMessageFormat::Format(s.msg,  _RES("GCRSS0000006")/*数据库处理出错，sqlcode=[{0}]。请联系系统维护人员。*/, arguments, 1);
		CString str = sqlstr + "\r\n" + ex.GetMsg();
		strncpy(s.sysmsg, (const char*)str, sizeof(s.sysmsg)-1);
		s.flag = -1;
		doFlag = -1; //数据库异常时返回-1，事务将被回滚
	}
	catch(const CApplicationException& ex)
	{
		s.flag = ex.GetCode();
		doFlag = -1;
	}
	catch(CException& ex)
	{
		strncpy(s.msg, (const char*)ex.GetMsg(), sizeof(s.msg)-1);
		s.flag = ex.GetCode();
		doFlag = -1;
	}

	return doFlag;
}