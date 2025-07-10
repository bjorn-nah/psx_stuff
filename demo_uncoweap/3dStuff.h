#include <psxgte.h>

/* For easier handling of vertex indices */
typedef struct {
	short v0,v1,v2,color;
} INDEX;

typedef struct {
	short v0,v1,v2,v3,color;
} INDEX4;

extern MATRIX color_mtx;
extern MATRIX color_mtx_2;

extern MATRIX light_mtx;

extern int curent_color;
extern int nb_faces;

extern SVECTOR xwing_verts[];
extern INDEX xwing_indices[];
extern SVECTOR xwing_norms[];
extern int xwing_faces;

extern SVECTOR so6_verts[];
extern INDEX so6_indices[];
extern SVECTOR so6_norms[];
extern int so6_faces;