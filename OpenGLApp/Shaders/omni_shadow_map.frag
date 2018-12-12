#version 330

in vec4 fragmentPosition;

uniform vec3 lightPosition;
uniform float farPlane;

void main()
{
	float Distance = length(fragmentPosition.xyz - lightPosition);
	Distance /= farPlane;	
	gl_FragDepth = Distance;
}