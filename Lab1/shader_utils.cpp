#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "shader_utils.h"

using namespace std;
using namespace glm;

/* Prints useful information about the client's graphics card.
 *
 * Code courtesy of:
 *   - http://www.cse.ohio-state.edu/~hwshen/5542/Site/Slides_files/shaderSetup.C
 * -------------------------------------------------------------------------- */
void checkGraphics() 
{
  const GLubyte *renderer = glGetString(GL_RENDERER); 
  const GLubyte *vendor = glGetString(GL_VENDOR); 
  const GLubyte *version = glGetString(GL_VERSION); 
  const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION); 
  GLint major, minor; 
  glGetIntegerv(GL_MAJOR_VERSION, &major); 
  glGetIntegerv(GL_MINOR_VERSION, &minor); 
  
  printf("Checking graphics capability...\n"); 
  printf("GL Vendor: %s \n", vendor); 
  printf("GL Renderer: %s \n", renderer); 
  printf("GL version: %s\n", version); 
  printf("GL version: %d.%d\n", major, minor); 
  printf("GLSL version: %s\n", glslVersion); 

  // check for shader support
  if (glewGetExtension("GL_ARB_fragment_shader")      != GL_TRUE ||
      glewGetExtension("GL_ARB_vertex_shader")        != GL_TRUE ||
      glewGetExtension("GL_ARB_shader_objects")       != GL_TRUE ||
      glewGetExtension("GL_ARB_shading_language_100") != GL_TRUE)
    {
      fprintf(stderr, "Driver does not support OpenGL Shading Language.\n");
      exit(1);
    }
  else fprintf(stderr, "GLSL supported and ready to go.\n");
}



/* Retrieves the source code of a file.
 * -------------------------------------------------------------------------- */
char* getFileContents(char *file_name)
{
    FILE *fp;
	char *file_contents = NULL;

	fp = fopen(file_name,"rt");
	if (fp != NULL) {

		// Get the size of the shader file.
		fseek(fp, 0, SEEK_END);
		int count = ftell(fp);
		rewind(fp);

		// Read in the shader file's contents.
		file_contents = (char*)malloc(sizeof(char) * (count + 1));
		count = fread(file_contents, sizeof(char), count, fp);
		file_contents[count] = '\0';
		
		fclose(fp);
	} else {
		printf("Could not open the shader file %s.\n", file_name);
	}

	return file_contents;
}

/* Prints errors that result from shader compilation issues.
 *
 * Code courtesy of:
 *   - http://www.cse.ohio-state.edu/~hwshen/5542/Site/Slides_files/shaderSetup.C
 * -------------------------------------------------------------------------- */
void printShaderErrors(GLuint shader_object) {
	GLint result; 
	glGetShaderiv(shader_object, GL_COMPILE_STATUS, &result); 
	if (result == GL_FALSE) {
		GLint logLen; 
		glGetShaderiv(shader_object, GL_INFO_LOG_LENGTH, &logLen); 
		if (logLen > 0) {
			char *log = (char*) malloc(logLen); 
			GLsizei written; 
			glGetShaderInfoLog(shader_object, logLen, &written, log); 
			printf("%s", log); 
			free(log); 
		}
	}
}

/* Loads and compiles shaders into a program object which is then returned.
 * 
 * Note that the naming convention requires that your shaders use file names
 * <shader_name>.vert and <shader_name>.frag
 *
 * Inspiration from:
 *   - http://www.cse.ohio-state.edu/~hwshen/5542/Site/Slides_files/shaderSetup.C
 *   - http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
 * -------------------------------------------------------------------------- */
GLuint setupShaders(char *shader_name) {

	GLuint vertex_shader_object = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_object = glCreateShader(GL_FRAGMENT_SHADER);

	// Load the vertex shader.
	char vert_shader_name[100];
    strcpy(vert_shader_name, shader_name);
	strcat(vert_shader_name, ".vert");
	char *vertext_shader = getFileContents(vert_shader_name);

	// Load the fragment shader.
	char frag_shader_name[100];
    strcpy(frag_shader_name, shader_name);
	strcat(frag_shader_name, ".frag");
	char *fragment_shader = getFileContents(frag_shader_name);

	glShaderSource(vertex_shader_object, 1, (const GLchar**)&vertext_shader, NULL);
	glShaderSource(fragment_shader_object, 1, (const GLchar**)&fragment_shader, NULL);

	free(vertext_shader); free(fragment_shader);

	glCompileShader(vertex_shader_object);
	printShaderErrors(vertex_shader_object);

	glCompileShader(fragment_shader_object);
	printShaderErrors(fragment_shader_object);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader_object);
	glAttachShader(program, fragment_shader_object);

	glLinkProgram(program);

	return program;
}

/* Calculates the normal matrix, and sends it as well as the model, view, and
 * projection matrices to the shader. Since this is the same in just about every
 * object, this function just saves on duplicated code.
 * -------------------------------------------------------------------------- */
void update_matrices(Scene *scene) {
	mat3 normal_matrix = mat3(transpose(inverse(scene->view * scene->model)));

    glUniformMatrix4fv(scene->modelId, 1, GL_FALSE, &scene->model[0][0]);
    glUniformMatrix4fv(scene->viewId, 1, GL_FALSE, &scene->view[0][0]);
    glUniformMatrix4fv(scene->projectionId, 1, GL_FALSE, &scene->projection[0][0]);
	glUniformMatrix3fv(scene->normalId, 1, GL_FALSE, &normal_matrix[0][0]);
}