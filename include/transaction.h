#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

class CRequest;
class CResponse;

class CTransaction
{
public:
	CTransaction();
	virtual ~CTransaction();

	/**
	 *	Get the attribute of request and process a transaction. when transaction is over
	 *	construct a response packet and return external according process result. 
	 *	This function will be call when receiving a request. derived class must implement
	 *	this function.
	 *	@param req request packet receive from client
	 *	@param rep return the process result 
	 *	@return success return true, fail return false
	**/
	virtual void ProcessTransaction( const CRequest& req, CResponse& rep ) = 0;
};

/**
 *	Process Register transaction
**/
class CRegisterTransaction : public CTransaction
{
public:
	CRegisterTransaction();
	virtual ~CRegisterTransaction();

	virtual void ProcessTransaction( const CRequest& req, CResponse& rep );

};

/**
 *	Process Sending code to user`s phone by sms
**/
class CIdentifyTransaction : public CTransaction
{
public:
	CIdentifyTransaction();
	~CIdentifyTransaction();

	virtual void ProcessTransaction( const CRequest& req, CResponse& rep );

};

/**
 *	Error transaction code process class
**/
class CInvalideTransaction : public CTransaction
{
public:
	CInvalideTransaction();
	~CInvalideTransaction();

	virtual void ProcessTransaction(const CRequest& req, CResponse& rep);

};

/**
 *	Create Transaction class by request type
**/
class CTransactionFactory
{
public:
	CTransactionFactory();
	~CTransactionFactory();

	/**
	 *	Create the right Transaction derived class by code and subCode
	 *	of this request packet
	 *	@return success return the right type derived class point
	 *			failed return NULL
	**/
	static CTransaction* CreateTransaction( const CRequest& req );

	static void FreeTransaction( CTransaction* &pTransaction );

};




#endif