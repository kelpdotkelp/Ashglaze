#version 400 core

uniform vec3 meshColor;
uniform bool renderIDMode = false;

uniform int selectedID = -1;//-1 maps to no face selected.
uniform int selectedIDObject = -1;
uniform vec3 selectedColorChange;

struct DirectionalLight
{
   vec3 direction;
   vec3 ambient;
   vec3 diffuse;
};

uniform DirectionalLight directionalLight0;
uniform DirectionalLight directionalLight1;

in vec3 normal;
flat in uint ID;//flat prevents interpolation, every fragment gets the same ID

out vec4 outputColor;

vec3 calcDirectionalLight(DirectionalLight dirLight);

void main()
{
   if (!renderIDMode)
   {
      outputColor = vec4(
         calcDirectionalLight(directionalLight0) +
         calcDirectionalLight(directionalLight1), 1.0);

      //Highlight selected object
      if (selectedID == ID)
      {
         outputColor = outputColor + vec4(selectedColorChange, 1.0);
      }
   }
   else
   {
      uint outputID = ID;
      if (selectedIDObject != -1)//Render entire mesh with model ID rather than each face ID.
         outputID = selectedIDObject;

      float r = ((outputID & 0x000000FF) >> 0)/255.0;
      float g = ((outputID & 0x0000FF00) >>  8)/255.0;
      float b = ((outputID & 0x00FF0000) >> 16)/255.0;
      outputColor = vec4(r, g, b, 1.0);
   }
}

vec3 calcDirectionalLight(DirectionalLight dirLight)
{
   vec3 ambient = dirLight.ambient * meshColor;

   //Diffuse lighting
   vec3 lightDir = normalize(-dirLight.direction);//Specified as from source to origin, reverse so dot product is positive
   float diffuseCoef = max(dot(lightDir, normal), 0.0);
   vec3 diffuse = diffuseCoef * dirLight.diffuse * meshColor;

   return vec3(ambient + diffuse);
}