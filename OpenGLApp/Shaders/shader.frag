#version 330														
																	
in vec4 vColor;	
in vec2 texCoord;
in vec3 normal;
in vec3 fragmentPosition;
																	
out vec4 colour;	

const int MAX_POINT_LIGHTS = 3;

struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
}

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
}
		
struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int poinLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLight[MAX_POINT_LIGHTS];

uniform sampler2D theTexture;
uniform Material material;

uniform vec3 eyePosition;



vec4 calcLightByDirection(Light _light, vec3 _direction)
{
	vec4 ambientColour = vec4(_light.colour, 1.0f) * _light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
	vec4 diffuseColour = vec4(_light.colour, 1.0f) * _light.diffuseIntensity * diffuseFactor;

	vec4 specularColour = vec4(0, 0, 0, 0);
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - fragmentPosition);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(normal)));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(_light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColour + diffuseColour + specularColour);
}

vec4 calcDirectionalLight()
{
	return calcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 calcPointLights()
{
	
}

void main()															
{
	vec4 finalColour = calcDirectionalLight();
	finalColour += calcPointLights();
	colour = texture(theTexture, texCoord) * finalColour;
}