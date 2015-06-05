#ifndef _PACKET_H_
#define _PACKET_H_

#include <map>
#include <string>
#include "fcgiapp.h"
#include "packetdefine.h"

//---------------------------------------------------------------------------    
//  The request packet. Send from client to server. "CHttpConnection"
//  parse the buffer date into this packet. And provide the general interface
//  to Get/Set the class member.
//
//  @AUTHOR: Ryan Peng 2015/02/11
//---------------------------------------------------------------------------
class CRequest
{
public:
    CRequest();
    ~CRequest();

    int GetCode() const;
    
    int GetAttributeCount() const;
    
    void SetCode(int code);

    /**
     *  Return the field value by specify field id
     *  @param key The identification of field
     *  @param value Return the corresponding value
     *  @return Sucess return true, failed return false
    **/
    bool GetAttribute(const std::string& key, std::string& value ) const;

    /**
     *  Add one key-value pair into the attribute list. 
     *  @return If id is exist,add failed and return false. otherwise return true
    **/
    bool AddAttribute( const std::string& key, const std::string& value );

	/**
	 *	Print all the attribute value on log file
	**/
	void print() const;

private:
    typedef std::map<std::string, std::string> FieldItemList;

    int _code;

    //Attribute key-value pair list. 
    //key store the name of field
    //value store the value of field
    FieldItemList _attr;
};

//---------------------------------------------------------------------------
//  The response packet. Created by CTransaction`s inheritance-class. the
//  CHttpConnection send it to client app.
//  
//  AUTHOR: Ryan Peng 2015/02/11
//---------------------------------------------------------------------------
class CResponse
{
public:
    CResponse();
    ~CResponse();

    int GetErrorCode() const;

    const std::string& GetErrorText() const;

    void SetErrorCode(int errCode);

    void SetDetail(const char* detail);


private:
    int _errCode;
    std::string _detail;
};

class CHttpConnection
{
public:
    CHttpConnection(FCGX_Request* pRequest = NULL);
    ~CHttpConnection();

    bool Recv( CRequest& req ) const;

    bool Send( const CResponse& rep ) const;

private:
    FCGX_Request* _pRequest;
};

#endif