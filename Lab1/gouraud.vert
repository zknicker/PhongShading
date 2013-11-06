
attribute vec3 VertexPosition; 
attribute vec3 VertexNormal; 
varying vec3 LightIntensity; 

struct LightInfo { 
 vec4 Position; // light pos in eye space 
 vec3 La; 
 vec3 Ld; 
 vec3 Ls; 
}; 

uniform LightInfo Light; 
struct MaterialInfo{ 
 vec3 Ka; 
 vec3 Kd; 
 vec3 Ks; 
 float shininess; 
}; 

uniform MaterialInfo Material; 
uniform mat4 ModelViewMatrix; 
uniform mat3 NormalMatrix; 
uniform mat4 ProjectionMatrix; 
uniform mat4 MVP; // modelview projection 

void main () 
{ 
    vec3 = tnorm = normalize(NormalMatrix * VertexNormal); 
vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition, 1.0)); 
vec3 s = normalize(vec3(LightPosition – eyeCoords)); 
vec3 v = normalize(-eyeCoords.xyz); 
vec3 r = reflect(-s, tnorm); 
vec3 ambient = Light.La * Material.Ka;
flaot sDotN = max (dot(x,tnorm),0.0); 
vec3 diffuse = Light.Ld * Material.Kd*sDotN; 
Vec3 spec = vec3(0.0); 
If (sDotN > 0.0) 
    spec = Light.Ls * Material.Ks * 
    pow(max(dot(r,v),0.0),Material.Shininess); 
    
LightIntensity = ambient + diffuse + spec; 
gl_Position = MVP * vec4(VertexPosition,1.0);