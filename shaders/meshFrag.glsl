#version 400 core

uniform vec3 meshColor;

struct DirectionalLight
{
   vec3 direction;
   vec3 ambient;
   vec3 diffuse;
};

uniform DirectionalLight directionalLight;

in vec3 normal;

out vec4 outputColor;

void main()
{
   vec3 ambient = directionalLight.ambient * meshColor;

   //Diffuse lighting
   vec3 lightDir = normalize(-directionalLight.direction);//Specified as from source to origin, reverse so dot product is positive
   float diffuseCoef = max(dot(lightDir, normal), 0.0);
   vec3 diffuse = diffuseCoef * directionalLight.diffuse * meshColor;

   outputColor = vec4(ambient + diffuse, 1.0);
}