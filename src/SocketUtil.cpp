#include "SocketUtil.h"
SOCKET createBIOSocket()
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	u_long mode = 0;
	ioctlsocket(sock,FIONBIO,&mode);
	return sock;
}
sockaddr_in makeSockAddr(const char *szIP, unsigned short port)
{
	sockaddr_in in;
	in.sin_family = AF_INET;
	in.sin_port = htons(port);
	in.sin_addr.S_un.S_addr = inet_addr(szIP);

	return in;
}
bool readSomeBytesFromSocket(SOCKET socket, char *buffer, int ncount) {
	int writeindex = 0;
	while (writeindex < ncount) {
		int remainsize = ncount - writeindex;
		int readsize = recv(socket, buffer + writeindex, remainsize,0);
		if (readsize <= 0) {
			return false;
		}
		writeindex += readsize;
	}
	return true;
}
bool writeSomeBytesToSocket(SOCKET socket, const char *buffer, int ncount) {
	
	int readindex = 0;
	while (readindex < ncount) {
		int remainsize = ncount - readindex;
		int writesize = send(socket, buffer + readindex, remainsize, 0);
		if (writesize <= 0) {
			return false;
		}
		readindex += writesize;
	}
	return true;
}
std::string dumpHex(const char *data,int count){
	std::string text;
	for (int i = 0; i < count; ++i){
		char buf[32];
		sprintf(buf,"0x%02x ",data[i]);
		text += buf;
	}
	return text;
}
std::string dumpHexAscii(const char *data, int count, int col)
{
	std::string text;
	char buf[64];

	for (int index = 0; index < count; index += col)
	{
		int remain = (count - index);
		remain = remain > col ? col : remain;
		for (int i = 0; i < remain; ++i)
		{
			sprintf(buf, "%02x ", (unsigned char)data[i + index]);
			text += buf;
		}
		for (int i = remain; i < col; ++i) {

			text += "   ";
		}
		for (int i = 0; i < remain; ++i)
		{
			sprintf(buf, "%c", getPrintChar(data[i + index]));
			text += buf;
		}
		text += "\n";
	}
	return text;
}


std::string getRandomAddrByHostName(const std::string &hostname) {
	std::vector<std::string> ips = getAddrsByHostName(hostname);
	if (ips.empty()) return "";
	size_t size = ips.size();
	size_t index = rand() % size;
	return ips[index];
	
}


std::vector<std::string> getAddrsByHostName(const std::string &hostname) {
	std::vector<std::string> rets;


		char    **pptr;
		struct hostent *hptr;
		hptr = gethostbyname(hostname.c_str());
		if (hptr == 0)
		{
			
			return rets;
		}

		
		
		switch (hptr->h_addrtype)
		{
		case AF_INET:

			pptr = hptr->h_addr_list;
			for (; *pptr != NULL; pptr++) {
				unsigned char *val = (unsigned char *)*pptr;
				char ipaddress[32];
				sprintf_s(ipaddress, 32, "%d.%d.%d.%d", val[0], val[1], val[2], val[3]);
				rets.push_back(ipaddress);
		}
			
			break;
		default:
			
			break;
		}


	return rets;
}
char getPrintChar(char val)
{
	if (val >= 32 && val < 128)
	{
		return val;
	}
	return '.';
}