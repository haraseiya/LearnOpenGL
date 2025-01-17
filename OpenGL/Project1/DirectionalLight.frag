#version 330 core

out vec4 FragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

uniform Material material;
uniform DirectionalLight light;

void main()
{
	// アンビエント
	vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;

	// ディフューズ
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse,TexCoords).rgb;

	// スペキュラー
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result,1.0);
}