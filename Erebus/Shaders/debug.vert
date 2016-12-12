#version 420 core

in vec3 pos ;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(){
	gl_Position = projectionMatrix * viewMatrix * vec4(pos, 1.0);
}