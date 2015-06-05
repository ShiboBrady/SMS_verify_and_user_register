#include "xmpp_register.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "category.h"

using namespace std;
using namespace xmpp;


CRegister::CRegister(std::string strUsername, std::string strDomainname, std::string strPassword)
	:strUsername_(strUsername),
	 strDomainname_(strDomainname),
	 strPassword_(strPassword),
	 iUid_(0)
{

}

CRegister::~CRegister()
{

}

//判断是否注册过
bool CRegister::IsRegisterd()
{
	DEBUG(__FILE__, __LINE__, "=========CRegister::IsRegisterd START=========\n");
	if (oMysql_.EscapeString(strUsername_) != strUsername_ || oMysql_.EscapeString(strDomainname_) != strDomainname_ || oMysql_.EscapeString(strPassword_) != strPassword_)
	{
		ERROR(__FILE__, __LINE__, "Illegal username [%s] or domainname [%s] or password [%s]\n", strUsername_.c_str(), strDomainname_.c_str(), strPassword_.c_str());
		return false;
	}

	stringstream ssSql;
	ssSql << "SELECT * FROM authreg "
		  << "WHERE username = '"
		  << strUsername_ << "'"
		  << " AND realm = '"
		  << strDomainname_ << "'";

	string strSql = ssSql.str();
	DEBUG(__FILE__, __LINE__, "Sql is : %s \n", strSql.c_str());

	if (0 != oMysql_.Query(strSql))
	{
		ERROR(__FILE__, __LINE__, "Failed to query with sql : %s\n", strSql.c_str());
		return false;
	}

	if (1 != oMysql_.GetRecordCount())
	{
		ERROR(__FILE__, __LINE__, "Doesn't register.\n");
		return false;
	}

	DEBUG(__FILE__, __LINE__, "Have registered!\n");
	DEBUG(__FILE__, __LINE__, "=========CRegister::IsRegisterd END===========\n");
	return true;
}

//注册用户
bool CRegister::Register()
{
	DEBUG(__FILE__, __LINE__, "=========CRegister::Register START=========\n");

	if (IsRegisterd())
	{
		ERROR(__FILE__, __LINE__, "Already registered user.\n");
		return false;
	}

	//步骤一：向authreg表中插入数据
	stringstream ssSql;
	ssSql << "INSERT INTO authreg VALUES ('"
		  << strUsername_ << "', '"
		  << strDomainname_ << "', '"
		  << strPassword_ << "')";
	string strSql = ssSql.str();
	DEBUG(__FILE__, __LINE__, "Sql is : %s \n", strSql.c_str());

	if (0 != oMysql_.Execute(strSql))
    {
        //sql语句执行错误
        ERROR(__FILE__, __LINE__, "failed to execute for sql : %s\n", strSql.c_str());
        return false;
    }

    if (1 != oMysql_.GetAffectedRows())
    {
        //插入失败
        ERROR(__FILE__, __LINE__, "DB has error for sql : %s\n", strSql.c_str());
        return false;
    }

	//步骤二：向active表中插入数据
	ssSql.str("");
	ssSql << "INSERT INTO active (`collection-owner`) VALUES ('"
		  << strUsername_ << "@" << strDomainname_ << "')";

	strSql = ssSql.str();
	DEBUG(__FILE__, __LINE__, "Sql is : %s \n", strSql.c_str());

	if (0 != oMysql_.Execute(strSql))
    {
        //sql语句执行错误
        ERROR(__FILE__, __LINE__, "failed to execute for sql : %s\n", strSql.c_str());
		DeleteFromAuthreg();
        return false;
    }

    if (1 != oMysql_.GetAffectedRows())
    {
        //插入失败
        ERROR(__FILE__, __LINE__, "DB has error for sql : %s\n", strSql.c_str());
		DeleteFromAuthreg();
        return false;
    }

	//步骤三：获取刚刚插入到active表中的那行记录的id
	strSql = "SELECT LAST_INSERT_ID()";

	if (0 != oMysql_.Query(strSql))
	{
		ERROR(__FILE__, __LINE__, "Failed to query with sql : %s\n", strSql.c_str());
		return false;
	}

	if (1 != oMysql_.GetRecordCount())
	{
		ERROR(__FILE__, __LINE__, "Doesn't get last insert id.");
		return false;
	}

	iUid_ = ::atoi(oMysql_.GetField(0, "LAST_INSERT_ID()").c_str());
	DEBUG(__FILE__, __LINE__, "Table active's last insert id is : %d\n", iUid_);

	DEBUG(__FILE__, __LINE__, "=========CRegister::Register ENd===========\n");
	return true;
}

//删除用户
bool CRegister::Unregister()
{
	DEBUG(__FILE__, __LINE__, "=========Register::Unregister START========\n");

	if (0 == iUid_)
	{
		ERROR(__FILE__, __LINE__, "There's no uid\n");
		return false;
	}

	//步骤一：从authreg表中删除数据
	if (!DeleteFromAuthreg())
	{
		return false;
	}

	//步骤二：从active表中删除数据
	if (!DeleteFromActive())
	{
		return false;
	}

	DEBUG(__FILE__, __LINE__, "=========Register::Unregister END========\n");
	return true;
}

bool CRegister::DeleteFromAuthreg()
{
	DEBUG(__FILE__, __LINE__, "=========Register::DeleteFromAuthreg START========\n");
	stringstream ssSql;
	ssSql << "DELETE FROM authreg "
		  << "WHERE username = '"
		  << strUsername_ << "' "
		  << "AND realm = '"
		  << strDomainname_ << "'";

	string strSql = ssSql.str();
	DEBUG(__FILE__, __LINE__, "Sql is : %s \n", strSql.c_str());

	if (0 != oMysql_.Execute(strSql))
    {
        //sql语句执行错误
        ERROR(__FILE__, __LINE__, "failed to execute for sql : %s\n", strSql.c_str());
        return false;
    }

    if (1 != oMysql_.GetAffectedRows())
    {
        //删除失败
        ERROR(__FILE__, __LINE__, "DB has error for sql : %s\n", strSql.c_str());
        return false;
    }
	DEBUG(__FILE__, __LINE__, "=========Register::DeleteFromAuthreg END==========\n");
	return true;
}

bool CRegister::DeleteFromActive()
{
	DEBUG(__FILE__, __LINE__, "=========Register::DeleteFromActive START========\n");
	stringstream ssSql;
	ssSql << "DELETE FROM active "
		  << "WHERE `object-sequence` = " << iUid_;

	string strSql = ssSql.str();
	ERROR(__FILE__, __LINE__, "Sql is : %s \n", strSql.c_str());

	if (0 != oMysql_.Execute(strSql))
    {
        //sql语句执行错误
        ERROR(__FILE__, __LINE__, "failed to execute for sql : %s\n", strSql.c_str());
        return false;
    }

    if (1 != oMysql_.GetAffectedRows())
    {
        //删除失败
        ERROR(__FILE__, __LINE__, "DB has error for sql : %s\n", strSql.c_str());
        return false;
    }
	DEBUG(__FILE__, __LINE__, "=========Register::DeleteFromActive END==========\n");
	return true;
}
