#include "countlock.h"

CountLock::CountLock(int bufferSize) :
	buffer(bufferSize),
	semaphore(bufferSize)
{
	Q_ASSERT(bufferSize > 0);
}

void CountLock::addLock()
{
	semaphore.acquire();
}

void CountLock::tryAddLock(int timeout)
{
	semaphore.tryAcquire(1, timeout);
}

void CountLock::releaseLock()
{
	semaphore.release();
}

void CountLock::fullLock()
{
	semaphore.acquire(buffer);
}

bool CountLock::tryfullLock(int timeout)
{
	return semaphore.tryAcquire(buffer, timeout);
}

void CountLock::releaseFullLock()
{
	Q_ASSERT(semaphore.available() == 0);
	semaphore.release(buffer);
}

void CountLock::setBufferSize(int bufferSize)
{
	Q_ASSERT(bufferSize > 0);
	Q_ASSERT(semaphore.available() == 0);
	buffer = bufferSize;
}

CountLocker::CountLocker(CountLock &countLock, bool unlockOnly) :
	CountLocker(&countLock, unlockOnly)
{}

CountLocker::CountLocker(CountLock *countLock, bool unlockOnly) :
	lock(countLock)
{
	if(!unlockOnly)
		lock->addLock();
}

CountLocker::~CountLocker()
{
	lock->releaseLock();
}
