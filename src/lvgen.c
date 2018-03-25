#include "map.h"
#include "tile.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>

struct room
{
	int x, y;
	int w, h;
	int id;
};

struct net
{
	struct room *r1, *r2;
	float dist;
};

int rangedrandom( int min, int max )
{
	return rand( ) % ( max + 1 - min ) + min;
}

int qcmp (const void * a, const void * b)
{
	//Increasing
 	struct net* a2 = ( struct net*)a;
    struct net* b2 = ( struct net*)b;
    if( a2->dist < b2->dist ) return -1;
    else if( a2->dist == b2->dist ) return 0;
    else return 1;
}

struct room mkroom( int x, int y, int w, int h, int id )
{
	struct room room =
	{
		x, y, w, h, id,
	};

	return room;

}

int roomcoll( struct room a, struct room *b, int margin )
{
	return ( abs(a.x - b->x) < (a.w + b->w) / 2 + margin ) && ( abs(a.y - b->y) < (a.h + b->h) / 2 + margin );
}

struct room* spread( int gx, int gy, float gr, int cnt )
{
	 int i = 0, j, w, h, ok;
	 float ang;
	 struct room *rooms = calloc( cnt, sizeof( struct room ) );

	 while( i < cnt )
	 {
		 int x,y,r;
		ok = 1;
	 	ang = ((rand( ) % 10000) / 10000.0) * 2 * M_PI;
		r = gr* ((rand( ) % 10000) / 10000.0);
		x = gx+ floor( cos(ang) * r );
		y = gy+ floor( sin(ang) * r );
		w = rangedrandom( 2, 10 );
		h = rangedrandom( 2, 10 );

		for( j = 0; j < i; j++ )
			if( roomcoll( mkroom( x, y, w, h, i ), rooms + j, 5 ) )
				ok = 0;

		if( ok )
		{
			rooms[i] = mkroom( x, y, w, h, i );
			i++;
		}
	 }

	 return rooms;
}

void drawroom( struct room* rooms, int cnt )
{
	int i, j, k;

	if( rooms == NULL )
		return;

	for( i = 0; i < cnt; i++ )
	{
		for( j = floor( rooms[i].x - rooms[i].w / 2.0 ) - 1; j < floor( rooms[i].x + rooms[i].w / 2.0 ) + 1; j++ )
		{
			for( k = floor( rooms[i].y - rooms[i].h / 2.0 ) - 1; k < floor( rooms[i].y + rooms[i].h / 2.0 ) + 1; k++ )
			{
					if( j == floor( rooms[i].x - rooms[i].w / 2.0 ) - 1 || j == floor( rooms[i].x + rooms[i].w / 2.0 ) || k  == floor( rooms[i].y - rooms[i].h / 2.0 ) - 1 || k == floor( rooms[i].y + rooms[i].h / 2.0 ) )
					{
						mapputtile( j, k, MAP_LSOLID, tile( "stone wall" ) );
					}
					else
					{
						mapputtile( j, k, MAP_LFLOOR, tile( "stone floor" ) );
					}
			}
		}
	}
}

struct net* mknet( struct room *rooms, int cnt)
{
	int i, j, k, c = 0, ok;
	float dist;
	struct net *net = calloc( 1024, sizeof( struct net ) );

	for( i = 0; i < cnt; i++ )
	{
		for( j = 0; j < cnt; j++ )
		{
			ok = 1;

			if( i != j )
			{
				for( k = 0; k < c; k++ )
				{
					if( ( rooms[i].id == net[k].r1->id && rooms[j].id == net[k].r2->id ) || ( rooms[i].id == net[k].r2->id && rooms[j].id == net[k].r1->id ) )
					{
						ok = 0;
					}
				}
				if( ok )
				{
					dist = (float) sqrt( pow( rooms[i].x - rooms[j].x, 2.0) + pow( rooms[i].y - rooms[j].y, 2.0 ) );
					net[c].r1 = rooms + i;
					net[c].r2 = rooms + j;
					net[c].dist = dist;
					printf("%d-%d\n", rooms[i].id, rooms[j].id );
					c++;
				}

			}
		}
	}
	printf("\n\n");

	qsort( net, c, sizeof( struct net ), qcmp );
	for( i = 0; i < c; i++ )
		printf("%f\n", net[i].dist );

	return net;
}

struct net *spantree( struct room *rooms, struct net *net, int cnt )
{
	int i, j = 0, k, id;
	struct net *spantree = calloc( cnt*(cnt - 1)/2 - 1, sizeof( struct net ) );

	for( i = 0; i < cnt*(cnt - 1)/2; i++)
	{
		if( net[i].r1->id != net[i].r2->id )
		{
			memcpy( spantree + j, net + i, sizeof( struct net ) );
			printf( "span:%d-%d, %d, %d ", spantree[j].r1->id, spantree[j].r2->id, spantree[j].r1->x, spantree[j].r2->y );
			printf("no cycle\n");
			j++;
			id = net[i].r2->id;
			for( k = 0; k < cnt; k++ )
			{
				if( rooms[k].id == id )
				{
				 	rooms[k].id = net[i].r1->id;
			 	}

			}

		}
		printf( "step: %d\n", i );
		for( int z = 0; z < cnt*(cnt - 1)/2; z++ )
		{
			printf("%d-%d\n", net[z].r1->id, net[z].r2->id );
		}
		if( j == cnt - 1 )
		{
			printf("MSP of %d edges\n", j );
			return spantree;
		}
	}
	printf( "%d\n", j);
	return spantree;
}

int genlvl( int gx, int gy, int gr, int room_cnt )
{
	srand( time(NULL) );
	struct room *rooms = spread( gx, gy, gr, room_cnt );
	struct net *net = mknet( rooms, room_cnt );

	net = spantree( rooms, net, room_cnt);

	drawroom( rooms, room_cnt );


	srand( time(NULL) );


return 0;
}