#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLight;

void main()
{
    // ambient
    vec3 ambient = pointLight.ambient * texture(material.diffuse1, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pointLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuse * diff * texture(material.diffuse1, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pointLight.specular * spec * texture(material.specular1, TexCoords).rgb;

    // attenuation
    float distance    = length(pointLight.position - FragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse   *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}