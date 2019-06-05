#pragma once
#include <WinSock.h>
#include <string>
#include <vector>
bool readSomeBytesFromSocket(SOCKET socket, char *buffer, int ncount);
bool writeSomeBytesToSocket(SOCKET socket, const char *buffer, int ncount);
// get ip and port  for socket
SOCKET createBIOSocket();
sockaddr_in makeSockAddr(const char *szIP, unsigned short port);
std::string dumpHex(const char *data,int count);
std::string dumpHexAscii(const char *data, int count, int col = 16);
std::vector<std::string> getAddrsByHostName(const std::string &hostname);
std::string getRandomAddrByHostName(const std::string &hostname);

char getPrintChar(char val);
