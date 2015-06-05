#include "configuration.h"

CConfAccess CConfiguration::_conf;
bool CConfiguration::_isInit = false;

// CContext::CContext()
// {
// }
// 
// CContext::~CContext()
// {
// }
// 
// void CContext::Initialize()
// {
// 	const char* confFile = "cfg/register-server.cfg";
// 	_isInit = _conf.Load( confFile );
// }
// 
// const char* CContext::GetLogPath()
// {
// 	return "log";
// }
// 
// int CContext::GetLogLevel()
// {
// 	return 5;
// }
// 
// int CContext::GetOutMode()
// {
// 	return 1;
// }
// 
// const char* CContext::GetModuleName()
// {
// 	return "register-server";
// }
// 
// const char* CContext::GetConfPath()
// {
// 	return "cgf/register-server.cfg";
// }
// 
// const char* CContext::GetDBKeyPath()
// {
// 	return "";
// }
// 
// int CContext::GetThreadCount()
// {
// 	return 3;
// }