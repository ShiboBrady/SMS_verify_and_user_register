#ifndef __XMPP_REGISTER_H__
#define __XMPP_REGISTER_H__

#include <string>
#include "db.h"

namespace xmpp
{
class CRegister
{
	public:
		CRegister(std::string strUsername, std::string strDomainname, std::string strPassword);
		~CRegister();
		
		//判断是否注册过
		bool IsRegisterd();
		
		//注册用户
		bool Register();
		
		//删除用户
		bool Unregister();
		
	private:
		std::string strUsername_;
		std::string strDomainname_;
		std::string strPassword_;
		int iUid_;
		
		db::Mysql oMysql_;
		
		bool DeleteFromAuthreg();
		bool DeleteFromActive();
		
};

}
#endif /*__XMPP_REGISTE_H__*/