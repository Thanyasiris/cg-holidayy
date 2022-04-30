#version 330


out vec4 colour;

uniform vec3 lightColour;



void main()
{
    /*colour = vCol;*/
    //float ambientStrength = 1.0f;
    //vec3 ambient = ambientStrength * lightColour;
    colour = vec4(lightColour, 1.0f);
}