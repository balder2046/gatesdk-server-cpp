#include "Windows.h"
#include "WaitEvent.h"
CWaitEvent::CWaitEvent(const char *name) {
	mHandle = 0;
	
	// not inherited by child process
	// auto reset 
	// initial state is false
	mHandle = ::CreateEventA(0, FALSE, FALSE, name);
	if (name != 0) {
		m_eventname = name;
	}


}
CWaitEvent::~CWaitEvent() {
	CloseHandle(mHandle);
}

int CWaitEvent::wait(long timeoutIndwMilliseconds) {
	DWORD nret = ::WaitForSingleObject(mHandle, timeoutIndwMilliseconds);
	if (nret == WAIT_OBJECT_0) {
		return 0;
	}
	else if(nret == WAIT_TIMEOUT) {
		return 1;
	}
	
	else if (nret == WAIT_FAILED) {
		return 2;
	}
	return 0;
}
void CWaitEvent::signal() {
	::SetEvent(mHandle);
}