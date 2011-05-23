#ifndef __DECODER_H__
#define __DECODER_H__

#include <stdint.h>
#include "Wrapper.h"
#include "playerdef.h"

typedef struct
{
	void *pOpaqueSource;
	AVFormatContext *pFmtCtx;
	AVCodecContext  *pCdcCtx;
	int videostream, width, height, fps;
	void *pParent;
	int (*fpRead)( void *opaque,     uint8_t *buffer, int size );
	int64_t (*fpSeek)( void *opaque, int64_t offset,  int whence );
	int (*fpWrite) ( void *opaque,   uint8_t *buffer, int pos );
	void (*__fpCallback)( void *pFrame, void *pPlayer );
}Decoder;

void decoder_reg_read( Decoder *obj, int (*fpRead)( void* opaque, uint8_t* buffer, int size ) );
void decoder_reg_seek( Decoder *obj, int64_t (*fpSeek)( void* opaque, int64_t offset, int whence ));

Decoder* decoder_new();
int decoder_init( Decoder *pDecoder );
int decoder_load_info( Decoder *pDecoder );
void decoder_release( Decoder *pDecoder );
int decoder_start( void *pDdr );

AVFrame* decoder_allot_yuv420Image( int width, int height );
void decoder_scale_image( AVFrame *pSrc, AVFrame *pFrame, Decoder *pDecoder );

#endif