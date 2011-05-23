#include "player.h"
#include "queue.h"

void __player_notify( void *frame, void *pPlayer );

Player* player_new()
{
	Player *pPlayer = (Player*) av_mallocz( sizeof( Player ));
	pPlayer->pQMutex = SDL_CreateMutex();
	return pPlayer;
}

void player_set_decoder( Player *pPlayer, Decoder *pDecoder )
{
	pDecoder->pParent = pPlayer;
	pPlayer->pDecoder = pDecoder;
	pPlayer->pDecoder->__fpCallback = __player_notify;
}

void player_set_display( Player *pPlayer, Display *pDisplay )
{
	pPlayer->pDisplay = pDisplay;
}

int player_run( void *pPr )
{
	Player *pPlayer = (Player*) pPr;
	//display_set_dimensions( pPlayer->pDecoder->width, pPlayer->pDecoder->height, pPlayer->pDisplay );
	//SDL_CreateThread( decoder_start, pPlayer->pDecoder );
	decoder_start( pPlayer->pDecoder );

	return SUCCESS;
}

int player_play( void *pPlr )
{
	SDL_Delay( 1000 );
	printf("Started playing");
	Player *pPlayer = (Player*) pPlr;
	SDL_Overlay *pFrame = NULL;
	int i = 0;
	do
	{
		SDL_LockMutex( pPlayer->pQMutex );
		Item *pCurr = pPlayer->pHead;
		if( pCurr == NULL || pCurr->pOverlay == NULL )
			goto unlock_mutex;

		display_overlay( pCurr->pOverlay, pPlayer->pDisplay );
		pPlayer->pHead = pCurr->pNext;

		FREE( pCurr );
unlock_mutex:
		SDL_UnlockMutex( pPlayer->pQMutex );
		
	} while( true );
	return SUCCESS;
}

void __player_notify( void *frame, void *pPlayer )
{
	struct Player *pPlr = (Player*) pPlayer;
	SDL_LockMutex( pPlr->pQMutex );
	Item* pParent = NULL;

	if( !pPlr->pDisplay->initialized )
	{
		display_set_dimensions( pPlr->pDecoder->width, pPlr->pDecoder->height, pPlr->pDisplay );
		pPlr->pDisplay->initialized = true;
	}

	if( pPlr->pHead == NULL )
	{
		pPlr->pHead = (struct Item*) malloc( sizeof( Item ));
		pPlr->pIndex = pPlr->pHead;
	}
	pPlr->framecount++;
	AVFrame *pFrame = decoder_allot_yuv420Image( pPlr->pDecoder->width, pPlr->pDecoder->height);

	if( display_image_grab( pFrame, pPlr->pDisplay ) == ERROR )
	{
		goto free;
	}
	decoder_scale_image( (AVFrame*) frame, pFrame, pPlr->pDecoder );
	pPlr->pIndex = queue_put( pPlr->pDisplay->pOverlay, pPlr->pIndex );
	
free:
	FREE_AV( pFrame );
	SDL_UnlockMutex( pPlr->pQMutex );
}
