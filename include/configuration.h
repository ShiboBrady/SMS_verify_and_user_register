#ifndef _CONTEXT_H_
#define _CONTEXT_H_
#include <stdlib.h>
#include <string>
#include "stringutil.h"
#include "confaccess.h"

class CConfiguration
{
public:
	CConfiguration();
	~CConfiguration();

	static inline void Initialize()
	{
		std::string confFile("cfg/");
		confFile += GetModuleName() + ".cfg";
		_isInit = _conf.Load( confFile );
	}

	/**
	 *	Return the log path
	**/
	static inline const std::string& GetLogPath()
	{
		static std::string value;
		if ( value.empty() && _isInit )
		{
			_conf.GetValue("Log", "LogPath", value);
		}
		return value;
	}

	/**
	 *	Return the level of logging
	 *	@param:
	 *	@return:
	**/
	static inline int GetLogLevel()
	{
		std::string value;
		if ( _isInit )
		{
			_conf.GetValue("Log", "Level", value);
		}
		return StringUtil::String2Int(value);
	}

	/**
	 *	Return the logging mode, using by log module
	 *	@return:
	**/
	static inline int GetOutMode()
	{
		std::string value;
		if ( _isInit )
		{
			_conf.GetValue("Log", "OutMode", value);
		}
		return StringUtil::String2Int(value);
	}

	/**
	 *	Return the name of this Module
	**/
	static inline std::string GetModuleName()
	{
#define REAL_STRING(S) (#S)
#define RETURN_STRING(S) REAL_STRING(S)

#ifdef _MODULE_NAME
		return RETURN_STRING(_MODULE_NAME);
#else
		return "register-server";
#endif
	}

	/**
	 *	Return the Database key file
	**/
	static inline const std::string& GetDBKeyPath()
	{
		static std::string value;
		if ( value.empty() && _isInit )
		{
			_conf.GetValue("Database", "KeyPath", value);
		}
		return value;
	}

	/**
	 *	Return the count of process transaction thread
	**/
	static inline int GetThreadCount()
	{
		std::string value;
		if ( _isInit )
		{
			_conf.GetValue("Thread", "ThreadNumber", value);
		}
		return StringUtil::String2Int(value);
	}

	static inline int GetSMSExpireTime()
	{
		std::string value;
		if ( _isInit )
		{
			_conf.GetValue("SMS", "ExpireTime", value);
		}
		return StringUtil::String2Int(value);
	}

	static inline std::string GetSMSConfFile()
	{
		std::string value;
		if ( _isInit )
		{
			_conf.GetValue("SMS", "ConfFile", value);
		}
		return value;
	}

	static inline int GetSMSCodeDigit()
	{
		std::string value;
		if ( _isInit )
		{
			_conf.GetValue("SMS", "CodeDigit", value);
		}
		return StringUtil::String2Int(value);
	}

    static inline std::string GetMySQLHostname()
    {
        std::string value;
        if (_isInit)
        {
            _conf.GetValue("MySQL", "HostName", value);
        }
        return value;
    }

    static inline std::string GetMySQLDataBase()
    {
        std::string value;
        if (_isInit)
        {
            _conf.GetValue("MySQL", "Database", value);
        }
        return value;
    }

    static inline std::string GetMySQLUsername()
    {
        std::string value;
        if (_isInit)
        {
            _conf.GetValue("MySQL", "Username", value);
        }
        return value;
    }

    static inline std::string GetMySQLPassword()
    {
        std::string value;
        if (_isInit)
        {
            _conf.GetValue("MySQL", "Password", value);
        }
        return value;
    }

    static inline int GetMySQLPort()
    {
        std::string value;
        if (_isInit)
        {
            _conf.GetValue("MySQL", "Port", value);
        }
        return StringUtil::String2Int(value);
    }

    static inline std::string GetXmppDomain()
    {
        std::string value;
        if (_isInit)
        {
            _conf.GetValue("XMPP", "Domain", value);
        }
        return value;
    }

private:
	static CConfAccess _conf;
	static bool _isInit;
};


#endif // !_CONTEXT_H_




