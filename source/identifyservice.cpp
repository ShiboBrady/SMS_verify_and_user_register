#include <stdlib.h>
#include <time.h>
#include <sstream>
#include "identifyservice.h"
#include "configuration.h"
#include "sms.h"
#include "category.h"

CIdentifyService::CIdentifyService():_mutex(PTHREAD_MUTEX_INITIALIZER)
{
}

CIdentifyService::~CIdentifyService()
{
}

int CIdentifyService::StartService()
{
	int ret = RTN_FAIL;
	if ( SMS::Configure(CConfiguration::GetSMSConfFile()) )
	{
		ret = RunThread();
	}
	return ret;
}

int CIdentifyService::ThreadProc(void)
{
	DEBUG(__FILE__, __LINE__, "Start Identify Service, thread id: %X", pthread_self());
	int expireTime = CConfiguration::GetSMSExpireTime();
	while (true)
	{
		sleep( expireTime );
		pthread_mutex_lock( &_mutex );
		//DEBUG(__FILE__, __LINE__, "start scan list to erase expire object");
		for ( std::list<Node>::iterator it = _timeoutList.begin(); it != _timeoutList.end(); )
		{
			time_t current = time(NULL);
			time_t interval = current - it->_createTime;
			//DEBUG(__FILE__, __LINE__, "interva: %d", interval);
			if ( interval >= expireTime )
			{
				//DEBUG(__FILE__, __LINE__, "erase mobile: %s, code: %s, expire time: %d",
				//	(it->_mobile).c_str(), (it->_idCode).c_str(), interval );
				it = _timeoutList.erase(it);
				
			}
			else
			{
				++it;
			}
		}
		//DEBUG(__FILE__, __LINE__, "end scan, the list`s size is %d", _timeoutList.size() );
		pthread_mutex_unlock( &_mutex );
		
	}
	return 0;
}

bool CIdentifyService::CheckCode( const std::string& mobile, const std::string& idCode ) const
{
	int expireTime = CConfiguration::GetSMSExpireTime();
	pthread_mutex_lock(&_mutex);
	for ( std::list<Node>::const_iterator it = _timeoutList.begin(); it != _timeoutList.end(); ++it)
	{
		time_t current = time(NULL);
		time_t interval = current - it->_createTime; 
		if ( it->_mobile == mobile 
			&& it->_idCode == idCode
			&& interval < expireTime )
		{
			pthread_mutex_unlock(&_mutex);
			return true;
		}
	}
	pthread_mutex_unlock(&_mutex);
	return false;
}

bool CIdentifyService::SendCode( const std::string& mobile )
{
	std::string idCode = CreateIdCode(CConfiguration::GetSMSCodeDigit());
	DEBUG(__FILE__, __LINE__, "Create id code %s for user mobile %s", idCode.c_str(), mobile.c_str());
//	int ret = SMS::SendSMS(mobile, idCode, CConfiguration::GetSMSExpireTime());
// 	if ( STATUS_SEND_SMS_SUCCESS != ret )
// 	{
// 		ERROR(__FILE__, __LINE__, "Failed to Send SMS to %s, id code is %s\nerror status is %d, please check your SMS module setting",
// 			mobile.c_str(), idCode.c_str(), ret );
// 		return false;
// 	}

	pthread_mutex_lock(&_mutex);
	Node node(mobile, idCode, time(NULL));
	_timeoutList.push_back(node);
	pthread_mutex_unlock(&_mutex);
	return true;
}

bool CIdentifyService::ExistMobile( const std::string& mobile ) const
{
	pthread_mutex_lock(&_mutex);
	for ( std::list<Node>::const_iterator it = _timeoutList.begin(); it != _timeoutList.end(); ++it)
	{
		if ( it->_mobile == mobile )
		{
			pthread_mutex_unlock(&_mutex);
			return true;
		}
	}
	pthread_mutex_unlock(&_mutex);
	return false;
}

std::string CIdentifyService::CreateIdCode(int digit) const
{
	std::ostringstream outputBuf;
	srand( time(NULL) );
	for ( int ii = 0; ii < digit; ii++ )
	{
		int n = random()%10;
		outputBuf << n;
	}
	return outputBuf.str();
}