#version 330

in vec4 fragmentPosition;

uniform vec3 lightPosition;
uniform float farPlane;

void main()
{
	float distance = length(fragmentPosition.xyz - lightPosition);
	distance /= farPlane;	
	gl_FragDepth = distance;
}