#version 330														
																	
in vec4 vColor;	
in vec2 texCoord;
in vec3 normal;
in vec3 fragmentPosition;
in vec4 directionalLightSpacePos;
																	
out vec4 colour;	

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

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
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};
		
struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;
uniform Material material;

uniform vec3 eyePosition;



float calcDirectionalShadowFactor(DirectionalLight _light)
{
	vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float closest = texture(directionalShadowMap, projCoords.xy).r;
	float current = projCoords.z;

	float shadow = current > closest ? 1.0 : 0.0;

	return shadow;
}

vec4 calcLightByDirection(Light _light, vec3 _direction, float _shadowFactor)
{
	vec4 ambientColour = vec4(_light.colour, 1.0f) * _light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(normal), normalize(_direction)), 0.0f);
	vec4 diffuseColour = vec4(_light.colour, 1.0f) * _light.diffuseIntensity * diffuseFactor;

	vec4 specularColour = vec4(0, 0, 0, 0);
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - fragmentPosition);
		vec3 reflectedVertex = normalize(reflect(_direction, normalize(normal)));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(_light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColour + (1.0 - _shadowFactor) * (diffuseColour + specularColour));
}

vec4 calcDirectionalLight()
{
	float shadowFactor = calcDirectionalShadowFactor(directionalLight);
	return calcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 calcPointLight(PointLight _pointLight)
{
	vec3 direction = fragmentPosition - _pointLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	
	vec4 colour = calcLightByDirection(_pointLight.base, direction, 0.0f);
	float attenuation = _pointLight.exponent * distance * distance + _pointLight.linear * distance + _pointLight.constant;
	return (colour / attenuation);
}

vec4 calcSpotLight(SpotLight _spotLight)
{
	vec3 rayDirection = normalize(fragmentPosition - _spotLight.base.position);
	float spotLightFactor = dot(rayDirection, _spotLight.direction);

	if(spotLightFactor > _spotLight.edge)
	{
		vec4 colour = calcPointLight(_spotLight.base);
		return colour * (1.0f - (1.0f - spotLightFactor)/(1.0f - _spotLight.edge)) ;
	}
	else
	{
		return vec4(0, 0, 0, 0);
	}
}

vec4 calcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++)
	{
		totalColour += calcPointLight(pointLights[i]);
	}
	return totalColour;
}

vec4 calcSpotLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++)
	{
		totalColour += calcSpotLight(spotLights[i]);
	}
	return totalColour;
}

void main()															
{
	vec4 finalColour = calcDirectionalLight();
	finalColour += calcPointLights();
	finalColour += calcSpotLights();
	colour = texture(theTexture, texCoord) * finalColour;
}