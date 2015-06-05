#ifndef _IDENTIFY_H
#define _IDENTIFY_H

#include <string>

#define STATUS_SEND_SMS_SUCCESS		0
#define STATUS_SEND_SMS_UNCONNECT	1
#define STATUS_SEND_SMS_PARSEERROR	2

namespace SMS
{
	/**
	 *	Read the configuration from file. It`s will be used in requesting send SMS
	 *	And that cfg file`s content must be changed to fit in kinds of third service
	 *	interface
	**/
	bool Configure(const std::string& cfgFileName);

	/**
	 *	Send a SMS to specified mobile phone
	 *	@param mobile the mobile phone number you want send the SMS
	 *	@param identifyCode code to identfiy, is used to fill the #code# field in SMS template
	 *	@param expireTime the expire time of code, is used to fill the #time# field in SMS template
	 *	@return return STATUS_SEND_SMS_SUCCESS on success
	 *			return STATUS_SEND_SMS_UNCONNECT on can`t connect third server
	 *			return STATUS_SEND_SMS_PARSEERROR on parse response packet error
	**/
	int SendSMS( const std::string& mobile, const std::string& identifyCode, int expireTime );
};

#endif // !_IDENTIFY_H
