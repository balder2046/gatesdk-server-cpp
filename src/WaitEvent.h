#pragma once
#include <string>
class CWaitEvent {
public:
	CWaitEvent(const char *name);
	~CWaitEvent();
public:

	virtual int wait(long timeoutIndwMilliseconds);
	virtual void signal();
private:
	void *mHandle;
	std::string m_eventname;
};
