#version 420 core

in vec3 pos ;
in vec3 normal;

out vec3 norm;

uniform mat4 VPmatrix ; 

void main(){
	gl_Position = VPmatrix * vec4(pos, 1.0);
	norm = normal;
}