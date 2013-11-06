
// Phong lighting vertex shader. Created using inspiration and code from:
//   * http://www.cse.ohio-state.edu/~hwshen/5542/Site/Slides_files/shading_glsl.pdf
//   * http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
//
// Note: *_model refers to model space, *_eye refers to camera/eye space, etc.

layout(location = 0) in vec3 vertex_model;
layout(location = 1) in vec3 normal_model;

out vec3 normal_eye;
out vec3 eyedir_eye;
out vec3 lightdir_eye;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform mat3 normal;
uniform vec3 light_world;

void main(){
	// Vertex position in clip space.
	gl_Position =  projection * view * model * vec4(vertex_model, 1);
    	
	// Eye space vector from vertex to camera.
	vec3 vertex_eye = (view * model * vec4(vertex_model, 1)).xyz;
	eyedir_eye = vec3(0,0,0) - vertex_eye;

	// Eye space vector from vertex to light.
	vec3 light_eye = (view * vec4(light_world, 1)).xyz;
	lightdir_eye = light_eye + eyedir_eye;
	
	// Eye space normal vector of the vertex.
    // Note: no adjustments made for non-uniform scaling.
	normal_eye = (normal * vec3(normal_model)).xyz;
}