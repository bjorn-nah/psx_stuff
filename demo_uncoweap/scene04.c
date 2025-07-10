#include <stdint.h>
#include <stdio.h>
#include <psxgpu.h>
#include <psxgte.h>
#include <inline_c.h>
#include "disp.h"
#include "modplayer.h"
#include "3dStuff.h"

//local fontions
void init_scene04(){
	/* Set primitive pointer address */
	// new_primitive should do the job :)
	
	color_mtx.m[0][0]=ONE * 1/4;
	color_mtx.m[1][0]=0;
	color_mtx.m[2][0]=0;
	
	/* Initialize the GTE */
	InitGeom();
	
	/* Set GTE offset (recommended method  of centering) */
	gte_SetGeomOffset( CENTERX, CENTERY );
	
	/* Set screen depth (basically FOV control, W/2 works best) */
	gte_SetGeomScreen( CENTERX );
	
	/* Set light ambient color and light color matrix */
	gte_SetBackColor( 63, 63, 63 );
	gte_SetColorMatrix( &color_mtx );
	
	curent_color = 1;
}

void make_so6(RenderContext *ctx, MATRIX mtx, MATRIX lmtx, VECTOR pos ,SVECTOR rot, int p){
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
	
	//nb_faces = sizeof(so6_indices) / sizeof(so6_indices[0]);
	for(i=0; i<so6_faces;i++){
		
		/* Load the first 3 vertices of a quad to the GTE */
		gte_ldv3( 
			&so6_verts[so6_indices[i].v0], 
			&so6_verts[so6_indices[i].v1], 
			&so6_verts[so6_indices[i].v2] );
			
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
		
		POLY_F3 *pol3 = (POLY_F3 *) new_primitive(ctx, p, sizeof(POLY_F3));
		
		/* Initialize a quad primitive */
		//setPolyFT4( pol3 );
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
		gte_ldv0( &so6_norms[i] );
		
		/* Normal Color Single */
		gte_ncs();
		
		/* Store result to the primitive */
		gte_strgb( &pol3->r0 );
		

	}
	
}

void scene04_play(RenderContext ctx){
	
	int p,j;
	
	int time = 0;
	
	int y1,y2,y3;
	int vertical[] = {-200,-550,-450,-600,-300,-300};
	int horizontal[] = {-200,0,200,-100,50,-150};


	setup_context(&ctx, SCREEN_XRES, SCREEN_YRES, 128, 128, 128);

	init_scene04();
	
	SVECTOR	rot = { 0 };			/* Rotation vector for Rotmatrix */
	SVECTOR	rot2 = { 0 };
	SVECTOR	rot3 = { 0 };
	VECTOR	pos = { 0, 0, 400 };
	MATRIX	mtx,lmtx;				/* Rotation matrices for geometry and lighting */

	
	gte_SetColorMatrix( &color_mtx );
	
	/* Main loop */
	while( time<1024 ) {
		
		time++;
		
		LINE_F2 *line1 = (LINE_F2 *) new_primitive(&ctx, OT_LENGTH-1, sizeof(LINE_F2));
		setLineF2(line1);
		setXY2(line1, 0, SCREEN_YRES-((time*8)%SCREEN_YRES), SCREEN_XRES, SCREEN_YRES-((time*8)%SCREEN_YRES));
		setRGB0(line1, 10, 10, 10);
		
		LINE_F2 *line3 = (LINE_F2 *) new_primitive(&ctx, OT_LENGTH-3, sizeof(LINE_F2));
		setLineF2(line3);
		setXY2(line3, SCREEN_XRES/4, 0,SCREEN_XRES/4, SCREEN_YRES);
		setRGB0(line3, 10, 10, 10);
		
		LINE_F2 *line4 = (LINE_F2 *) new_primitive(&ctx, OT_LENGTH-4, sizeof(LINE_F2));
		setLineF2(line4);
		setXY2(line4, SCREEN_XRES*3/4, 0,SCREEN_XRES*3/4, SCREEN_YRES);
		setRGB0(line4, 10, 10, 10);
		
		rot.vx += 16;
		rot.vz += 16;
		
		rot2.vz += 16;

		rot3.vx = 8*time;
		rot3.vx += 16*time;
		

		for (j=0;j<6;j++){
			pos.vy = vertical[j];
			pos.vx = horizontal[j];
			if(j%3==0){make_so6(&ctx, mtx, lmtx, pos, rot, p);};
			if(j%3==1){make_so6(&ctx, mtx, lmtx, pos, rot2, p);};
			if(j%3==2){make_so6(&ctx, mtx, lmtx, pos, rot3, p);};
			vertical[j]+=2;
			if(vertical[j]>350){vertical[j]=-350;};
		}
		
		flip_buffers(&ctx);
		checkMusic();
		
	}

}