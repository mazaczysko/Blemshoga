#ifndef ENT_H
#define ENT_H

#include "tile.h"

extern int ent_init( const char *path );
extern struct tile ***entmove( struct tile ***eptr, int dx, int dy );
extern void spawn( const char *name, int x, int y );
extern void entai( );
extern void ent_destroy( );


extern int entckhostile( struct tile *a, struct tile *b );
extern int entattack( struct tile *a, struct tile *b );

#endif
