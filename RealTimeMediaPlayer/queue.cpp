#include "queue.h"

Item* queue_put( SDL_Overlay *pBmp, struct Item* pParent )
{
	Item *pItem = (Item*) malloc( sizeof( Item ));
	pItem->pNext = NULL;
	pItem->pOverlay = NULL;

	pParent->pOverlay = pBmp;
	pParent->pNext = pItem;
	return pItem;
}

SDL_Overlay* queue_next( Player *pPlayer )
{
	SDL_Overlay *pFrame = pPlayer->pHead->pOverlay;

	Item *pNext = pPlayer->pHead->pNext;
	free( pPlayer->pHead );
	pNext->pNext = NULL;
	pNext->pOverlay = NULL;

	if( pPlayer->pHead->pNext != NULL )
	{
		pPlayer->pHead = pNext;
	}

	return pFrame;
}