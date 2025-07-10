#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <psxgpu.h>
#include <psxgte.h>
#include <inline_c.h>
#include "disp.h"
#include "modplayer.h"
#include "3dStuff.h"

SVECTOR trappe1_verts[] = {
	{ 0, 0, 0, 0 },
	{ 10, 0, 0, 0 },
	{ 10, 0, 20, 0 },
	{ 0, 0, 20,0 }
};

/* Cube vertex indices */
INDEX trappe1_indices[] = {
	{ 0, 1, 3,0},
	{ 1, 2, 3,0},
	{ 1, 0, 3,0},
	{ 2, 1, 3,0}
};

static SVECTOR trappe1_norms[] = {
	{ 0, 0, ONE, 0 },
	{ 0, 0, ONE, 0 },
	{ 0, 0, -ONE, 0 },
	{ 0, 0, -ONE, 0 }
};

void init_scene07(){
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
	
	nb_faces = sizeof(trappe1_indices) / sizeof(trappe1_indices[0]);
	curent_color = 1;
	
}

void make_trappeg(RenderContext *ctx, MATRIX mtx, MATRIX lmtx, VECTOR pos ,SVECTOR rot, int p){
	
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
	
	for(i=0; i<nb_faces;i++){
		POLY_F3 *pol4;
		
		/* Load the first 3 vertices of a quad to the GTE */
		gte_ldv3( 
			&trappe1_verts[trappe1_indices[i].v0], 
			&trappe1_verts[trappe1_indices[i].v1], 
			&trappe1_verts[trappe1_indices[i].v2] );
			
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
		
		pol4 = (POLY_F3 *) new_primitive(ctx, p, sizeof(POLY_F3));
		
		/* Initialize a quad primitive */
		//setPolyFT4( pol4 );
		setPolyF3(pol4);
		
		/* Set the projected vertices to the primitive */
		gte_stsxy0( &pol4->x0 );
		gte_stsxy1( &pol4->x1 );
		gte_stsxy2( &pol4->x2 );
		
		
		/* Load primitive color even though gte_ncs() doesn't use it. */
		/* This is so the GTE will output a color result with the */
		/* correct primitive code. */
		gte_ldrgb( &pol4->r0 );
		
		/* Load the face normal */
		gte_ldv0( &trappe1_norms[i] );
		
		/* Normal Color Single */
		gte_ncs();
		
		/* Store result to the primitive */
		gte_strgb( &pol4->r0 );
		
	}
}

void make_xwing7(RenderContext *ctx, MATRIX mtx, MATRIX lmtx, VECTOR pos ,SVECTOR rot, int p){
	
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

void make_littleso6(RenderContext *ctx, int x,int y, int p){
	TILE *tile = (TILE *) new_primitive(ctx, p, sizeof(TILE));

	setTile(tile);
	setXY0 (tile, x, y);
	setWH  (tile, 2, 8);
	setRGB0(tile, 255, 0, 0);
}

void scene07_play(RenderContext ctx){


	int j, p;
	
	int time = 0;
	int rot_trappes = 0;
	int tunel_pos = 100;
		
	int rot_xwing = 1536;
	
	const int midle = SCREEN_XRES/2;
	
	int so6_x[] = {midle,midle-10,midle+3,midle-5,midle+10,midle-2};
	int so6_y[] = {-30,-100,-10,-50,-140,-46};
	
	setup_context(&ctx, SCREEN_XRES, SCREEN_YRES, 10, 10, 10);

	init_scene07();
	
	SVECTOR	rot = { 0 };			/* Rotation vector for Rotmatrix */
	SVECTOR	rot_g = { 0 };
	VECTOR	pos = { 0, 0, 100 };
	MATRIX	mtx,lmtx;				/* Rotation matrices for geometry and lighting */
	
	
	/* Main loop */
	while( time <1024 ) {
		time++;

		
		light_mtx.m[0][0]=0;
		light_mtx.m[0][1]=0;
		light_mtx.m[0][2]=4096;
		
		pos.vx = 0;
		pos.vy = -40 +isin(time*16)/512;
		pos.vz = 100;

		rot.vx = 0;
		if(time<=256){
			rot_xwing += 2;
		}
			
		rot.vy = rot_xwing;
		rot.vz = 2048;
		gte_SetColorMatrix( &color_mtx_2 );
		make_xwing7(&ctx, mtx, lmtx, pos, rot, p);
		
		if(time>256){
			pos.vy = -39 +isin(time*16)/512;
			pos.vx = -10;
			if(rot_trappes<256){
				rot.vz = 2048 - rot_trappes *4;
			} else{
				rot.vz = 1024;
			}
			
			make_trappeg(&ctx, mtx, lmtx, pos, rot, p);
			
			pos.vx = 10;
			if(rot_trappes<256){
				rot.vz = rot_trappes *4;
				rot_trappes++;
			}else{
				rot.vz = 1024;
			}
			
			make_trappeg(&ctx, mtx, lmtx, pos, rot, p);
		
		}
		
		if(rot_trappes==256){
			for(j=0;j<6;j++){
				if(so6_y[j]==0){
					so6_y[j]=50;
				}
				if(so6_y[j]>0){
					make_littleso6(&ctx, so6_x[j],so6_y[j], j);
				}
				so6_y[j]+=2;
				if(so6_y[j]>SCREEN_XRES){
					so6_y[j]=0;
				}
			}
		}
		
				
		//FntPrint(-1, "time: %02d\n", time);
		//FntFlush(-1);
		flip_buffers(&ctx);
		checkMusic();
		
	}


}