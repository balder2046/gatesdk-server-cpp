#pragma once
#include "gateconnection.h"
class CSGateConnection:public CGateConnection{
public:
	CSGateConnection();
public:
	void connectToGate(const char *ipaddress,unsigned short port,const char *servername, bool defaultserver);
	void sendData(long long networkid, const char *data, int length);
	void sendDataToMulti(const long long *networkid, int count, const char *data, int length);
	void kickClient(long long networkid);
	void kickMultiClient(const long long *networkids, int count);
	void setClientLevel(long long networkid, int level);
	void sendPing();
	void disconnectFromGate();
protected:
	virtual IGateEvent *onRecvData(const char *data,int length);
private:
	/**
	 * \brief start to regster the server with the servername 
	 * \param servername : the name of the server to be registed
	 * \param defaultserver : if true, it's the default server of the node
	 * \return successful?
	 */
	bool startRegisterServer(const char *servername,bool defaultserver);
public:
	bool isConnected(){
		return m_bConnected;
	}
private:
	bool m_bConnected;

}; 