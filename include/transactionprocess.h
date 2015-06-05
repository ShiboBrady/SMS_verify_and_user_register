#ifndef _TRANSACTION_PROCESS_H_
#define _TRANSACTION_PROCESS_H_

#include "basethread.h"
//---------------------------------------------------------------------------
//  MODULE:    
//
//  PURPOSE:  
//
//	COMMENTS:
//    
//
//  AUTHOR: Ryan Peng
//---------------------------------------------------------------------------
class CTransactionProcess : public CBaseThread
{
public:
	CTransactionProcess(void);
	virtual ~CTransactionProcess(void);

	virtual int ThreadProc();

};

#endif // !_TRANSACTION_PROCESS_H_



