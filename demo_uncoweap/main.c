/*
 * PSn00bSDK basic graphics example
 * (C) 2020-2023 Lameguy64, spicyjpeg - MPL licensed
 *
 * A comprehensive "advanced hello world" example showing how to set up the
 * screen with double buffering, draw basic graphics (a bouncing square) and use
 * PSn00bSDK's debug font API to quickly print some text, all while following
 * best practices. This is not necessarily the simplest hello world example and
 * may look daunting at first glance, but it is a good starting point for more
 * complex programs.
 *
 * In order to avoid cluttering the program with global variables (as many Sony
 * SDK examples and other PSn00bSDK examples written before this one do) two
 * custom structures are employed:
 *
 * - a RenderBuffer structure containing the DISPENV and DRAWENV objects that
 *   represent the location of the framebuffer in VRAM, as well as the ordering
 *   table (OT) used to sort GPU commands/primitives by their Z index and the
 *   actual buffer commands will be written to;
 * - a RenderContext structure holding two RenderBuffer instances plus some
 *   variables to keep track of which buffer is currently being drawn and how
 *   much of its primitive buffer has been filled up so far.
 *
 * A C++ version of this example is also available (see examples/hellocpp).
 */

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <psxgpu.h>
#include "disp.h"
#include "hardware/counters.h"
#include "modplayer.h"
#include "scene03.h"
#include "scene04.h"
#include "scene05.h"
#include "scene06.h"
#include "scene07.h"
#include "scene08.h"

extern const uint8_t _binary_dark_saucisse_start[];


void setup_context(RenderContext *ctx, int w, int h, int r, int g, int b);
void flip_buffers(RenderContext *ctx);
void *new_primitive(RenderContext *ctx, int z, size_t size);
void draw_text(RenderContext *ctx, int x, int y, int z, const char *text);

/* Main */

int main(int argc, const char **argv) {
	// Initialize the GPU and load the default font texture provided by
	// PSn00bSDK at (960, 0) in VRAM.
	ResetGraph(0);
	FntLoad(960, 0);

	SetVideoMode(MODE_PAL);
	
	COUNTERS[1].mode = 0x0100;
	MOD_Load((struct MODFileFormat*)_binary_dark_saucisse_start);
	
	s_nextCounter = COUNTERS[1].value + MOD_hblanks;

	// Set up our rendering context.
	RenderContext ctx;
	
	scene03_play(ctx);
	scene05_play(ctx);
	scene07_play(ctx);
	scene04_play(ctx);
	scene06_play(ctx);
	scene08_play(ctx);

	return 0;
}
