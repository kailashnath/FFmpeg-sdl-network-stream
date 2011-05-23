#include "decoder.h"

/* Private function definitions */
int __load_codec_ctx( Decoder *pDecoder );
/* End private function definitions */

	void 
decoder_reg_read( Decoder* obj, int (*fpRead)( void* opaque, uint8_t* buffer, int size )) {

	obj->fpRead = fpRead;
}

	void 
decoder_reg_seek( Decoder* obj, int64_t (*fpSeek)( void* opaque, int64_t offset, int whence )) {

	obj->fpSeek = fpSeek;
}

	Decoder* 
decoder_new() {
	Decoder *pDecoder = (Decoder*) av_mallocz( sizeof( Decoder ));
	
	avcodec_init();
	avcodec_register_all();
	av_register_all();

	return pDecoder;
}

	int
decoder_init( Decoder *pDecoder ) {
	
	if( av_open_input_file( &pDecoder->pFmtCtx, FILENAME, NULL, 0, NULL ) < 0 )
		return ERROR;

	if( av_find_stream_info( pDecoder->pFmtCtx ) < 0 )
		return ERROR;
	return SUCCESS;
}

	int
decoder_load_info( Decoder *pDecoder ) {

	unsigned int i = 0;
	AVProbeData prbData;
	uint8_t *pBuffer, *pIoBuffer;
	AVInputFormat *pInputFmt;
	ByteIOContext *pIoCtx;

	prbData.filename = "DUMMY";
	prbData.buf_size = LARGE_BUFFER_SIZE;
	pBuffer = (uint8_t*) av_mallocz( LARGE_BUFFER_SIZE );
	memset( pBuffer + INBUF_SIZE, 0, FF_INPUT_BUFFER_PADDING_SIZE );

	pDecoder->fpRead( pDecoder->pOpaqueSource, pBuffer, LARGE_BUFFER_SIZE );
	prbData.buf = pBuffer;

	pInputFmt = av_probe_input_format( &prbData, 1 );

	pDecoder->fpSeek( pDecoder->pOpaqueSource, 0, SEEK_SET );

	pIoBuffer = (uint8_t*) av_mallocz( INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE );

	pIoCtx = av_alloc_put_byte( pIoBuffer, INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE,
		0, pDecoder->pOpaqueSource, pDecoder->fpRead, pDecoder->fpWrite, pDecoder->fpSeek );

	if( pIoCtx == NULL )
	{
		printf("Failed getting the byte io context \n");
		goto error;
	}

	if( av_open_input_stream( &pDecoder->pFmtCtx, pIoCtx, "DUMMY", pInputFmt, NULL ) < 0 )
	{
		error("Failed opening input stream");
		goto error;
	}

	if( __load_codec_ctx( pDecoder ) < 0 )
	{
		error("Failed loading the codec details");
		goto error;
	}

	av_free( pBuffer );
	pBuffer = NULL;
	
	return SUCCESS;

error:
	av_free( pBuffer );
	pBuffer = NULL;
	return ERROR;

}
	void
decoder_release( Decoder *pDecoder )
{
	avcodec_close( pDecoder->pCdcCtx );
	av_close_input_file( pDecoder->pFmtCtx );
	free( pDecoder );
}
	int
decoder_start( void *pDdr )
{
	AVPacket packet;
	int gotpicture;
	Decoder *pDecoder = (Decoder*) pDdr;

	AVFrame *pFrame = avcodec_alloc_frame();
	while( av_read_frame( pDecoder->pFmtCtx, &packet ) >= 0 )
	{
		if( packet.stream_index == pDecoder->videostream )
		{
			
			avcodec_decode_video2( pDecoder->pCdcCtx, pFrame, &gotpicture, &packet );
			pDecoder->width = pDecoder->pCdcCtx->width;
			pDecoder->height = pDecoder->pCdcCtx->height;
			if( gotpicture )
			{
				//info("Got picture");
				printf("PTS : %d\n", packet.pts);
				if( pDecoder->__fpCallback != NULL )
					pDecoder->__fpCallback( pFrame, pDecoder->pParent );
			}
		}
	}
	av_free( pFrame );
	return SUCCESS;
}
	AVFrame*
decoder_allot_yuv420Image( int width, int height )
{
	int imgsize;
	uint8_t *pBuffer;
	AVFrame *pDstFrame;

	imgsize = avpicture_get_size( PixelFormat::PIX_FMT_YUV420P, width, height );
	pBuffer = (uint8_t*) av_mallocz( imgsize * sizeof( uint8_t ));

	pDstFrame = avcodec_alloc_frame();
	avpicture_fill( (AVPicture*) pDstFrame, pBuffer, PixelFormat::PIX_FMT_YUV420P, width, height );
	av_free( pBuffer );
	return pDstFrame;
}

	void 
decoder_scale_image( AVFrame *pSrc, AVFrame *pFrame, Decoder *pDecoder )
{
	struct SwsContext *pSwsctx = sws_getContext( pDecoder->width, pDecoder->height, pDecoder->pCdcCtx->pix_fmt,
		pDecoder->width, pDecoder->height, PixelFormat::PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL );
	sws_scale( pSwsctx, pSrc->data, pSrc->linesize, 0, pDecoder->height, pFrame->data, pFrame->linesize );
	sws_freeContext( pSwsctx );
}

	int
__load_codec_ctx( Decoder *pDecoder )
{
	AVCodec *pCodec;
	int videostream = -1;
	unsigned int index = 0;

	for( ; index < pDecoder->pFmtCtx->nb_streams; index ++ )
	{
		if( pDecoder->pFmtCtx->streams[index]->codec->codec_type == CODEC_TYPE_VIDEO )
		{
			videostream = index;
			break;
		}
	}

	pDecoder->videostream = videostream;
	if( videostream == -1 )
	{
		error("Video stream not found");
		return ERROR;
	}
	else
	{
		pDecoder->pCdcCtx = pDecoder->pFmtCtx->streams[videostream]->codec;
		pCodec = avcodec_find_decoder( pDecoder->pCdcCtx->codec_id );
	}

	if( pCodec == NULL )
	{
		error("Unable to find codec");
		return ERROR;
	}

	if( pCodec->capabilities & CODEC_CAP_TRUNCATED )
		pDecoder->pCdcCtx->flags |= CODEC_FLAG_TRUNCATED;

	if( avcodec_open( pDecoder->pCdcCtx, pCodec ) < 0 )
	{
		error("Failed opening the codec");
		return ERROR;
	}
	return SUCCESS;
}