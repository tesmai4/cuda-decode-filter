#ifndef STD_HEADER_H_
#define STD_HEADER_H_

#pragma warning(disable:4819)

#include <windows.h>
#include <InitGuid.h>
#include <streams.h>
#include <dvdmedia.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <d3d9.h>
#include <nvcuvid.h>
#include <cudad3d9.h>

#define SMART_CACHE_SIZE	1024*1024// testing! repeat pattern 1024 1024
#define MIN_WORK_SIZE		10*1024
#define DECODER_BUFFER_SIZE 256*1024 // tesing ! 65536

#define STORE_RGB24		1
#define STORE_IYUY		2
#define ERROR_FLUSH     200

#define MAX_FRM_CNT             16
#define DISPLAY_DELAY           1  // Attempt to decode up to 4 frames ahead of display // why  4 will wrong? 1 is ok
#define USE_ASYNC_COPY          1
#define USE_FLOATING_CONTEXTS   1   // Use floating contexts


// Specify H.264 GUID manually
DEFINE_GUID(MEDIATYPE_H264, 0x34363248, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);

// CUDA Decoder Filter GUID
// {BFA29735-1A9B-46f4-B2CE-0EF7ABEF2F7C}
DEFINE_GUID(CLSID_CudaDecodeFilter, 0xbfa29735, 0x1a9b, 0x46f4, 0xb2, 0xce, 0xe, 0xf7, 0xab, 0xef, 0x2f, 0x7c);



#endif