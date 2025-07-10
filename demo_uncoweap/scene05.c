#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <psxgpu.h>
#include <psxgte.h>
#include <inline_c.h>
#include "disp.h"
#include "modplayer.h"
#include "3dStuff.h"
#include "scene05.h"

SVECTOR tunel_verts[] = {
	{ -100, -100, -100, 0 },
	{  100, -100, -100, 0 },
	{ -100,  100, -100, 0 },
	{  100,  100, -100, 0 },
	{  100, -100,  100, 0 },
	{ -100, -100,  100, 0 },
	{  100,  100,  100, 0 },
	{ -100,  100,  100, 0 }

};

/* Cube vertex indices */
INDEX4 tunel_indices[] = {
	{ 1, 0, 3, 2 ,0},
	{ 5, 4, 7, 6 ,0},
	{ 3, 2, 6, 7 ,0},

};

static SVECTOR tunel_norms[] = {
	{ 0, 0, -ONE, 0 },
	{ 0, 0, ONE, 0 },
	{ 0, -ONE, 0, 0 },

};


void init_scene05(){
	TIM_IMAGE tim;
	
	color_mtx.m[0][0]=ONE * 3/4;
	color_mtx.m[1][0]=ONE * 3/4;
	color_mtx.m[2][0]=ONE * 3/4;
	
	color_mtx_2.m[0][0]=ONE * 2/4;
	color_mtx_2.m[1][0]=ONE * 2/4;
	color_mtx_2.m[2][0]=ONE * 2/4;


	//FntLoad(960, 0);
	//FntOpen(4, 200, 312, 32, 2, 256);
	
	/* Initialize the GTE */
	InitGeom();
	
	/* Set GTE offset (recommended method  of centering) */
	gte_SetGeomOffset( CENTERX, CENTERY );
	
	/* Set screen depth (basically FOV control, W/2 works best) */
	gte_SetGeomScreen( CENTERX );
	
	/* Set light ambient color and light color matrix */
	gte_SetBackColor( 63, 63, 63 );
	gte_SetColorMatrix( &color_mtx );
	
	nb_faces = sizeof(tunel_indices) / sizeof(tunel_indices[0]);
	curent_color = 1;
	
}

void make_tunel(RenderContext *ctx, MATRIX mtx, MATRIX lmtx, VECTOR pos ,SVECTOR rot, int p){
	
	int i;
	/* Set rotation and translation to the matrix */
	RotMatrix( &rot, &mtx );
	TransMatrix( &mtx, &pos );
	
	/* Multiply light matrix by rotation matrix so light source */
	/* won't appear relative to the model's rotation */
	MulMatrix0( &light_mtx, &mtx, &lmtx );
	
	/* Set rotation and translation matrix */
	gte_SetRotMatrix( &mtx );
	gte_SetTransMatrix( &mtx );
	
	/* Set light matrix */
	gte_SetLightMatrix( &lmtx );
	
	/* Make the cube SPEEN */
	//rot.vx += 16;
	//rot.vz += 16;
	//rot.vy += 16;
	
	//gte_SetColorMatrix( &color_mtx );
	//curent_color=1;
	
	for(i=0; i<nb_faces;i++){
		POLY_F4 *pol4;
		
		/* Load the first 3 vertices of a quad to the GTE */
		gte_ldv3( 
			&tunel_verts[tunel_indices[i].v0], 
			&tunel_verts[tunel_indices[i].v1], 
			&tunel_verts[tunel_indices[i].v2] );
			
		/* Rotation, Translation and Perspective Triple */
		gte_rtpt();
		
		/* Compute normal clip for backface culling */
		gte_nclip();
		
		/* Get result*/
		gte_stopz( &p );
		
		/* Skip this face if backfaced */
		if( p < 0 )
			continue;
		
		/* Calculate average Z for depth sorting */
		gte_avsz4();
		gte_stotz( &p );
		
		/* Skip if clipping off */
		/* (the shift right operator is to scale the depth precision) */
		if( (p>>2) > OT_LENGTH )
			continue;
		
		pol4 = (POLY_F4 *) new_primitive(ctx, p, sizeof(POLY_F4));
		
		/* Initialize a quad primitive */
		//setPolyFT4( pol4 );
		setPolyF4(pol4);
		
		/* Set the projected vertices to the primitive */
		gte_stsxy0( &pol4->x0 );
		gte_stsxy1( &pol4->x1 );
		gte_stsxy2( &pol4->x2 );
		
		/* Compute the last vertex and set the result */
		gte_ldv0( &tunel_verts[tunel_indices[i].v3] );
		gte_rtps();
		gte_stsxy( &pol4->x3 );
		
		/* Load primitive color even though gte_ncs() doesn't use it. */
		/* This is so the GTE will output a color result with the */
		/* correct primitive code. */
		gte_ldrgb( &pol4->r0 );
		
		/* Load the face normal */
		gte_ldv0( &tunel_norms[i] );
		
		/* Normal Color Single */
		gte_ncs();
		
		/* Store result to the primitive */
		gte_strgb( &pol4->r0 );
		
	}
}

void make_xwing5(RenderContext *ctx, MATRIX mtx, MATRIX lmtx, VECTOR pos ,SVECTOR rot, int p){
	
	int i;
	/* Set rotation and translation to the matrix */
	RotMatrix( &rot, &mtx );
	TransMatrix( &mtx, &pos );
	
	/* Multiply light matrix by rotation matrix so light source */
	/* won't appear relative to the model's rotation */
	MulMatrix0( &light_mtx, &mtx, &lmtx );
	
	/* Set rotation and translation matrix */
	gte_SetRotMatrix( &mtx );
	gte_SetTransMatrix( &mtx );
	
	/* Set light matrix */
	gte_SetLightMatrix( &lmtx );
	
	//gte_SetColorMatrix( &color_mtx );
	//curent_color=1;
	
	for(i=0; i<xwing_faces;i++){
		POLY_F3 *pol3;
		
		/* Load the first 3 vertices of a quad to the GTE */
		gte_ldv3( 
			&xwing_verts[xwing_indices[i].v0], 
			&xwing_verts[xwing_indices[i].v1], 
			&xwing_verts[xwing_indices[i].v2] );
			
		/* Rotation, Translation and Perspective Triple */
		gte_rtpt();
		
		/* Compute normal clip for backface culling */
		gte_nclip();
		
		/* Get result*/
		gte_stopz( &p );
		
		/* Skip this face if backfaced */
		if( p < 0 )
			continue;
		
		/* Calculate average Z for depth sorting */
		gte_avsz4();
		gte_stotz( &p );
		
		/* Skip if clipping off */
		/* (the shift right operator is to scale the depth precision) */
		if( (p>>2) > OT_LENGTH )
			continue;
		
		pol3 = (POLY_F3 *) new_primitive(ctx, p, sizeof(POLY_F3));
		
		/* Initialize a quad primitive */
		//setPolyFT4( pol4 );
		setPolyF3(pol3);
		
		/* Set the projected vertices to the primitive */
		gte_stsxy0( &pol3->x0 );
		gte_stsxy1( &pol3->x1 );
		gte_stsxy2( &pol3->x2 );
		
		
		/* Load primitive color even though gte_ncs() doesn't use it. */
		/* This is so the GTE will output a color result with the */
		/* correct primitive code. */
		gte_ldrgb( &pol3->r0 );
		
		/* Load the face normal */
		gte_ldv0( &xwing_norms[i] );
		
		/* Normal Color Single */
		gte_ncs();
		
		/* Store result to the primitive */
		gte_strgb( &pol3->r0 );
		
	}
}

void scene05_play(RenderContext ctx){
	int j, p;
	
	int time = 0;
	int tunel_pos = 100;
	
	const int rot_step = 48;
	const int distance = 2000;
	
	const int tile_large = SCREEN_XRES/2 - 15;
	const int tile_high = 85;

	setup_context(&ctx, SCREEN_XRES, SCREEN_YRES, 10, 10, 10);

	init_scene05();
	
	SVECTOR	rot = { 0 };			/* Rotation vector for Rotmatrix */
	VECTOR	pos = { 0, 0, 100 };
	MATRIX	mtx,lmtx;				/* Rotation matrices for geometry and lighting */
	
	//rot.vy = 1024;
	
	/* Main loop */
	while( time<1024 ) {
		time++;
				
		TILE *tile1 = (TILE *) new_primitive(&ctx, OT_LENGTH-1, sizeof(TILE));
		
		setTile(tile1);
		setXY0 (tile1, 0, tile_high);
		setWH  (tile1, tile_large, SCREEN_YRES-tile_high);
		setRGB0(tile1, 128, 128, 128);
		
		TILE *tile2 = (TILE *) new_primitive(&ctx, OT_LENGTH-2, sizeof(TILE));
		
		setTile(tile2);
		setXY0 (tile2, SCREEN_XRES - tile_large, tile_high);
		setWH  (tile2, tile_large, SCREEN_YRES-tile_high);
		setRGB0(tile2, 128, 128, 128);
		
		light_mtx.m[0][0]=-2048;
		light_mtx.m[0][1]=-2048;
		light_mtx.m[0][2]=-2048;
		
		rot.vy = 1024;
		rot.vz = 0;
		pos.vx = 0;
		
		tunel_pos = (time*4)%(rot_step*2);
		
		for (j=0;j<10;j++){
			if(j%2==0){
				gte_SetColorMatrix( &color_mtx );
			}else{
				gte_SetColorMatrix( &color_mtx_2 );
			}
			pos.vy = distance - 200 - (icos(tunel_pos+j*rot_step)*distance)/4096;
			pos.vz = distance - 400 -  (isin(tunel_pos+j*rot_step)*distance)/4096;
			rot.vx = tunel_pos+j*rot_step;
			make_tunel(&ctx, mtx, lmtx, pos, rot, p);
		}
		//FntPrint(-1, "tunel_pos: %02d\n", tunel_pos);
		
		
		light_mtx.m[0][0]=0;
		light_mtx.m[0][1]=0;
		light_mtx.m[0][2]=4096;
		
		pos.vx = isin(time*32)/64;
		pos.vy = - 80 - time /4;
		pos.vz = time;
		rot.vx = 256;
		rot.vy = 2048;
		rot.vz = 2048+isin(time*32)/6;
		
		gte_SetColorMatrix( &color_mtx_2 );
		make_xwing5(&ctx, mtx, lmtx, pos, rot, p);
		
		//FntPrint(-1, "time: %02d\n", time);
		//FntFlush(-1);

		flip_buffers(&ctx);
		checkMusic();
		

		
	}


}