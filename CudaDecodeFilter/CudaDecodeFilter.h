#ifndef CUDA_DECODE_FILTER_H_ 
#define CUDA_DECODE_FILTER_H_

#include "StdHeader.h"

class CudaDecodeInputPin;
class DecodedStream;
class MediaController;

class CudaDecodeFilter : public CSource
{
	friend class CudaDecodeInputPin;
	friend class DecodedStream;
	friend class CudaH264Decoder;

public:

	static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

private:

	CudaDecodeFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);

	virtual ~CudaDecodeFilter();

private:

	DecodedStream * OutputPin() {return (DecodedStream *) m_paStreams[0];};

public:

	DECLARE_IUNKNOWN;
	// Basic COM - used here to reveal our own interfaces
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	// you need to supply these to access the pins from the enumerator
	// and for default Stop and Pause/Run activation.
	virtual int GetPinCount();
	virtual CBasePin *GetPin(int n);
	STDMETHODIMP FindPin(LPCWSTR Id, IPin ** ppPin);

	STDMETHODIMP Stop();
	STDMETHODIMP Pause();
	HRESULT StartStreaming();
	HRESULT StopStreaming();

	// Input pin's delegating methods
	HRESULT Receive(IMediaSample *pSample);
	// if you override Receive, you may need to override these three too
	HRESULT EndOfStream(void);
	HRESULT BeginFlush(void);
	HRESULT EndFlush(void);
	HRESULT NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);

	// Output pin's delegating methods
	HRESULT CompleteConnect(PIN_DIRECTION inDirection, IPin * inReceivePin);

private:

	CudaDecodeInputPin *	m_CudaDecodeInputPin;

	CCritSec				m_csReceive;

	MediaController*		m_MediaController;

	BOOL					mIsFlushing;
	BOOL					mEOSDelivered;
	BOOL					mEOSReceived;

	// Bitmap information
	LONG               mImageWidth;
	LONG               mImageHeight;
	LONG               mOutputImageSize;
	REFERENCE_TIME     mSampleDuration;
};


#endif