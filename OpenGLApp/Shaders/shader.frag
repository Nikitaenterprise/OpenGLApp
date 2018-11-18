#version 330														
																	
in vec4 vColor;	
in vec2 texCoord;
																	
out vec4 colour;													
		
uniform sampler2D theTexture;

void main()															
{																	
	colour = texture(theTexture, texCoord);											
}