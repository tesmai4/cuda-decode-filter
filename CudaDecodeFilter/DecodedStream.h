#ifndef DECODED_STREAM_H_
#define DECODED_STREAM_H_

#include "StdHeader.h"


class CudaDecodeFilter;
class MediaController;

class DecodedStream : public CSourceStream
{
	friend class CudaDecodeFilter;
	friend class CudaH264Decoder;

public:
	DecodedStream(TCHAR * inObjectName, HRESULT * outResult, CudaDecodeFilter * inFilter);
	virtual ~DecodedStream();

	void SetController(MediaController * inController);
	// override to expose IMediaPosition
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP BeginFlush(void);
	STDMETHODIMP EndFlush(void);
	STDMETHODIMP EndOfStream(void);

	HRESULT StopThreadSafely(void);
	HRESULT RunThreadSafely(void);

protected:
	// Override this to provide the worker thread a means
	// of processing a buffer
	virtual HRESULT FillBuffer(IMediaSample *pSample); // PURE
	virtual HRESULT DecideBufferSize(IMemAllocator * pAllocator, ALLOCATOR_PROPERTIES *pprop); // PURE

	virtual HRESULT CheckMediaType(const CMediaType *mtOut);
	virtual HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);
	// IQualityControl
	STDMETHODIMP Notify(IBaseFilter * pSender, Quality q);

	HRESULT CompleteConnect(IPin *pReceivePin);
	STDMETHODIMP QueryId(LPWSTR * Id);

	virtual HRESULT DoBufferProcessingLoop(void);
	virtual HRESULT OnThreadStartPlay(void);
	virtual HRESULT OnThreadDestroy(void);

	HRESULT DeliverCurrentPicture(IMediaSample * pSample);

	// Media type
public:
	CMediaType& CurrentMediaType(void) { return m_mt; }

private:
	CudaDecodeFilter *		mDecodeFilter;
	MediaController *		mMpegController;

	// implement IMediaPosition by passing upstream
	IUnknown *   mPosition;
	BOOL         mFlushing;
	ULONG        mSamplesSent;
	CCritSec     mDataAccess;

	BOOL		 m_EOS_Flag;

};

#endif