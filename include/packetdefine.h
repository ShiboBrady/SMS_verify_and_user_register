#ifndef _PACKET_DEFINE_H
#define _PACKET_DEFINE_H

//request code define
#define REQ_CODE_IDENTIFY	            0
#define REQ_CODE_REGISTER	            1

//response code and text define
#define RSP_CODE_UNPROCESS		        -1
#define RSP_TEXT_UNPROCESS		        "未处理"

#define RSP_CODE_SUCCESS		        0
#define RSP_TEXT_SUCCESS		        "OK"

#define RSP_CODE_SMSSENDING		        1
#define RSP_TEXT_SMSSENDING		        "已发送验证信息"

#define RSP_CODE_ERRORCODE		        2
#define RSP_TEXT_ERRORCODE		        "验证码错误"

#define RSP_CODE_ERRORFORMAT	        3
#define RSP_TEXT_ERRORFORMAT	        "格式错误"

#define RSP_CODE_ERRORDATA		        4
#define RSP_TEXT_ERRORDATA		        "数据错误"

#define RSP_CODE_INVALIDE_REQUEST		5
#define RSP_TEXT_INVALIDE_REQUEST		"非法请求"

#define RSP_CODE_INVALIDE_PHONENUM      6
#define RSP_TEXT_INVALIDE_PHONENUM      "手机号码格式不正确"

#define RSP_CODE_REGISTED_MOBILE        7
#define RSP_TEXT_REGISTED_MOBILE        "已注册的手机号"

#define RSP_CODE_REGISTE_FAILED         8
#define RSP_TEXT_REGISTE_FAILED         "注册失败"

#define RSP_CODE_XMPP_REGISTE_FAILED    9
#define RSP_TEXT_XMPP_REGISTE_FAILED    "xmpp注册失败"

//request field key define
#define REQ_FIELD_KEY_MOBILE	"mobile"
#define REQ_FIELD_KEY_PASSWORD	"password"
#define REQ_FIELD_KEY_IDCODE	"idcode"

#endif