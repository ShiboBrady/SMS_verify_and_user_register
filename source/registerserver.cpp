#include <assert.h>
#include <unistd.h>
#include <syslog.h>
#include "sam_init.h"
#include "transactionprocess.h"
#include "fcgi_stdio.h"
#include "category.h"
#include "configuration.h"
#include "identifyservice.h"

CIdentifyService g_identifyService;

static void HandleSignal()
{

}

static void InitCategory()
{
	if( 0 != CCategory::initialize
		( CConfiguration::GetLogPath().c_str(), 
		CConfiguration::GetLogLevel(), 
		CConfiguration::GetModuleName().c_str(), 
		R_NOSOCKET, 0, 0, 
		CConfiguration::GetOutMode(), O_FILE) )
	{
		printf("Initial log error, program pid=%d is gonna shutdown", getpid());
		exit(-1) ;
	}
	INFO(__FILE__, __LINE__, "Initialize log system success!" );
	
}

static void InitSam()
{
	//SAM模块初始化
	if (0 != msas::sam::SamInit())
	{
		ERROR(__FILE__, __LINE__, "Sam init error!");
		exit(-1);
	}
	INFO(__FILE__, __LINE__, "Sam init Successfuly");
}

static void StartWorkTread()
{
	int iThreadNum = CConfiguration::GetThreadCount();
	if ( iThreadNum <= 0)
	{
		ERROR(__FILE__, __LINE__, "Thread number can`t be less than 0");
		iThreadNum = 1;
	};

	//Create the transaction process thread
	CTransactionProcess* transactions = new CTransactionProcess[iThreadNum];
	if ( !transactions )
	{
		FATAL(__FILE__, __LINE__, "Create transaction process thread failed, No free memory");
		exit(-1);
	}

	INFO(__FILE__, __LINE__, "%d Threads is gonna to start", iThreadNum);
	for ( int i = 0; i < iThreadNum; ++i )
	{
		//Start thread
		CTransactionProcess& ts= transactions[i];
		ts.RunThread();
	}
}


static void InitIdentifyService()
{
	if ( RTN_FAIL == g_identifyService.StartService() )
	{
		ERROR(__FILE__, __LINE__, "Start SMS Service Error");
		exit(-1);
	}
	INFO(__FILE__, __LINE__, "Success to Start SMS Service");
}


int main(/*int argc, char* argv[]*/)
{
	chdir( ".." );
	CConfiguration::Initialize();
	
	//Initialize the log module 
	InitCategory();
	
	//Inititalize the sam module
	InitSam();
	
	char home[MAX_PATH_LEN] = {0};
	getcwd(home,MAX_PATH_LEN);
	INFO(__FILE__, __LINE__, "The Program`s Workspace is %s, PID is %d", home, getpid());

	//register the signal handle
	HandleSignal();

	//Initialize the fcgi module
	FCGX_Init();

	InitIdentifyService();

	StartWorkTread();

	INFO(__FILE__, __LINE__, "The main thread is waiting for signal");
	pause();

	return 0;
}
