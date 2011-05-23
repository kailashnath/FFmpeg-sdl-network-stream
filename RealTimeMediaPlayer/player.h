#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <SDL.h>
#include "decoder.h"
#include "display.h"
#include "queue.h"

struct Player
{
	Decoder *pDecoder;
	Display *pDisplay;
	int framecount;
	struct Item *pHead;
	struct Item *pIndex;
	SDL_mutex *pQMutex;
};

Player* player_new();
void player_set_decoder( Player *pPlayer, Decoder *pDecoder );
void player_set_display( Player *pPlayer, Display *pDisplay );
int player_run( void *pPr );
int player_play( void *pPlr );
#endif