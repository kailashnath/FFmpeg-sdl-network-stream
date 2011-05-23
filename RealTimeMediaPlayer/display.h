#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <SDL.h>
#include "Wrapper.h"
#include "playerdef.h"

typedef struct
{
	int width, height;
	bool initialized;
	SDL_Surface *pSurface;
	SDL_Overlay *pOverlay;

} Display;

Display* display_new();
void display_init( Display *pDisplay );
void display_set_dimensions( int width, int height, Display* pDisplay );
void display_free( Display* pDisplay );
int display_image_grab( void *pImage, Display *pDisplay );
int display_image_draw( Display *pDisplay );
int display_image_release( Display *pDisplay );
void display_overlay( SDL_Overlay *pBmp, Display *pDisplay );
#endif