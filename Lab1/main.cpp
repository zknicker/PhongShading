/* ------------------------------------------------------------
 * Lab 3 - Custom .OBJ and Shaders!
 * CSE 5542
 * Zach Knickerbocker
 *
 * This program draws each of the 3 primitives (cube, cylinder,
 * and sphere), as well as a .OBJ gathered from the web (a
 * pumpkin). The FFP is entirely abandoned in favor of shaders,
 * which use the Phong lighting technique to light the objects
 * in the scene.
 *
 * (q) quit the program.
 * (w) toggle wireframes.
 * (left mouse button) hold and drag to pan the camera.
 * (right mouse button) hold and drag to zoom.
 * ------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <stack> 
#include <math.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <GL/glew.h> // includes gl.h
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "shader_utils.h"
#include "obj_utils.h"
#include "primitives.h"

using namespace glm;
using namespace std;

/* Global transform control. */
#define XFORM_NONE 0 
#define XFORM_ROTATE 1
#define XFORM_SCALE 2 
int xform_mode = 0; 

int press_x, press_y; 
int release_x, release_y; 

/* Camera vars. */
float camera_angle_x = 90.0;
float camera_angle_y = 0.0;
double camera_scale = 5.0;

/* The .OBJ that is imported and drawn. */
vector<Vertex> obj;
GLuint obj_buffer;

/* The scene struct that describes the scene being drawn. */
Scene scene;

/* Wireframe mode flag. */
bool wireframe_mode = false;

/* Initializes the desired model's (.obj) VBO and prepares it to be drawn.
 * -------------------------------------------------------------------------- */
void initObj(string obj_path) {
	// Read our .obj file
	ifstream ifile(obj_path);
	obj = LoadOBJ(ifile);

	// Load it into a VBO
	glGenBuffers(1, &obj_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, obj_buffer);
	glBufferData(GL_ARRAY_BUFFER, obj.size() * sizeof(Vertex), &obj[0], GL_STATIC_DRAW);
}

/* Draws the desired model (.obj).
 * -------------------------------------------------------------------------- */
void drawObj() {
	// Matrices
	scene.model = glm::scale(scene.model, vec3(0.1, 0.1, 0.1));
	update_matrices(&scene);

	// Shader Vars
	glUniform3f(scene.ambientMatId, 0.2, 0, 0);
	glUniform3f(scene.diffuseMatId, 0.7, 0, 0);
	glUniform3f(scene.specularMatId, 0.3, 0.3, 0.3);
	glUniform1f(scene.specularPowerId, 5);
	
	// Drawing
    glBindBuffer(GL_ARRAY_BUFFER, obj_buffer);
    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // Vertices
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16); // Normals

    glDrawArrays(GL_TRIANGLES, 0, obj.size());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

/* Main animation function. Draws the screen.
 * -------------------------------------------------------------------------- */
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(scene.program);

	// Camera.
	vec3 position = vec3(cos(camera_angle_x) * camera_scale, 0, sin(camera_angle_x) * camera_scale);
	vec3 center = vec3(0, 0, 0);
	vec3 up = vec3(0, 1, 0);

    // Matrices.
	scene.projection = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	scene.view = glm::lookAt(position, center, up);
    scene.model = glm::mat4(1.0);

	// Update the light.
	scene.light_anim += 0.0001;
    scene.light_pos = glm::vec3(sin(scene.light_anim) * 6.0, 0, 3);
    glUniform3f(scene.lightId, scene.light_pos.x, scene.light_pos.y, scene.light_pos.z);
	glUniform3f(scene.lightMatId, 1.0, 1.0, 1.0);

	if (wireframe_mode) glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	else glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    // Draw objects.
	stack<mat4> model_stack;
	model_stack.push(scene.model);
	
	scene.model = glm::translate(scene.model, vec3(-0.75, 0.0, 0.0));
	drawCube(&scene);
	scene.model = glm::translate(scene.model, vec3(-1.5, 0.0, 0.0));
	drawSphere(&scene);

	scene.model = model_stack.top();

	scene.model = glm::translate(scene.model, vec3(0.75, 0.0, 0.0));
	drawCylinder(&scene);
	scene.model = glm::translate(scene.model, vec3(1.5, -0.5, 0.0));
	drawObj();

    glutSwapBuffers();
}

/* Responses to mouse click events.
 *    - code credited to Dr.Shen's examples at his website.
 *      http://www.cse.ohio-state.edu/~hwshen/5542/Site/Slides.html
 * -------------------------------------------------------------------------- */
void mouseController(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		press_x = x; press_y = y; 
		if (button == GLUT_LEFT_BUTTON) {
			xform_mode = XFORM_ROTATE; 
		
		} else if (button == GLUT_RIGHT_BUTTON) {
			xform_mode = XFORM_SCALE; 
		}
	
	} else if (state == GLUT_UP) {
		xform_mode = XFORM_NONE; 
	}
}

/* Responses to mouse movement events.
 *    - code credited to Dr.Shen's examples at his website.
 *      http://www.cse.ohio-state.edu/~hwshen/5542/Site/Slides.html
 * -------------------------------------------------------------------------- */
void motionController(int x, int y) {
    if (xform_mode == XFORM_ROTATE) {
		camera_angle_x += (x - press_x)/60.0; 
		if (camera_angle_x > 180) camera_angle_x -= 360; 
		else if (camera_angle_x <-180) camera_angle_x += 360;
		press_x = x;

    } else if (xform_mode == XFORM_SCALE){
		float old_size = camera_scale;
		camera_scale *= (1+ (y - press_y)/60.0); 
		if (camera_scale <0) camera_scale = old_size; 
		press_y = y;
	} 
}

/* Responses to keyboard key down events.
 * -------------------------------------------------------------------------- */
void keyboardController(unsigned char key, int x, int y)
{
	if (key == 'q') {
		exit(1);
	}

	if (key == 'w') {
		wireframe_mode = !wireframe_mode;
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600,600);
	glutCreateWindow("Lab 3: Custom .OBJ and Shaders!");

	cout << "Lab 3: Custom .OBJ and Shaders!\nCSE 5542\nZach Knickerbocker\n\n";
	cout << "----------------------------------------\n";
	checkGraphics();
	cout << "\n----------------------------------------\n";
	cout << "Initializing application.\n";

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(mouseController); 
	glutMotionFunc(motionController);
	glutKeyboardFunc(keyboardController);

	glewInit();
	if (!glewIsSupported("GL_VERSION_3_0")) {
		cout << "Could not detect OpenGL 3.0. This could mean trouble.\n";
	}

	cout << "Loading the cube, cylinder, and sphere primitives.\n";
	initPrimitives();

	// Get the obj from the command line param.
	string obj_path;
	if(argc == 2) {
		cout << "Found command line param for the object to use.\n";
		cout << "Note that the object is scaled to 10% of its size.\n";
		cout << "Loading specified object...\n";
		obj_path = argv[1];
	} else {
		cout << "Could not find command line param for the object to use.\n";
		cout << "Loading the default: pumpkin.obj (887kb)\n";
		obj_path = "pumpkin.obj";
	}
	initObj(obj_path);

	// Setup the shaders.
	cout << "Initializing shaders.\n";
	scene.program = setupShaders("phong");

    // Save uniform locations for quick access later.
    scene.viewId = glGetUniformLocation(scene.program, "view");
    scene.modelId = glGetUniformLocation(scene.program, "model");
    scene.projectionId = glGetUniformLocation(scene.program, "projection");
    scene.normalId = glGetUniformLocation(scene.program, "normal");
    scene.lightId = glGetUniformLocation(scene.program, "light_world");
	scene.lightMatId = glGetUniformLocation(scene.program, "light_mat");
	scene.ambientMatId = glGetUniformLocation(scene.program, "ambient_mat");
	scene.diffuseMatId = glGetUniformLocation(scene.program, "diffuse_mat");
	scene.specularMatId = glGetUniformLocation(scene.program, "specular_mat");
	scene.specularPowerId = glGetUniformLocation(scene.program, "specular_power");
		
	cout << "\n----------------------------------------\n";
	cout << "(q) to quit.\n(w) to toggle wireframes.\n";
	cout << "(left mouse button) rotate camera.\n";
	cout << "(right mouse button) zoom camera in/out.\n";
	glutMainLoop();

	return 0;
}