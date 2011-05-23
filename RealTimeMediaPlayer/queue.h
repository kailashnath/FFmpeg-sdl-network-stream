#ifndef __QUEUE_H_
#define __QUEUE_H_

#include "player.h"
#include "Wrapper.h"

struct Item
{
	SDL_Overlay *pOverlay;
	struct Item *pNext;
};

Item* queue_put( SDL_Overlay *pBmp, struct Item* pParent );
SDL_Overlay* queue_next( struct Player *pPlayer );
#endif