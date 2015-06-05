#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>
#include <iconv.h>
#include <fstream>
#include "json/json.h"
#include "sms.h"
#include "stringutil.h"

#define MAXPARAM 2048
#define MAXLINE 4096
#define SUCCESS_CODE 0
#define CHECK(res) do {if (!res){return false;}} while (0)

namespace SMS
{
	static struct SMSConf
	{
		std::string		apiKey;
		std::string		textTemplate;
		std::string		hostName;
		std::string		page;
		int				port;
	}_conf;


	static int HttpConnect( const std::string& hostName, int port )
	{
		//create socket
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if ( -1 == sockfd )
		{
			return -1;
		}
		sockaddr_in servaddr;
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port);

		hostent *hptr = NULL;
		if (NULL == (hptr = gethostbyname(hostName.c_str())) ) 
		{
			return -1;
		}

		for (char** pptr = hptr->h_addr_list; *pptr; pptr++ )
		{
			//char addr_buf[256]={0};
			//inet_ntop( hptr->h_addrtype, *pptr, addr_buf, sizeof(addr_buf));
			servaddr.sin_addr = *((in_addr*)*pptr);

			//keep connect server until success
			if ( -1 != connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)) )
			{
				return sockfd;
			}
		}
		return -1;
	}

	bool Configure(const std::string& cfgFileName)
	{
 		std::ifstream jsonFileStream( cfgFileName.c_str() );
		if ( !jsonFileStream )
		{
			return false;
		}
		Json::Reader reader;
		Json::Value jsonInput;
		CHECK( reader.parse(jsonFileStream, jsonInput) );

		CHECK( jsonInput["apiKey"].isString() );
		_conf.apiKey = jsonInput["apiKey"].asString();

		CHECK( jsonInput["textTemplate"].isString() );
		_conf.textTemplate = jsonInput["textTemplate"].asString();

		CHECK( jsonInput["hostName"].isString() );
		_conf.hostName = jsonInput["hostName"].asString();

		CHECK( jsonInput["page"].isString() );
		_conf.page = jsonInput["page"].asString();

		CHECK( jsonInput["port"].isInt() );
		_conf.port = jsonInput["port"].asInt();

		return true;
	}

	int SendSMS( const std::string& mobile, const std::string& identifyCode, int expireTime )
	{

		//connect to server
		int sockfd = HttpConnect(_conf.hostName, _conf.port);
		if ( -1 == sockfd )
		{
			return STATUS_SEND_SMS_UNCONNECT;
		}

		std::string text(_conf.textTemplate);
		StringUtil::Replace(text, "#code#", identifyCode );
		StringUtil::Replace(text, "#time#", StringUtil::Int2String(expireTime));

		//get http packet body
		std::ostringstream bufStream;
		bufStream << "apikey=" << _conf.apiKey 
			<< "&mobile=" << mobile 
			<< "&text=" << text;
		std::string content(bufStream.str());

		//get http packet head, and connect body to head
		char sendline[MAXLINE + 1] = {0};
		snprintf(sendline, MAXLINE,
			"POST %s HTTP/1.0\r\n"
			"Host: %s\r\n"
			"Content-type: application/x-www-form-urlencoded\r\n"
			"Content-length: %zu\r\n\r\n"
			"%s", _conf.page.c_str(), _conf.hostName.c_str(), content.size(), content.c_str());

		//send request
		write(sockfd, sendline, strlen(sendline));

		char recvline[MAXLINE + 1] = {0};
		size_t n = 0;
		if (( n = read(sockfd, recvline, MAXLINE)) > 0) {
			recvline[n] = '\0';
		}
		//get response packet body
		char* packetBody = strstr(recvline, "\r\n\r\n");

		//parse response
		Json::Reader reader;
		Json::Value jsonInput;
		if ( !reader.parse(packetBody, jsonInput) )
		{
			return STATUS_SEND_SMS_PARSEERROR;
		}
		if (jsonInput["code"].isNumeric() && SUCCESS_CODE == jsonInput["code"].asInt())
		{
			return STATUS_SEND_SMS_SUCCESS;
		}
		return STATUS_SEND_SMS_PARSEERROR;
	}
};
