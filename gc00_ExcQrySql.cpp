
/// <summary>
/// 功能说明:执行动态查询Sql
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
BM2F_ENTERACE(gc00_ExcQrySql)

int f_gc00_ExcQrySql(EIClass * bcls_rec, EIClass * bcls_ret,CDbConnection * conn) 
{
	CTracer log(__FUNCTION__);
	// 数据库SQL操作字符串
	CString  sqlstr("");    

	int doFlag = 0;
	try
	{
		CString query_sql ;
		for (int i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++ ) 
		{
			query_sql = (CString)bcls_rec->Tables[0].Rows[i]["QUERY_SQL"];
			CDbCommand cmd(conn);
			cmd.SetCommandText(query_sql);
			if (i >0)
			{
				bcls_ret->Tables.Add( );
			}
			cmd.ExecuteQuery(bcls_ret->Tables[i]);
			cmd.Close();
		}
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