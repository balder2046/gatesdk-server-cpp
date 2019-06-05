#include "time.h"
#include "csgateconnection.h"
#include "ClientSessionMessage.pb.h"
#include "ServerSessionMessage.pb.h"
#include "servergateevents.h"
using  namespace com::starlit::network::message::client;
using  namespace com::starlit::network::message::csserver;
using namespace csserverevents;
using namespace std;
CSGateConnection::CSGateConnection(): CGateConnection(2)
{

}
void CSGateConnection::connectToGate(const char *hostname,unsigned short port,const char *servername,bool defaultserver){
	// parse the ipaddress
	string ip = getRandomAddrByHostName(hostname);
	if (ip.empty())
	{
		// todo: notify connect gs error
		return;
	}
	logFormat("connect the hostname:%s ip:%s \n",hostname,ip.c_str());
	// first connect 
	bool ok = connect(ip.c_str(),port);
	
	if (!ok){
		// todo: notify connect gs error
		return;
	}
	log("connect gate successfully,and start register the server's name");
	startRegisterServer(servername,defaultserver);

	
}
void CSGateConnection::sendData(long long networkid, const char *data, int length)
{
	s2c_message bigmessage;
	s2c_data_message *msg = bigmessage.mutable_data();
	msg->set_networkid(networkid);
	msg->set_data(data, length);
	sendMessage(&bigmessage);
	
}
void CSGateConnection::sendDataToMulti(const long long *networkid, int count, const char *data, int length)
{
	s2c_message bigmessage;

	sendMessage(&bigmessage);
}
void CSGateConnection::kickClient(long long networkid)
{
	s2c_message bigmessage;

	sendMessage(&bigmessage);
}
void CSGateConnection::kickMultiClient(const long long *networkids, int count)
{
	s2c_message bigmessage;

	sendMessage(&bigmessage);
}
void CSGateConnection::setClientLevel(long long networkid, int level)
{
	s2c_message bigmessage;

	sendMessage(&bigmessage);
}


void CSGateConnection::sendPing(){
	s2c_message bigmessage;
	sendMessage(&bigmessage);
}
void CSGateConnection::disconnectFromGate(){
	Close();
}
IGateEvent *CSGateConnection::onRecvData(const char *data,int length){
	IGateEvent *evt = 0;
	c2s_message bigmessage;
	bigmessage.ParseFromArray(data, length);
	if (bigmessage.has_client_connected())
	{
		const c2s_client_connected &msg = bigmessage.client_connected();
		evt = new ClientConnectedEvent(msg.networkid());
		
	}
	else if (bigmessage.has_client_diconnected())
	{
		const c2s_client_disconnected &msg = bigmessage.client_diconnected();
		evt = new ClientDisConnectedEvent(msg.networkid());

	}
	else if (bigmessage.has_client_data())
	{
		const c2s_client_data &msg = bigmessage.client_data();

		evt = new ClientDataEvent(msg.networkid(), msg.data().data(), msg.data().size());
	}
	else if (bigmessage.has_register_response())
	{
		const g2s_register_response &msg = bigmessage.register_response();
		evt = new RegisterResponseEvent(msg.registerok(), msg.errorcode(), msg.reason());
		if (msg.registerok())
		{
			m_bConnected = true;
		}
		else
		{
			m_bConnected = false;
			
		}


	}

	return evt;
	
}

bool CSGateConnection::startRegisterServer(const char* servername, bool defaultserver)
{
	s2c_message bigmessage;
	s2g_register_server *msg = bigmessage.mutable_register_server();
	msg->set_isdefault(defaultserver);
	msg->set_servername(servername);
	sendMessage(&bigmessage);
	return true;
}
