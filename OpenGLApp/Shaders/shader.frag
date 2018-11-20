#version 330														
																	
in vec4 vColor;	
in vec2 texCoord;
																	
out vec4 colour;	

struct DirectionalLight
{
	vec3 colour;
	float ambientIntensity;
};
		
uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;

void main()															
{
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
	colour = texture(theTexture, texCoord) * ambientColour;											
}