#include "transaction.h"
#include <assert.h>
#include "packet.h"
#include "identifyservice.h"
#include "category.h"
#include "sam_init.h"
#include "sam_packet.h"
#include "sam_req_parser.h"
#include "sam_rsp_parser.h"
#include "sam_processor.h"
#include "configuration.h"
#include "xmpp_register.h"
#include "configuration.h"


extern CIdentifyService g_identifyService;

CTransaction::CTransaction()
{
}

CTransaction::~CTransaction()
{
}


/**
 *	CRegisterTransaction Funcation
**/
CRegisterTransaction::CRegisterTransaction()
{
}

CRegisterTransaction::~CRegisterTransaction()
{
}

/**
 *@@breif : 处理用户注册业务，将新用户的资料加入数据??
 *@@param1 : req:http请求数据
 *@@param2 : rep:http回复数据
 *return value : 执行结果
 */
void CRegisterTransaction::ProcessTransaction( const CRequest& req, CResponse& rsp )
{
	//TODO: Add register process transaction
	DEBUG(__FILE__, __LINE__, "========== CRegisterTransaction::ProcessTransaction START ========");
	rsp.SetErrorCode(RSP_CODE_UNPROCESS);
	rsp.SetDetail(RSP_TEXT_UNPROCESS);

	std::string strMobile;
	bool isGet = req.GetAttribute(REQ_FIELD_KEY_MOBILE, strMobile);
	std::string strPassword;
	isGet = isGet && req.GetAttribute(REQ_FIELD_KEY_PASSWORD, strPassword);
	std::string strIdCode;
	isGet = isGet && req.GetAttribute(REQ_FIELD_KEY_IDCODE, strIdCode);
	if ( !isGet )
	{
		rsp.SetErrorCode(RSP_CODE_ERRORFORMAT);
		rsp.SetDetail(RSP_TEXT_ERRORFORMAT);
		ERROR(__FILE__, __LINE__, "Can`t get complete data! mobile: %s, idCode: %d", strMobile.c_str(), strIdCode.c_str());
		return ;
	}
    else if ( 11 != strMobile.length() )
    {
        rsp.SetErrorCode(RSP_CODE_INVALIDE_PHONENUM);
        rsp.SetDetail(RSP_TEXT_INVALIDE_PHONENUM);
        return ;
    }
    //0--表示正式运行 1--表示测试模式
    #define STRESS_TEST 0
    #if (0 == STRESS_TEST)
	else if (!g_identifyService.CheckCode(strMobile, strIdCode))
	{
		rsp.SetErrorCode(RSP_CODE_ERRORCODE);
		rsp.SetDetail(RSP_TEXT_ERRORCODE);
		return ;
	}
    #endif
    #undef STRESS_TEST
    else
    {
        DEBUG(__FILE__, __LINE__, "A illegal request from mobile: %s. Begin to invoke Sam.", strMobile.c_str());
    }

    //++++++++++++注册xmpp服务器账号+++++++++++++++++++++

    xmpp::CRegister oRegister(strMobile, CConfiguration::GetXmppDomain(), strPassword);
	if (!oRegister.Register())
	{
		ERROR(__FILE__, __LINE__, "Register failed!\n");
        rsp.SetErrorCode(RSP_CODE_XMPP_REGISTE_FAILED);
	    rsp.SetDetail(RSP_TEXT_XMPP_REGISTE_FAILED);
		return;
	}
	else
	{
		DEBUG(__FILE__, __LINE__, "Register successed!\n");
	}

    //++++++++++++++++xmpp服务器注册完毕++++++++++++++++++


	//创建Sam类业务请求响应数据
	msas::sam::SamPacket sam_req_packet;

	//创建Sam类业务请求解析器
	msas::sam::SamReqParser sam_req_parser;

	//把CRequest解析到SamPacket
	int ret;
	ret = sam_req_parser.Parse(req, sam_req_packet);
	if (-1 == ret)
	{
		ERROR(__FILE__, __LINE__, "Parse CRequest to SamPacket error!");
        rsp.SetErrorCode(RSP_CODE_REGISTE_FAILED);
	    rsp.SetDetail(RSP_TEXT_REGISTE_FAILED);
		return;
	}

	//创建Sam类业务处理器
	msas::sam::SamProcessor sam_processor;

	//处理SamPacket
	ret = sam_processor.Process(sam_req_packet);
	if (-1 == ret)
	{
		ERROR(__FILE__, __LINE__, "Process SamPacket error!");
        rsp.SetErrorCode(RSP_CODE_REGISTE_FAILED);
	    rsp.SetDetail(RSP_TEXT_REGISTE_FAILED);
		return;
	}

	//创建Sam类业务响应解析器
	msas::sam::SamRspParser sam_rsp_parser;

	//把SamPacket解析到CResponse
	for (msas::sam::SamPacket * sam_rsp_packet = sam_processor.FetchRspPacket(); sam_rsp_packet != NULL; sam_rsp_packet = sam_processor.FetchRspPacket())
	{
		ret = sam_rsp_parser.Parse(rsp, *sam_rsp_packet);
		if (-1 == ret)
		{
			ERROR(__FILE__, __LINE__, "Parse SamPacket to CResponse error!");
		}

        int iErrorCode = rsp.GetErrorCode();

        //对sam的处理结果进行判断
        if (0 != iErrorCode)
        {
            //-1968 是sam业务定义的用户已经注册过的返回码
            if (-1968 == iErrorCode)
            {
                rsp.SetErrorCode(RSP_CODE_REGISTED_MOBILE);
	            rsp.SetDetail(RSP_TEXT_REGISTED_MOBILE);
            }
            else
            {
                rsp.SetErrorCode(RSP_CODE_REGISTE_FAILED);
	            rsp.SetDetail(RSP_TEXT_REGISTE_FAILED);

                //++++++++++++注销xmpp服务器账号+++++++++++++++++++++

                if (!oRegister.Unregister())
            	{
            		ERROR(__FILE__, __LINE__, "Unregister failed!");
            		//return -1;
            	}
            	else
            	{
            		DEBUG(__FILE__, __LINE__, "Unregister successed!\n");
            	}
                //++++++++++++++++xmpp服务器账号注销完毕++++++++++++++++
            }

			return;
        }
	}

	rsp.SetErrorCode(RSP_CODE_SUCCESS);
	rsp.SetDetail(RSP_TEXT_SUCCESS);
	DEBUG(__FILE__, __LINE__, "========== CRegisterTransaction::ProcessTransaction END ========");
}

/**
 *	CIdentifyTransaction Funcation
**/
CIdentifyTransaction::CIdentifyTransaction()
{

}

CIdentifyTransaction::~CIdentifyTransaction()
{
}

void CIdentifyTransaction::ProcessTransaction( const CRequest& req, CResponse& rep )
{
	std::string mobile;
	req.GetAttribute( REQ_FIELD_KEY_MOBILE, mobile );

	int errCode = RSP_CODE_SUCCESS;
	std::string expireTime = StringUtil::Int2String(CConfiguration::GetSMSExpireTime());
	const char* detail = expireTime.c_str();
    if ( 11 != mobile.length() )
    {
        errCode = RSP_CODE_INVALIDE_PHONENUM;
        detail = RSP_TEXT_INVALIDE_PHONENUM;
    }
	else if ( g_identifyService.ExistMobile(mobile) )
	{
		errCode = RSP_CODE_SMSSENDING;
		detail = RSP_TEXT_SMSSENDING;
	}
	else if( !g_identifyService.SendCode(mobile) )
	{
		errCode = RSP_CODE_ERRORDATA;
		detail = RSP_TEXT_ERRORDATA;
	}
    else
    {
        DEBUG(__FILE__, __LINE__, "Send a SMS!" );
    }

	rep.SetErrorCode( errCode );
	rep.SetDetail( detail );
}

CInvalideTransaction::CInvalideTransaction()
{

}

CInvalideTransaction::~CInvalideTransaction()
{

}

void CInvalideTransaction::ProcessTransaction( const CRequest& req, CResponse& rsp )
{
	rsp.SetErrorCode( RSP_CODE_INVALIDE_REQUEST );
	rsp.SetDetail( RSP_TEXT_INVALIDE_REQUEST );
}

CTransactionFactory::CTransactionFactory()
{
}

CTransactionFactory::~CTransactionFactory()
{
}

CTransaction* CTransactionFactory::CreateTransaction( const CRequest& req )
{
	int code = req.GetCode();
	CTransaction* pTransaction = NULL;
	switch (code)
	{
	case REQ_CODE_IDENTIFY:
		pTransaction = new CIdentifyTransaction;
		break;
	case REQ_CODE_REGISTER:
		pTransaction = new CRegisterTransaction;
		break;
	default:
		pTransaction = new CInvalideTransaction;
		break;
	}
	assert(pTransaction);
	return pTransaction;
}

void CTransactionFactory::FreeTransaction( CTransaction* &pTransaction )
{
	if ( pTransaction )
	{
		delete pTransaction;
		pTransaction = NULL;
	}
}
