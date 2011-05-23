#include "display.h"
#include "queue.h"

Display* display_new()
{
	Display *pDisplay = (Display*) av_mallocz( sizeof( Display ));
	return pDisplay;
}

void display_init( Display *pDisplay )
{
	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER );
}

void display_set_dimensions( int width, int height, Display *pDisplay )
{
	pDisplay->width = width;
	pDisplay->height = height;

	pDisplay->pSurface = SDL_SetVideoMode( width, height, 0, SDL_SWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF );
}

int display_image_grab( void *pImage, Display *pDisplay )
{
	pDisplay->pOverlay = SDL_CreateYUVOverlay( pDisplay->width, pDisplay->height, SDL_YV12_OVERLAY, pDisplay->pSurface );
	if( pDisplay->pOverlay == NULL )
		return ERROR;
	SDL_LockYUVOverlay( pDisplay->pOverlay );
	AVFrame *pFrame = (AVFrame*) pImage;
	pFrame->data[0] = pDisplay->pOverlay->pixels[0];
	pFrame->data[1] = pDisplay->pOverlay->pixels[2];
	pFrame->data[2] = pDisplay->pOverlay->pixels[1];

	pFrame->linesize[0] = pDisplay->pOverlay->pitches[0];
	pFrame->linesize[1] = pDisplay->pOverlay->pitches[2];
	pFrame->linesize[2] = pDisplay->pOverlay->pitches[1];

	return SUCCESS;
}

int display_image_draw( Display *pDisplay )
{
	SDL_Rect rect;
	rect.x = rect.y = 0;
	rect.w = pDisplay->width;
	rect.h = pDisplay->height;
	
	SDL_DisplayYUVOverlay( pDisplay->pOverlay, &rect );
	SDL_Delay( 16 );
	return SUCCESS;
}

void display_overlay( SDL_Overlay *pBmp, Display *pDisplay )
{
	SDL_Rect rect;
	rect.x = rect.y = 0;
	rect.w = pBmp->w;
	rect.h = pBmp->h;

	SDL_DisplayYUVOverlay( pBmp, &rect );
	SDL_UnlockYUVOverlay( pBmp );
	SDL_FreeYUVOverlay( pBmp );
	pBmp = NULL;
}

int display_image_release( Display *pDisplay )
{
	SDL_UnlockYUVOverlay( pDisplay->pOverlay );

	return SUCCESS;
}