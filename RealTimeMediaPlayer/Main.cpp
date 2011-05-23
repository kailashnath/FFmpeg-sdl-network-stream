#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Wrapper.h"
#include "decoder.h"
#include "player.h"
#include "queue.h"

#ifdef _DEBUG

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)

#define new DEBUG_NEW

#endif

int read_fp( void* opaque, uint8_t* buffer, int size )
{
	size_t read = fread( buffer, 1, size, (FILE*) opaque );
    return read;
}

int64_t seek_fp( void* opaque, int64_t offset, int whence )
{
    fseek( (FILE*) opaque, offset, whence );
	//rewind( (FILE*) opaque );
	return 0;
}

Decoder* init_decoder()
{
	Decoder* obj = decoder_new();
	char *filename = FILENAME;
	FILE *pF = fopen( filename, "rb" );
	obj->pOpaqueSource = (void*) pF;

	decoder_reg_read( obj, read_fp );
	decoder_reg_seek( obj, seek_fp );

	decoder_init( obj );
	decoder_load_info( obj );
	return obj;
}

Display* init_display()
{
	Display *pDisplay = display_new();
	display_init( pDisplay );
	return pDisplay;
}

int main( int argc, char** argv )
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	Player *pPlayer = player_new();
	player_set_decoder( pPlayer, init_decoder() );
	player_set_display( pPlayer, init_display() );
	SDL_CreateThread( player_play, pPlayer );
	player_run( pPlayer );
	_CrtDumpMemoryLeaks();
	getchar();
	return 0;
}