//Point Light

#version 330 core

out vec4 FragColor;

uniform sampler2D tex0;

uniform sampler2D norm_tex;

uniform vec3 lightPos;

uniform vec3 lightColor;


uniform float ambientStr;
uniform vec3 ambientColor;


uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;


uniform float baseConstant;
uniform float baseLinear;
uniform float baseQuadratic;

uniform float multConstant;
uniform float multLinear;
uniform float multQuadratic;

in vec2 texCoord;

in vec3 normCoord;

in vec3 fragPos;

in mat3 TBN;

void main()
{
	
	/*
	
		References:
				https://learnopengl.com/Lighting/Light-casters
				https://learnopengl.com/Lighting/Multiple-lights

	
	*/

	vec4 pixelColor = texture(tex0, texCoord);
	if(pixelColor.a < 0.1){
		discard;
	}

	//vec3 normal = normalize(normCoord);

	vec3 normal = texture(norm_tex, texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);

	//light pos is modified in the main file
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 diffuse = diff * lightColor;

	vec3 ambientCol = ambientColor * ambientStr;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);

	vec3 specColor = spec * specStr * lightColor;

	float distance = length(lightPos - fragPos);


	/*
	
		default values are configured in main.cpp

		Kc = 1.0f
		Kl = 0.027
		Kq = 0.0028

	*/

	//computing attenuation, light intensity manipulation also happens here

	float constant = baseConstant * multConstant;
	float linear = baseLinear * multLinear;
	float quadratic = baseQuadratic * multQuadratic;


	float attenuation = constant + linear * distance + quadratic * (distance * distance);
	float luminosity = 1.0f / attenuation; 

	specColor *= luminosity;
	diffuse *= luminosity;
	ambientCol *= luminosity;


	FragColor = vec4(specColor + diffuse + ambientCol,1.0) * texture(tex0, texCoord);
}