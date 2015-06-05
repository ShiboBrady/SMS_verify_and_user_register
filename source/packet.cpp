#include <map>
#include <sstream>
#include <stdlib.h>
#include <assert.h>
#include "category.h"
#include "json/json.h"
#include "packet.h"
#include "stringutil.h"

CRequest::CRequest():_code(-1)
{
}

CRequest::~CRequest()
{
}

int CRequest::GetCode() const
{
    return _code;
}

bool CRequest::GetAttribute( const std::string& key, std::string& value ) const
{
    FieldItemList::const_iterator iter = _attr.find(key);
    if ( _attr.end() == iter )
    {
        value = "";
        return false;
    }
    else
    {
        value = iter->second;
        return true;
    }
}

/**
 *   Return the attribute`s count
**/
int CRequest::GetAttributeCount() const
{
    return _attr.size();
}
    
void CRequest::SetCode(int code)
{
    _code = code;
}


bool CRequest::AddAttribute( const std::string& key, const std::string& value )
{
    
    if ( _attr.end() == _attr.find( key ) )
    {
        _attr[key] = value;
        return true;
    }
    else
    {
        return false;
    }
}

void CRequest::print() const
{
	DEBUG(__FILE__, __LINE__, "%d", _code);
	for ( FieldItemList::const_iterator it = _attr.begin(); it != _attr.end(); ++it )
	{
		DEBUG(__FILE__, __LINE__, "key: %s, value: %s", (it->first).c_str(), (it->second).c_str());
	}
}

CResponse::CResponse():_errCode(0),_detail("OK")
{
}

CResponse::~CResponse()
{

}

int CResponse::GetErrorCode() const
{
    return _errCode;
}

const std::string& CResponse::GetErrorText() const
{
    return _detail;
}

void CResponse::SetErrorCode(int errCode)
{
    _errCode = errCode;
}

void CResponse::SetDetail(const char* datail)
{
    _detail = datail;
}


CHttpConnection::CHttpConnection(FCGX_Request* pRequest):_pRequest(pRequest)
{

}

CHttpConnection::~CHttpConnection()
{
}


 /*brief : Send back http request to App.
 *param : Analytical parameters into rep.
 *return : success or not.
 */
bool CHttpConnection::Recv( CRequest& req ) const
{
	if ( !_pRequest )
	{
		ERROR(__FILE__, __LINE__, "CHttpConnection doesn`t initialize");
		return false;
	}
    DEBUG(__FILE__, __LINE__, "========== CHttpConnection::Recv START ========");
    int iPostDataLength;
    char* strLength = FCGX_GetParam("CONTENT_LENGTH", _pRequest->envp);
    if (strLength)
    {
        iPostDataLength = ::atoi(strLength);
    }
    else
    {
        ERROR(__FILE__, __LINE__, "Doesn't have post data! ");
		return false;
    }
	//int iPostDataLength = ::atoi(FCGX_GetParam("CONTENT_LENGTH", _pRequest->envp));
	char* data = new char[iPostDataLength + 1];
	assert(data);
	bzero(data, iPostDataLength + 1);
	FCGX_GetStr(data, iPostDataLength, _pRequest->in);

	Json::Reader reader;
	Json::Value jsonInput;

	//Transcate to json format.
	bool isSuccess = reader.parse(data, jsonInput);
	delete[] data;
	data = NULL;
 
	if (!isSuccess)
	{
		ERROR(__FILE__, __LINE__, "Post data is not json format!");
		return false;
	}

	//Get code 
	if (!jsonInput["code"].isInt())
	{
		ERROR(__FILE__, __LINE__, "Can't get code value.");
		return false;
	}
	int code  = jsonInput["code"].asInt();   
	req.SetCode(code);
	DEBUG(__FILE__, __LINE__, "code: %d", code);

	//get attribute array
	const Json::Value& jsonValue = jsonInput["attribute"];
	if ( jsonValue.isNull() || !jsonValue.isArray() )
	{
		ERROR(__FILE__, __LINE__, "Can't get attribute value.");
		return false;
	}
	//Get parameter from each json object.
	int iValueSize = jsonValue.size();
	for (int index = 0; index != iValueSize; index++)
	{
		if (!jsonValue[index]["key"].isString())
		{
			ERROR(__FILE__, __LINE__, "Can't get key value.");
			return false;
		}
		if (!jsonValue[index]["value"].isString())
		{
			ERROR(__FILE__, __LINE__, "Can't get value.");
			return false;
		}
		std::string key = jsonValue[index]["key"].asString();
		std::string value = jsonValue[index]["value"].asString();
		DEBUG(__FILE__, __LINE__, "key: %s, value: %s", key.c_str(), value.c_str());

		req.AddAttribute(key, value);
	}

    DEBUG(__FILE__, __LINE__, "========== CHttpConnection::Recv END ========");
    return true;
}

/*brief : Send back http request to App.
 *param : packaging parameters from rep to json.
 *return : success or not.
 */
bool CHttpConnection::Send( const CResponse& rsp ) const
{
	if ( !_pRequest )
	{
		ERROR(__FILE__, __LINE__, "CHttpConnection doesn`t initialize\n");
		return false;
	}
    DEBUG(__FILE__, __LINE__, "========== CHttpConnection::Send START ========");
    Json::Value jsonOutput;
    jsonOutput["errcode"] = rsp.GetErrorCode();
    jsonOutput["detail"] = rsp.GetErrorText();

    Json::FastWriter fast_writer;
    std::string strResult = fast_writer.write(jsonOutput);
    DEBUG(__FILE__, __LINE__, "Send result is : %s", strResult.c_str());

	//add this packet head for ajax cross Domain access
	FCGX_PutS("Access-Control-Allow-Origin: *\r\n", _pRequest->out);
    FCGX_PutS("Content-type: text/json\r\n\r\n", _pRequest->out);
    FCGX_PutS(strResult.c_str(), _pRequest->out);
    DEBUG(__FILE__, __LINE__, "========== CHttpConnection::Send END ========");
    return true;
}
