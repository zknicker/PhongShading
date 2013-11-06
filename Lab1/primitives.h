#ifndef PRIMITIVES_H
#define PRIMITIVES_H

/* ------------------------------------------------------------
 * Defines methods for initializing and drawing primitives,
 * including a cube, cylinder, and sphere. When the shapes are
 * initialized, they are given a set of randomized colors for
 * the vertices. When drawing them, you can choose to supply
 * a single color which will override that generated random
 * array of colors. The randomly generated colors look pretty
 * cool though!
 *
 * The cylinder does not draw its top and bottom, but 
 * Dr. Shen clarified that to be acceptable.
 *
 * The size of all shapes can be configured via the variables
 * below. Those variables are then final for the duration of
 * the program. Dr. Shen also mentioned this to be acceptable.
 *
 * A cube of size 1,1,1 is of world size 1,1,1. The same logic
 * applies to the cylinder and sphere.
 * ------------------------------------------------------------ */

#include "shader_utils.h"

#define PI 3.141592653589793

#define CUBE_SIZE 1
#define CUBE_VERTICES 8
#define CUBE_INDICES (CUBE_VERTICES * 3)

#define CYLINDER_SLICES 32
#define CYLINDER_STACKS 6
#define CYLINDER_HEIGHT	1
#define CYLINDER_TOPRAD 0.5
#define CYLINDER_BOTRAD 1.0
#define CYLINDER_VERTICES (CYLINDER_STACKS * CYLINDER_SLICES)
#define CYLINDER_INDICES (CYLINDER_STACKS * (CYLINDER_SLICES) * 6)

#define SPHERE_SLICES 24
#define SPHERE_STACKS 24
#define SPHERE_RADIUS 0.5
#define SPHERE_VERTICES (SPHERE_SLICES * SPHERE_STACKS)
#define SPHERE_INDICES (SPHERE_VERTICES * 4)

using namespace glm;

typedef struct {
	float point[4]; 
	float normal[4];
	float color[4]; 
	vec2 texcoord;
} Vertex; 

typedef struct {
	float red;
	float green;
	float blue;
	float alpha;
} Color;

extern Vertex cube[CUBE_VERTICES];
extern GLushort cube_indices[CUBE_INDICES];
extern Vertex cylinder[CYLINDER_VERTICES];
extern GLushort cylinder_indices[CYLINDER_INDICES];
extern Vertex sphere[SPHERE_VERTICES];
extern GLushort sphere_indices[SPHERE_INDICES];

// VBOs for cube, cylinder, and sphere primitives.
extern GLuint primitives[6];

/* Initialize VBOs for primitives.
 * -------------------------------------------------------------------------- */
void initPrimitives();

/* Initalize Cube - initializes cube vertices, colors, and indices.
 * -------------------------------------------------------------------------- */
void initCube();

/* Draw Cube - draws a cube.
 * -------------------------------------------------------------------------- */
void drawCube(Scene *scene);

/* Draw Cube - draws a cube with some specified solid color.
 * -------------------------------------------------------------------------- */
void drawCube(Scene *scene, Color *color);

/* Initalize Cylinder - initializes cylinder vertices, colors, and indices.
 * -------------------------------------------------------------------------- */
void initCylinder();

/* Draw Cylinder - draws a cylinder.
 * -------------------------------------------------------------------------- */
void drawCylinder(Scene *scene);

/* Draw Cylinder - draws a cylinder with some specified solid color.
 * -------------------------------------------------------------------------- */
void drawCylinder(Scene *scene, Color *color);

/* Initalize Sphere - initializes sphere vertices, colors, and indices.
 * -------------------------------------------------------------------------- */
void initSphere();

/* Draw Sphere - draws a sphere.
 * -------------------------------------------------------------------------- */
void drawSphere(Scene *scene);

/* Draw Sphere - draws a sphere with some specified solid color.
 * -------------------------------------------------------------------------- */
void drawSphere(Scene *scene, Color *color);

#endif