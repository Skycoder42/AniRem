#ifndef COUNTLOCK_H
#define COUNTLOCK_H

#include <QSemaphore>

class CountLock
{
public:
	CountLock(int bufferSize = 100000);

	void addLock();
	void tryAddLock(int timeout = 0);
	void releaseLock();

	void fullLock();
	bool tryfullLock(int timeout = 0);
	void releaseFullLock();

	void setBufferSize(int bufferSize);

private:
	int buffer;
	QSemaphore semaphore;
};

class CountLocker
{
public:
	CountLocker(CountLock &countLock, bool unlockOnly = false);
	CountLocker(CountLock *countLock, bool unlockOnly = false);
	~CountLocker();

private:
	CountLock *lock;
};

#endif // COUNTLOCK_H
