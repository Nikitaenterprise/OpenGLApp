#version 330

in vec4 fragmentPosition;

uniform vec3 lightPosition;
uniform float farPlanes;

void main()
{
	float distance = length(fragmentPosition.xyz - lightPosition);
	distance /= farPlanes;	
	gl_FragDepth = distance;
}