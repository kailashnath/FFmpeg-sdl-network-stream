#ifndef __PLAYERDEF_H__
#define __PLAYERDEF_H__
//#define FILENAME "J:\\samples\\video_ts.mpg"
#define FILENAME "C:\\Documents and Settings\\kkavalak\\My Documents\\My Videos\\AP11.wmv"
#define SUCCESS 0
#define ERROR -1

#define INBUF_SIZE 1024
#define LARGE_BUFFER_SIZE 4096

#define error(msg) printf("ERROR : " msg "\n");
#define info(msg) printf("INFO : " msg "\n");
#define FREE(ptr) free(ptr); ptr = NULL
#define FREE_AV(ptr) av_free(ptr); ptr = NULL
#define _QUEUE_ 1
#endif