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

struct OmniShadowMap
{
	samplerCube shadowMap;
	float farPlane;
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
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform Material material;

uniform vec3 eyePosition;

vec3 sampleOffestDirections[20] = vec3[]
(
	vec3( 1, 1, 1), vec3( 1, -1, 1), vec3( -1, -1, 1), vec3( -1, 1, 1),
	vec3( 1, 1, -1), vec3( 1, -1, -1), vec3( -1, -1, -1), vec3( -1, 1, -1),
	vec3( 1, 1, 0), vec3( 1, -1, 0), vec3( -1, -1, 0), vec3( -1, 1, 0),
	vec3( 1, 0, 1), vec3( -1, 0, 1), vec3( 1, 0, -1), vec3( -1, 0, -1),
	vec3( 0, 1, 1), vec3( 0, -1, 1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float calcShadowFactor(vec4 _directionalLightSpacePos)
{
	vec3 projCoords = _directionalLightSpacePos.xyz / _directionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float current = projCoords.z;

	vec3 normalized = normalize(normal);
	vec3 lightDirection = normalize(directionalLight.direction);
	
	float bias = max(0.05 * (1 - dot(normalized, lightDirection)), 0.005);

	float shadow = 0.0f;

	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += (current - bias) > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9.0f;

	if(projCoords.z > 1.0)
	{
		shadow = 0.0;
	}

	return shadow;
}

float calcOmniShadowFactor(PointLight _light, int _shadowIndex)
{
	vec3 fragToLight = fragmentPosition - _light.position;
	float current = length(fragToLight);

	float shadow = 0.0;
	float bias = 0.05;
	int samples = 20;

	float viewDistance = length(eyePosition - fragmentPosition);
	float discRadius = (1.0 + (viewDistance/omniShadowMaps[_shadowIndex].farPlane)) / 25.0;

	for(int i = 0; i < samples; i++)
	{
		float closest = texture(omniShadowMaps[_shadowIndex].shadowMap, fragToLight + sampleOffestDirections[i] * discRadius).r;
		closest *= omniShadowMaps[_shadowIndex].farPlane;
		if(current - bias > closest)
		{
			shadow += 1.0;
		}
	}

	shadow /= float(samples);
	return shadow;
}

vec4 calcLightByDirection(Light _light, vec3 _direction, float _shadowFactor)
{
	vec4 ambientColour = vec4(_light.colour, 1.0f) * _light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(normal), normalize(_direction)), 0.0f);
	vec4 diffuseColour = vec4(_light.colour * _light.diffuseIntensity * diffuseFactor, 1.0f);

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

vec4 calcDirectionalLight(vec4 _directionalLightSpacePos)
{
	//float shadowFactor = calcDirectionalShadowFactor(directionalLight);
	float shadowFactor = calcShadowFactor(_directionalLightSpacePos);
	return calcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 calcPointLight(PointLight _pointLight, int _shadowIndex)
{
	vec3 direction = fragmentPosition - _pointLight.position;
	float distance = length(direction);
	direction = normalize(direction);

	float shadowFactor = calcOmniShadowFactor(_pointLight, _shadowIndex);
	
	vec4 colour = calcLightByDirection(_pointLight.base, direction, shadowFactor);
	float attenuation = _pointLight.exponent * distance * distance + _pointLight.linear * distance + _pointLight.constant;
	return (colour / attenuation);
}

vec4 calcSpotLight(SpotLight _spotLight, int _shadowIndex)
{
	vec3 rayDirection = normalize(fragmentPosition - _spotLight.base.position);
	float spotLightFactor = dot(rayDirection, _spotLight.direction);

	if(spotLightFactor > _spotLight.edge)
	{
		vec4 colour = calcPointLight(_spotLight.base, _shadowIndex);
		return colour * (1.0f - (1.0f - spotLightFactor)*(1.0f/(1.0f - _spotLight.edge)));
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
		totalColour += calcPointLight(pointLights[i], i);
	}
	return totalColour;
}

vec4 calcSpotLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++)
	{
		totalColour += calcSpotLight(spotLights[i], i + pointLightCount);
	}
	return totalColour;
}

void main()															
{
	vec4 finalColour = calcDirectionalLight(directionalLightSpacePos);
	finalColour += calcPointLights();
	finalColour += calcSpotLights();
	colour = texture(theTexture, texCoord) * finalColour;
}