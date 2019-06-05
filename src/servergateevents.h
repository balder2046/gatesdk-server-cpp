#pragma once
#include <string>
#include "gateevents.h"
#define ID_CLIENT_CONNECTED 20001

#define ID_CLIENT_DISCONNECTED 20002


#define ID_CLIENT_DATA 20003


// the response for the register server 
#define ID_REGISTER_RESPONSE 20004






namespace csserverevents
{
	class ClientConnectedEvent : public IGateEvent
	{
	public:
		ClientConnectedEvent(long long networkid)
		{
			this->networkid = networkid;
		}
	
		long long networkid;
	public:
		int getType() const
		{
			return ID_CLIENT_CONNECTED;
		}

	};

	class ClientDisConnectedEvent : public IGateEvent
	{
	public:
		ClientDisConnectedEvent(long long networkid)
		{
			this->networkid = networkid;
		}

		long long networkid;
	public:
		int getType() const
		{
			return ID_CLIENT_DISCONNECTED;
		}

	};

	class ClientDataEvent : public IGateEvent
	{
	public:
		ClientDataEvent(long long networkid,const char *_data,int length)
		{
			this->data.append(_data, length);
			this->networkid = networkid;
		}

	public:
		int getType() const
		{
			return ID_CLIENT_DATA;
		}
		long long networkid;
		std::string data;
	};

	class RegisterResponseEvent : public IGateEvent
	{
	public:
		RegisterResponseEvent(bool _ok,int _errorcode,std::string _reason)
		{
			ok = _ok;
			errorcode = _errorcode;
			reason = _reason;
		}
		bool ok;
		int errorcode;
		std::string reason;
	public:
		int getType() const
		{
			return 0;
		}
	};





}
