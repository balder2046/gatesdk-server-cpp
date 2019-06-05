#include "windows.h"

#include "CLock.h"
CLock::CLock() {
	::InitializeCriticalSection(&criticalSection);
}
void CLock::Lock() {
	::EnterCriticalSection(&criticalSection);
}
void CLock::UnLock() {
	::LeaveCriticalSection(&criticalSection);
}
CLock::~CLock() {
	::DeleteCriticalSection(&criticalSection);
}

LockHelper::LockHelper(CLock &locker) :m_locker(locker) {
	m_locker.Lock();
}
LockHelper::~LockHelper() {
	m_locker.UnLock();
}