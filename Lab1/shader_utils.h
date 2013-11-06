#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

/* --------------------------------------------------------------------------
 *  Defines functions to make using shaders really easy.
 * -------------------------------------------------------------------------- */

typedef struct {
	// The shader program in use.
	GLuint program;

	// Graphics pipeline matrices.
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;

	// Shader vars.
	GLuint lightId;
	GLuint modelId;
	GLuint viewId;
	GLuint projectionId;
	GLuint normalId;

	GLuint ambientMatId;
	GLuint diffuseMatId;
	GLuint specularMatId;
	GLuint specularPowerId;
	GLuint lightMatId;

	// Lighting
	glm::vec3 light_pos;
	float light_anim;
} Scene;

/* Prints useful information about the client's graphics card.
 * -------------------------------------------------------------------------- */
void checkGraphics();

/* Loads and compiles shaders into a program object which is then returned.
 * 
 * Note that the naming convention requires that your shaders use file names
 * <shader_name>.vert and <shader_name>.frag
 * -------------------------------------------------------------------------- */
GLuint setupShaders(char *shader_name);

/* Calculates the normal matrix, and sends it as well as the model, view, and
 * projection matrices to the shader. Since this is the same in just about every
 * object, this function just saves on duplicated code.
 * -------------------------------------------------------------------------- */
void update_matrices(Scene *scene);

#endif