// Length of the ordering table, i.e. the range Z coordinates can have, 0-15 in
// this case. Larger values will allow for more granularity with depth (useful
// when drawing a complex 3D scene) at the expense of RAM usage and performance.
//#define OT_LENGTH 1024
#define OT_LENGTH 2048

// Size of the buffer GPU commands and primitives are written to. If the program
// crashes due to too many primitives being drawn, increase this value.
//#define BUFFER_LENGTH 8192
#define BUFFER_LENGTH 16384

#define SCREEN_XRES 320
#define SCREEN_YRES 240

#define CENTERX			SCREEN_XRES>>1
#define CENTERY			SCREEN_YRES>>1

/* Framebuffer/display list class */
typedef struct {
	DISPENV disp_env;
	DRAWENV draw_env;

	uint32_t ot[OT_LENGTH];
	uint8_t  buffer[BUFFER_LENGTH];
} RenderBuffer;

typedef struct {
	RenderBuffer buffers[2];
	uint8_t      *next_packet;
	int          active_buffer;
} RenderContext;

void setup_context(RenderContext *ctx, int w, int h, int r, int g, int b);
void flip_buffers(RenderContext *ctx);
void *new_primitive(RenderContext *ctx, int z, size_t size);
void draw_text(RenderContext *ctx, int x, int y, int z, const char *text);