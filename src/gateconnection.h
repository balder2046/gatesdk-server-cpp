#pragma once
#include <list>

#include "SocketUtil.h"

#include <string>
#include "gateevents.h"
#include <memory>
namespace google {
	namespace protobuf {
		class MessageLite;
	}
}
typedef  IGateEvent *EVENT_PTR;
typedef std::list<EVENT_PTR> LIST_EVENT_PTR;


class CGateConnection {
public:
	CGateConnection(int framesizebytes);
	virtual~CGateConnection();

protected:
	void log(const char *message);
protected:
	void logFormat(const char *format,...);
protected:
	bool connect(const char *szGateIP, unsigned short port);
	void sendData(const char *buf, int length);
	void sendMessage(::google::protobuf::MessageLite *message);

public:
	void Close();
	EVENT_PTR PopEvent();
	LIST_EVENT_PTR PopEventList();

private:
	void createandrunThreads();
private:
	static void sendLogicWrapper(void *data);
	static void recvLogicWrapper(void *data);
	void sendLogic();
	void recvLogic();
private:
	unsigned short recvLengthShort();
	int recvLengthLong();
protected:
	virtual IGateEvent *onRecvData(const char *data,int length);
	bool m_bConnected;
private:
	SOCKET m_socket;
	unsigned long sendThread;
	unsigned long recvThread;

	LIST_EVENT_PTR m_listEvent;
	std::list<std::string>  m_listSendData;
private:
	std::string &allocSendData();
private:
	int waitSendReady(long timeoutInMilliseconds);
	void signalSendReady();
	void lockSendData();
	void unLockSendData();
	void lockReceiveData();
	void unLockReceiveData();
private:
	class CWaitEvent *m_pWaitEvent;
	class CLock *m_pLock;
	class CLock *m_pReceiveLock;
	bool m_socketactive;
private:
	int m_nFrameSizeBytes;
	

};