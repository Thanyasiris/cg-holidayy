#version 330

in vec4 vCol;
in vec2 TexCoord;
out vec4 colour;

uniform vec3 lightColour;

uniform sampler2D texture2D;

void main()
{
    //colour = vec4(1.0, 0.0, 0.0, 1.0); 
    //colour = vCol;
    float ambientStrength = 0.8f;
    vec3 ambient = ambientStrength * lightColour;
    colour = texture(texture2D, TexCoord) * vec4(lightColour, 0.3);
}