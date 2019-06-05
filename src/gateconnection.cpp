#include<windows.h>
#include <process.h>
#include <vector>
#include <iostream>
#include "CLock.h"
#include "google/protobuf/message_lite.h"
#include "WaitEvent.h"
#include "ByteBuf.h"
#include "gateconnection.h"
#include<stdarg.h>

#pragma comment(lib,"Ws2_32.lib") 
CGateConnection::CGateConnection(int framesizebytes)
{
	m_pLock = new CLock();
	m_pReceiveLock = new CLock();
	m_pWaitEvent = new CWaitEvent(0);
	m_socket = createBIOSocket();
	m_socketactive = false;
	m_nFrameSizeBytes = framesizebytes;
}
CGateConnection::~CGateConnection()
{
	shutdown(m_socket, 3);
	delete m_pLock;
	delete m_pReceiveLock;
	delete m_pWaitEvent;
	if (m_socket != 0)
	closesocket(m_socket);

}
void CGateConnection::log(const char *message) {
	std::cout << message << std::endl;
}

bool CGateConnection::connect(const char *szGateIP, unsigned short port)
{
	sockaddr_in in = makeSockAddr(szGateIP, port);
	int ret = ::connect(m_socket, (sockaddr *)&in, sizeof(in));
	
	if (ret != 0) {
		log("connect server fail!");
		return false;
	}
	m_socketactive = true;
	
	log("connect server ok!");
	createandrunThreads();
	return true;
}


void CGateConnection::createandrunThreads() {
	recvThread = _beginthread(recvLogicWrapper, 0, this);
	sendThread = _beginthread(sendLogicWrapper, 0, this);
}
void CGateConnection::Close() {
	shutdown(m_socket, 1);
	m_socketactive = false;
	closesocket(m_socket);
	m_socket = 0;
	log("shutdown called!");

}

void CGateConnection::sendLogic()
{
	log("sending thread started");
	try {
		while (m_socketactive) {
			// 100 ms
			int ret = waitSendReady(100);
			if (ret == 1) {
				// time out continue
				continue;
			}
			else
			if (ret == 0) {
				std::list<std::string> datas;
				{
					//lock
					lockSendData();
					datas.splice(datas.begin(), m_listSendData);
					unLockSendData();
				}
				for (std::list<std::string>::iterator iter = datas.begin(); iter != datas.end(); ++iter) {
					bool ret = false;
					if (m_nFrameSizeBytes == 4) {
						int length = iter->size();
						length = htonl(length);
						writeSomeBytesToSocket(m_socket, (const char *)&length,4);
					}
					else if (m_nFrameSizeBytes == 2)
					{
						unsigned short length = (unsigned short)iter->size();
						length = htons(length);
						writeSomeBytesToSocket(m_socket, (const char *)&length, 2);
					}
					else {
						throw - 10;
					}
					ret = writeSomeBytesToSocket(m_socket, iter->data(), iter->size());
					if (!ret) {
						throw - 1;
					}
				}
			}
			else {
				throw - 2;
			}
			
			
		};
	}
	catch (int errorcode) {
		char szbuf[256];
		sprintf(szbuf, "sending thread catched the exception %d", errorcode);
		log(szbuf);
	}
	log("sending thread exited");
}
 IGateEvent *CGateConnection::onRecvData(const char *data, int length) {
	 return 0;
}
 unsigned short CGateConnection::recvLengthShort() {
	 unsigned short length;
	 bool ok = readSomeBytesFromSocket(m_socket, (char *)&length, 2);
	 if (!ok) {
		 throw - 1;
	 }
	 length = ntohs(length);
	 return length;
 }
 int CGateConnection::recvLengthLong() {
	 int length;
	 bool ok = readSomeBytesFromSocket(m_socket, (char *)&length, 4);
	 if (!ok) {
		 throw - 1;
	 }
	 length = ntohl(length);
	 return length;
 }
void CGateConnection::recvLogic()
{
	log("receiving thread started");
	// 收消息 并且 解析消息
	// 收取4字节的长度
	CByteBuf recvbuffer(1024 * 1024 * 2);
	try {


		while (m_socketactive) {
			// first ,get the length;
			int length;
			if (m_nFrameSizeBytes == 4) {
				length = recvLengthLong();
			}
			else if (m_nFrameSizeBytes == 2) {
				length = recvLengthShort();
			}
			else {
				throw - 10;
			}
			char *buffer = recvbuffer.getWriteBuffer();
			int count = recvbuffer.getCapcityForWrite();
			if (length > count) {
				throw -2;
			}
			readSomeBytesFromSocket(m_socket, buffer, length);
			recvbuffer.writeSkip(length);
			// parse for protobuf
			IGateEvent *evt = onRecvData(buffer, length);
			EVENT_PTR evtptr(evt);
			if (evt != 0) {
				lockReceiveData();
				m_listEvent.push_back(evtptr);
				unLockReceiveData();
			}
			
			// trunc
			recvbuffer.trunc();



		}
	}
	catch (int errorcode)
	{
		char szbuf[128];
		sprintf(szbuf,"receiving thread error catched error code %d",errorcode);
		log(szbuf);
	}
	log("recving thread exited");
}
void CGateConnection::sendData(const char *buf, int length) {
	std::list<std::string> listData;
	listData.push_back(std::string());
	std::string &newdata = listData.back();
	newdata.reserve(length);
	// move the sring to data
	newdata.append(buf, length);
	lockSendData();
	m_listSendData.splice(m_listSendData.end(), listData);
	unLockSendData();
	signalSendReady();
}

void CGateConnection::sendMessage(::google::protobuf::MessageLite *message) {
	std::list<std::string> listData;
	listData.push_back(std::string());
	std::string &newdata = listData.back();
	message->SerializeToString(&newdata);
	lockSendData();
	m_listSendData.splice(m_listSendData.end(), listData);
	unLockSendData();
	signalSendReady();
	
	
}


void CGateConnection::sendLogicWrapper(void *data)
{

	CGateConnection *connection = (CGateConnection *)data;
	connection->sendLogic();
}
void CGateConnection::recvLogicWrapper(void *data) {
	
	CGateConnection *connection = (CGateConnection *)data;
	connection->recvLogic();
}

EVENT_PTR CGateConnection::PopEvent() {
	EVENT_PTR evt;
	lockReceiveData();
	if (!m_listEvent.empty()) {
		evt = m_listEvent.front();
		m_listEvent.pop_front();
	}
	unLockReceiveData();
	return evt;
}
LIST_EVENT_PTR CGateConnection::PopEventList() {
	LIST_EVENT_PTR listret;
	lockReceiveData();
	listret.splice(listret.begin(), m_listEvent);
	unLockReceiveData();
	return listret;

}
int CGateConnection::waitSendReady(long timeoutInMilliseconds) {
	int ret = m_pWaitEvent->wait(timeoutInMilliseconds);
	return ret;
}
void CGateConnection::signalSendReady() {
	m_pWaitEvent->signal();
}
void CGateConnection::lockSendData() {
	m_pLock->Lock();
}
void CGateConnection::unLockSendData() {
	m_pLock->UnLock();
}
void CGateConnection::lockReceiveData() {
	m_pReceiveLock->Lock();
}
void CGateConnection::unLockReceiveData() {
	m_pReceiveLock->UnLock();
}

void CGateConnection::logFormat(const char *format, ...)
{
	char szbuf[2048];
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(szbuf, 2048, format, argptr);
	va_end(argptr);
	log(szbuf);
}