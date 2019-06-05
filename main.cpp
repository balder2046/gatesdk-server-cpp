#include "Windows.h"
#include <iostream>
#include <stdio.h>
#include <memory>

#include "csgateconnection.h"
#include "servergateevents.h"
using namespace csserverevents;
int testNetwork()
{
	printf("hello world");

	// Test Code ......
	WORD wVersionRequested;
	WSADATA wsaData;

	int err;
	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {


		return 1;
	}


	CSGateConnection *conn = new CSGateConnection();
	conn->connectToGate("127.0.0.1", 10003,"gsserver",true);
	int count = 0;
	while (true) {
		Sleep(30);
		count++;
		LIST_EVENT_PTR evtlist = conn->PopEventList();
		LIST_EVENT_PTR::iterator iter;

		for (iter = evtlist.begin(); iter != evtlist.end(); ++iter) {
			EVENT_PTR ptr = *iter;
			switch ((ptr)->getType()) {
			case ID_REGISTER_RESPONSE:
			{
				RegisterResponseEvent *pevt = (RegisterResponseEvent*)ptr;
				if (pevt->ok)
				{
					printf("register server ok\n");
				
				}
				else
				{
					printf("register server failed ! errorcode:%d , reason %s \n",pevt->errorcode,pevt->reason.c_str());
					
				}
				
				
				break;
			}
			case ID_CLIENT_DATA: {
				ClientDataEvent *pevt = (ClientDataEvent *)ptr;
				printf("receiving data:\n %s \n", dumpHex(pevt->data.data(), pevt->data.size()).c_str());

				break;
			}
			case ID_CLIENT_CONNECTED:
			{
				ClientConnectedEvent *pevt = (ClientConnectedEvent *)ptr;
				printf("the client connected  using netid %lld\n",pevt->networkid);
				break;
			}
			case ID_CLIENT_DISCONNECTED:
			{
				ClientDisConnectedEvent *pevt = (ClientDisConnectedEvent *)ptr;
				printf("the client  netid %lld disconnected \n", pevt->networkid);
				
				break;
			}
			

			}
			//Ò»¶¨ÒªÉ¾³ý£¡£¡£¡£¡
			delete ptr;
		}
		evtlist.clear();
		if (count > 1000) {
			break;
		}

	}
	WSACleanup();
	return 0;
}
int testDumpText()
{
	int col = 16;
	for (int i = 0; i < 256; ++ i)
	{
		printf("%c", getPrintChar(i));
		if (i % 16 == 0 && i > 0)
		{
			printf(" |      %d - %d ", i - 16, i);
			printf("\n");
		}

	}
	return 0;
}
int main(){
	
	return testNetwork();
}
