#ifndef OBJ_UTILS_H
#define OBJ_UTILS_H

/* --------------------------------------------------------------------------
 *  Defines functions to make loading OBJs really easy.
 * -------------------------------------------------------------------------- */

#include "primitives.h"

using namespace std;
using namespace glm;

/* Loads an OBJ into a Vertex vector such that it can be drawn.
 *
 * Lovingly taken (and then modified) from:
 *   - http://stackoverflow.com/questions/14887012/object-loader-in-opengl
 * -------------------------------------------------------------------------- */
vector<Vertex> LoadOBJ(istream& in);

#endif