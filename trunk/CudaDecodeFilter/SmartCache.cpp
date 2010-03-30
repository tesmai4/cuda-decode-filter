#include "SmartCache.h"

long SmartCache::Init(void)
{
	gInputCache = (unsigned char *)malloc(gCacheSize);
	gReadingOffset = 0;
	gWritingOffset = 0;
	inputWaiting  = FALSE;
	outputWaiting = FALSE;
	cacheChecking = TRUE;  // When checking, maybe return to the cache header
	// Critical section to access smart cache
	InitializeCriticalSection(&singleAccess); 
	return (gInputCache != NULL);
}

void SmartCache::Release(void)
{
	DeleteCriticalSection(&singleAccess);
	if (gInputCache)
	{
		free(gInputCache);
		gInputCache = NULL;
	}
}

// Blocking receive...
long SmartCache::Receive(unsigned char * inData, long inLength)
{	
	while (!gIsFlushing && !HasEnoughSpace(inLength))
	{
		inputWaiting = TRUE;
		MakeSpace();
		Sleep(2);
	}
	inputWaiting = FALSE;

	if (!gIsFlushing && HasEnoughSpace(inLength))
	{
		EnterCriticalSection(&singleAccess); // Enter
		memcpy(gInputCache + gWritingOffset, inData, inLength);
		gWritingOffset += inLength;
		LeaveCriticalSection(&singleAccess); // Leave
		return 1;
	}
	return 0;
}

// Blocking read...
long SmartCache::FetchData(BYTE * outBuffer, ULONG inLength)
{
	if (inLength <= 0)
		return 0;

	while (!gIsFlushing && !HasEnoughData(inLength))
	{
		outputWaiting = TRUE;
		Sleep(1);
	}
	outputWaiting = FALSE;

	if (!gIsFlushing && HasEnoughData(inLength))
	{
		EnterCriticalSection(&singleAccess); // Enter
		memcpy(outBuffer, gInputCache + gReadingOffset, inLength);
		gReadingOffset += inLength;
		LeaveCriticalSection(&singleAccess); // Leave
		return inLength;
	}
	return 0;
}

long SmartCache::GetAvailable(void)
{
	return (gWritingOffset - gReadingOffset);
}

// Determine whether enough space to hold coming mpeg data
long SmartCache::HasEnoughSpace(long inNeedSize)
{
	return (inNeedSize <= gCacheSize - gWritingOffset);
}

// Determine data in smart cache at this moment is enough to decode
long SmartCache::HasEnoughData(long inNeedSize)
{
	return (inNeedSize <= gWritingOffset - gReadingOffset);
}

void SmartCache::MakeSpace(void)
{
	long workingSize = gWritingOffset - gReadingOffset;
	// When cache checking, don't drop any data
	if (!cacheChecking && workingSize < gMinWorkSize)
	{
		EnterCriticalSection(&singleAccess); // Enter
		memmove(gInputCache, gInputCache + gReadingOffset, workingSize);
		gReadingOffset = 0;
		gWritingOffset = workingSize;
		LeaveCriticalSection(&singleAccess); // Leave
	}
}

void SmartCache::BeginFlush(void)
{
	gIsFlushing = TRUE;
	waitingCounter = 0;
	while (inputWaiting && waitingCounter < 15)  // Make sure NOT block in receiving or reading
	{
		waitingCounter++;
		Sleep(1);
	}
	waitingCounter = 0;
	while (outputWaiting && waitingCounter < 15)
	{
		waitingCounter++;
		Sleep(1);
	}
	//	Sleep(10);
	EnterCriticalSection(&singleAccess); // Enter
	gReadingOffset = 0;
	gWritingOffset = 0;
	LeaveCriticalSection(&singleAccess); // Leave
}

void SmartCache::EndFlush(void)
{
	gIsFlushing = FALSE;
}

BOOL SmartCache::CheckInputWaiting(void)
{
	return inputWaiting;
}

BOOL SmartCache::CheckOutputWaiting(void)
{
	return outputWaiting;
}

// We can reuse the data having been read out
void SmartCache::SetCacheChecking(void)
{
	cacheChecking = TRUE;
}

void SmartCache::ResetCacheChecking(void)
{
	cacheChecking = FALSE;
	//EnterCriticalSection(&singleAccess); // Enter
	//gReadingOffset = 0; // ????? testing!!
	//LeaveCriticalSection(&singleAccess); // Leave
}

SmartCache::SmartCache() : 	gInputCache(NULL), 
							gCacheSize(SMART_CACHE_SIZE), 
							gMinWorkSize(MIN_WORK_SIZE), 
							gReadingOffset(0), 
							gWritingOffset(0), 
							gIsFlushing(FALSE),
							inputWaiting(FALSE),
							outputWaiting(FALSE),
							cacheChecking(TRUE),
							waitingCounter(0)
{
	this->Init();
}

SmartCache::~SmartCache()
{
	this->Release();
}