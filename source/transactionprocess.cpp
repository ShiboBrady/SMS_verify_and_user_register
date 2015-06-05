#include "transactionprocess.h"
#include "fcgiapp.h"
#include "transaction.h"
#include "packet.h"
#include "category.h"


CTransactionProcess::CTransactionProcess(void)
{
}


CTransactionProcess::~CTransactionProcess(void)
{
}


int CTransactionProcess::ThreadProc()
{
	INFO(__FILE__, __LINE__, "Start 0x%X transaction process thread", pthread_self() );

	FCGX_Request request = {0};
	FCGX_InitRequest(&request, 0, 0);
	for (;;)
	{
		int ret = FCGX_Accept_r(&request);

		if (ret < 0)
		{
			FATAL(__FILE__, __LINE__, "Accept fatal, Thread is gonna to stop");
			break;
		}
		char* server_name = FCGX_GetParam("SERVER_NAME", request.envp);
		INFO(__FILE__, __LINE__, "Accept a connection from %s", server_name);
		
/*#ifndef NDEBUG
		pid_t pid = getpid();
		FCGX_FPrintF(request.out,
			"Content-type: text/html\r\n"
			"\r\n"
			"<title>FastCGI Hello! (multi-threaded C, fcgiapp library)</title>"
			"<h1>FastCGI Hello! (multi-threaded C, fcgiapp library)</h1>"
			"Thread 0x%X, Process %ld<p>"
			"Threads running on host <i>%s</i><p><code>",
			pthread_self(), pid, server_name ? server_name : "?");
#endif*/
			
		CRequest req;
		CResponse rep;		

		//recive a request
		CHttpConnection httpConnection(&request);
		if ( httpConnection.Recv(req) )
		{
			//create an transaction by the request type
			CTransaction* pTransaction = CTransactionFactory::CreateTransaction( req );
			if ( !pTransaction )
			{
				ERROR(__FILE__, __LINE__, "Create transaction failed");
			}
			else
			{
				//invocation the function to process the transaction
				pTransaction->ProcessTransaction(req, rep);
			}
			CTransactionFactory::FreeTransaction(pTransaction);
		}
		else
		{
			ERROR(__FILE__, __LINE__, "Failed to receive the packet");
			rep.SetErrorCode(10);
			rep.SetDetail("Failed to receive the packet");	
		}

		//send response
		if ( !httpConnection.Send(rep) )
		{
			ERROR(__FILE__, __LINE__, "Send response packet error");
		}

		//finish one connection, accept next
		FCGX_Finish_r(&request);
	}
	DEBUG(__FILE__, __LINE__, "Thread stop");
	return RTN_SUCCESS;
}