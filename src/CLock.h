#pragma once

class CLock {
public:
	CLock();
	~CLock();
public:

	void Lock();
	void UnLock();
private:
	CRITICAL_SECTION criticalSection;
};

class LockHelper {
public:
	LockHelper(CLock &locker);
	~LockHelper();
private:
	CLock &m_locker;

};