#ifndef __WRAPPER_H_
#define __WRAPPER_H_

#using <mscorlib.dll>
#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#define from( x ) [DllImport("lib\\"x)]

#pragma comment(lib, "SDL.lib")

using namespace System;
using namespace System::Runtime::InteropServices;


from("avcodec")
void avcodec_init(void);

from("avcodec")
void avcodec_register_all();

from("avcodec")
AVCodec *avcodec_find_decoder(enum CodecID id);

from("avcodec")
int avcodec_open(AVCodecContext *avctx, AVCodec *codec);

from("avcodec")
int avcodec_decode_video2(AVCodecContext *avctx, AVFrame *picture,
                         int *got_picture_ptr,
                         AVPacket *avpkt);

/**
 * Allocate an AVCodecContext and set its fields to default values.  The
 * resulting struct can be deallocated by simply calling av_free().
 *
 * @return An AVCodecContext filled with default values or NULL on failure.
 * @see avcodec_get_context_defaults
 */
from("avcodec")
AVCodecContext *avcodec_alloc_context(void);

from("avcodec")
int avcodec_close(AVCodecContext *avctx);

from("avcodec")
void av_close_input_file(AVFormatContext *s);

from("avcodec")
void av_free_packet(AVPacket *pkt);

/**
 * Calculate the size in bytes that a picture of the given width and height
 * would occupy if stored in the given picture format.
 * Note that this returns the size of a compact representation as generated
 * by avpicture_layout, which can be smaller than the size required for e.g.
 * avpicture_fill.
 *
 * @param pix_fmt the given picture format
 * @param width the width of the image
 * @param height the height of the image
 * @return Image data size in bytes or -1 on error (e.g. too large dimensions).
 */
from("avcodec")
int avpicture_get_size(enum PixelFormat pix_fmt, int width, int height);

/**
 * Fill in the AVPicture fields.
 * The fields of the given AVPicture are filled in by using the 'ptr' address
 * which points to the image data buffer. Depending on the specified picture
 * format, one or multiple image data pointers and line sizes will be set.
 * If a planar format is specified, several pointers will be set pointing to
 * the different picture planes and the line sizes of the different planes
 * will be stored in the lines_sizes array.
 * Call with ptr == NULL to get the required size for the ptr buffer.
 *
 * @param picture AVPicture whose fields are to be filled in
 * @param ptr Buffer which will contain or contains the actual image data
 * @param pix_fmt The format in which the picture data is stored.
 * @param width the width of the image in pixels
 * @param height the height of the image in pixels
 * @return size of the image data in bytes
 */
from("avcodec")
int avpicture_fill(AVPicture *picture, uint8_t *ptr,
                   enum PixelFormat pix_fmt, int width, int height);

/**
 * Allocate an AVFrame and set its fields to default values.  The resulting
 * struct can be deallocated by simply calling av_free().
 *
 * @return An AVFrame filled with default values or NULL on failure.
 * @see avcodec_get_frame_defaults
 */
from("avcodec")
AVFrame *avcodec_alloc_frame(void);

/**
 * Free a picture previously allocated by avpicture_alloc().
 *
 * @param picture the AVPicture to be freed
 */
from("avcodec")
void avpicture_free(AVPicture *picture);
/**
 * Allocate an AVFormatContext.
 * Can be freed with av_free() but do not forget to free everything you
 * explicitly allocated as well!
 */
from("avformat")
AVFormatContext *avformat_alloc_context(void);

from("avformat")
ByteIOContext *av_alloc_put_byte(
                  unsigned char *buffer,
                  int buffer_size,
                  int write_flag,
                  void *opaque,
                  int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),
                  int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),
                  int64_t (*seek)(void *opaque, int64_t offset, int whence));

from("avformat")
void av_register_all();

from("avformat")
int av_open_input_file(AVFormatContext **ic_ptr, const char *filename,
                       AVInputFormat *fmt,
                       int buf_size,
                       AVFormatParameters *ap);

from("avformat")
int av_find_stream_info(AVFormatContext *ic);

from("avformat")
int av_read_frame(AVFormatContext *s, AVPacket *pkt);

/**
 * Find AVInputFormat based on the short name of the input format.
 */
from("avformat")
AVInputFormat *av_find_input_format(const char *short_name);

/**
 * Allocate all the structures needed to read an input stream.
 *        This does not open the needed codecs for decoding the stream[s].
 */
from("avformat")
int av_open_input_stream(AVFormatContext **ic_ptr,
                         ByteIOContext *pb, const char *filename,
                         AVInputFormat *fmt, AVFormatParameters *ap);

/**
 * Guess the file format.
 *
 * @param is_opened Whether the file is already opened; determines whether
 *                  demuxers with or without AVFMT_NOFILE are probed.
 */
from("avformat")
AVInputFormat *av_probe_input_format(AVProbeData *pd, int is_opened);

from("avformat")
void dump_format(AVFormatContext *ic,
                 int index,
                 const char *url,
                 int is_output);


from("swscale")
SwsContext *sws_getContext(int srcW, int srcH, enum PixelFormat srcFormat,
                           int dstW, int dstH, enum PixelFormat dstFormat,
                           int flags, SwsFilter *srcFilter,
                           SwsFilter *dstFilter, const double *param);

from("swscale")
int sws_scale(struct SwsContext *context, const uint8_t* const srcSlice[], const int srcStride[],
              int srcSliceY, int srcSliceH, uint8_t* const dst[], const int dstStride[]);

from("swscale")
void sws_freeContext(struct SwsContext *swsContext);

/**
 * Frees the swscaler context swsContext.
 * If swsContext is NULL, then does nothing.
 */
from("swscale")
void sws_freeContext(struct SwsContext *swsContext);

/**
 * Free a memory block which has been allocated with av_malloc(z)() or
 * av_realloc().
 * @param ptr Pointer to the memory block which should be freed.
 * @note ptr = NULL is explicitly allowed.
 * @note It is recommended that you use av_freep() instead.
 * @see av_freep()
 */
from("avutil")
void av_free(void *ptr);

/**
 * Allocate a block of size bytes with alignment suitable for all
 * memory accesses (including vectors if available on the CPU) and
 * zero all the bytes of the block.
 * @param size Size in bytes for the memory block to be allocated.
 * @return Pointer to the allocated block, NULL if it cannot be allocated.
 * @see av_malloc()
 */
from("avutil")
void *av_mallocz(unsigned int size) av_malloc_attrib av_alloc_size(1);

#endif