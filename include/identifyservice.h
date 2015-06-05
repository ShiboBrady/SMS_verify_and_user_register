#ifndef _IDENTIFY_TRANSACTION_H
#define _IDENTIFY_TRANSACTION_H

#include "basethread.h"
#include <list>
#include <pthread.h>
#include <string>

class CIdentifyService : public CBaseThread
{
public:
	CIdentifyService();
	~CIdentifyService();

	int StartService();

	bool CheckCode( const std::string& mobile, const std::string& idCode ) const;

	bool SendCode( const std::string& mobile );

	bool ExistMobile( const std::string& mobile ) const;

	virtual int ThreadProc(void);

private:
	std::string CreateIdCode( int digit ) const;

private:
	struct Node
	{
		Node(){}
		Node(const std::string& mobile, const std::string& idCode, time_t createTime)
			:_mobile(mobile), _idCode(idCode), _createTime(createTime){}
		std::string _mobile;
		std::string _idCode;
		time_t _createTime;
	};

	std::list<Node> _timeoutList;

	mutable pthread_mutex_t _mutex;
};



#endif // !_IDENTIFY_TRANSACTION_H
