#ifndef SMART_CACHE_H_
#define SMART_CACHE_H_

#include "StdHeader.h"

class SmartCache
{
public:

	SmartCache();
	virtual ~SmartCache();

	long Init(void);
	void Release(void);

	long Receive(unsigned char * inData, long inLength);
	long FetchData(BYTE * outBuffer, ULONG inLength);

	void BeginFlush(void);
	void EndFlush(void);
	long GetAvailable(void);

	BOOL CheckInputWaiting(void);
	BOOL CheckOutputWaiting(void);

	void SetCacheChecking(void);
	void ResetCacheChecking(void);

protected:

	void MakeSpace(void);
	long HasEnoughSpace(long inNeedSize);
	long HasEnoughData(long inNeedSize);

private:

	CRITICAL_SECTION singleAccess;

	unsigned char * gInputCache ;
	long gCacheSize;
	long gMinWorkSize;
	long gReadingOffset;
	long gWritingOffset;
	BOOL gIsFlushing;

	BOOL inputWaiting;
	BOOL outputWaiting;
	BOOL cacheChecking;
	int  waitingCounter;
};


#endif